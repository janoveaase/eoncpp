#include "../Graph.h"
#include "LocEnd.h"
#include "OpNot.h"
#include "../OpOr.h"
#include "../NodeGroup.h"
#include "FixedValue.h"
#include "Backreference.h"
#include "LocWordBoundary.h"
#include "WordChar.h"
#include "Special.h"
#include "Case.h"
#include "Letter.h"
#include "Digit.h"
#include "Any.h"
#include "CaptureGroup.h"


namespace eon
{
	namespace rx
	{
		Graph& Graph::operator=( const Graph& other )
		{
			Source = other.Source;
			Head = other.Head->copy();
			MyFlags = other.MyFlags;
			return *this;
		}
		Graph& Graph::operator=( Graph&& other ) noexcept
		{
			Source = std::move( other.Source );
			Head = other.Head; other.Head = nullptr;
			MyFlags = std::move( other.MyFlags );
			return *this;
		}

		void Graph::parse( substring source, substring flags )
		{
			if( source.empty() )
				return;

			Source = source;
			parseFlags( flags );

			parse();

			if( Head )
			{
				_removeDuplicates();
				_combineFixed();
				if( !( MyFlags & Flag::no_ungroup ) )
					_removeSuperfluousGroups();
				if( !( MyFlags & Flag::no_exposing ) )
					_exposeLiterals();
				if( !(MyFlags & Flag::lines ) && ( MyFlags & Flag::failfast_fixed_end ) )
					_failFastFixedEnd();
				_countMinCharsRemaining();
			}
		}



		void Graph::parse()
		{
			ParseParam param( Source );
			if( !parse( param ) )
				Head = param.extract();
		}

		void Graph::parseFlags( substring flags )
		{
			std::vector<string> invalid;
			bool _not_ = false;
			for( auto c : flags )
			{
				if( !_not_ )
				{
					switch( c )
					{
						case 'i':
							MyFlags |= Flag::icase;
							break;
						case 'l':
							MyFlags |= Flag::lines;
							break;
						case 'f':
							MyFlags |= Flag::failfast_fixed_end;
							break;
						case '!':
							_not_ = true;
							break;

						case 's':
							MyFlags |= Flag::speed;
							break;
						case 'a':
							MyFlags |= Flag::accuracy;
							break;
						default:
							invalid.push_back( string( c ) );
							break;
					}
				}
				else
				{
					switch( c )
					{
						case 'g':
							MyFlags |= Flag::no_ungroup;
							break;
						case 'e':
							MyFlags |= Flag::no_exposing;
							break;
						default:
							invalid.push_back( string( "!" ) << c );
							break;
					}
				}
			}
			if( invalid.size() == 1 )
				throw InvalidExpression( "Invalid flag: " + *invalid.begin() );
			else if( invalid.size() > 1 )
				throw InvalidExpression( "Invalid flags: " + string( ", " ).join( invalid ) );
		}

		void Graph::ParseParam::set( Node* node ) noexcept
		{
			if( HeadNode == nullptr )
			{
				HeadNode = node;
				PrevNode = nullptr;
				CurNode = HeadNode;
			}
			else if( node != CurNode )
			{
				PrevNode = CurNode;
				CurNode->Next = node;
				CurNode = CurNode->Next;
			}
		}
		void Graph::ParseParam::replaceCur( Node* node ) noexcept
		{
			if( HeadNode == CurNode )
			{
				HeadNode = node;
				PrevNode = nullptr;
			}
			else
				PrevNode->Next = node;
			CurNode = node;
		}

		bool Graph::parse( ParseParam& param, int64_t end )
		{
			while( param )
			{
				if( end >= 0 && param() == end )
				{
					param.advance();
					return true;
				}

				auto start = param.pos();
				auto node = parseNode( param );
				if( node != nullptr )
				{
					node->PreAnchoring = param.preAnchor();
					param.resetPreAnchor();
					if( node->Type == NodeType::op_or )
					{
						node = parseOr( (OpOr*)node, param, start );
						if( node )
							param.set( node );
					}
					else
						param.set( node );
				}
			}
			return false;
		}





