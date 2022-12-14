#include "ToHtml.h"
#include <eontypes/String.h>

namespace eon
{
	string ToHtml::convert( const DataTuple& edoc )
	{
		string html = "<div class=\"eon_frame\">\n";
		for( auto& elm : edoc )
		{
			if( elm.type() == name_data )
				_convert( *(const DataTuple*)elm.value(), false, html );
		}
		for( auto toc = TocInserts.rbegin(); toc != TocInserts.rend(); ++toc )
			_insertToc( toc->NumChar, toc->NumByte, toc->Level, html );
		_endl( html );
		html << "</div>\n";
		return html;
	}

	string ToHtml::css()
	{
		static string str{
			".eon_frame {max-width: 125ch;}\n"

			".eon_title {margin: 0pt 0pt 8pt 0pt; font: bold 175% Georgia, serif;}\n"
			".eon_h1 {margin: 12pt 0pt 4pt 0pt;}\n"
			".eon_h2 {margin: 12pt 0pt 4pt 0pt;}\n"
			".eon_h3 {margin: 12pt 0pt 4pt 0pt;}\n"
			".eon_h4 {margin: 12pt 0pt 4pt 0pt;}\n"
			".eon_paragraph {margin: 0ch 0ch 1ch 0ch; padding: 0pt 0pt 0pt 0pt;}\n"

			".eon_shout {margin: 8pt 0pt 8pt 0pt; overflow: auto;}\n"
			".eon_note {font: 150% bold sans-serif; background-color: Gold; padding: 0pt 4pt 0pt 4pt; margin-right: 0.5ch; float: left;}\n"
			".eon_note_text {font-family: sans-serif; margin-top: 0.5ch; color: #585528;}\n"
			".eon_warning {font: 150% bold sans-serif; color: white; background-color: FireBrick; padding: 0pt 4pt 0pt 4pt; margin-right: 0.5ch; float: left;}\n"
			".eon_warning_text {font-family: sans-serif; margin-top: 0.5ch; color: DarkRed;}\n"
			".eon_todo {font: 150% bold sans-serif; background-color: LightSteelBlue; padding: 0pt 4pt 0pt 4pt; margin-right: 0.5ch; float: left;}\n"
			".eon_todo_text {font-family: sans-serif; margin-top: 0.5ch; color: MidnightBlue;}\n"
			".eon_tip {font: 150% bold sans-serif; background-color: YellowGreen; padding: 0pt 4pt 0pt 4pt; margin-right: 0.5ch; float: left;}\n"
			".eon_tip_text {font-family: sans-serif; margin-top: 0.5ch; color: DarkGreen;}\n"

			".eon_define {margin: 1ch 0ch 0ch 3ch;}\n"
			".eon_phrase {}\n"
			".eon_phrase_normal {font-style: italic; color: navy;}\n"
			".eon_phrase_anonymous {font-family: Monaco, Monospace; color: Indigo;}\n"
			".eon_phrase_quoted{font-family: Monaco, Monospace; color: Indigo; background-color: #ffede6;}\n"
			".eon_definition {font-family: Helvetica, sans-serif; margin: 0ch 0ch 1ch 3ch;}\n"
			".eon_def_popup {text-decoration-line: underline; text-decoration-style: dotted;}\n"
			".eon_def_popup:hover {cursor: help;}\n"

			".eon_insert_frame {margin: 1ch 1ch 1ch 2ch;}\n"
			".eon_insert_header_line {font: 125% bold; background-color: #FFECEC;}\n"
			".eon_insert_lead {margin-left: 1ch; font-size: 70%; float: left; color: brown;}\n"
			".eon_insert_title {padding: 0ch 1ch 0ch 1ch; text-transform: capitalize;}\n"
			".eon_insert_subtext {margin: 0ch; font-size: 85%; padding: 0.5ch; background-color: WhiteSmoke;}\n"

			".eon_code_lang {color: #8B0000; font: 125% bold;}\n"
			".eon_code {font-family: Monospace; border: 1pt solid #FFECEC; margin: 0pt; padding: 1ch; color: #004310; background-color: #F9FEF7;}\n"
			
			".eon_example {border: 1pt solid #FFECEC; margin: 0pt; padding: 1ch; background-color: #FFFAFA;}\n"

			".eon_quote {border: 1pt solid #FFECEC; margin: 0pt; padding: 1ch; font-style: italic; background-color: #F9FBFD;}\n"

			".eon_toc {border: 1pt solid #FFECEC; margin: 0pt; padding: 1ch; Background-color: #FAFAFE; font-family: sans-serif;}\n"
			".eon_toc1 {margin-left: 0ch; font-size: 120%;}\n"
			".eon_toc2 {margin-left: 2ch; font-size: 110%;}\n"
			".eon_toc3 {margin-left: 4ch; font-size: 100%;}\n"
			".eon_toc4 {margin-left: 6ch; font-size: 90%;}\n"

			".eon_image {border: 1pt solid #FFECEC; margin: 0pt; padding: 1ch;}\n"

			".eon_emphasized {color: DarkViolet; text-shadow: 1px 1px Lavender;}\n"
			".eon_quoted {color: SaddleBrown; font-family: Monaco, monospace;}\n"
			".eon_reference {}\n"
		};
		return str;
	}

