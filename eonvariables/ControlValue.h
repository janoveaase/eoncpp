#pragma once

#include "Value.h"


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
		//* Contcol value types
		enum class ctrl_t
		{
			undef,
			_return
		};

		/**********************************************************************
		  Eon Tuple Control Value Class - eon::vars::controlval

		  This is a specialization of [eon::vars::value] used for control
		  operations (such as return).
		**********************************************************************/
		class controlval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline controlval() : value( type_code::control_t) {}
			inline controlval( ctrl_t code, valueptr val ) : value( type_code::control_t ) {
				Code = code; Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( ctrl_t code, valueptr value) {
				return valueptr( new controlval( code, value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline ctrl_t code() const noexcept { return Code; }
			inline const valueptr& val() const noexcept{ return Val; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new controlval( Code, Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override { return ""; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override { throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override {}




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			ctrl_t Code{ ctrl_t::undef };
			valueptr Val;
		};
	}
}
