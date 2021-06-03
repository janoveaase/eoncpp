#pragma once

#include <eonsource/SourceRef.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eontuple/Tuple.h>
#include <eontuple/BoolValue.h>
#include <eontuple/CharValue.h>
#include <eontuple/IntValue.h>
#include <eontuple/FloatValue.h>
#include <eontuple/NameValue.h>
#include <eontuple/StringValue.h>
#include <eontuple/BinaryValue.h>
#include <eontuple/RawValue.h>
#include <eontuple/RegexValue.h>
#include <eontuple/TupleValue.h>
#include <eontuple/ReferenceValue.h>
#include <eontuple/MetaValue.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown if an ETF document has a syntax error
	EONEXCEPT( EtfBadSyntax );




	/**************************************************************************
	  The 'tf' namespace encloses all non-public ETF functionality
	**************************************************************************/
	namespace tf
	{
		class Parser
		{
		public:

			Parser() = delete;
			Parser( const Parser& ) = delete;
			Parser( Parser&& ) = delete;
			Parser( const sourceref& source );



			
			// Parse the start of a new document
			// Returns a tuple containing zero attributes if there is nothing
			// more to parse, or 2 attributes indicating it is OK to continue
			// by calling parse():
			//   1. name    : Name of document, no_name if not specified.
			//   2. metaval : Meta information for the document (if any).
			tuple parseDocumentStart();

			// Parse the next attribute of the current document
			// Returns false when no more attributes.
			// If false is returned, you should call parseDocumentStart to
			// see if there are more documents.
			bool parseDocumentAttribute( tuple& document );


		private:

			// Parse an attribute, with optional name and meta data
			void parseAttribute( tuple& result );

			// Parse a value
			tup::valueptr parseValue();

			// Skip tokens, return false if nothing was skipped
			bool skipComments();
			bool skipSpaces();
			bool skipNoise();	// Comments and spaces
			bool skipEmtpyLines();

			tup::valueptr parseChar();
			tup::valueptr parseInt( int64_t sign = 1 );
			tup::valueptr parseFloat( double sign = 1.0 );
			tup::valueptr parseName();	// This can return nullptr!
			tup::valueptr parseString();
			tup::valueptr parseBinary();
			tup::valueptr parseRaw();
			tup::valueptr parseRegex();
			tup::valueptr parseTuple( size_t base_indentation,
				string group_end = string() );
			tup::valueptr parseRef();
			tup::valueptr parseMeta();

			std::string unescape( const std::string& str ) const noexcept;
			inline bool inrange( byte_t c, byte_t lower, byte_t upper )
				const noexcept { return c >= lower && c <= upper; }
			inline size_t indentation() const noexcept {
				return TP.at( TP.lineStart() ).space() ? TP.at(
					TP.lineStart() ).substr().numChars() : 0; }
			size_t indentationStrict() const;




		private:

			sourceref Source;
			tokenparser TP;
			string DecimalSep{ '.' };
		};
	}
}