	string ToHtml::makeDocument( const DataTuple& edoc, string title, const string& doc_css )
	{
		string str;
		str << "<!DOCTYPE html>\n";
		str << "<html>\n";
		str << "<head>\n";
		str << "  <meta charset=\"UTF-8\">\n";
		if( !title.empty() )
			str << "  <title>" << title << "</title>\n";
		str << "  <link rel=\"icon\" type=\"image/png\" href=\"https://aaseweb.no/eon_logo_64.png\">\n";
		if( doc_css.empty() )
		{
			str << "  <style>\n";
			str << "    " << css().replace( "\n", "\n    " ).trimTrailing();
			str << "\n  </style>\n";
		}
		else
		{
			if( doc_css.lower().endsWith( ".css" ) )
				str << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"" << doc_css << "\">\n";
			else
			{
				str << "  <style>\n";
				str << doc_css.replace( "\n", "\n    " );
				str << "  </style>\n";
			}
		}
		str << "</head>\n";
		str << "<body>\n";
		str << convert( edoc );
		str << "</body>";
		str << "</html>\n";
		return str;
	}




	void ToHtml::_convert( const DataTuple& dt, bool is_element, string& html )
	{
		using std::placeholders::_1;
		using std::placeholders::_2;
		std::function<void (const DataTuple&, string&)> func = std::bind( &ToHtml::_convertH1, this, _1, _2 );
		std::map<name_t, std::function<void (const DataTuple&, string& html)>> converters{
			{ name_title, std::bind( &ToHtml::_convertTitle, this, _1, _2 ) },
			{ name_h1, std::bind( &ToHtml::_convertH1, this, _1, _2 ) },
			{ name_h2, std::bind( &ToHtml::_convertH2, this, _1, _2 ) },
			{ name_h3, std::bind( &ToHtml::_convertH3, this, _1, _2 ) },
			{ name_h4, std::bind( &ToHtml::_convertH4, this, _1, _2 ) },
			{ name_list, std::bind( &ToHtml::_convertList, this, _1, _2 ) },
			{ name_note, std::bind( &ToHtml::_convertNote, this, _1, _2 ) },
			{ name_warning, std::bind( &ToHtml::_convertWarning, this, _1, _2 ) },
			{ name_todo, std::bind( &ToHtml::_convertTodo, this, _1, _2 ) },
			{ name_tip, std::bind( &ToHtml::_convertTip, this, _1, _2 ) },
			{ name_paragraph, std::bind( &ToHtml::_convertParagraph, this, _1, _2 ) },
			{ name_text, std::bind( &ToHtml::_convertText, this, _1, _2 ) },
			{ name_definition, std::bind( &ToHtml::_convertDefinition, this, _1, _2 ) },
			{ name_insert, std::bind( &ToHtml::_convertInsert, this, _1, _2 ) }
		};
		if( !dt.exists( name_type ) )
			return;
		name_t type = dt.value<name_t>( name_type );
		auto converter = converters.find( type );
		if( converter != converters.end() )
		{
			if( !is_element && !html.empty() && !html.endsWith( NewlineChr ) )
				html << "\n";
			converter->second( dt, html );
		}
	}

