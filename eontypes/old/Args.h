#pragma once
//#include "TypeSystem.h"
//#include "DynamicTuple.h"
//#include "OperatorCodes.h"
//#include <eonscopes/Scope.h>
//#include <eonexpression/OperatorNode.h>
//#include <eonexpression/OperandNode.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace type
	{
		class Object;


		/**********************************************************************
		  Args Class - eon::type::Args

		  Arguments for actions
		**********************************************************************/
		class Args
		{
		public:

			//* Construct with no argments
			Args() = default;

			//* Construct with arguments
			Args( std::initializer_list<Node> args ) { Arguments = args; }

			virtual ~Args() = default;


			//* Access an argument
			//* Returns nullptr if no such argument!
			inline Node* arg( size_t i ) { return i < Arguments.size() ? &Arguments[ i ] : nullptr; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:

			std::vector<Node> Arguments;
		};
	}
}