		Node* Graph::parseNode( ParseParam& param )
		{
			while( param )
			{
				auto c = param();
				if( param.inGroup() && c == ')' )
					break;
				auto start = param.pos();
				switch( c )
				{
					case '^':
					{
						param.advance();
						param.preAnchor( MyFlags & Flag::lines ? Anchor::line : Anchor::input );
						break;
					}
					case '$':
					{
						param.advance();
						return new LocEnd( substring( start, param.pos() ) );
					}
					case '!':
						return parseNot( param, start );
					case '|':
						param.advance();
						return new OpOr( substring( start, param.pos() ) );
					case '(':
						return parseNodeGroup( param, start );
					case '[':
						return parseCharGroup( param, start );
					case '@':
						return parseCaptureRelated( param, start );
					case '\\':
					{
						auto node = parseEscaped( param, start );
						if( node )
							return node;
						break;
					}
					case '?':
						param.advance();
						if( param.cur() == nullptr )
							return new FixedValue( "?", substring( start, param.pos() ) );
						if( param.cur()->Quant )
						{
							if( param.cur()->Quant.Greedy )
							{
								param.cur()->Quant.Greedy = false;
								return nullptr;
							}
							else
								return new FixedValue( "?", substring( start, param.pos() ) );
						}
						else
							return setQuantifier( param, 0, 1 );
					case '*':
						param.advance();
						return setQuantifier( param, 0, INDEX_MAX );
					case '+':
						param.advance();
						return setQuantifier( param, 1, INDEX_MAX );
					case '{':
					{
						if( param.cur() != nullptr
							&& substring( param.pos() + 1, param.pos() + 5 ).compare( substring( "name" ) ) == 0 )
						{
							param.pos( param.pos() + 6 );
							param.cur()->Name = true;
							break;
						}
						index_t min{ 0 }, max{ INDEX_MAX };
						try
						{
							if( parseQuantifier( param, min, max, start ) )
								return setQuantifier( param, min, max );
							else
								return nullptr;
						}
						catch( ... )
						{
							throw;
						}
					}
					case '.':
						param.advance();
						return new Any( substring( start, param.pos() ) );
					default:
						return parseFixed( param, start );
				}
			}
			return nullptr;
		}

		Node* Graph::parseNot( ParseParam& param, const string_iterator& start )
		{
			if( param.advance() )			// the '!'
			{
				auto value = parseNode( param );
				if( value != nullptr )
				{
					value->PreAnchoring = param.preAnchor();
					param.resetPreAnchor();
					if( param.pos() && *param.pos() == '{' )
					{
						index_t min{ 0 }, max{ INDEX_MAX };
						try
						{
							if( parseQuantifier( param, min, max, param.pos() ) )
								value->Quant._set( min, max, true );
						}
						catch( ... )
						{
							delete value;
							throw;
						}
					}
					if( value->Quant.minQ() != value->Quant.maxQ() )
					{
						if( value->type() == NodeType::val_chargroup )
						{
							delete value;
							throw InvalidExpression();
						}
					}
					return new OpNot( value, substring( start, param.pos() ) );
				}
			}
			return new FixedValue( "!", substring( start, param.pos() ) );
		}
		Node* Graph::parseOr( OpOr* node_or, ParseParam& param,
			const string_iterator& start )
		{
			// We have the '|' operator, which means we must have a node prior
			// to it, to start the sequence of optionals
			if( param.cur() == nullptr )
			{
				// No - this is an error!
				delete node_or;
				throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
					+ ": Operator '|' cannot be the first element" );
				return nullptr;
			}

			// Move the prior node in as an option of this
			node_or->Optionals.push_back( param.cur() );
			param.replaceCur( node_or );