	string ToHtml::_encode( const string& str )
	{
		static std::unordered_map<char_t, string> encodings{
			{ '<', "&lt;" },
			{ '>', "&gt;" }
		};
		string html;
		for( auto c : str )
		{
			auto found = encodings.find( c );
			if( found != encodings.end() )
				html << found->second;
			else
				html << c;
		}
		return html;
	}

	void ToHtml::_convertTitle( const DataTuple& dt, string& html )
	{
		if( dt.exists( name_value ) )
		{
			if( dt.at( name_value ).type() == name_string )
			{
				html << "<div class=\"eon_title\" id=\"";
				html << _encode( dt.value<string>( name_value ).replace( " ", "_" ) );
				html << "\">" << _encode( dt.value<string>( name_value ) );
				html << "</div>";
				if( !dt.hasFlag( name_no_indexing ) )
					Headers.push_back( Header( dt.value<string>( name_value ), 1 ) );
			}
		}
	}
	void ToHtml::_convertH( const DataTuple& dt, int level, string& html )
	{
		if( dt.exists( name_value ) )
		{
			if( dt.at( name_value ).type() == name_string )
			{
				html << "<h" << level << " class=\"eon_h" << level << "\" id=\"";
				html << _encode( dt.value<string>( name_value ).replace( " ", "_" ) );
				html << "\">" << _encode( dt.value<string>( name_value ) );
				html << "</h" << level << ">";
				if( !dt.hasFlag( name_no_indexing ) )
					Headers.push_back( Header( dt.value<string>( name_value ), level ) );
			}
		}
	}

	void ToHtml::_convertList( const DataTuple& dt, string& html )
	{
		if( !dt.exists( name_list ) || !dt.exists( name_value ) || dt.at( name_value).type() != name_data )
			return;
		name_t type = dt.value<name_t>( name_list );
		_endl( html );
		if( type == name_bullet )
			html << "<ul style=\"list-style-type:disc;\">\n";
		else if( type == name_dash )
			html << "<ul style=\"list-style-type:square;\">\n";
		else
			html << "<ol>\n";
		auto& value = dt.value<DataTuple>( name_value );
		for( auto& value : value )
		{
			if( value.type() != name_data )
				continue;
			html << "  <li>";
			_convert( *(DataTuple*)value.value(), true, html );
			html << "</li>\n";
		}
		if( type == name_enum )
			html << "</ol>";
		else
			html << "</ul>";
	}

