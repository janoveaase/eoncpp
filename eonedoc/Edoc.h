#pragma once
#include <eontypesystem/TupleFactory.h>
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
	EON_NAME( h1 );
	EON_NAME( h2 );
	EON_NAME( h3 );
	EON_NAME( h4 );
	EON_NAME( text );
	EON_NAME( bullet );
	EON_NAME( dash );
	EON_NAME( todo );
	EON_NAME( tip );
	EON_NAME( phrase );
	EON_NAME( anonymous );
	EON_NAME( insert );
	EON_NAME( lang );
	EON_NAME( example );
	EON_NAME( toc );
	EON_NAME( level );
	EON_NAME( image );
	EON_NAME( align );
	EON_NAME( hidden );
	EON_NAME( paragraph );
	EON_NAME( http );
	EON_NAME( quoted );
	EON_NAME( emphasized );
	EON_NAME( caption );
	EON_NAME( no_indexing );
	EON_NAME( enum );
	EON_NAME( title );
	EON_NAME( value );
	EON_NAME( definition );
	EON_NAME( source );
	EON_NAME( target );
	EON_NAME( reference );

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
		inline Tuple parse( const file& input_edoc ) {
			Source = input_edoc.fpath().str(); return parse( input_edoc.loadText() ); }

		// Parse a string
		Tuple parse( const eon::string& input_edoc );




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
		bool _parseInsertCode( substring& elements, Tuple& dt );
		bool _parseInsertExample( substring& elements, Tuple& dt );
		bool _parseInsertQuote( substring& elements, Tuple& dt );
		bool _parseInsertToc( substring& elements, Tuple& dt );
		bool _parseInsertImage( substring& elements, Tuple& dt );
		bool _parseInsertHidden( substring& elements, Tuple& dt );
		bool _parseInsertContents( Tuple& dt, string::iterator end_of_elements );

		void _parseText( int indentation = 0 );
		void _patchHttp( std::list<std::pair<string, name_t>>& elements );
		void _patchReference( std::list<std::pair<string, name_t>>& elements );
		void _patchQuote( std::list<std::pair<string, name_t>>& elements );
		void _patchEmphasize( std::list<std::pair<string, name_t>>& elements );

		void _processText( string&& text, Tuple& dt, bool first );
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
		Tuple Para;
		State ParaState{ State::clear };
		Tuple Output;
	};
}
