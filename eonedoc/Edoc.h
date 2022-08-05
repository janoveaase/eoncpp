#pragma once
#include <eontypes/DataTuple.h>
#include <eonfilesys/Path.h>
#include <eonfilesys/File.h>
#include <eonregex/RegEx.h>
#include <eonexcept/Exception.h>


///////////////////////////////////////////////////////////////////////////////
//
//  The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// Name definitions for EDOC
	static const name_t name_h1{ name( "h1" ) };
	static const name_t name_h2{ name( "h2" ) };
	static const name_t name_h3{ name( "h3" ) };
	static const name_t name_h4{ name( "h4" ) };
	static const name_t name_text{ name( "text" ) };
	static const name_t name_bullet{ name( "bullet" ) };
	static const name_t name_dash{ name( "dash" ) };
	static const name_t name_todo{ name( "todo" ) };
	static const name_t name_tip{ name( "tip" ) };
	static const name_t name_phrase{ name( "phrase" ) };
	static const name_t name_anonymous{ name( "anonymous" ) };
	static const name_t name_insert{ name( "insert" ) };
	static const name_t name_lang{ name( "lang" ) };
	static const name_t name_example{ name( "example" ) };
	static const name_t name_toc{ name( "toc" ) };
	static const name_t name_level{ name( "level" ) };
	static const name_t name_image{ name( "image" ) };
	static const name_t name_align{ name( "align" ) };
	static const name_t name_hidden{ name( "hidden" ) };
	static const name_t name_paragraph{ name( "paragraph" ) };
	static const name_t name_http{ name( "http" ) };
	static const name_t name_quoted{ name( "quoted" ) };
	static const name_t name_emphasized{ name( "emphasized" ) };
	static const name_t name_caption{ name( "caption" ) };
	static const name_t name_no_indexing{ name( "no_indexing" ) };

	// Exception raised when parsing fails
	EONEXCEPT( ParseError );




	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Documentation Class - eon::edoc
	//
	// Parse edoc files and generate a documentation tree in the form of an
	// data tuple.
	//
	class edoc
	{
		//////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		edoc() = default;
		virtual ~edoc() = default;




		//////////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		// Parse a file
		inline DataTuple parse( const file& input_edoc ) {
			Source = input_edoc.fpath().str(); return parse( input_edoc.loadText() ); }

		// Parse a string
		DataTuple parse( const eon::string& input_edoc );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _parse();

		void _parseTitle();
		void _parseHeader();

		inline void _parseBulletList() { _parseList( name_bullet ); }
		inline void _parseDashList() { _parseList( name_dash ); }
		inline void _parseEnumList() { _parseList( name_enum ); }
		void _parseList( name_t type );

		void _parseExclamation( name_t type );

		void _parseDefinition();

		void _parseInsert();
		substring _insertElements();
		bool _parseInsertElements( substring& elements );
		bool _parseInsertCode( substring& elements, DataTuple& dt );
		bool _parseInsertExample( substring& elements, DataTuple& dt );
		bool _parseInsertQuote( substring& elements, DataTuple& dt );
		bool _parseInsertToc( substring& elements, DataTuple& dt );
		bool _parseInsertImage( substring& elements, DataTuple& dt );
		bool _parseInsertHidden( substring& elements, DataTuple& dt );
		bool _parseInsertContents( DataTuple& dt, string::iterator end_of_elements );

		void _parseText( int indentation = 0 );
		void _patchHttp( std::list<std::pair<string, name_t>>& elements );
		void _patchReference( std::list<std::pair<string, name_t>>& elements );
		void _patchQuote( std::list<std::pair<string, name_t>>& elements );
		void _patchEmphasize( std::list<std::pair<string, name_t>>& elements );

		void _processText( string&& text, DataTuple& dt, bool first );
		void _endPara();




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		enum class State
		{
			clear,
			started
		};

		string Source;
		std::vector<string> Input;
		std::vector<string>::iterator CurLine;
		DataTuple Para;
		State ParaState{ State::clear };
		DataTuple Output;
	};
}
