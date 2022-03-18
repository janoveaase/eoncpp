#pragma once
#include "Element.h"
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonsource/SourceReporter.h>
#include <eontypes/BasicTuple.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		/**********************************************************************
		  Eon Raw Parser Class - eon::parser::Raw

		  Will turn a token stream into a stream of identified elements
		**********************************************************************/
		class Raw
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			Raw() = default;
			Raw( source::Ref source, source::Reporter& reporter ) { Source = source; Reporter = &reporter; _prepare(); }
			Raw( const Raw& ) = delete;
			Raw( Raw&& other ) noexcept { *this = std::move( other ); }
			virtual ~Raw() = default;


			Raw operator=( const Raw& ) = delete;
			inline Raw& operator=( Raw&& other ) noexcept {
				Source = other.Source; Parser = std::move( other.Parser ); Reporter = other.Reporter;
				other.Reporter = nullptr; Elements = std::move( other.Elements ); return *this; }




			/******************************************************************
			  Execution
			******************************************************************/
		public:

			//* Parse raw token stream in order to produce stream of identified elements
			//* Will report issues to the specified reporter
			//* Returns true if successfully parsed, false if there were errors
			std::vector<Element> operator()();




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			bool _parse();


			// Process newline and indentation
			void _processNewline();

			// Process operators
			bool _processOperators();
			std::vector<Element>::iterator _skipEnclosed( std::vector<Element>::iterator first );
			bool _processSquareBraceOperator( std::vector<Element>::iterator first );

			// Prepare for parsing by tokenizing and re-tokenizing to get an
			// optimal token stream
			void _prepare();

			// Get fully configured Tokenizer object
			Tokenizer& tokenizer();

			// Prepare fully configured ReTokenizer object
			ReTokenizer& _retokenize();




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			source::Ref Source;
			TokenParser Parser;
			source::Reporter* Reporter{ nullptr };
			std::vector<Element> Elements;
		};
	}
}
