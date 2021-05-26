#include "Parser.h"
#include "Title.h"
#include "Heading.h"
#include "Listing.h"
#include "Exclamation.h"
#include "Paragraph.h"
#include "Definition.h"


namespace eon
{
	namespace doc
	{
		Parser::Parser( const sourceref& source )
		{
			Source = source;
			TP = tokenparser( tokenizer()( source.source() ) );
		}




		document Parser::parseDocument()
		{
			document doc;
			string anchor;
			while( TP )
			{
				if( TP.current().newline() )
				{
					TP.forward();
					continue;
				}
				else if( TP.current().match( "@" ) )
					anchor = parseAnchor();
				else if( TP.match( { ">", ">" } ) )
				{
					anchor.clear();
					parseTitleOrHeading( doc );
				}
				else if( TP.match( { "  ", "-", "-" } ) )
				{
					anchor.clear();
					parseInsert( doc );
				}
				else if( TP.match( { "  ", "-", " " } ) )
					parseList( doc, std::move( anchor ) );
				else if( TP.exists( 2 ) && TP.current().match( "  " )
					&& TP.ahead( 1 ).number() && TP.ahead( 2 ).match( " " ) )
					parseNumberedList( doc, std::move( anchor ) );
				else if( TP.match( { "*", ":" } ) )
				{
					anchor.clear();
					parseExclamation( doc );
				}
				else if( TP.current().match( "|" ) )
					parseTable( doc, std::move( anchor ) );
				else if( isDefinitionStart() )
					parseDefinition( doc, std::move( anchor ) );
				else
					parseParagraph( doc, doc, ParaType::plain,
						std::move( anchor ) );
			}
			return doc;
		}




