#pragma once

#include <unordered_map>
#include <eonname/NameRef.h>
#include <eontuple/Tuple.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::vars' namespace is for specific Æon Variables elements
	**************************************************************************/
	namespace vars
	{
		class value;
		using valueptr = std::shared_ptr<value>;

		class function;
		using functionptr = std::shared_ptr<function>;


		/**********************************************************************
		  Eon Source Class - eon::vars::source

		  A virtual class defining the interface for a class that provides
		  access to variables referenced by nameref and not name_t.
		**********************************************************************/
		class refsource
		{
		public:
			virtual valueptr _get( const nameref& ref, valueptr context ) const noexcept = 0;
			virtual void _set( const nameref& ref, valueptr context, const valueptr& value ) = 0;
		};


		/**********************************************************************
		  Eon Variables Class - eon::vars::variables

		  Uses name_t and nameref as keys for accessing valueptr objects.
		  Uses nameref as keys for accessing function objects.

		  NOTE: Functions will push another variables scope onto the variables
		        stack, allowing for local variables to be separate from global/
				shared ones.

		  WARNING: This class is not thread safe!
		**********************************************************************/
		class variables
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			variables() = default;
			inline variables( refsource& src ) { RefSource = &src; }
			virtual ~variables() = default;



			/******************************************************************
			  Scopes

			  Scopes are organized in a stack-like structure, using push and
			  pop methods.
			  There is a global scope (at the bottom of the stack) which cannot
			  be popped.
			  New variables are created in the current scope (at the top of the
			  stack).
			  When accessing a variable, each scope will be searched from top
			  to bottom, first found is used.
			******************************************************************/
		public:

			//* Create a new scope
			inline void push() { VarStack.push_front( std::unordered_map<name_t, valueptr>() ); }

			//* Pop the current scope
			//* Has no effect if only one scope (global) left.
			inline void pop() { if( VarStack.size() > 1 ) VarStack.erase( VarStack.begin() ); }



			/******************************************************************
			  Accessing variables within the current scope
			******************************************************************/
		public:

			//* Check if a variable exists
			inline bool exists( name_t name ) const noexcept { return static_cast<bool>( getActual( name ) ); }

			// Get a variable
			// Returns vars::boolval with false value if not found
			valueptr get( name_t name ) const noexcept;

			// Get a variable
			// Returns void valueptr if not found
			valueptr getActual( name_t name ) const noexcept;

			//* Set existing or create new variable - but only at the top of
			//* the stack
			inline void setTop( name_t name, const valueptr& value ) {
				VarStack.begin()->insert( std::make_pair( name, value ) ); }

			//* If a variable exists anywhere on the stack, then set it,
			//* otherwise create a new variable at the top of the stack
			void set( name_t name, const valueptr& value );



			/******************************************************************
			  Accessing variables using name references
			******************************************************************/
		public:

			//* Check if a variable exists
			inline bool exists( const nameref& name, valueptr context ) const noexcept {
				return RefSource != nullptr ? static_cast<bool>( RefSource->_get( name, context ) ) : false; }

			// Get a variable
			// Returns vars::boolval with false value if not found
			valueptr get( const nameref& name, valueptr context ) const noexcept;

			// Get a variable
			// Returns void valueptr if not found
			valueptr getActual( const nameref& name, valueptr context ) const noexcept;

			// Set existing or create new variable
			// No effect if no source has been set!
			inline void set( const nameref& name, valueptr context, const valueptr& value ) {
				if( RefSource != nullptr ) RefSource->_set( name, context, value ); }



			/******************************************************************
			  Accessing built-in functions using name references
			******************************************************************/
		public:

			//* Get function
			// Returns nullptr if not found
			function* getFunction( const nameref& name );

			//* Register a function
			void setFunction( const nameref& name, functionptr func );




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::list<std::unordered_map<name_t, valueptr>> VarStack{ std::unordered_map<name_t, valueptr>() };
			std::unordered_map<nameref, functionptr> Functions;
			refsource* RefSource{ nullptr };
		};
	}
}
