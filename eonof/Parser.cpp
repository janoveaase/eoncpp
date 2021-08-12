#include "Parser.h"


namespace eon
{
	namespace tf
	{
		Parser::Parser( const sourceref& source )
		{
			Source = source;
			TP = tokenparser( tokenizer()( source.source() ) );
		}




		tuple Parser::parseDocumentStart( vars::variables& vars, bool merge )
		{
			tuple result;
			if( !TP || ( skipEmtpyLines() && !TP ) )
				return result;
			result.append( vars::nameval::create( no_name ), vars );
			result.append( vars::metaval::create(), vars );

			if( !TP.match( { "-", "-", "-" } ) )
				return result;
			TP.forward( 3 );
			skipNoise();

			// End of header?
			if( !TP || TP.current().newline() )
				return result;

			// Next must be a name or we have an error
			auto name = parseRawName();
			if( name == no_name )
				throw EofBadSyntax( TP.current().source().textRefRange()
					+ "\nExpected a document name here!" );
			result.set( 0, vars::nameval::create( name ), vars );

			skipNoise();

			// End if end of line
			if( !TP || TP.current().newline() )
				return result;

			// Expect meta information to follow
			auto meta = parseMeta( vars, merge );
			if( !meta )
				throw EofBadSyntax( TP.current().source().textRefRange()
					+ "\nExpected document meta information here!" );
			result.set( 1, meta, vars );
			return result;
		}

		bool Parser::parseDocumentAttribute( tuple& document, vars::variables& vars, bool merge )
		{
			if( !TP || ( skipEmtpyLines() && !TP ) || TP.match( { "-", "-", "-" } ) )
				return false;

			indentationStrict();
			parseAttribute( document, vars, merge );

			if( !TP || ( skipEmtpyLines() && !TP )
				|| TP.match( { "-", "-", "-" } ) )
				return false;
			return true;
		}


		vars::valueptr Parser::parseValue( tokenparser& parser, vars::variables& vars, ContextType context, bool merge )
		{
			auto tmp = std::move( TP );
			TP = std::move( parser );
			auto val = parseValue( vars, context, merge );
			parser = std::move( TP );
			TP = std::move( tmp );
			return val;
		}




		void Parser::parseAttribute( tuple& result, vars::variables& vars, bool merge )
		{
			auto value = parseValue( vars, result.function() ? ContextType::function : ContextType::plain, merge );
			if( !value )
			{
				if( TP )
				{
					if( !TP.current().newline() )
						throw EofBadSyntax( TP.current().source().textRefRange() + "\nUnexpected!" );
				}
 				return;
			}

			// If we have a name, it may be an attribute name
			if( value->isName() )
			{
				auto main_indent = indentationStrict();
				skipNoise();

				// Check for meta data
				vars::valueptr meta;
				if( TP && TP.current().match( "<" ) )
				{
					meta = parseMeta( vars, merge );
					skipNoise();
				}

				// If next we have a colon, then we have a sub-tuple that must
				// be indented on the next line.
				if( TP && TP.current().match( ":" ) )
				{
					TP.forward();
					auto pos = TP.current().source().pos();
					skipNoise();
					if( !TP )
						throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos()
							+ "\nExpected an attribute value following this!" );

					auto name = value->actualName();

					// Expect a newline
					if( !TP.current().newline() )
						throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos()
							+ "\nExpected an indented sub-tuple to follow the colon!" );

					auto& cur = TP.current();
					while( TP && TP.current().newline() )
					{
						TP.forward();
						if( TP.current().space() && TP.ahead().newline() )
							TP.forward();
					}
					auto indent = indentationStrict();
					if( indent < main_indent + 2 )
						throw EofBadSyntax( TP.current().source().textRefRange()
							+ "\nInvalid indentation of sub-tuple value here!" );
					auto type = meta && meta->actualMeta().containsUnnamedValue( name_function )
						? tupletype::function : tupletype::plain;
					value = parseTuple( vars, type, merge );
					if( !value )
						throw EofBadSyntax( cur.source().textRefPos()
							+ "\nExpected an indented sub-tuple value here!" );

					if( merge )
						result.addMerge( value, vars, name,
							meta ? tupleptr( new tuple( ( (vars::metaval*)&*meta )->claim() ) ) : tupleptr() );
					else
						result.addReplace( value, vars, name,
							meta ? tupleptr( new tuple( ( (vars::metaval*)&*meta )->claim() ) ) : tupleptr() );
					return;
				}

