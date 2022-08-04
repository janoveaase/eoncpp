#include "Edoc.h"
#include <eonparser/BoolLiteralRule.h>
#include <eonparser/DefineRules.h>
#include <eonparser/NumericalRules.h>
#include <eonparser/NamePathRule.h>
#include <eontypes/Primitives.h>
#include <eontypes/StandardTypes.h>
#include <eontypes/TypeTupleObj.h>


namespace eon
{
	DataTuple edoc::parse( const eon::string& input_edoc )
	{
		Input = input_edoc.splitSequential<std::vector<string>>( NewlineChr );
		for( CurLine = Input.begin(); CurLine != Input.end(); )
			_parse();
		_endPara();
		return Output;
	}



	void edoc::_parse()
	{
		static regex enumListPattern{ R"(  (\d+)|# )" };
		static regex definitionPattern{ R"(  \!?@<quot>(\")?\S(\w| )*@:<quot>: |$)" };

		if( CurLine->empty() )
		{
			_endPara();
			++CurLine;
		}
		else
		{
			if( CurLine->startsWith( ">>" ) )
				_parseHeader();
			else if( CurLine->startsWith( "  * " ) )
				_parseBulletList();
			else if( CurLine->startsWith( "  - " ) )
				_parseDashList();
			else if( enumListPattern.match( *CurLine ) )
				_parseEnumList();
			else if( CurLine->startsWith( "NOTE:" ) )
				_parseExclamation( name_note );
			else if( CurLine->startsWith( "WARNING:" ) )
				_parseExclamation( name_warning );
			else if( CurLine->startsWith( "TODO:" ) )
				_parseExclamation( name_todo );
			else if( CurLine->startsWith( "TIP:" ) )
				_parseExclamation( name_tip );
			else if( definitionPattern.match( *CurLine ) )
				_parseDefinition();
			else if( CurLine->startsWith( "  --" ) )
				_parseInsert();
			else
				_parseText();
		}
	}

	void edoc::_parseHeader()
	{
		_endPara();
		static std::vector<regex> patterns{
			regex{ R"(^>> !>@<text>(\S.+))" },
			regex{ R"(^>> > @<text>(\S.+))" },
			regex{ R"(^>> >> @<text>(\S.+))" },
			regex{ R"(^>> >>> @<text>(\S.+))" } };

		for( size_t i = patterns.size(); i > 0; --i )
		{
			auto match = patterns[ i - 1 ].match( *CurLine );
			if( match )
			{
				DataTuple dt;
				string title = match.group( name_text );
				bool no_indexing = title.startsWith( '!' );
				if( no_indexing )
					title = title.substr( title.begin() + 1 );
				dt.addName( name_type, name( "h" + string( i ) ) );
				if( no_indexing )
					dt.addName( name_no_indexing );
				dt.addRaw( name_value, title );
				Output.addRaw( std::move( dt ) );
				break;
			}
		}
		++CurLine;
	}

	void edoc::_parseList( name_t type )
	{
		_endPara();
		string _lead{ "  " };
		if( type == name_bullet )
			_lead << "*";
		else if( type == name_dash )
			_lead << "\\-";
		else
			_lead << R"((\d+)|#)";
		regex lead{ _lead + " " };
		DataTuple dt;
		dt.addName( name_type, name_list );
		dt.addName( name_list, type );
		DataTuple value;
		do
		{
			DataTuple elm;
			elm.addName( name_type, name_text );
			_processText( string( CurLine->substr( CurLine->begin() + 4 ).trim() ), elm, true );
			value.addRaw( std::move( elm ) );
		} while( ++CurLine != Input.end() && lead.match( *CurLine ) );
		dt.addRaw( name_value, std::move( value ) );
		Output.addRaw( std::move( dt ) );
	}

	void edoc::_parseExclamation( name_t type )
	{
		_endPara();
		DataTuple dt;
		dt.addName( name_type, type );
		string value = CurLine->substr( CurLine->findFirst( ColonChr ).begin() + 2 ).trim();
		while( ++CurLine != Input.end() && CurLine->startsWith( "  " ) )
		{
			if( !value.empty() )
				value += " ";
			value += CurLine->trim();
		}
		dt.addRaw( name_value, value );
		Output.addRaw( std::move( dt ) );
	}

