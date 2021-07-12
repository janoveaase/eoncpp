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
		  Eon Tuple Variable Value Class - eon::tup::variableval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class variableval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline variableval() : value( basic_type::var_t ) {}
			inline variableval( const name_t& val )
				: value( basic_type::var_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return vars.get( Val )->softBool( vars ); }
			inline char_t softChar( variables& vars ) const override {
				return vars.get( Val )->softChar( vars ); }
			inline int64_t softInt( variables& vars ) const override {
				return vars.get( Val )->softInt( vars ); }
			inline double softFloat( variables& vars ) const override {
				return vars.get( Val )->softFloat( vars ); }
			inline name_t softName( variables& vars ) const override {
				return vars.get( Val )->softName( vars ); }
			inline const string& softString( variables& vars ) const override {
				return vars.get( Val )->softString( vars ); }
			inline const hex& softBinary( variables& vars ) const override {
				return vars.get( Val )->softBinary( vars ); }
			inline const std::vector<string>& softRaw( variables& vars )
				const override { return vars.get( Val )->softRaw( vars ); }
			inline const regex& softRegex( variables& vars ) const override {
				return vars.get( Val )->softRegex( vars ); }
			inline const path& softRef( variables& vars ) const override {
				return vars.get( Val )->softRef( vars ); }
			inline name_t hardVar() const override { return Val; }
			inline const tuple& softTuple( variables& vars ) const override {
				return vars.get( Val )->softTuple( vars ); }
			inline const tuple& softMeta( variables& vars ) const override {
				return vars.get( Val )->softMeta( vars ); }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardVar();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			int softCompare( const valueptr& other, variables& vars )
				const override;

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new variableval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override {
				pos_on_line += Val->numChars(); return *Val; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val = no_name; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			name_t Val{ no_name };
		};
	}
}