				// If next is an equal sign and not colon, we have a basic
				// value (which may still be a braced tuple!)
				else if( TP && TP.current().match( "=" ) )
				{
					TP.forward();
					auto pos = TP.current().source().pos();
					skipNoise();
					if( !TP )
						throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos()
							+ "\nExpected an attribute value following this!" );

					auto name = value->actualName();

					// Indented sub-tuples not allowed following '='!
					if( TP.current().newline() )
						throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos()
							+ "\nExpected an attribute value following \"=\" - on the same line!" );

					// Get value
					else
					{
						value = parseValue( vars, ContextType::plain, merge );
						if( !value )
							throw EofBadSyntax( sourceref(
								Source.source(), pos ).textRefRange()
								+ "\nExpected an attribute value here!" );
						if( merge )
							result.addMerge( value, vars, name, meta
								? tupleptr( new tuple( ( (vars::metaval*)&*meta )->claim() ) )
								: nullptr );
						else
							result.addReplace( value, vars, name, meta
								? tupleptr( new tuple( ( (vars::metaval*)&*meta )->claim() ) )
								: nullptr );
						return;
					}
				}
			}

			// Whatever we have, it is a nameless value
			result.append( value, vars );
		}

		vars::valueptr Parser::parseValue( vars::variables& vars, ContextType context, bool merge )
		{
			// Expression?
			if( context == ContextType::function )
				return parseExpr( vars );

			// Parenthesized tuple?
			if( TP.current().match( "{" ) )
				return parseTuple( vars, tupletype::curly, merge );

			// Dash-marked tuple?
			else if( TP.match( { "-", " " } ) )
				return parseTuple( vars, tupletype::dash, merge );

			// Meta data tuple?
			else if( TP.current().match( "<" ) )
				return parseMeta( vars, merge );

			// Character?
			else if( TP.current().match( "'" ) )
				return parseChar();

			// String?
			else if( TP.current().match( "\"" ) )
				return parseString();

			// Binary?
			else if( TP.current().match( "%" ) )
				return parseBinary();

			// Raw?
			else if( TP.current().match( "|" ) )
				return parseRaw();

			// Regex?
			else if( TP.current().match( "?" ) )
				return parseRegex();

			// Reference?
			else if( TP.current().match( "@" ) )
				return parseRef();

			// Name?
			else if( context == ContextType::expression && TP.current().match( "#" ) )
				return parseName();

			// Expression?
			else if( TP.match( { "&", "(" } ) || context == ContextType::function )
				return parseExpr( vars );

			// Positive number?
			else if( TP.current().match( "+" ) )
			{
				TP.forward();
				if( !TP )
					throw EofBadSyntax( TP.last().source().textRefRange() + "\nExpected a number to follow this!" );
				if( !TP.current().number() )
					throw EofBadSyntax( TP.current().source().textRefRange() + "\nExpected a number here!" );
				if( TP.exists() && TP.ahead().match( DecimalSep.substr() ) )
					return parseFloat();
				else
					return parseInt();
			}

			// Negative number?
			else if( TP.current().match( "-" ) )
			{
				TP.forward();
				if( !TP )
					throw EofBadSyntax( TP.last().source().textRefRange() + "\nExpected a number to follow this!" );
				if( !TP.current().number() )
					throw EofBadSyntax( TP.current().source().textRefRange() + "\nExpected a number here!" );
				if( TP.exists() && TP.ahead().match( DecimalSep.substr() ) )
					return parseFloat( -1.0 );
				else
					return parseInt( -1 );
			}

			// Name, literal byte/bytes, hex or reference?
			else if( TP.current().letter() || TP.current().extendedNumber()
				|| TP.current().number() || TP.current().match( "_" ) )
			{
				auto name = parseRawName();
				if( name != no_name )
				{
					// Is it a boolean literal?
					if( name == name_true || name == name_false )
						return vars::boolval::create( name == name_true );

					// Must be a name if plain context, variable if expression
					if( context == ContextType::plain )
						return vars::nameval::create( name );
					else
						return vars::variableval::create( name );
				}
			}

			// Unsigned number?
			if( TP.current().number() )
			{
				if( TP.exists() && TP.ahead().match( DecimalSep.substr() ) )
					return parseFloat();
				else
					return parseInt();
			}

			return nullptr;
		}

		bool Parser::skipComments()
		{
			bool skipped = false;
			while( TP )
			{
				if( TP.match( { "/", "/" } ) )
				{
					TP.skipToEol();
					skipped = true;
				}
				else if( TP.match( { "/", "-" } ) )
				{
					skipped = true;
					for( TP.forward( 2 ); TP; TP.forward() )
					{
						if( TP.match( { "-", "/" } ) )
						{
							TP.forward( 2 );
							break;
						}
					}
				}
				else
					break;
			}
			return skipped;
		}
		bool Parser::skipSpaces()
		{
			if( !TP || !TP.current().space() )
				return false;
			TP.forward();
			return true;
		}
		bool Parser::skipNoise()
		{
			bool skipped = false;
			while( TP )
			{
				if( skipComments() || skipSpaces() )
					skipped = true;
				else
					break;
			}
			return skipped;
		}
		bool Parser::skipEmtpyLines()
		{
			bool skipped = false;
			while( TP )
			{
				if( skipNoise() )
					skipped = true;
				else if( TP.current().newline() )
				{
					skipped = true;
					TP.forward();
				}
				else
					break;
			}
			return skipped;
		}


		vars::valueptr Parser::parseChar()
		{
			TP.forward();	// Skip opening "'"
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange() + "\nExpected a character value to follow this!" );
			if( TP.current().substr().numChars() != 1 )
				throw EofBadSyntax( TP.current().source().textRefRange() + "\nExpected a character value here!" );
			auto value = *TP.current().substr().begin();
			auto& pos = TP.current().source().pos();
			TP.forward();
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange() + "\nMissing end of char value!" );
			if( value == '\\' )
			{
				string tmp = "\\";
				while( TP && !TP.current().match( "'" ) )
				{
					tmp += TP.current().substr();
					TP.forward();
				}
				tmp = tmp.unescape();
				if( tmp.numChars() != 1 )
					throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos() + "\nInvalid escape sequence!" );
				value = *tmp.begin();
			}
			else
			{
				if( !TP )
					throw EofBadSyntax( TP.last().source().textRefRange()
						+ "\nExpected a single quote character to follow this!" );
				if( !TP.current().match( "'" ) )
					throw EofBadSyntax( TP.current().source().textRefPos()
						+ "\nExpected a single quote character here!" );
			}
			TP.forward();
			return vars::charval::create( value );
		}
		vars::valueptr Parser::parseInt( int64_t sign )
		{
			int64_t val = string( TP.current().substr() ).toInt64();
			TP.forward();
			return vars::intval::create( val * sign );
		}
		vars::valueptr Parser::parseFloat( double sign )
		{
			string val = TP.current().substr();
			TP.forward( 2 );	// Skip past the decimal separator as well
			if( TP && TP.current().number() )
			{
				val += "." + TP.current().substr();
				TP.forward();
			}
			return vars::floatval::create( val.toDouble() * sign );
		}
		vars::valueptr Parser::parseName()
		{
			auto pos = TP.current().source().pos();
			TP.forward();		// Skip the "#"
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange() + "\nExpected a name to follow this!" );
			pos = TP.current().source().pos();

			// Expect a name
			auto name = parseRawName();
			if( name == no_name )
				throw EofBadSyntax(
					sourceref( Source.source(), pos ).textRefPos() + "\nExpected a name here!" );
			return vars::nameval::create( name );
		}
		name_t Parser::parseRawName()
		{
			// This is a bit tricky. We need to scan ahead to the next non-name
			// token, and if we have a valid name (not numbers only), we can
			// return a name object - otherwise we have a number and must
			// return nullptr!
			string name = TP.current().substr();
			size_t i = 1;
			for( ; TP.exists( i ); ++i )
			{
				auto next = TP.ahead( i );
				if( next.letter() || next.extendedNumber() || next.number()
					|| next.match( "_" ) )
					name += next.substr();
				else
					break;
			}
			if( name.numeralsOnly() )
				return no_name;
			TP.forward( i );
			return eon::name::get( name );
		}
		vars::valueptr Parser::parseString()
		{
			TP.forward();	// Skip opening "\""
			auto start_line = TP.current().source().pos().line();
			auto start_pos = TP.current().source().pos().pos( TP.current().source().source() );
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange()
					+ "\nExpected a string value to follow this!" );
			string value;
			bool escape = false;
			for( ; TP; TP.forward() )
			{
				if( !escape )
				{
					if( TP.current().match( "\"" ) )		// End condition
						break;
					else if( TP.current().match( "\\" ) )	// Escape sequence
					{
						value += TP.current().substr();
						escape = true;
					}
					else
						value += TP.current().substr();
				}
				else
				{
					escape = false;
					value += TP.current().substr();
				}
			}
			if( !TP || !TP.current().match( "\"" ) )
				throw EofBadSyntax( Source.name() + ":" + string( start_line + 1 ) + ":" + string( start_pos + 1 )
					+ "\nExpected a double quote character here!" );
			TP.forward();
			return vars::stringval::create( value.unescape() );
		}
		vars::valueptr Parser::parseBinary()
		{
			auto base_indent = indentationStrict();
			auto main_indent = base_indent + 2;

			TP.forward();	// Skip opening "%"
			if( !TP )		// Empty binary value is legal!
				return vars::binaryval::create();
			std::string line;
			hex hx, value;
			sourcepos pos;
			while( true )
			{
				skipNoise();
				if( !TP )
					break;
				if( TP.current().newline() )
				{
					if( !line.empty() )
					{
						try
						{
							hx.setHex( line );
						}
						catch( ... )
						{
							throw EofBadSyntax( sourceref( Source.source(), pos ).textRefRange()
								+ "\nNot a valid hex digits section!" );
						}
						value += hx;
					}
					TP.forward();
					if( !TP || indentation() < main_indent )
						break;
					continue;
				}
				pos = TP.current().source().pos();
				line += TP.current().substr().stdstr();
				TP.forward();
			}
			if( !line.empty() )
			{
				try
				{
					hx.setHex( line );
				}
				catch( ... )
				{
					throw EofBadSyntax( sourceref( Source.source(), pos ).textRefRange()
						+ "\nNot a valid hex digits section!" );
				}
				value += hx;
			}
			return vars::binaryval::create( value );
		}
		vars::valueptr Parser::parseRaw()
		{
			auto base_indent = indentationStrict();
			auto main_indent = base_indent + 2;

			TP.forward();	// Skip opening "|"
			if( !TP )		// Empty raw value is legal!
				return vars::rawval::create();

			// If the next token is space or a line comment or spaces + line
			// comment, we can skip those.
			if( TP.current().space() )
			{
				if( TP.exists() )
				{
					if( TP.ahead().newline() )
						TP.forward();
					else if( TP.exists( 2 ) && TP.ahead().match( "/" ) && TP.ahead( 2 ).match( "/" ) )
						TP.skipToEol();
				}
			}
			else if( TP.match( { "/", "/" } ) )
				TP.skipToEol();

			if( TP.current().newline() )
			{
				TP.forward();
				auto indent = indentation();
				if( indent < main_indent )		// Empty raw value is legal!
					return vars::rawval::create();
			}

			// Keep reading until indentation drops below main indentation
			std::vector<string> lines;
			string line;
			while( TP )
			{
				if( TP.current().newline() )
				{
					lines.push_back( std::move( line ) );
					TP.forward();
					continue;
				}

				// Backslash + newline means we don't add newline to the string
				if( TP.current().match( "\\" ) && TP.exists() && TP.ahead().newline() )
				{
					TP.forward(); TP.forward();
					auto indent = indentation();
					if( indent < main_indent )
						break;
					if( indent > main_indent )
						line += string( indent - main_indent, SpaceChr );
					TP.forward();
					continue;
				}

				// Spaces at the start of the line must be adjusted according
				// to indentation
				if( line.empty() && TP.current().space() )
				{
					auto indent = TP.current().substr().numChars();
					if( indent < main_indent )
						break;
					if( indent > main_indent )
						line += string( indent - main_indent, SpaceChr );
					TP.forward();
					continue;
				}

				// Anything else gets added as-is
				line += TP.current().substr();
				TP.forward();
			}
			if( !line.empty() )
				lines.push_back( std::move( line ) );
			return vars::rawval::create( std::move( lines ) );
		}
		vars::valueptr Parser::parseRegex()
		{
			TP.forward();	// Skip opening "?"
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange()
					+ "\nExpected a regex boundary character to follow this!" );
			if( TP.current().substr().numChars() != 1 )
				throw EofBadSyntax( TP.current().source().textRefPos()
					+ "\nExpected a regex boundary character here!" );
			auto start_line = TP.current().source().pos().line();
			auto start_pos = TP.current().source().pos().pos( TP.current().source().source() );
			string boundary = TP.current().substr();
			string pattern{ boundary };
			TP.forward();
			if( !TP )
				throw EofBadSyntax( TP.last().source().textRefRange()
					+ "\nExpected a regex expression to follow this!" );

			// Keep reading until we see the boundary character again
			bool escape = false;
			for( ; TP; TP.forward() )
			{
				if( !escape )
				{
					if( TP.current().match( boundary.substr() ) )
					{
						pattern += boundary;
						boundary.clear();
						TP.forward();
						break;
					}
					if( TP.current().match( "\\" ) )
					{
						pattern += TP.current().substr();
						escape = true;
					}
					else
						pattern += TP.current().substr();
				}
				else
				{
					escape = false;
					pattern += TP.current().substr();
				}
			}
			if( !boundary.empty() )
			{
				auto pos = TP ? TP.current().source().pos() : TP.last().source().pos();
				throw EofBadSyntax(
					sourceref( Source.source(), pos ).textRefRange()
					+ "\nExpected the boundary character to end the regular expression!" );
			}

			// Got any flags?
			if( TP && TP.current().letter() )
			{
				string flags = TP.current().substr();
				if( TP.current().substr().containsOtherThan( substring( "ilsSo" ) ) )
					throw EofBadSyntax( TP.current().source().textRefRange()
						+ "\nOnly legal regex flags are: ilsSo!" );
				pattern += TP.current().substr();
				TP.forward();
			}

			regex value;
			try
			{
				value = pattern.substr();
			}
			catch( rx::InvalidExpression& e )
			{
				throw EofBadSyntax( Source.name() + ":" + string( start_line + 1 )
					+ ":" + string( start_pos + 1 ) + "\n" + e.what() );
			}

			return vars::regexval::create( std::move( value ) );
		}
		vars::valueptr Parser::parseRef()
		{
			auto pos = TP.current().source().pos();
			TP.forward();		// Skip the first opening "@"

			// Keep reading until no more names followed by forward slash
			nameref path;
			while( TP )
			{
				// Expect a name
				auto name = parseRawName();
				if( name == no_name )
					throw EofBadSyntax( TP.current().source().textRefRange()
						+ "\nExpected a name part of a path here!" );
				path.add( name );

				// Expect end or forward slash
				if( !TP || !TP.current().match( "/" ) )
					break;
				TP.forward();
			}
			if( path.empty() )
				throw EofBadSyntax( sourceref( Source.source(), pos ).textRefRange()
					+ "\nExpected a name part of a path here!" );
			return vars::refval::create( std::move( path ), nullptr );
		}
		vars::valueptr Parser::parseExpr( vars::variables& vars )
		{
			auto base_indent = indentationStrict();
			auto main_indent = base_indent + 2;

			auto pos = TP.current().source().pos();
			
			// Check if braced
			auto braced = TP.match( { "&", "(" } );
			if( braced )
				TP.forward( 2 );	// Skip opening "&("

			// Get everything up to the closing ')' (if braced) or indentation
			// reduction
			size_t nested = 1;
			string expr;
			while( TP )
			{
				if( TP.current().match( "(" ) )
					++nested;
				else if( TP.current().match( ")" ) )
				{
					if( ( braced && nested == 0 ) || ( !braced && nested == 1 ) )
						throw EofBadSyntax( TP.current().source().textRefRange() + "\nMissing matching '('!" );
					if( --nested == 0 )
					{
						TP.forward();
						break;
					}
				}
				else if( TP.current().newline() )
				{
					TP.forward();
					if( !TP )
					{
						if( nested == 0 )
							break;
						throw EofBadSyntax( TP.last().source().textRefRange()
							+ "\nExpression started at line " + string( pos.line() )
							+ " pos " + string( pos.pos( Source.source() ) ) + " is not complete!" );
					}
					auto indent = indentation();
					if( indent < main_indent )
					{
						if( nested == 1 )
						{
							TP.backward();
							break;
						}
						throw EofBadSyntax( TP.current().source().textRefRange()
							+ "\nIndentation reduced but expression started at line " + string( pos.line() )
							+ " pos " + string( pos.pos( Source.source() ) ) + " is not complete!" );
					}
				}
				expr += TP.current().substr();
				TP.forward();
			}
			if( braced && nested == 1 )
				throw EofBadSyntax( sourceref( Source.source(), pos ).textRefRange()
					+ "\nExpression must end in ')'" );
			else if( ( braced && nested > 1 ) || ( !braced && nested > 1 ) )
				throw EofBadSyntax( sourceref( Source.source(), pos ).textRefRange()
					+ "\nInvalid expression (Check parenthesis!)" );

			expression ex;
			try
			{
				ex = expression( std::move( expr ), vars );
			}
			catch( exception& e )
			{
				throw EofBadSyntax( sourceref( Source.source(), pos ).textRefPos() + "\n" + e.what() );
			}
			return vars::expressionval::create( std::move( ex ), braced );
		}
		vars::valueptr Parser::parseTuple( vars::variables& vars, tupletype type, bool merge )
		{
			// We have five types of tuples:
			// 1. By indentation (at or greater than 'base_indentation')
			// 2. Started by dash + space
			// 3. Curly braced
			// 4. Meta (angle braced)
			// 5. Function (marked by "function" keyword in named attribute)
			//    (Only detection is handled here!)
			size_t main_indent = indentation();
			string group_end;
			switch( type )
			{
				case tupletype::curly:
					main_indent += 2;
					group_end = "}";
					break;
				case tupletype::meta:
					main_indent += 2;
					group_end = ">";
					break;
				case tupletype::dash:
					main_indent += 2;
					break;
				default:
					break;
			}

			auto pos = TP.current().source().pos();

			// If we have dash-start, then skip dash + space
			if( type == tupletype::dash )
				TP.forward( 2 );

			// If we have a group end, then skip the group start character
			if( !group_end.empty() )
				TP.forward();

			skipNoise();

			// Keep reading until the end of the tuple
			tuple result(
				type == tupletype::curly ? tuple::form::braced
				: type == tupletype::meta ? tuple::form::metadata
				: type == tupletype::function ? tuple::form::function
				: tuple::form::plain );
			bool end_marker = group_end.empty();
			while( TP )
			{
				// Check for end of tuple marker
				if( !group_end.empty() && TP.current().match( group_end.substr() ) )
				{
					TP.forward();
					end_marker = true;
					break;
				}

				// Newline requires checking indentation
				if( TP.current().newline() )
				{
					TP.forward();
					if( !TP )
						break;
					auto indent = indentation();
					if( indent < main_indent )
					{
						auto pos = TP.pos();
						skipNoise();
						if( !TP || !TP.current().newline() )
						{
							TP.pos( pos );
							if( indent < main_indent - 2 )
								TP.backward();
							break;
						}
					}
					skipNoise();
				}

				// We most likely have something
				parseAttribute( result, vars, merge );

				skipNoise();

				// If we now have a comma, just skip it
				if( TP && TP.current().match( "," ) )
				{
					TP.forward();
					skipNoise();
				}
			}
			if( end_marker )
			{
				vars::valueptr val;
				switch( type )
				{
					case tupletype::meta:
						if( !result.empty() )
						{
							val = vars::metaval::create( std::move( result ) );
							dynamic_cast<vars::metaval*>( &*val )->meta_value().setAttributeContext( val );
						}
						else
							val = vars::metaval::create();
						break;
					case tupletype::function:
						if( !result.empty() )
						{
							val = vars::functionval::create( std::move( result ) );
							dynamic_cast<vars::functionval*>( &*val )->function_value().setAttributeContext( val );
						}
						else
							val = vars::functionval::create();
						break;
					default:
						if( !result.empty() )
						{
							val = vars::tupleval::create( std::move( result ) );
							dynamic_cast<vars::tupleval*>( &*val )->tuple_value().setAttributeContext( val );
						}
						else
							val = vars::tupleval::create();
						break;
				}
				return val;
			}
			else
				throw EofBadSyntax( TP.last().source().textRefRange()
					+ "\nExpected tuple starting at line "
					+ string( pos.line() + 1 ) + " position "
					+ string( pos.pos( TP.last().source().source() ) + 1 )
					+ " to end in '" + group_end + "' here!" );
		}


		std::string Parser::unescape( const std::string& str ) const noexcept
		{
			static std::unordered_map<byte_t, byte_t> singletons{
				{ '\'', '\'' }, { '"', '"' }, { '?', '?' }, { '\\', '\\' },
				{ 'a', '\a' }, { 'b', '\b' }, { 'f', '\f' }, { 'n', '\n' },
				{ 'r', '\r' }, { 't', '\t' }, { 'v', '\v' } };
			std::string out;
			for( size_t i = 0; i < str.size(); ++i )
			{
				auto c = str[ i ];
				if( c == '\\' && i < str.size() - 1 )
				{
					auto c1 = str[ i + 1 ];
					int c2 = i < str.size() - 2 ? str[ i + 2 ] : -1;
					auto found = singletons.find( c1 );
					if( found != singletons.end() )
					{
						out += found->second;
						++i;
					}

					// Unicode
					else if( ( c1 == 'u' || c1 == 'U' ) && i < str.size() - 2 && isHexDigit( str[ i + 2 ] ) )
					{
						++i;
						char_t val{ 0 };
						for( int j = 0; j < 8; ++j )
						{
							val <<= 4;
							val += hexToNum( str[ ++i ] );
							if( i == str.size() - 1
								|| !isHexDigit( str[ i + 1 ] ) )
								break;
						}
						out += string( val ).stdstr();
						continue;
					}

					// Hex
					else if( c1 == '0' && ( c2 == 'x' || c2 == 'X' ) && i < str.size() - 3 && isHexDigit( str[ i + 3 ] ) )
					{
						i += 2;
						byte_t val{ 0 };
						int shifts = 0;
						while( true )
						{
							val <<= 4;
							val += hexToNum( str[ ++i ] );
							if( ++shifts == 2 )
							{
								shifts = 0;
								out += val;
								val = 0;
							}
							if( i == str.size() - 1
								|| !isHexDigit( str[ i + 1 ] ) )
								break;
						}
						continue;
					}

					// Octal
					else if( isOctalDigit( c1 ) )
					{
						uint32_t val{ 0 };
						bool have_val = false;
						while( true )
						{
							val = ( val << 3 ) + ( str[ ++i ] - '0' );
							have_val = true;
							if( val > UINT8_MAX )
							{
								uint32_t chr = val >> 1;
								out += static_cast<byte_t>( chr );
								val -= chr << 1;
							}
							if( i == str.size() - 1 || !isOctalDigit( str[ i + 1 ] ) )
							{
								if( have_val )
								{
									out += static_cast<byte_t>( val );
									have_val = false;
								}
								break;
							}
						}
						continue;
					}
				}
				out += c;
			}
			return out;
		}

		size_t Parser::indentationStrict() const
		{
			auto indent = TP.at( TP.lineStart() ).space() ? TP.at( TP.lineStart() ).substr().numChars() : 0;
			if( indent % 2 != 0 )
				throw EofBadSyntax( TP.current().source().textRefRange()
					+ "\nIndentation must be a factor of 2!" );
			return indent;
		}
	}
}