	void edoc::_parseDefinition()
	{
		_endPara();
		DataTuple dt;
		dt.addName( name_type, name_definition );
		auto beg = CurLine->begin() + 2;
		bool anonymous{ false };
		if( *beg == '!' )
		{
			anonymous = true;
			++beg;
		}
		string phrase = CurLine->substr( beg, CurLine->findFirst( ColonChr ).begin() ).trim();
		if( phrase.isDoubleQuoted() )
		{
			anonymous = true;
			phrase = phrase.unQuote();
		}
		if( anonymous )
			dt.addName( name_anonymous );
		dt.addRaw( name_phrase, phrase );
		string value = CurLine->substr( CurLine->findFirst( ColonChr ).begin() + 2 ).trim();
		while( ++CurLine != Input.end() && CurLine->startsWith( "    " ) )
		{
			if( !value.empty() )
				value += " ";
			value += CurLine->trim();
		}
		dt.addRaw( name_value, value );
		Output.addRaw( std::move( dt ) );
	}

	void edoc::_parseInsert()
	{
		_endPara();
		auto elements = _insertElements();
		if( !elements || !_parseInsertElements( elements ) )
			++CurLine;
	}
	substring edoc::_insertElements()
	{
		auto beg = CurLine->begin() + 4;
		auto end = CurLine->findFirst( "-->", CurLine->substr( beg ) );
		if( end )
			return CurLine->substr( beg, end.begin() );
		return substring();
	}
	bool edoc::_parseInsertElements( substring& elements )
	{
		DataTuple dt;

		dt.addName( name_type, name_insert );
		if( _parseInsertCode( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else if( _parseInsertExample( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else if( _parseInsertQuote( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else if( _parseInsertToc( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else if( _parseInsertImage( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else if( _parseInsertHidden( elements, dt ) )
			return _parseInsertContents( dt, elements.end() + 3 );
		else
			return false;
	}
	bool edoc::_parseInsertCode( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(code @<lang>(\S+)\s*,?\s+(@<title>(\S.*))?)" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_code );
			dt.addRaw( name_lang, string( match.group( name_lang ) ) );
			dt.addRaw( name_title, string( match.group( name_title ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertExample( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(example +@<title>(\S.*))" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_example );
			dt.addRaw( name_title, string( match.group( name_title ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertQuote( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(quote +@<source>(\S.*))" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_quote );
			dt.addRaw( name_source, string( match.group( name_source ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertToc( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(toc +(level=@<level>(\d) +)?@<title>(\S.*))" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_toc );
			if( match.group( name_level ) )
				dt.addRaw( name_level, static_cast<int_t>( match.group( name_level ).toInt() ) );
			dt.addRaw( name_title, string( match.group( name_title ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertImage( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(image +source=(@<source>(\S+)|(\"@<source>([^\"]+)\"))( +align=@<align>(\w+{name}))? +@<title>(\S.*))" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_image );
			dt.addRaw( name_source, string( match.group( name_source ) ) );
			if( match.group( name_align ) )
				dt.addName( name_align, name( string( match.group( name_align ) ) ) );
			dt.addRaw( name_title, string( match.group( name_title ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertHidden( substring& elements, DataTuple& dt )
	{
		static regex pattern{ R"(hidden +@<title>(\S.*))" };
		auto end_of_details = elements.findFirst( substring( "-->" ) );
		auto match = pattern.match( end_of_details ? substring( elements.begin(), end_of_details.begin() ) : elements );
		if( match )
		{
			dt.addName( name_insert, name_hidden );
			dt.addRaw( name_title, string( match.group( name_title ) ) );
			return true;
		}
		return false;
	}
	bool edoc::_parseInsertContents( DataTuple& dt, string::iterator end_of_elements )
	{
		auto next_line = CurLine->substr( end_of_elements ).trim();
		std::vector<string> lines{ "" };
		int cur_line = 0;
		while( true )
		{
			if( !next_line )
			{
				if( ++CurLine == Input.end() )
					return false;
				if( CurLine->empty() )
					continue;
				if( CurLine->startsWith( "  " ) )
					next_line = CurLine->substr( CurLine->begin() + 2 );
				else
					next_line = CurLine->substr();
			}
			auto end_of_insert = next_line.findFirst( substring( "<--" ) );
			if( end_of_insert )
			{
				lines[ cur_line ] += CurLine->substr( next_line.begin(), end_of_insert.begin() );
				++CurLine;
				break;
			}
			else
			{
				if( next_line.endsWith( BackSlashChr ) )
					lines[ cur_line ] += CurLine->substr( next_line.begin(), next_line.end() - 1 );
				else
				{
					lines[ cur_line ] += next_line;
					lines.push_back( "" );
					++cur_line;
					next_line.clear();
				}
			}
		}
		if( !lines[ cur_line ].empty() )
			++cur_line;
		if( cur_line == 1 )
			dt.addRaw( name_value, std::move( lines[ 0 ] ) );
		else if( cur_line > 1 )
		{
			DataTuple value;
			for( size_t i = 0; i < cur_line; ++i )
				value.addRaw( std::move( lines[ i ] ) );
			dt.addRaw( name_value, std::move( value ) );
		}
		if( !dt.empty() )
			Output.addRaw( std::move( dt ) );
		return true;
	}

	void edoc::_parseText( int indentation )
	{
		bool first = false;
		if( ParaState == State::clear )
		{
			Para = DataTuple();
			Para.addName( name_type, name_paragraph );
			Para.addRaw( name_value, DataTuple() );
			ParaState = State::started;
			first = true;
		}

		auto& values = Para.value<DataTuple>( name_value );
		DataTuple dt;
		dt.addName( name_type, name_text );

		string line;
		do
		{
			if( CurLine->endsWith( BackSlashChr ) )
				line += CurLine->slice( indentation, -2 );
			else
			{
				line += *CurLine;
				break;
			}
		} while( ++CurLine != Input.end() );

		_processText( std::move( line ), dt, first );
		values.addRaw( std::move( dt ) );
		++CurLine;
	}
	void edoc::_patchHttp( std::list<std::pair<string, name_t>>& elements )
	{
		static regex pattern( R"(\b!\!@<target>((http(s)?)|ftp://\S+)\B)", "ib" );
		for( auto line = elements.begin(); line != elements.end(); )
		{
			if( line->second != name_text )
			{
				++line;
				continue;
			}
			auto found = pattern.findAll( line->first );
			if( found.empty() )
			{
				line->first = line->first.replace( "!http", "http" ).replace( "!ftp", "ftp" );
				++line;
				continue;
			}

			auto pos = line;
			auto beg = line->first.begin();
			for( auto& elm : found )
			{
				if( elm.group( name_complete ).begin() != beg )
				{
					pos = elements.insert( pos, std::make_pair( string( line->first.substr( beg, elm.group(
						name_complete ).begin() ) ).replace( "!http", "http" ).replace( "!ftp", "ftp" ), name_text ) );
					++pos;
				}
				auto end = Characters::get().isPunctuation( *elm.group( name_target ).last() )
					? elm.group( name_target ).last() : elm.group( name_target ).end();
				pos = elements.insert( pos, std::make_pair( string( elm.group( name_target ).begin(), end ), name_http ) );
				++pos;
				beg = end;
			}
			if( beg != line->first.end() )
				elements.insert( pos, std::make_pair( string( line->first.substr( beg ) ), name_text ) );
			line = elements.erase( line );
		}
	}
	void edoc::_patchReference( std::list<std::pair<string, name_t>>& elements )
	{
		static regex pattern{ R"(([^!]\[)|(^\[)@<reference>(([^:\]]+:)?[^\]]+)\])" };
		for( auto line = elements.begin(); line != elements.end(); )
		{
			if( line->second != name_text )
			{
				++line;
				continue;
			}
			auto found = pattern.findAll( line->first );
			if( found.empty() )
			{
				line->first = line->first.replace( "![", "[" );
				++line;
				continue;
			}

			auto pos = line;
			auto beg = line->first.begin();
			for( auto& elm : found )
			{
				if( elm.group( name_complete ).begin() != beg )
				{
					pos = elements.insert( pos, std::make_pair( string( line->first.substr(
						beg, elm.group( name_reference ).begin() - 1 ) ).replace( "![", "[" ), name_text ) );
					++pos;
				}
				pos = elements.insert( pos, std::make_pair( string( elm.group( name_reference ) ), name_reference ) );
				++pos;
				beg = elm.group( name_complete ).end();
			}
			if( beg != line->first.end() )
				elements.insert( pos, std::make_pair( string( line->first.substr( beg ) ), name_text ) );
			line = elements.erase( line );
		}
	}
	void edoc::_patchQuote( std::list<std::pair<string, name_t>>& elements )
	{
		static regex pattern{ R"(\"@<quoted>([^\"]+)\")" };
		for( auto line = elements.begin(); line != elements.end(); )
		{
			if( line->second != name_text )
			{
				++line;
				continue;
			}
			auto found = pattern.findAll( line->first );
			if( found.empty() )
			{
				++line;
				continue;
			}

			auto pos = line;
			auto beg = line->first.begin();
			for( auto& elm : found )
			{
				if( elm.group( name_complete ).begin() != beg )
				{
					pos = elements.insert( pos, std::make_pair(
						string( line->first.substr( beg, elm.group( name_complete ).begin() ) ), name_text ) );
					++pos;
				}
				pos = elements.insert( pos, std::make_pair( string( elm.group( name_quoted ) ), name_quoted ) );
				++pos;
				beg = elm.group( name_complete ).end();
			}
			if( beg != line->first.end() )
				elements.insert( pos, std::make_pair( string( line->first.substr( beg ) ), name_text ) );
			line = elements.erase( line );
		}
	}
	void edoc::_patchEmphasize( std::list<std::pair<string, name_t>>& elements )
	{
		static regex pattern( R"(\b!\!\*!\s@<emphasized>(.+?)\*(\B|!\*\p))", "b" );
		for( auto line = elements.begin(); line != elements.end(); )
		{
			if( line->second != name_text )
			{
				++line;
				continue;
			}
			auto found = pattern.findAll( line->first );
			if( found.empty() )
			{
				line->first = line->first.replace( "![", "[" );
				++line;
				continue;
			}

			auto pos = line;
			auto beg = line->first.begin();
			for( auto& elm : found )
			{
				if( elm.group( name_complete ).begin() != beg )
				{
					pos = elements.insert( pos, std::make_pair(
						string( line->first.substr( beg, elm.group( name_complete ).begin() ) ).replace( "!*", "*" ),
						name_text ) );
					++pos;
				}
				pos = elements.insert( pos, std::make_pair( string( elm.group( name_emphasized ) ), name_emphasized ) );
				++pos;
				beg = elm.group( name_complete ).end();
			}
			if( beg != line->first.end() )
				elements.insert( pos, std::make_pair( string( line->first.substr( beg ) ), name_text ) );
			line = elements.erase( line );
		}
	}

	void edoc::_processText( string&& text, DataTuple& dt, bool first )
	{
		std::list<std::pair<string, name_t>> elements{ { std::move( text ), name_text } };
		_patchReference( elements );
		_patchHttp( elements );
		_patchQuote( elements );
		_patchEmphasize( elements );

		if( elements.size() == 1 && elements.begin()->second == name_text && first )
			dt.addRaw( name_value, std::move( elements.begin()->first ) );
		else
		{
			DataTuple sub_values;
			if( !first )
			{
				DataTuple value;
				value.addName( name_type, name_newline );
				sub_values.addRaw( std::move( value ) );
			}
			for( auto& elm : elements )
			{
				DataTuple value;
				if( elm.second == name_text || elm.second == name_quoted || elm.second == name_emphasized )
				{
					value.addName( name_type, elm.second );
					value.addRaw( name_value, std::move( elm.first ) );
				}
				else if( elm.second == name_http )
				{
					value.addName( name_type, name_reference );
					value.addRaw( name_target, std::move( elm.first ) );
				}
				else if( elm.second == name_reference )
				{
					value.addName( name_type, name_reference );
					auto colon = elm.first.findFirst( ColonChr );
					if( colon )
					{
						value.addRaw( name_caption,
							string( elm.first.substr( elm.first.begin(), colon.begin() ) ) );
						value.addRaw( name_target, string( elm.first.substr( colon.end() ) ) );
					}
					else
						value.addRaw( name_target, string( elm.first ) );
				}
				sub_values.addRaw( std::move( value ) );
			}
			dt.addRaw( name_value, std::move( sub_values ) );
		}
	}

	void edoc::_endPara()
	{
		if( ParaState == State::started )
			Output.addRaw( std::move( Para ) );
		ParaState = State::clear;
	}
}
