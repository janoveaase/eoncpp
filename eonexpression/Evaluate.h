#pragma once
#include "Node.h"
#include <eontuple/Value.h>


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

			static tup::valueptr unary( tup::operators::code op_code,
				tup::variables& vars, const nodeptr& arg1 );
			static tup::valueptr binary( tup::operators::code op_code,
				tup::variables& vars, const nodeptr& arg1, const nodeptr& arg2 );
			static tup::valueptr ternary( tup::operators::code op_code,
				tup::variables& vars,
				const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );




			/******************************************************************
			  Helpers
			******************************************************************/
		private:
			static tup::valueptr _unary( tup::operators::code op_code,
				tup::variables& vars, const nodeptr& arg1 );
			static tup::valueptr _binary( tup::operators::code op_code,
				tup::variables& vars, const nodeptr& arg1, const nodeptr& arg2 );
			static tup::valueptr _ternary( tup::operators::code op_code,
				tup::variables& vars,
				const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );
		};
	}
}