	void ToHtml::_convertText( const DataTuple& dt, string& html )
	{
		if( !dt.exists( name_value ) )
			return;
		if( dt.at( name_value ).type() == name_string )
			_convertPlainText( dt.value<string>( name_value ), html );
		else if( dt.at( name_value ).type() == name_data )
			_convertTextElements( dt.value<DataTuple>( name_value ), html );
	}
	void ToHtml::_convertParagraph( const DataTuple& dt, string& html )
	{
		if( !dt.exists( name_value ) )
			return;
		html << "<p class=\"eon_paragraph\">\n  ";
		if( dt.at( name_value ).type() == name_string )
			_convertPlainText( dt.value<string>( name_value ), html );
		else if( dt.at( name_value ).type() == name_data )
			_convertTextElements( dt.value<DataTuple>( name_value ), html );
		_endl( html );
		html << "</p>";
	}
	void ToHtml::_convertTextElements( const DataTuple& dt, string& html, bool insert_definitions )
	{
		for( auto& elm : dt )
		{
			if( elm.type() == name_data )
			{
				auto& value = *(const DataTuple*)elm.value();
				if( value.exists( name_type ) && value.at( name_type ).type() == name_name && value.exists( name_value ) )
				{
					auto& value_type = value.at( name_value ).type();
					if( value_type == name_string )
					{
						name_t type = value.value<name_t>( name_type );
						if( type == name_text )
							_convertPlainText( value.value<string>( name_value ), html, insert_definitions );
						else if( type == name_emphasized )
							_convertEmphasizedText( value.value<string>( name_value ), html );
						else if( type == name_quoted )
							_convertQuotedText( value.value<string>( name_value ), html );
					}
					else if( value_type == name_data )
						_convertTextElements( value.value<DataTuple>( name_value ), html, insert_definitions );
				}
				else
				{
					name_t type = value.value<name_t>( name_type );
					if( type == name_reference
						&& value.exists( name_target ) && value.at( name_target ).type() == name_string )
						_convertReference( value, html );
					else if( type == name_newline )
						html << "<br />\n  ";
				}
			}
		}
	}
	void ToHtml::_convertPlainText( string text, string& html, bool insert_definitions )
	{
		text = _encode( text );
		if( insert_definitions )
		{
			auto split = _splitForDef( text );
			for( auto& elm : split )
			{
				if( elm.second )
				{
					string value = elm.first.lower();
					for( string::iterator c1 = elm.first.begin(), c2 = value.begin(); c2 != value.end(); )
					{
						for( auto& def : Definitions )
						{
							if( value.substr( c2, c2 + def.Phrase.numChars() ) == def.Phrase.substr() )
							{
								html << "<span class=\"eon_def_popup\" title=\"" << def.Def << "\">";
								for( index_t i = 0; i < def.Phrase.numChars(); ++i )
								{
									html << *c1;
									++c1;
									++c2;
								}
								html << "</span>";
								goto done;
							}
						}
						html << *c1;
						++c1;
						++c2;
					done:
						;
					}
				}
				else
					html << elm.first;
			}
		}
		else
			html << text;
		return;
	}
	void ToHtml::_convertEmphasizedText( const string& text, string& html )
	{
		html << "<span class=\"eon_emphasized\">";
		_convertPlainText( text, html, false );
		html << "</span>";
	}
	void ToHtml::_convertQuotedText( const string& text, string& html )
	{
		html << "<span class=\"eon_quoted\">\"";
		_convertPlainText( text, html, false );
		html << "\"</span>";
	}
	void ToHtml::_convertReference( const DataTuple& dt, string& html )
	{
		static regex http_pattern{ R"((http(s)?)|ftp://)" };
		string target = _encode( dt.value<string>( name_target ) );
		html << "<a href=\"";
		if( !http_pattern.match( target ) )
		{
			html << "#";
			target = target.replace( " ", "_" );
		}
		html << target << "\" class=\"eon_reference\">";
		if( dt.exists( name_caption ) && dt.at( name_caption ).type() == name_string )		
			_convertPlainText( dt.value<string>( name_caption ), html, false );
		else
			_convertPlainText( target, html, false );
		html << "</a>";
	}

	void ToHtml::_convertNote( const DataTuple& dt, string& html )
	{
		if( dt.exists( name_value ) )
			_convertShout( dt, "note", html );
	}
	void ToHtml::_convertWarning( const DataTuple& dt, string& html )
	{
		if( dt.exists( name_value ) )
			_convertShout( dt, "warning", html );
	}
	void ToHtml::_convertTodo( const DataTuple& dt, string& html )
	{
		if( dt.exists( name_value ) )
			_convertShout( dt, "todo", html );
	}
	void ToHtml::_convertTip( const DataTuple& dt, string& html )
	{
		if( dt.exists( name_value ) )
			_convertShout( dt, "tip", html );
	}
	void ToHtml::_convertShout( const DataTuple& dt, string shout, string& html )
	{
		html << "<div class=\"eon_shout\">\n";
		html << "  <div class=\"eon_" + shout + "\">" << shout.upper() << "!</div>\n";
		html << "  <div class=\"eon_" + shout + "_text\">";
		if( dt.at( name_value ).type() == name_string )
			_convertPlainText( dt.value<string>( name_value ), html );
		else if( dt.at( name_value ).type() == name_data )
			_convertTextElements( *(DataTuple*)dt.at( name_value ).value(), html );
		html << "</div>\n</div>";
	}

