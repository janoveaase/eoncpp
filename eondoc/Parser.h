#pragma once

#include <eonsource/SourceRef.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/TokenParser.h>
#include "Document.h"
#include "Insert.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown if an EDOC document has a syntax error
	EONEXCEPT( EdocBadSyntax );




	/**************************************************************************
	  The 'eon::doc' namespace is for specific Æon Document Format elements
	**************************************************************************/
	namespace doc
	{
		class Parser
		{
		public:

			Parser() = delete;
			Parser( const Parser& ) = delete;
			Parser( Parser&& ) = delete;
			Parser( const sourceref& source );



			
			// Parse a new document
			document parseDocument();


		private:

			enum class ParaType
			{
				plain,
				list,
				def
			};

			void parseTitleOrHeading( document& doc );
			void parseInsert( document& doc );
			void insertExample( document& doc, string&& heading,
				string&& anchor, std::vector<string>&& lines );
			void insertCode( document& doc, string&& lang, string&& heading,
				string&& anchor, std::vector<string>&& lines );
			void insertQuote( document& doc, string&& source, string&& heading,
				string&& anchor, std::vector<string>&& lines );
			void insertInsert( document& doc, const sourcepos& pos,
				string&& type, string&& heading, string&& anchor,
				std::vector<string>&& lines );
			void parseList( document& doc, string&& anchor );
			void parseNumberedList( document& doc, string&& anchor );
			void parseExclamation( document& doc );
			void parseTable( document& doc, string&& anchor );
			void parseParagraph( document& doc, block& parent, ParaType type,
				string&& anchor );
			string parseEmphasize();
			string parseLink();
			void parseDefinition( document& doc, string&& anchor );
			bool isDefinitionStart();

			string parseAnchor();
			string makeAnchor( const string& text ) const;

			bool skipSpaces();
			bool skipEmtpyLines();




		private:
			sourceref Source;
			tokenparser TP;
		};
	}
}
