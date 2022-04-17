#include "Stringifier.h"
#include <stack>


namespace eon
{
	string Stringifier::str() const
	{
		std::vector<string> lines;
		string line;
		index_t indentation{ 0 };
		bool end_block{ false };
		std::vector<index_t> splitpoints;
		Type prev{ Type::none };
		for( auto& elm : Elements )
		{
			switch( elm.What )
			{
				case Type::lf:				// Hard end of line
					_processLine( line, lines, splitpoints, indentation, true );
					prev = Type::lf;
					continue;
				case Type::startblock:
					line += elm.Value;
					_processLine( line, lines, splitpoints, indentation, true );
					prev = Type::lf;
					++indentation;
					continue;
				case Type::endblock:
					if( !line.empty() )
						_processLine( line, lines, splitpoints, indentation, false );
					--indentation;
					end_block = true;
					continue;
				case Type::opr:
				case Type::opname:
				case Type::specop:
					splitpoints.push_back( line.numChars() - 1 );
					break;
				case Type::stop:
				case Type::open:
					splitpoints.push_back( line.numChars() + 1 );
					break;
				case Type::close:
					splitpoints.push_back( line.numChars() );
					line += elm.Value;
					prev = elm.What;
					continue;
				default:
					break;
			}
			if( end_block )
			{
				if( prev != Type::lf && elm.What != Type::stop )
					_processLine( line, lines, splitpoints, indentation, true );
				end_block = false;
			}
			if( prev == Type::lf && indentation > 0 )
				line += string( indentation * 2, ' ' );
			line += elm.Value;
			prev = elm.What;
		}
		if( !line.empty() || end_block )
			_processLine( line, lines, splitpoints, indentation, end_block );

		return string().join( lines );
	}




	Stringifier& Stringifier::push( Type what, string&& value )
	{
		switch( what )
		{
			case Type::word:
			case Type::prefix:
				if( afterWord() || ( afterOp1() && OpSpaces1 ) || ( afterOp2() && OpSpaces2 ) || afterStop()
					|| ( afterOpen() && OpenCloseSpaces ) )
					_push( Type::space, " " );
				return _push( what, std::move( value ) );
			case Type::append:
				if( afterWord() )
					return _push( Type::word, std::move( value ) );
				else
					return push( Type::word, std::move( value ) );				
			case Type::opr:
				if( OpSpaces1 && !afterSpace() && !afterLF() )
					_push( Type::space, " " );
				return _push( what, std::move( value ) );
			case Type::opname:
				return _push( what, std::move( value ) );
			case Type::specop:
				if( OpSpaces2 && !afterSpace() && !afterLF() )
					_push( Type::space, " " );
				return _push( what, std::move( value ) );
			case Type::stop:
				return _push( what, std::move( value ) );
			case Type::open:
				if( afterWord() || ( afterOp1() && OpSpaces1 ) || ( afterOp2() && OpSpaces2 ) || afterStop()
					|| ( afterOpen() && OpenCloseSpaces ) )
					_push( Type::space, " " );
				return _push( what, std::move( value ) );
			case Type::close:
				if( OpenCloseSpaces && !afterSpace() && !afterLF() )
					_push( Type::space, " " );
				return _push( what, std::move( value ) );
			case Type::startblock:
				return _push( what, std::move( value ) );
			case Type::endblock:
				return _push( what, std::move( value ) );
			default:
				return *this;
		}
	}

	void Stringifier::_processLine( string& line, std::vector<string>& lines, std::vector<index_t>& splitpoints,
		index_t indentation, bool endline ) const
	{
		if( line.numChars() <= MaxLineWidth || splitpoints.empty() )
		{
			lines.push_back( std::move( line ) );
			if( endline )
				lines[ lines.size() - 1 ] += "\n";
		}
		else
		{
			string indent;
			auto beg = line.begin();
		restart:
			for( size_t i = splitpoints.size(); line.numChars() - beg.numChar() > MaxLineWidth && i > 0; --i )
			{
				if( splitpoints[ i - 1 ] + indent.numChars() <= MaxLineWidth )
				{
					auto cut = splitpoints[ i - 1 ];
					lines.push_back( indent + line.substr( beg, beg + cut ) );
					lines[ lines.size() - 1 ] += "\n";
					if( indent.empty() )
						indent.assign( ++indentation * 2, ' ' );
					beg += cut;
					while( beg && *beg == ' ' )
						++beg;
					auto tmp = std::move( splitpoints );
					for( auto j = i; j < tmp.size(); ++j )
						splitpoints.push_back( tmp[ j ] - cut - 1 );
					goto restart;
				}
			}
			if( beg )
			{
				lines.push_back( indent + line.substr( beg ) );
				if( endline )
					lines[ lines.size() - 1 ] += "\n";
			}
			line.clear();
		}
		splitpoints.clear();
	}
}
