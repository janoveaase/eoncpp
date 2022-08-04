#pragma once
#include <eontypes/DataTuple.h>
#include "Edoc.h"


///////////////////////////////////////////////////////////////////////////////
//
//  The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon To HTML Class - eon::doc::ToHtml
	//
	// Convert Æon documentation converted into data tuple by eon::edoc into
	// HTML.
	//
	class ToHtml
	{
		//////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		ToHtml() = default;
		virtual ~ToHtml() = default;




		//////////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		// Convert data tuple into a UTF-8 string HTML format
		// NOTE: This is not a complete HTML document, just a 'div' containing
		//       the basic documentation.
		// The following style classes are used:
		//   eon_frame         : Top level 'div'
		//   eon_h1            : For 'h1'
		//   eon_h2            : For 'h2'
		//   eon_h3            : For 'h3'
		//   eon_h4            : For 'h4'
		//   eon_shout         : Framing 'div' for NOTE, WARNING, TODO, and TIP
		//   eon_note          : NOTE title 'div'
		//   eon_warning       : WARNING title 'div'
		//   eon_todo          : TODO title 'div'
		//   eon_tip           : TIP title 'div'
		//   eon_note_text     : NOTE text 'div'
		//   eon_warning_text  : WARNING text 'div'
		//   eon_todo_text     : TODO text 'div'
		//   eon_tip_text      : TIP text 'div'
		//   eon_define        : Framing 'div' for definitions
		//   eon_phrase        : Phrase to be defined 'div'
		//   eon_definition    : Definition of phrase 'div'
		//   eon_insert_code   : Framing 'div' for inserted code elements
		//   eon_insert_example: Framing 'div' for inserted example elements
		//   eon_insert_quote  : Framing 'div' for inserted quote elements
		//   eon_insert_toc    : Framing 'div' for inserted toc elements
		//   eon_insert_image  : Framing 'div' for inserted image elements
		//   eon_insert_header : Insert header 'div'
		//   eon_code_lang     : Code language 'span'
		//   eon_quote_source  : Quote source 'span'
		//   eon_insert_title  : Insert title 'span'
		//   eon_code          : Code 'div'
		//   eon_example       : Example 'div'
		//   eon_quote         : Quote 'div'
		//   eon_toc           : Table of contents 'div'
		//   eon_image         : Image 'div'
		//   eon_insert_subtext: Sub-text 'div' for insertet elements
		//   eon_paragraph     : New main paragraph 'p'
		//   eon_emphasized    : Emphasized text 'span'
		//   eon_quoted        : Quoted text 'span'
		//   eon_reference     : Reference 'a'
		//   eon_toc1          : A toc element/line 'div' at indentation level 1
		//   eon_toc2          : A toc element/line 'div' at indentation level 2
		//   eon_toc3          : A toc element/line 'div' at indentation level 3
		//   eon_toc4          : A toc element/line 'div' at indentation level 4
		string convert( const DataTuple& edoc );

		// Get Æon default style sheet (CSS)
		string css();

		// Generate a complete HTML document
		// Parameters:
		//   edoc  : DataTuple containing EDOC documentation.
		//   title : Document title.
		//   css   : Name of style sheet to use (path should be relative to
		//           where the HTML document is to be saved). If empty, the Æon
		//           default style sheet will be inserted into the header of
		//           the document.
		string makeDocument( const DataTuple& edoc, string title, const string& doc_css = "" );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _convert( const DataTuple& dt, bool is_element, string& html );

		inline void _endl( string& html ) { if( !html.empty() && !html.endsWith( NewlineChr ) ) html << "\n"; }

		inline void _convertH1( const DataTuple& dt, string& html ) { _convertH( dt, 1, html ); }
		inline void _convertH2( const DataTuple& dt, string& html ) { _convertH( dt, 2, html ); }
		inline void _convertH3( const DataTuple& dt, string& html ) { _convertH( dt, 3, html ); }
		inline void _convertH4( const DataTuple& dt, string& html ) { _convertH( dt, 4, html ); }
		void _convertH( const DataTuple& dt, int level, string& html );

		void _convertList( const DataTuple& dt, string& html );

		void _convertText( const DataTuple& dt, string& html );
		void _convertParagraph( const DataTuple& dt, string& html );
		void _convertTextElements( const DataTuple& dt, string& html );
		void _convertPlainText( string text, string& html );
		void _convertEmphasizedText( const string& text, string& html );
		void _convertQuotedText( const string& text, string& html );
		void _convertReference( const DataTuple& dt, string& html );

		void _convertNote( const DataTuple& dt, string& html );
		void _convertWarning( const DataTuple& dt, string& html );
		void _convertTodo( const DataTuple& dt, string& html );
		void _convertTip( const DataTuple& dt, string& html );
		void _convertShout( const DataTuple& dt, string shout, string& html );

		void _convertDefinition( const DataTuple& dt, string& html );

		void _convertInsert( const DataTuple& dt, string& html );
		void _convertCode( const DataTuple& dt, string& html );
		void _convertExample( const DataTuple& dt, string& html );
		void _convertQuote( const DataTuple& dt, string& html );
		void _convertToc( const DataTuple& dt, string& html );
		void _convertImage( const DataTuple& dt, string& html );
		void _convertInsertHeader( const DataTuple& dt, std::list<std::pair<name_t, string>> details, string& html );
		void _convertInsertBody( const DataTuple& dt, string div_class, string& html );

		void _insertToc( index_t num_char, index_t num_byte, int level, string& html );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		struct Header
		{
			Header() = default;
			inline Header( string title, int level ) { Title = title; Level = level; }
			string Title;
			int Level{ 1 };
		};
		std::list<Header> Headers;

		struct Definition
		{
			Definition() = default;
			inline Definition( string phrase, string definition ) { Phrase = phrase; Def= definition; }
			string Phrase;
			string Def;
		};
		std::list<Definition> Definitions;

		struct TocInsert
		{
			TocInsert() = default;
			inline TocInsert( string::iterator pos, int level = 4 ) {
				NumChar = pos.numChar(); NumByte = pos.numByte(); Level = level; }
			index_t NumChar{ 0 };
			index_t NumByte{ 0 };
			int Level{ 4 };
		};
		std::list<TocInsert> TocInserts;
	};
}