	void ToHtml::_convertDefinition( const DataTuple& dt, string& html )
	{
		if( !dt.exists( name_phrase ) || dt.at( name_phrase ).type() != name_string
			|| !dt.exists( name_value ) || (
				dt.at( name_value ).type() != name_string && dt.at( name_value ).type() != name_data ) )
			return;
		html << "<div class=\"eon_define\"";
		if( !dt.hasFlag( name_anonymous ) && !dt.hasFlag( name_quoted ) )
		{
			Definitions.push_back( Definition( dt.value<string>( name_phrase ), dt.value<string>( name_text ) ) );
			html << " id=\"" << dt.value<string>( name_phrase ).replace( " ", "_" ) << "\"";
		}
		html << ">\n";
		html << "  <div class=\"eon_phrase\"><span class=\"eon_phrase_";
		if( dt.hasFlag( name_anonymous ) )
			html << "anonymous";
		else if( dt.hasFlag( name_quoted ) )
			html << "quoted";
		else
			html << "normal";
		html << "\">" << _encode( dt.value<string>( name_phrase ) ) << "</span> :</div>\n";
		html << "  <div class=\"eon_definition\">";
		if( dt.at( name_value ).type() == name_string )
			_convertPlainText( dt.value<string>( name_value ), html, false );
		else if( dt.at( name_value ).type() == name_data )
			_convertTextElements( dt.value<DataTuple>( name_value ), html, false );
		html << "</div>\n";
		html << "</div>";
	}

	void ToHtml::_convertInsert( const DataTuple& dt, string& html )
	{
		if( !dt.exists( name_insert ) || dt.at( name_insert ).type() != name_name )
			return;
		name_t type = dt.value<name_t>( name_insert );
		if( type == name_hidden )
			return;
		html << "<div class=\"eon_insert_frame\">\n";
		if( type == name_code )
			_convertCode( dt, html );
		else if( type == name_example )
			_convertExample( dt, html );
		else if( type == name_quote )
			_convertQuote( dt, html );
		else if( type == name_toc )
			_convertToc( dt, html );
		else if( type == name_image )
			_convertImage( dt, html );
		html << "</div>\n";
	}
	void ToHtml::_convertCode( const DataTuple& dt, string& html )
	{
		html << "<div class=\"eon_insert_header_line\">";
		html << "<span class=\"eon_insert_lead\">Code:</span>";
		_convertInsertHeader( dt, { { name_lang, "eon_code_lang" }, { name_title, "eon_insert_title" } }, html );
		html << "</div>\n";
		_convertInsertBody( dt, "code", html );
	}
	void ToHtml::_convertExample( const DataTuple& dt, string& html )
	{
		html << "<div class=\"eon_insert_header_line\">";
		html << "<span class=\"eon_insert_lead\">Example!</span>";
		_convertInsertHeader( dt, { { name_title, "eon_insert_title" } }, html );
		html << "</div>\n";
		_convertInsertBody( dt, "example", html );
	}
	void ToHtml::_convertQuote( const DataTuple& dt, string& html )
	{
		html << "<div class=\"eon_insert_header_line\">";
		html << "<span class=\"eon_insert_lead\">Quote!</span>";
		_convertInsertHeader( dt, { { name_source, "eon_insert_title" } }, html );
		html << "</div>\n";
		_convertInsertBody( dt, "quote", html );
	}
	void ToHtml::_convertToc( const DataTuple& dt, string& html )
	{
		html << "<div class=\"eon_insert_header_line\">";
		_convertInsertHeader( dt, { { name_title, "eon_insert_title" } }, html );
		html << "</div>\n";
		html << "  <div class=\"eon_toc\"><";
		TocInserts.push_back( TocInsert( html.last(), dt.exists( name_level )
			? static_cast<int>( dt.value<int_t>( name_level ) ) : 4 ) );
		html << "/div>\n";
		_convertInsertBody( dt, "insert_subtext", html );
	}
	void ToHtml::_convertImage( const DataTuple& dt, string& html )
	{
		html << "<div class=\"eon_insert_header_line\">";
		_convertInsertHeader( dt, { { name_title, "eon_insert_title" } }, html );
		html << "</div>\n";
		if( dt.exists( name_source ) )
		{
			html << "  <div class=\"eon_image\"";
			if( dt.exists( name_align ) && dt.at( name_align ).type() == name_name )
				html << " style=\"float:" << str( dt.value<name_t>( name_align ) ) << ";\"";
			html << "><img src=\"" << dt.value<string>( name_source ) << "\" ";
			if( dt.exists( name_title ) && dt.at( name_title ).type() == name_string )
				html << "alt=\"" << _encode( dt.value<string>( name_title ) ) << "\" ";
			html << "/></div>\n";
		}
		_convertInsertBody( dt, "insert_subtext", html );
	}
	void ToHtml:: _convertInsertHeader( const DataTuple& dt, std::list<std::pair<name_t, string>> details, string& html )
	{
		bool have_any{ false };
		for( auto& detail : details )
		{
			have_any = dt.exists( detail.first ) && dt.at( detail.first ).type() == name_string;
			if( have_any )
				break;
		}
		if( !have_any )
			return;

		for( auto& detail : details )
		{
			if( dt.exists( detail.first ) && dt.at( detail.first ).type() == name_string )
				html << "<span class=\"" << detail.second << "\">" << _encode( dt.value<string>( detail.first ) )
				<< "</span>";
		}
	}
	void ToHtml::_convertInsertBody( const DataTuple& dt, string div_class, string& html )
	{
		if( dt.exists( name_value ) )
		{
			bool code = div_class == "code";
			if( !code )
				html << "  ";
			html << "<" << ( code ? "pre" : "div" ) << " class=\"eon_" << div_class << "\">";
			if( !code )
				html << "\n";
			if( dt.at( name_value ).type() == name_string )
			{
				if( code )
					html << "\n" << dt.value<string>( name_value );
				else
					html << "    " << _encode( dt.value<string>( name_value ) );
			}
			else if( dt.at( name_value ).type() == name_data )
			{
				auto& values = dt.value<DataTuple>( name_value );
				bool first = true;
				for( auto& val : values )
				{
					if( code )
						html << "\n" << ( (StringInstance*)val.value() )->value();
					else
					{
						if( first )
							first = false;
						else
							html << "<br />\n";
						if( val.type() == name_string )
							html << "    " << _encode( ( (StringInstance*)val.value() )->value() );
					}
				}
			}
			if( !code )
			{
				_endl( html );
				html << "  ";
			}
			html << "</" << ( code ? "pre" : "div" ) << ">\n";
		}
	}