		void Parser::parseTitleOrHeading( document& doc )
		{
			auto pos = TP.current().source().pos();
			TP.forward( 2 );	// Skip ">", ">"
			TP.skipSpaces();
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nHeading started, but contains nothing!" );
			string text;
			size_t level = 1;
			bool is_title = false;
			while( TP )
			{
				if( TP.current().newline() )
					break;
				else if( *TP.current().substr().begin() == '>' )
				{
					if( !text.empty() )
						throw EdocBadSyntax( Source.name() + ":" + string(
							TP.current().source().pos().line() + 1 ) + ":"
							+ string( TP.current().source().pos().pos(
								TP.current().source().source() ) + 1 )
							+ "\nIllegal character inside title or heading!" );
					level += TP.current().substr().numChars();
					TP.forward();
				}
				else if( TP.match( { "<", "<" } ) )
				{
					is_title = true;
					TP.forward( 2 );	// Skip "<", "<"
					skipSpaces();
					if( TP && !TP.current().newline() )
						throw EdocBadSyntax( Source.name() + ":" + string(
							TP.current().source().pos().line() + 1 ) + ":"
							+ string( TP.current().source().pos().pos(
								TP.current().source().source() ) + 1 )
							+ "\nExpected newline after title!" );
				}
				else
				{
					text += TP.current().substr();
					TP.forward();
				}
			}
			text = text.trim();
			if( text.empty() )
			{
				string type = is_title ? "title" : "heading";
				throw EdocBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos(
						TP.last().source().source() ) + 1 )
					+ "\nEmpty " + type + " is not permitted!" );
			}
			string anchor = makeAnchor( text );
			if( is_title )
				doc.add( elementptr( new title( std::move( text ) ) ),
					std::move( anchor ) );
			else
				doc.add( elementptr( new heading( std::move( text ),
					level ) ), std::move( anchor ) );
		}

		void Parser::parseInsert( document& doc )
		{
			TP.forward();		// Skip "  "
			auto pos = TP.current().source().pos();
			TP.forward( 2 );		// Skip "-", "-"
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nInsert started, but contains nothing!" );
			instype itype{ instype::example_t };
			string typeinfo;
			string keyword = TP.current().substr();
			TP.forward();
			if( keyword == "example" )
				;		// Already set
			else if( keyword == "code" )
			{
				itype = instype::code_t;
				skipSpaces();
				if( !TP || TP.current().match( "," ) )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nExpected code language name here!" );
				do
				{
					typeinfo += TP.current().substr();
					TP.forward();
				} while( TP && !TP.match( { "-", "-", ">" } )
					&& !TP.current().space() && !TP.current().match( "," ) );
			}
			else if( keyword == "quote" )
			{
				itype = instype::quote_t;
				skipSpaces();
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nExpected quote source reference here!" );
				do
				{
					typeinfo += TP.current().substr();
					TP.forward();
				} while( TP && !TP.match( { "-", "-", ">" } )
					&& !TP.current().match( "," ) );
			}
			else if( keyword == "insert" )
			{
				itype = instype::insert_t;
				skipSpaces();
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nExpected insert type here!" );
				do
				{
					typeinfo += TP.current().substr();
					TP.forward();
				} while( TP && !TP.match( { "-", "-", ">" } )
					&& !TP.current().space() && !TP.current().match( "," ) );
			}
			else
				throw EdocBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos(
						TP.last().source().source() ) + 1 )
					+ "\nIllegal insert construct!" );
			skipSpaces();
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nIncomplete insert!" );
			string anchor;
			if( TP.current().match( "@" ) )
				anchor = parseAnchor();
			skipSpaces();
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nIncomplete insert!" );
			string heading;
			if( TP.current().match( "," ) )
			{
				TP.forward();
				while( !TP.match( { "-", "-", ">" } ) )
				{
					if( TP.current().newline() )
						throw EdocBadSyntax( Source.name() + ":" + string(
							TP.current().source().pos().line() + 1 ) + ":"
							+ string( TP.current().source().pos().pos(
								TP.current().source().source() ) + 1 )
							+ "\nExpected \"-->\" before newline!" );
					heading += TP.current().substr();
					TP.forward();
				}
			}
			if( !TP || !TP.match( { "-", "-", ">" } ) )
				throw EdocBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos(
						TP.last().source().source() ) + 1 )
					+ "\nInsert section is incomplete!" );
			TP.forward( 3 );	// Skip "-", "-", ">"
			std::vector<string> lines;
			string line;
			while( !TP.match( { "<", "-", "-" } ) )
			{
				if( TP.current().newline() )
				{
					if( !line.empty() )
						lines.push_back( std::move( line ) );
					else if( !lines.empty() )
						lines.push_back( string() );
					TP.forward();
					if( !TP )
						break;
					if( !TP.current().space() )
						throw EdocBadSyntax( Source.name() + ":" + string(
							TP.current().source().pos().line() + 1 ) + ":"
							+ string( TP.current().source().pos().pos(
								TP.current().source().source() ) + 1 )
							+ "\nEvery insert line must start with two spaces!" );
					if( TP.current().substr().numChars() > 2 )
						line += string( TP.current().substr().numChars() - 2,
							SpaceChr );
				}
				else
					line += TP.current().substr();
				TP.forward();
			}
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nIncomplete insert!" );
			TP.forward( 3 );	// Skip "<", "-", "-"
			if( !line.empty() )
				lines.push_back( std::move( line ) );
			switch( itype )
			{
				case instype::example_t:
					insertExample( doc, heading.trim(),
						std::move( anchor ), std::move( lines ) );
					break;
				case instype::code_t:
					insertCode( doc, std::move( typeinfo ),
						heading.trim(), std::move( anchor ),
						std::move( lines ) );
					break;
				case instype::quote_t:
					insertQuote( doc, std::move( typeinfo ),
						heading.trim(), std::move( anchor ),
						std::move( lines ) );
					break;
				case instype::insert_t:
					insertInsert( doc, pos, std::move( typeinfo ),
						heading.trim(), std::move( anchor ),
						std::move( lines ) );
					break;
			}
		}
		void Parser::insertExample( document& doc, string&& heading,
			string&& anchor, std::vector<string>&& lines )
		{
			doc.add( elementptr( new insert( instype::example_t,
				std::move( heading ), std::move( lines ) ) ),
				std::move( anchor ) );
		}
		void Parser::insertCode( document& doc, string&& lang,
			string&& heading, string&& anchor, std::vector<string>&& lines )
		{
			doc.add( elementptr( new insert( instype::code_t,
				std::move( lang ), std::move( heading ), std::move( lines ) ) ),
				std::move( anchor ) );
		}
		void Parser::insertQuote( document& doc, string&& source,
			string&& heading, string&& anchor, std::vector<string>&& lines )
		{
			doc.add( elementptr( new insert( instype::quote_t,
				std::move( source ), std::move( heading ),
				std::move( lines ) ) ), std::move( anchor ) );
		}
		void Parser::insertInsert( document& doc, const sourcepos& pos,
			string&& type, string&& heading, string&& anchor, std::vector<string>&& lines )
		{
			string data = string( " " ).join( lines );
			data = data.trim();
			if( type == "image" )
			{
				if( data.empty() )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nImage insert must have an image URL between \"-->\" and \"<--\"!" );
			}
			else if( type == "TOC" )
			{
				if( !data.empty() )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nTable-of-contents (TOC) insert cannot contain anything between \"-->\" and \"<--\"!" );
			}
			else if( type == "CTOC" )
			{
				if( !data.empty() )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nChapter table-of-contents (CTOC) insert cannot contain anything between \"-->\" and \"<--\"!" );
			}
			else if( type == "hidden" )
			{
				if( data.empty() )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nHidden insert must have something (other than spaces) between \"-->\" and \"<--\"!" );
			}
			else
				throw EdocBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos(
						TP.last().source().source() ) + 1 )
					+ "\n\"" + type + "\" is not a legal insert type!" );
			doc.add( elementptr( new insert( instype::insert_t,
				std::move( type ), std::move( heading ),
				std::vector<string>( { std::move( data ) } ) ) ), std::move( anchor ) );
		}

		void Parser::parseList( document& doc, string&& anchor )
		{
			auto list = new listing();
			auto elm = elementptr( list );
			while( TP )
			{
				TP.forward( 3 );		// Skip "  ", "-", " "
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						TP.last().source().pos().line() + 1 ) + ":" + string(
							TP.last().source().pos().pos(
								TP.last().source().source() ) + 1 )
						+ "\nList element cannot contain nothing!" );
				parseParagraph( doc, *list, ParaType::list, "" );
			}
			doc.add( elm, std::move( anchor ) );
		}

		void Parser::parseNumberedList( document& doc, string&& anchor )
		{
			auto list = new listing( true );
			auto elm = elementptr( list );
			while( TP )
			{
				TP.forward( 3 );		// Skip "  ", "<num>", " "
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						TP.last().source().pos().line() + 1 ) + ":" + string(
							TP.last().source().pos().pos(
								TP.last().source().source() ) + 1 )
						+ "\nList element cannot contain nothing!" );
				parseParagraph( doc, *list, ParaType::list, "" );
			}
			doc.add( elm, std::move( anchor ) );
		}

		void Parser::parseExclamation( document& doc )
		{
			auto xtype = mapExcltype( TP.current().substr() );
			if( xtype == excltype::undef )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.current().source().pos().line() + 1 ) + ":"
					+ string( TP.current().source().pos().pos(
						TP.current().source().source() ) + 1 )
					+ "\nNot a valid exclamation type!" );

			TP.forward( 2 );		// Skip "<exclamation>", ":"
			skipSpaces();
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nExclamation cannot contain nothing!" );
			string text;
			while( true )
			{
				text += TP.current().substr();
				TP.forward();
				if( !TP )
					break;
				if( TP.current().newline() )
				{
					if( TP.ahead().space()
						&& TP.ahead().substr().numChars() >= 2 )
					{
						TP.forward();
						if( !text.endsWith( SpaceChr ) )
							text += SpaceChr;
					}
					else
						break;
				}
			}
			doc.add( elementptr( new exclamation( xtype,
				std::move( text ) ) ) );
		}

		void Parser::parseTable( document& doc, string&& anchor )
		{
		}

		void Parser::parseParagraph( document& doc, block& parent,
			ParaType type, string&& anchor )
		{
			// This can be pure text, en emphasized element, or a
			// link.
			// The paragraph ends at the first empty line.
			// We can ignore newlines, except when the next line starts with
			// two spaces.

			auto para = new paragraph();
			elementptr elm( para );
			size_t hard_indent = type != ParaType::plain ? 4 : 0;
			bool have_space = true;
			while( TP )
			{
				if( have_space && TP.current().match( "*" ) )
				{
					auto emph = parseEmphasize();
					if( !emph.empty() )
					{
						para->add( section( sectiontype::emphasized,
							std::move( emph ) ) );
						continue;
					}
				}
				else if( TP.match( { "@", "[" } ) )
				{
					auto link = parseLink();
					if( !link.empty() )
					{
						para->add( section( sectiontype::link,
							std::move( link ) ) );
						continue;
					}
				}
				else if( TP.current().newline() )
				{
					TP.forward();
					if( !TP || TP.current().newline() )
						break;
					if( type == ParaType::list
						&& ( TP.match( { "  ", "-", " " } )
							|| ( TP.exists( 2 ) && TP.current().match( "  " )
								&& TP.ahead( 1 ).number()
								&& TP.ahead( 2 ).match( " " ) ) ) )
						break;
					else if( type == ParaType::def && isDefinitionStart() )
						break;
					if( TP.current().space() )
					{
						if( TP.current().substr().numChars()
							>= hard_indent + 2 )
							para->add( section( sectiontype::newline, "\n" ) );
						else
						{
							para->add( section( sectiontype::space, " " ) );
							have_space = true;
						}
						TP.forward();
					}
					else
						para->add( section( sectiontype::space, " " ) );
					continue;
				}
				else if( TP.current().space() )
				{
					para->add( section( sectiontype::space, " " ) );
					TP.forward();
					have_space = true;
					continue;
				}
				have_space = false;
				para->add( section( sectiontype::text,
					string( TP.current().substr() ) ) );
				TP.forward();
			}
			para->trimTrailing();
			parent.add( elm );
			if( !anchor.empty() )
			{
				elm->setAnchor( anchor );
				doc.setAnchor( std::move( anchor ), elm );
			}
		}
		string Parser::parseEmphasize()
		{
			size_t i = 1;
			for( ; TP.exists( i ); ++i )
			{
				if( TP.ahead( i ).match( "*" ) )
				{
					TP.forward();		// Skip the first "*"
					string s;
					for( --i; i > 0; TP.forward(), --i )
						s += TP.current().substr();
					TP.forward();		// Skip the final "*"
					return s;
				}
				else if( TP.ahead( i ).newline() )
				{
					if( !TP.ahead( i + 1 ).space()
						|| TP.ahead( i + 1 ).substr().numChars() < 2 )
						break;
				}
				else if( TP.ahead( i ).match( "\\" )
					&& !TP.ahead( i + 1 ).newline() )
					++i;
			}

			return string();
		}
		string Parser::parseLink()
		{
			size_t i = 2;
			for( ; TP.exists( i ); ++i )
			{
				if( TP.ahead( i + 1 ).match( "]" ) )
				{
					TP.forward( 2 );	// Skip "@" and "["
					string s;
					for( --i; i > 0; TP.forward(), --i )
						s += TP.current().substr();
					TP.forward();		// Skip "]"
					return s.trim();
				}
				else if( TP.ahead( i ).newline() )
				{
					if( !TP.ahead( i + 1 ).space()
						|| TP.ahead( i + 1 ).substr().numChars() < 2 )
						break;
				}
				else if( TP.ahead( i ).match( "\\" )
					&& !TP.ahead( i + 1 ).newline() )
					++i;
			}
			return string();
		}

		void Parser::parseDefinition( document& doc, string&& anchor )
		{
			TP.forward();		// Skip "  "
			string what;
			for( ; !TP.current().match( ":" ); TP.forward() )
				what += TP.current().substr();
			TP.forward();		// Skip ":"
			auto def = new definition( std::move( what ) );
			elementptr elm( def );
			skipSpaces();
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					TP.last().source().pos().line() + 1 ) + ":" + string(
						TP.last().source().pos().pos(
							TP.last().source().source() ) + 1 )
					+ "\nDefinition is element is incomplete!" );
			if( !TP.current().newline() )
				parseParagraph( doc, *def, ParaType::def, "" );
			else
			{
				auto pos = TP.current().source().pos();
				TP.forward();
				if( !TP || !TP.current().space()
					|| TP.current().substr().numChars() < 4 )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nExpected definition to continue after four spaces on the second line!" );
				skipSpaces();
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						pos.line() + 1 ) + ":" + string( pos.pos(
							TP.last().source().source() ) + 1 )
						+ "\nExpected definition to continue after four spaces on the second line!" );
				parseParagraph( doc, *def, ParaType::def, "" );
			}
			doc.add( elm, std::move( anchor ) );
		}
		bool Parser::isDefinitionStart()
		{
			// We have a definition start if there are two spaces, then 'words'
			// and finally a colon, without any newlines inbetween.
			if( !TP.current().match( "  " ) )
				return false;
			for( size_t i = 1; TP.exists( i ); ++i )
			{
				if( TP.ahead( i ).letter() || TP.ahead( i ).number()
					|| TP.ahead( i ).match( "_" ) )
					continue;
				else if( TP.ahead( i ).space() )
					continue;
				else if( TP.ahead( i ).match( ":" ) )
					return true;
				else
					return false;
			}
			return false;
		}


		string Parser::parseAnchor()
		{
			auto pos = TP.current().source().pos();
			TP.forward();	// Skip "@"
			if( !TP )
				throw EdocBadSyntax( Source.name() + ":" + string(
					pos.line() + 1 ) + ":" + string( pos.pos(
						TP.last().source().source() ) + 1 )
					+ "\nAnchor declaration started but not finished!" );
			string anchor;
			while( true )
			{
				if( !TP )
					throw EdocBadSyntax( Source.name() + ":" + string(
						TP.last().source().pos().line() + 1 ) + ":" + string(
							TP.last().source().pos().pos(
								TP.last().source().source() ) + 1 )
						+ "\nAnchor declaration missing terminating colon!" );
				if( TP.current().match( ":" ) )
					break;
				if( TP.current().newline() )
					throw EdocBadSyntax( Source.name() + ":" + string(
						TP.current().source().pos().line() + 1 ) + ":"
						+ string( TP.current().source().pos().pos(
							TP.current().source().source() ) + 1 )
						+ "\nAnchors cannot contain newline!" );
				anchor += TP.current().substr();
				TP.forward();
			}
			TP.forward();
			return makeAnchor( anchor );
		}
		string Parser::makeAnchor( const string& text ) const
		{
			auto& chars = Characters::get();
			string anchor;
			for( auto c : text )
			{
				if( chars.isLetter( c ) || chars.isNumberDecimalDigit( c ) || c == '_' )
					anchor += c;
				else
					anchor += '_';
			}
			return anchor;
		}

		bool Parser::skipSpaces()
		{
			if( !TP || !TP.current().space() )
				return false;
			TP.forward();
			return true;
		}
		bool Parser::skipEmtpyLines()
		{
			bool skipped = false;
			while( TP )
			{
				if( skipSpaces() )
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
	}
}