			// Add remaining options
			while( param )
			{
				auto node = parseNode( param );
				if( node == nullptr )
					continue;
				node->PreAnchoring = param.preAnchor();
				param.resetPreAnchor();
				if( node->type() == NodeType::op_or )
				{
					// Two '|' in a row, assume second is a fixed value
					// TODO: Is it better to ignore it? (Or report error?)
					node = new FixedValue( "|", substring( start, param.pos() ) );
				}
				else if( node->type() == NodeType::op_not )
				{
					// Got a negation, the next node should be next of this
					node->Next = parseNode( param );
					if( node->Next == nullptr )
					{
						delete node;
						continue;
					}
					node->Next->PreAnchoring = param.preAnchor();
					param.resetPreAnchor();
				}
				node_or->Optionals.push_back( node );

				// If the next node is not a '|', then the optionals is ended
				// We must also handle optional quantifiers
				ParseParam param2( param );
				param2.set( node );
				while( true )
				{
					if( param2.inGroup() && param2() == ')' )
						return node_or;
					node = parseNode( param2 );
					param.pos( param2.pos() );
					if( node == nullptr )
					{
						if( !param2 )
							return node_or;

						// Must be a quantifier, so try again
						continue;
					}
					node->PreAnchoring = param.preAnchor();
					param.resetPreAnchor();
					break;
				}
				if( node->type() != NodeType::op_or )
				{
					param.set( node_or );
					return node;
				}
			}
			return node_or;
		}

