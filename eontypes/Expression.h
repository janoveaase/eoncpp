#pragma once

#include "TypeSystem.h"
#include "Node.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::type' namespace encloses all hidden type functionality
	**************************************************************************/
	namespace type
	{
		/**********************************************************************
		  Expression class - eon::Expression

		  Dynamic tuples can store the following types (only):
			bool, byte, char, long, high, name,
			string, bytes, regex, namepath, path,
			metadata, array, set, dynamic

		  int and short will be automatically converted into long, and float and
		  low into high when added to the dynamic tuple.
		**********************************************************************/
		class Expression : public type::Object
		{
		public:
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			//* Construct an empty expression
			Expression() : Object( name_expression ) {}

			//* Construct for an expression
			inline Expression( const Node& value, scope::Scope& scope ) : Object( name_expression ) {
				Value.copy( value, scope ); }
			inline Expression( Node&& value ) noexcept : Object( name_expression ) { Value = std::move( value ); }
			
			// Take ownership of other expression
			inline Expression( Expression&& other ) noexcept : Object( name_expression ) { Value = std::move( other.Value ); }

			// Destruct expression
			~Expression() = default;


			// Delete 'this' object
			inline void die() override { delete this; }


			// Transfer ownership
			inline Expression& operator=( Expression&& other ) noexcept { Value = std::move( other.Value ); return *this; }


			inline void callDestructor() override { this->~Expression(); }




			/******************************************************************
			  Basic Access
			******************************************************************/
		public:

			//* Get the C++ type
			inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

			//* Identify this as an 'expression'
			inline name_t generalType() const noexcept override { return name_expression; }

			//* Create a copy of 'this'
			inline type::Object* copy( scope::Scope& scope ) override { return new Expression( Value, scope ); }

			//* Get object as string representation
			inline void str( type::Stringifier& str ) const override { Value.infixStr( str ); }

			//* Check if the expression is empty or not
			inline bool empty() const noexcept { return Value.type() == NodeType::undef; }
			inline operator bool() const noexcept { return Value.type() != NodeType::undef; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			Node Value;		// Root node in expression tree
		};
	}
}
