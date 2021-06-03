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




		tuple Parser::parseDocumentStart()
		{
			tuple result;
			if( !TP || ( skipEmtpyLines() && !TP ) )
				return result;
			result.append( tup::valueptr( new tup::nameval( no_name ) ) );
			result.append( tup::valueptr( new tup::metaval() ) );

			if( !TP.current().match( "---" ) )
				return result;
			TP.forward();
			skipNoise();

			// End of header?
			if( !TP || TP.current().newline() )
				return result;

			// Next must be a name or we have an error
			auto name = parseName();
			if( name == nullptr )
				throw EtfBadSyntax( Source.name() + ":" + string( TP.current().source().pos().line() + 1 )
					+ ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nExpected a document name here!" );
			result.set( 0, name );

			skipNoise();

			// End if end of line
			if( !TP || TP.current().newline() )
				return result;

			// Expect meta information to follow
			auto meta = parseMeta();
			if( meta == nullptr )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nExpected document meta information here!" );
			result.set( 1, meta );
			return result;
		}

		bool Parser::parseDocumentAttribute( tuple& document )
		{
			if( !TP || ( skipEmtpyLines() && !TP )
				|| TP.current().match( "---" ) )
				return false;

			indentationStrict();
			parseAttribute( document );

			if( !TP || ( skipEmtpyLines() && !TP ) || TP.match( { "-", "-", "-" } ) )
				return false;
			return true;
		}




		void Parser::parseAttribute( tuple& result )
		{
			auto value = parseValue();
			if( value == nullptr )
				return;

			// If we have a name, it may be an attribute name
			if( value->isName() )
			{
				skipNoise();

				// Check for meta data
				tup::valueptr meta;
				if( TP && TP.current().match( "<" ) )
				{
					meta = parseMeta();
					skipNoise();
				}

				if( TP && TP.current().match( "=" ) )
				{
					TP.forward();
					auto& cur = TP.current();
					skipNoise();
					if( !TP )
						throw EtfBadSyntax( Source.name() + ":" + string(
							cur.source().pos().line() + 1 ) + ":" + string( cur.source().pos().pos( TP.current().source().source() ) + 1 )
							+ "\nExpected an attribute value following this!" );

					auto name = value->name_value();

					// If we got a newline, then the value is a sub-tuple
					if( TP.current().newline() )
					{
						auto& cur = TP.current();
						auto main_indent = indentationStrict();
						TP.forward();
						auto indent = indentationStrict();
						if( indent < main_indent + 2 )
							throw EtfBadSyntax( Source.name() + ":" + string(
								TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
								+ "\nInvalid indentation of sub-tuple value here!" );
						value = parseTuple( indent );
						if( value == nullptr )
							throw EtfBadSyntax( Source.name() + ":" + string(
								cur.source().pos().line() + 1 ) + ":" + string( cur.source().pos().pos( TP.current().source().source() ) + 1 )
								+ "\nExpected an indented sub-tuple value here!" );

						result.append( value, name, meta
							? tupleptr( new tuple(
								((tup::metaval*)&*meta)->claim() ) )
							: tupleptr() );
						return;
					}

					// Get value
					else
					{
						value = parseValue();
						if( value == nullptr )
							throw EtfBadSyntax( Source.name() + ":" + string(
								cur.source().pos().line() + 1 ) + ":" + string( cur.source().pos().pos( TP.current().source().source() ) + 1 )
								+ "\nExpected an attribute value here!" );
						result.append( value, name, meta
							? tupleptr( new tuple(
								((tup::metaval*)&*meta)->claim() ) )
							: nullptr );
						return;
					}
				}
			}

			// Whatever we have, it is a nameless value
			result.append( value );
		}

		tup::valueptr Parser::parseValue()
		{
			// Parenthesized tuple?
			if( TP.current().match( "(" ) )
				return parseTuple( indentation() + 2, ")" );

			// Dash-marked tuple?
			else if( TP.match( { "-", " " } ) )
				return parseTuple( indentation() + 2 );

			// Meta data tuple?
			else if( TP.current().match( "<" ) )
				return parseMeta();

			// Character?
			else if( TP.current().match( "'" ) )
				return parseChar();

			// String?
			else if( TP.current().match( "\"" ) )
				return parseString();

			// Binary?
			else if( TP.current().match( "#" ) )
				return parseBinary();

			// Raw?
			else if( TP.current().match( "|" ) )
				return parseRaw();

			// Regex?
			else if( TP.current().match( "?" ) )
				return parseRegex();

			// Path?
			else if( TP.current().match( "@" ) )
				return parseRef();

			// Positive number?
			else if( TP.current().match( "+" ) )
			{
				TP.forward();
				if( !TP || !TP.current().number() )
					throw EtfBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
						+ "\nExpected a number here!" );
				if( TP.exists() && TP.ahead().match(
					DecimalSep.substr() ) )
					return parseFloat();
				else
					return parseInt();
			}

			// Negative number?
			else if( TP.current().match( "-" ) )
			{
				TP.forward();
				if( !TP || !TP.current().number() )
					throw EtfBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
						+ "\nExpected a number here!" );
				if( TP.exists() && TP.ahead().match(
					DecimalSep.substr() ) )
					return parseFloat( -1.0 );
				else
					return parseInt( -1 );
			}

			// Name, literal byte/bytes, hex or reference?
			else if( TP.current().letter() || TP.current().extendedNumber()
				|| TP.current().number() || TP.current().match( "_" ) )
			{
				auto name = parseName();
				if( name )
				{
					// Is it a boolean literal?
					if( name->name_value() == name_true
						|| name->name_value() == name_false )
						return tup::valueptr( new tup::boolval(
							name->name_value() == name_true ) );

					// Must be a name
					return name;
				}
			}

			// Unsigned number?
			if( TP.current().number() )
			{
				if( TP.exists()
					&& TP.ahead().match( DecimalSep.substr() ) )
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
					TP.skipPastEol();
					skipped = true;
				}
				else if( TP.match( { "/", "-" } ) )
				{
					skipped = true;
					for( TP.forward( 2 ); TP; TP.forward() )
					{
						if( TP.match( { "-", "/" } ) )
							TP.forward();
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


		tup::valueptr Parser::parseChar()
		{
			TP.forward();	// Skip opening "'"
			if( !TP || TP.current().substr().numChars() != 1 )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nExpected a character value here!" );
			auto value = *TP.current().substr().begin();
			auto& pos = TP.current().source().pos();
			TP.forward();
			if( !TP )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nMissing end of char value!" );
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
					throw EtfBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos( TP.current().source().source() ) + 1 )
						+ "\nInvalid escape sequence!" );
				value = *tmp.begin();
			}
			else
			{
				if( !TP || !TP.current().match( "'" ) )
					throw EtfBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
						+ "\nExpected a single quote character here!" );
			}
			TP.forward();
			return tup::valueptr( new tup::charval( value ) );
		}
		tup::valueptr Parser::parseInt( int64_t sign )
		{
			int64_t val = string( TP.current().substr() ).toInt64();
			TP.forward();
			return tup::valueptr( new tup::intval( val * sign ) );
		}
		tup::valueptr Parser::parseFloat( double sign )
		{
			string val = TP.current().substr();
			TP.forward( 2 );	// Skip past the decimal separator as well
			if( TP && TP.current().number() )
			{
				val += "." + TP.current().substr();
				TP.forward();
			}
			return tup::valueptr( new tup::floatval( val.toDouble() * sign ) );
		}
		tup::valueptr Parser::parseName()
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
				return nullptr;
			TP.forward( i );
			return tup::valueptr( new tup::nameval( eon::name::get( name ) ) );
		}
		tup::valueptr Parser::parseString()
		{
			TP.forward();	// Skip opening "\""
			auto start_line = TP.current().source().pos().line();
			auto start_pos = TP.current().source().pos().pos( TP.current().source().source() );
			if( !TP )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nExpected a string value here!" );
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
				throw EtfBadSyntax( Source.name() + ":" + string(
					start_line + 1 ) + ":" + string( start_pos + 1 )
					+ "\nExpected a double quote character here!" );
			TP.forward();
			return tup::valueptr( new tup::stringval( value.unescape() ) );
		}
		tup::valueptr Parser::parseBinary()
		{
			auto base_indent = indentationStrict();
			auto main_indent = base_indent + 2;

			TP.forward();	// Skip opening "#"
			if( !TP )		// Empty binary value is legal!
				return tup::valueptr( new tup::binaryval() );
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
							throw EtfBadSyntax( Source.name() + ":" + string(
								pos.line() + 1 ) + ":" + string( pos.pos( TP.current().source().source() ) + 1 )
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
					throw EtfBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos( TP.last().source().source() ) + 1 )
						+ "\nNot a valid hex digits section!" );
				}
				value += hx;
			}
			return tup::valueptr( new tup::binaryval( value ) );
		}
		tup::valueptr Parser::parseRaw()
		{
			auto base_indent = indentationStrict();
			auto main_indent = base_indent + 2;

			TP.forward();	// Skip opening "|"
			if( !TP )		// Empty raw value is legal!
				return tup::valueptr( new tup::rawval() );

			// If the next token is space or a line comment or spaces + line
			// comment, we can skip those.
			if( TP.current().space() )
			{
				if( TP.exists() )
				{
					if( TP.ahead().newline() )
						TP.forward();
					else if( TP.exists( 2 )
						&& TP.ahead().match( "/" )
						&& TP.ahead( 2 ).match( "/" ) )
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
					return tup::valueptr( new tup::rawval() );
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
				if( TP.current().match( "\\" )
					&& TP.exists() && TP.ahead().newline() )
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
			return tup::valueptr( new tup::rawval( std::move( lines ) ) );
		}
		tup::valueptr Parser::parseRegex()
		{
			TP.forward();	// Skip opening "?"
			if( !TP || TP.current().substr().numChars() != 1 )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
					+ "\nExpected a regex boundary character here!" );
			auto start_line = TP.current().source().pos().line();
			auto start_pos = TP.current().source().pos().pos( TP.current().source().source() );
			string boundary = TP.current().substr();
			string pattern{ boundary };
			TP.forward();
			if( !TP )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nExpected a regex expression here!" );

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
				auto pos = TP ? TP.current().source().pos()
					: TP.last().source().pos();
				throw EtfBadSyntax( Source.name() + ":" + string( pos.line() + 1 )
					+ ":" + string( pos.pos( TP.last().source().source() ) + 1 )
					+ "\nExpected the boundary character to end the regular expression!" );
			}

			// Got any flags?
			if( TP && TP.current().letter() )
			{
				if( TP.current().substr().containsOtherThan(
					substring( "ilsSo" ) ) )
					throw EtfBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
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
				throw EtfBadSyntax( Source.name() + ":" + string( start_line + 1 )
					+ ":" + string( start_pos + 1 ) + "\n" + e.what() );
			}

			return tup::valueptr( new tup::regexval( std::move( value ) ) );
		}
		tup::valueptr Parser::parseRef()
		{
			auto pos = TP.current().source().pos();
			TP.forward();		// Skip the first opening "@"

			// Keep reading until no more names followed by forward slash
			tup::path path;
			while( TP )
			{
				// Expect a name
				auto name = eon::name::get( TP.current().substr() );
				if( name == no_name )
					throw EtfBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":" + string( TP.current().source().pos().pos( TP.current().source().source() ) + 1 )
						+ "\nExpected a name part of a path here!" );
				path.add( name );
				TP.forward();

				// Expect end or forward slash
				if( !TP || !TP.current().match( "/" ) )
					break;
				TP.forward();
			}
			if( path.empty() )
				throw EtfBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos( TP.last().source().source() ) + 1 )
					+ "\nExpected a name part of a path here!" );
			return tup::valueptr( new tup::refval( std::move( path ) ) );
		}
		tup::valueptr Parser::parseTuple( size_t base_indentation,
			string group_end )
		{
			// We have three types of tuples:
			// 1. By indentation (at or greater than 'base_indentation')
			// 2. Started by dash + space
			// 3. Ending in 'group_end' (greater than 'base_indentation' if
			//    multi-line)
			size_t main_indent = group_end.empty()
				? base_indentation : base_indentation + 2;

			// If we have dash-start, then skip dash + space
			if( TP.current().match( "-" ) )
				TP.forward( 2 );

			// If we have a group end, then skip the group start character
			if( !group_end.empty() )
				TP.forward();

			// Keep reading until the end of the tuple
			tuple result;
			while( TP )
			{
				// Check for end of tuple marker
				if( !group_end.empty()
					&& TP.current().match( group_end.substr() ) )
				{
					TP.forward();
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
						break;
					skipNoise();
				}

				// We most likely have something
				parseAttribute( result );

				skipNoise();

				// If we now have a comma, just skip it
				if( TP && TP.current().match( "," ) )
					TP.forward();

				skipNoise();
			}

			if( !result.empty() )
				return tup::valueptr( new tup::tupleval( std::move( result ) ) );
			else
				return tup::valueptr( new tup::tupleval() );
		}
		tup::valueptr Parser::parseMeta()
		{
			auto tuple = parseTuple( indentation(), ">" );
			return tup::valueptr( new tup::metaval(
				((tup::metaval*)&*tuple)->claim() ) );
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
					else if( ( c1 == 'u' || c1 == 'U' ) && i < str.size() - 2
						&& isHexDigit( str[ i + 2 ] ) )
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
					else if( c1 == '0' && ( c2 == 'x' || c2 == 'X' )
						&& i < str.size() - 3 && isHexDigit( str[ i + 3 ] ) )
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
							if( i == str.size() - 1
								|| !isOctalDigit( str[ i + 1 ] ) )
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
			auto indent = TP.at( TP.lineStart() ).space() ? TP.at(
				TP.lineStart() ).substr().numChars() : 0;
			if( indent % 2 != 0 )
				throw EtfBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":" + string(
						TP.current().source().pos().pos(
							TP.current().source().source() ) + 1 )
					+ "\nIndentation must be a factor of 2!" );
			return indent;
		}
	}
}