		Node* Graph::parseCharGroup( ParseParam& param, const string_iterator& start )
		{
			param.advance();			// The '['

			CharGroup::CharGrp value;
			if( param && param() == '^' )
			{
				value.Negate = true;
				param.advance();
			}
			bool esc = false;
			int64_t prev = -1;
			while( param )
			{
				auto c = param();
				if( !esc )
				{
					switch( c )
					{
						case '\\':
							if( prev != -1 )
							{
								value.Chars.insert( static_cast<char_t>( prev ) );
								prev = -1;
							}
							esc = true;
							param.advance();
							break;
						case ']':
							if( prev != -1 )
							{
								value.Chars.insert( static_cast<char_t>( prev ) );
								prev = -1;
							}
							param.advance();
							return new CharGroup( std::move( value ), substring( start, param.pos() ) );
						case '-':
							if( prev == -1 )
								value.Chars.insert( c );
							else
								parseCharGroupRange( param, value, static_cast<char_t>( prev ) );
							break;
						default:
							if( prev != -1 )
								value.Chars.insert( static_cast<char_t>( prev ) );
							prev = c;
							param.advance();
					}
				}
				else
				{
					switch( c )
					{
						case '\\':
						case ']':
						case '-':
							value.Chars.insert( c );
							param.advance();
							break;
						case 'w':
						case 'W':
						case 'u':
						case 'U':
						case 'd':
						case 'D':
						case 's':
						case 'S':
						case 'p':
						case 'P':
							value.Special.insert( c );
							param.advance();
							break;
						case 'e':
							// TODO: Handle escape sequence!
							break;
						default:
							value.Chars.insert( '\\' );
							break;
					}
					esc = false;
				}
			}
			if( prev != -1 )
				value.Chars.insert( static_cast<char_t>( prev ) );
			throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
				+ ": Character group starting here is missing ending ']'" );
			return nullptr;
		}
		void Graph::parseCharGroupRange( ParseParam& param, CharGroup::CharGrp& value, char_t start )
		{
			if( !param.advance() )			// The '-'
			{
				value.Chars.insert( start );
				value.Chars.insert( '-' );
			}
			else
			{
				value.Ranges.push_back( std::make_pair( start, param() ) );
				param.advance();
			}
		}

		Node* Graph::parseCaptureRelated( ParseParam& param,
			const string_iterator& start )
		{
			if( !param.advance() )			// The '@'
				return new FixedValue( "@", substring( start, param.pos() ) );
			auto type = param();
			if( type == '<' )
				return parseCapture( param, start );
			else if( type == ':' )
				return parseBackref( param, start );
			else
				return new FixedValue( "@", substring( start, param.pos() ) );
		}
		Node* Graph::parseCapture( ParseParam& param, const string_iterator& start )
		{
			param.advance();			// The '<'
			eon::string name;
			while( param )
			{
				auto c = param();
				if( c == '>' )
				{
					if( name.empty() )
					{
						throw InvalidExpression( "At position "
							+ string( static_cast<long_t>( start - Source.begin() + 1 ) )
							+ ": Capture group must have a name" );
						return nullptr;
					}
					else
					{
						if( !param.advance() || param() != '(' )
						{
							throw InvalidExpression( "At position "
								+ string( static_cast<long_t>( start - Source.begin() + 1 ) )
								+ ": Capture group must have a parenthesized group following immediately after the '>'" );
							return nullptr;
						}
						auto start = param.pos();
						auto nodegrp = parseNodeGroup( param, start );
						if( nodegrp == nullptr )
						{
							throw InvalidExpression( "At position "
								+ string( static_cast<long_t>( start - Source.begin() + 1 ) )
								+ ": Capture group starting here is missing terminating ')'" );
							return nullptr;
						}
						auto _name = eon::name( name );
						if( _name == no_name )
						{
							throw InvalidExpression( "At position "
								+ string( static_cast<long_t>( param.pos() - Source.begin() ) )
								+ ": Name of capture group can only contain letters, "
									"numbers and underscore, and not all numbers" );
							return nullptr;
						}
						return new CaptureGroup( _name, (NodeGroup*)nodegrp,
							substring( start, param.pos() ) );
					}
				}
				else if( string::isWordChar( c ) )
				{
					param.advance();
					name += c;
				}
				else
				{
					throw InvalidExpression( "At position " + string( static_cast<long_t>( param.pos() - Source.begin() ) )
						+ ": Invalid character in group name" );
					return nullptr;
				}
			}
			throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
				+ ": Incomplete capture group started here" );
			return nullptr;
		}
		Node* Graph::parseBackref( ParseParam& param, const string_iterator& start )
		{
			eon::string name;
			if( !param.advance() )			// The ':'
				goto error;
			if( param() != '<' || !param.advance() )
				goto error;
			while( param )
			{
				auto c = param();
				if( string::isWordChar( c ) )
				{
					param.advance();
					name += c;
				}
				else
				{
					if( c == '>' )
					{
						param.advance();
						if( name.empty() )
						{
							throw InvalidExpression( "At position "
								+ string( static_cast<long_t>( start - Source.begin() + 1 ) )
								+ ": Back-reference must name a previously defined capture group" );
							return nullptr;
						}
						auto _name = eon::name( name );
						if( _name == no_name )
						{
							throw InvalidExpression( "At position "
								+ string( static_cast<long_t>( param.pos() - Source.begin() ) )
								+ ": Back-reference name can only contain letters, "
									"numbers and underscore, and not all numbers" );
							return nullptr;
						}
						return new Backreference( _name, substring( start, param.pos() ) );
					}
					throw InvalidExpression("At position " + string( static_cast<long_t>( param.pos() - Source.begin() ) )
						+ ": Invalid character in group name" );
					return nullptr;
				}
			}
		error:
			throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
				+ ": Incomplete back-reference started here" );
			return nullptr;
		}

		Node* Graph::parseEscaped( ParseParam& param, const string_iterator& start )
		{
			if( !param.advance() )				// The '\'
				return new FixedValue( "\\", substring( start, param.pos() ) );
			auto c = param();
			param.advance();
			switch( c )
			{
				case 'b':
					param.advance();
					param.preAnchor( Anchor::word );
					return nullptr;
				case 'B':
					return new LocWordEnd( substring( start, param.pos() ) );
				case 'w':
					return new WordChar( substring( start, param.pos() ) );
				case 'W':
					return new NotWordChar( substring( start, param.pos() ) );
				case 'l':
					return new Letter( substring( start, param.pos() ) );
				case 'L':
					return new NotLetter( substring( start, param.pos() ) );
				case 'u':
					return new LowerCase( substring( start, param.pos() ) );
				case 'U':
					return new UpperCase( substring( start, param.pos() ) );
				case 'd':
					return new Digit( substring( start, param.pos() ) );
				case 'D':
					return new NotDigit( substring( start, param.pos() ) );
				case 's':
					return new Space( substring( start, param.pos() ) );
				case 'S':
					return new NotSpace( substring( start, param.pos() ) );
				case 'p':
					return new Punctuation( substring( start, param.pos() ) );
				case 'P':
					return new NotPunctuation( substring( start, param.pos() ) );
				case '\\':
					return new FixedValue( "\\", substring( start, param.pos() ) );
				case '\"':
					return new FixedValue( "\"", substring( start, param.pos() ) );
				case 'n':
					return new FixedValue( "\n", substring( start, param.pos() ) );
				case 't':
					return new FixedValue( "\t", substring( start, param.pos() ) );

				default:
					return new FixedValue( string( c ), substring( start, param.pos() ) );
			}
		}

		bool Graph::parseQuantifier( ParseParam& param, index_t& min, index_t& max, const string_iterator& start )
		{
			if( !param.advance() )			// The '{'
				return new FixedValue( "{", substring( start, param.pos() ) );
			string digit;
			while( param )
			{
				if( param() == '}' )
				{
					if( !digit.empty() )
					{
						min = std::stoull( digit );
						max = min;
					}
					param.advance();
					return true;
				}
				else if( param() == ',' )
				{
					param.advance();
					break;
				}
				else if( string::isNumberDecimalDigit( param() ) )
					digit += param();
				else
				{
					throw InvalidExpression( "At position " + string( static_cast<long_t>( param.pos() - Source.begin() ) )
						+ ": Expected digit, comma, or '}' here" );
				}
				param.advance();
			}
			if( !digit.empty() )
			{
				min = std::stoull( digit );
				digit.clear();
			}
			while( param )
			{
				if( param() == '}' )
				{
					if( !digit.empty() )
						max = std::stoull( digit );
					param.advance();
					return true;
				}
				else if( string::isNumberDecimalDigit( param() ) )
					digit += param();
				else
				{
					throw InvalidExpression( "At position " + string( static_cast<long_t>( param.pos() - Source.begin() ) )
						+ ": Expected digit, comma, or '}' here" );
					return false;
				}
				param.advance();
			}
			throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
				+ ": Expected '}' and the end the sequence starting here" );
			return false;
		}

		Node* Graph::parseNodeGroup( ParseParam& param, const string_iterator& start )
		{
			if( !param.advance() )			// The '('
				return new FixedValue( "(", substring( start, param.pos() ) );
			param.startGroup();
			ParseParam param2( param );
			if( !parse( param2, ')' ) )
			{
				param.endGroup();
				throw InvalidExpression( "At position " + string( static_cast<long_t>( start - Source.begin() + 1 ) )
					+ ": Group started here is missing terminating ')'" );
				param.pos( param2.pos() );
				return nullptr;
			}
			param.endGroup();
			param.pos( param2.pos() );
			return new NodeGroup( param2.head(), substring( start, param.pos() ) );
		}

		Node* Graph::parseFixed( ParseParam& param, const string_iterator& start )
		{
			string value{ param() };
			if( string::isSpaceChar( param() ) )
			{
				while( param.advance() && string::isSpaceChar( param() ) )
					value += param();
			}
			else if( string::isWordChar( param() ) )
			{
				while( param.advance() && string::isWordChar( param() ) )
					value += param();
			}
			else
				param.advance();
			return new FixedValue( std::move( value ), substring( start, param.pos() ) );
		}

		Node* Graph::setQuantifier( ParseParam& param, index_t min, index_t max )
		{
			if( param.cur() == nullptr )
			{
				auto start = param.pos();
				param.advance();
				return new FixedValue( "?", substring( start, param.pos() ) );
			}
			else
			{
				param.cur()->Quant._set( min, max, true );
				return nullptr;
			}
		}


		void Graph::_removeDuplicates() {
			Head->removeDuplicates(); }
		void Graph::_combineFixed() {
			Head->combineFixed(); }
		void Graph::_countMinCharsRemaining() {
			Head->_countMinCharsRemaining(); }
		void Graph::_removeSuperfluousGroups() noexcept {
			Head = Head->_removeSuperfluousGroups(); }
		void Graph::_exposeLiterals() {
			Head = Head->_exposeLiterals(); }
		void Graph::_failFastFixedEnd() {
			Head->_failFastFixedEnd( *Head ); }
	}
}
