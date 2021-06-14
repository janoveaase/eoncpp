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
		  Eon Tuple String Value Class - eon::tup::stringval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class stringval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline stringval() : value( basic_type::string_t ) {}
			inline stringval( const string& val )
				: value( basic_type::string_t ) { Val = val; }
			inline stringval( string&& val ) noexcept
				: value( basic_type::string_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline int64_t softInt( variables& vars ) const override {
				if( Val.isInt() || Val.isFloat() ) return Val.toInt64();
					else throw UnsupportedOperand( "Cannot convert string \""
						+ Val + "\" to 'int'!" ); }
			inline double softFloat( variables& vars ) const override {
				if( Val.isInt() || Val.isFloat() ) return Val.toDouble();
					else throw UnsupportedOperand( "Cannot convert string \""
						+ Val + "\" to 'float'!" ); }
			inline const string& hardString() const override { return Val; }
			inline const string& softString( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				return Val.compare( other->hardString() ); }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { return Val.compare(
					other->softString( vars ) ); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new stringval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line
					+= Val.numChars() + 2; return "\"" + Val.escape() + "\""; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			inline string& string_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			eon::string Val;
		};
	}
}