	void ToHtml::_insertToc( index_t num_char, index_t num_byte, int level, string& html )
	{
		if( level < 1 || level > 4 )
			return;
		string toc;
		for( auto& header : Headers )
		{
			if( header.Level > level )
				continue;
			toc << "\n    <div class=\"eon_toc" << header.Level << "\"><a href=\"#";
			toc << header.Title.replace( " ", "_" ) << "\">" << header.Title << "</a></div>";
		}
		toc << "\n  ";
		string::iterator pos( html.begin(), html.c_str() + num_byte, num_char );
		html = html.substr( html.begin(), pos ) + toc + html.substr( pos );
	}

	std::vector<std::pair<string, bool>> ToHtml::_splitForDef( const string& text )
	{
		// We want to split out that which can contain defined words and phrases from that which cannot.
		// That means that anything that isn't word characters and space gets split out.
		std::vector<std::pair<string, bool>> split;
		if( text.empty() )
			return split;
		size_t pos = 0;
		for( auto c : text )
		{
			if( c == SpaceChr || string::isWordChar( c ) )
			{
				if( split.empty() || !split[ pos ].second )
				{
					if( !split.empty() )
						++pos;
					split.push_back( std::make_pair( "", true ) );
				}
			}
			else
			{
				if( split.empty() || split[ pos ].second )
				{
					if( !split.empty() )
						++pos;
					split.push_back( std::make_pair( "", false ) );
				}
			}
			split[ pos ].first << c;
		}
		return split;
	}
}
