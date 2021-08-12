#pragma once

#include <eonsource/SourceRef.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eontuple/Tuple.h>
#include <eonvariables/BoolValue.h>
#include <eonvariables/CharValue.h>
#include <eonvariables/IntValue.h>
#include <eonvariables/FloatValue.h>
#include <eonvariables/NameValue.h>
#include <eonvariables/StringValue.h>
#include <eonvariables/BinaryValue.h>
#include <eonvariables/RawValue.h>
#include <eonvariables/RegexValue.h>
#include <eonvariables/VariableValue.h>
#include <eonvariables/ExpressionValue.h>
#include <eonvariables/TupleValue.h>
#include <eonvariables/ReferenceValue.h>
#include <eonvariables/MetaValue.h>
#include <eonvariables/FunctionValue.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown if an EOF document has a syntax error
	EONEXCEPT( EofBadSyntax );




	/**************************************************************************
	  The 'tf' namespace encloses all non-public ETF functionality
	**************************************************************************/
	namespace tf
	{
		class Parser
		{
		public:

			Parser() = default;
			Parser( const Parser& ) = delete;
			Parser( Parser&& ) = delete;
			Parser( const sourceref& source );



			
			// Parse the start of a new document
			// Returns a tuple containing zero attributes if there is nothing
			// more to parse, or 2 attributes indicating it is OK to continue
			// by calling parse():
			//   1. name    : Name of document, no_name if not specified.
			//   2. metaval : Meta information for the document (if any).
			tuple parseDocumentStart( vars::variables& vars, bool merge );

			// Parse the next attribute of the current document
			// Returns false when no more attributes.
			// If false is returned, you should call parseDocumentStart to
			// see if there are more documents.
			bool parseDocumentAttribute( tuple& document, vars::variables& vars,
				bool merge );


			// Parse a single value
			enum class ContextType
			{
				plain,
				expression,
				function
			};
			vars::valueptr parseValue( tokenparser& parser, vars::variables& vars, ContextType context, bool merge );


		private:

			// Parse an attribute, with optional name and meta data
			void parseAttribute( tuple& result, vars::variables& vars, bool merge );

			// Parse a value
			vars::valueptr parseValue( vars::variables& vars, ContextType context, bool merge );

			// Skip tokens, return false if nothing was skipped
			bool skipComments();
			bool skipSpaces();
			bool skipNoise();	// Comments and spaces
			bool skipEmtpyLines();

			vars::valueptr parseChar();
			vars::valueptr parseInt( int64_t sign = 1 );
			vars::valueptr parseFloat( double sign = 1.0 );
			vars::valueptr parseName();
			name_t parseRawName();	// This can return no_name!
			vars::valueptr parseString();
			vars::valueptr parseBinary();
			vars::valueptr parseRaw();
			vars::valueptr parseRegex();
			enum class tupletype
			{
				plain,
				dash,
				curly,
				meta,
				function
			};
			vars::valueptr parseTuple( vars::variables& vars, tupletype type, bool merge );
			vars::valueptr parseRef();
			vars::valueptr parseExpr( vars::variables& vars );
			inline vars::valueptr parseMeta( vars::variables& vars, bool merge ) {
				return parseTuple( vars, tupletype::meta, merge ); }

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
