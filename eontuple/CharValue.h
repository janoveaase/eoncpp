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
		  Eon Tuple Character Value Class - eon::tup::charval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class charval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline charval() : value( basic_type::char_t ) {}
			inline charval( char_t val ) noexcept
				: value( basic_type::char_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return Val != '0'; }
			inline char_t hardChar() const override { return Val; }
			inline char_t softChar( variables& vars ) const override {
				return Val; }
			int64_t softInt( variables& vars ) const override;
			double softFloat( variables& vars ) const override;
			inline const string& softString( variables& vars ) const override {
				static string str; str = string( Val ); return str; }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardChar();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto o = other->softChar( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new charval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line += 2;
					return "'" + string( Val ).escape() + "'"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val = 0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			char_t Val{ 0 };
		};
	}
}
