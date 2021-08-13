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

			static vars::valueptr unary( vars::operators::code op_code, vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr binary( vars::operators::code op_code, vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr ternary( vars::operators::code op_code,
				vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );




			/******************************************************************
			  Helpers
			******************************************************************/
		private:
			static vars::valueptr _unary( vars::operators::code op_code, vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _binary( vars::operators::code op_code, vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _ternary( vars::operators::code op_code,
				vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );

			static vars::valueptr _sqrt( vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _unary_minus( vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _exists( vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _len( vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _is_set( vars::variables& varcache, const nodeptr& arg1 );
			static vars::valueptr _delete( vars::variables& varcache, const nodeptr& arg1 );

			static vars::valueptr _call( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _plus( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _minus( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _multiply( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _divide( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _pow( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _match( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _assign( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _resize( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _saveto( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _element( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );
			static vars::valueptr _in( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 );

			static vars::valueptr _if_else( vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );
			static vars::valueptr _slice( vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );

			static vars::UnsupportedOperand unsupported( vars::operators::code op_code, vars::variables& varcache,
				const nodeptr& arg1 );
			static vars::UnsupportedOperand unsupported( vars::operators::code op_code, vars::valueptr arg1 );
			static vars::UnsupportedOperand unsupported( vars::operators::code op_code, vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2 );
			static vars::UnsupportedOperand unsupported( vars::operators::code op_code,
				vars::valueptr arg1, vars::valueptr arg2 );
			static vars::UnsupportedOperand unsupported( vars::operators::code op_code, vars::variables& varcache,
				const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 );
			static vars::UnsupportedOperand unsupported( vars::operators::code op_code,
				vars::valueptr arg1, vars::valueptr arg2, vars::valueptr arg3 );
		};
	}
}
