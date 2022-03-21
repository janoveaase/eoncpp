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
			Expression( bool variable = false ) : Object( name_expression, source::Ref() ) { Variable = variable; }

			//* Construct for an expression
			inline Expression( const Node& value, bool variable = false ) : Object( name_expression, value.source() ) {
				Value.copy( value ); Variable = variable; }
			inline Expression( Node&& value, bool variable = false ) noexcept : Object( name_expression, value.source() ) {
				Value = std::move( value ); Variable = variable; }
			
			//* Copy and move
			inline Expression( const Expression& other ) : Object( name_expression, other.source() ) { Value = other.Value; }
			inline Expression( Expression&& other ) noexcept : Object( name_expression, other.source() ) {
				Value = std::move( other.Value ); Variable = other.Variable; }

			// Destruct expression
			~Expression() = default;


			// Delete 'this' object
			inline void die() override { delete this; }


			// Copy and move
			inline Expression& operator=( const Expression& other ) { Value = other.Value; return *this; }
			inline Expression& operator=( Expression&& other ) noexcept {
				Value = std::move( other.Value ); Variable = other.Variable; return *this; }


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
			inline type::Object* copy() override { return new Expression( Value ); }

			//* Get object as string representation
			void str( type::Stringifier& str ) const override;

			//* Check if the expression is empty or not
			inline bool empty() const noexcept { return Value.type() == NodeType::undef; }
			inline operator bool() const noexcept { return Value.type() != NodeType::undef; }

			//* Check if it is an expression variable
			inline bool variable() const noexcept { return Variable; }

			//* Access the value
			inline const Node& value() const noexcept { return Value; }
			inline Node& value() noexcept { return Value; }

			//* Get the type that this expression will produce
			inline EonType resultType() const noexcept { return Value.resultType(); }




			/******************************************************************
			  Comparison
			******************************************************************/
		public:

			inline int compare( const Expression& other ) const noexcept { return Value.compare( other.Value ); }




			/******************************************************************
			  Execute
			******************************************************************/
		public:

			//* Run the expression and get whatever is left




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			Node Value;		// Root node in expression tree
			bool Variable{ true };
		};
	}
}
