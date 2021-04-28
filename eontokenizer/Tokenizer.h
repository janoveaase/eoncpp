#pragma once
#include "Token.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Tokenizer Class - eon::tokenizer

	  A tokenizer object will take an [eon::source], scan through it and return
	  a vector of [eon::token]s.
	**************************************************************************/
	class tokenizer
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, invalid tokenizer
		tokenizer() = default;

		//* Disallow copy and move
		tokenizer( const tokenizer& ) = delete;
		tokenizer( tokenizer&& ) = delete;

		//* Default destruction
		virtual ~tokenizer() = default;




		/**********************************************************************
		  Tokenizing
		**********************************************************************/

		//* Get sequence of [eon::token]s from an [eon::source]
		std::vector<token> operator()( const source& source );




	private:
		void scanSameChar( tokencat category );
		void scanLetter( tokencat category );
		void scanNumber( tokencat category );

	private:
		sourceref SrcRef;
		std::vector<token> Tokens;
	};
};
