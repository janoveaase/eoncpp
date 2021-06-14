#pragma once

#include "Value.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon Tuple Format elements
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Eon Tuple Boolean Value Class - eon::tup::boolval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class boolval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline boolval() : value( basic_type::bool_t ) {}
			inline boolval( bool val ) noexcept
				: value( basic_type::bool_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool hardBool() const override { return Val; }
			inline bool softBool( variables& vars ) const override {
				return Val; }
			inline char_t softChar( variables& vars ) const override {
				return Val ? '1' : '0'; }
			inline int64_t softInt( variables& vars ) const override {
				return Val ? 1 : 0; }
			inline double softFloat( variables& vars ) const override {
				return Val ? 1.0 : 0.0; }
			inline name_t softName( variables& vars ) const override {
				return Val ? name_true : name_false; }
			inline const string& softString( variables& vars ) const override {
				static string str; str = Val ? "true" : "false"; return str; }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardBool();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto o = other->softBool( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new boolval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line += Val ? 4 : 5; return
					Val ? "true" : "false"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val = false; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			bool Val{ false };
		};
	}
}
