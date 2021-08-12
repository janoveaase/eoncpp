#pragma once
#include "Node.h"
//#include <eontuple/Value.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::expr' namespace encloses all hidden expression functionality
	**************************************************************************/
	namespace expr
	{
		/**********************************************************************
		  Eon Evaluate Class - eon::expr::evaluate

		  Evaluates unary, binary and ternary expressions.
		**********************************************************************/
		class evaluate
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			evaluate() = default;
			virtual ~evaluate() = default;




			/******************************************************************
			  Evaluate
			******************************************************************/
		public:

			static vars::valueptr unary( vars::operators::code op_code, vars::variables& vars, const nodeptr& arg1 );
			static vars::valueptr binary( vars::operators::code op_code, vars::variables& vars,
				const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr ternary( vars::operators::code op_code,
				vars::variables& vars, const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );




			/******************************************************************
			  Helpers
			******************************************************************/
		private:
			static vars::valueptr _unary( vars::operators::code op_code, vars::variables& vars, const nodeptr& arg1 );
			static vars::valueptr _binary( vars::operators::code op_code, vars::variables& vars,
				const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _ternary( vars::operators::code op_code,
				vars::variables& vars, const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );
		};
	}
}
