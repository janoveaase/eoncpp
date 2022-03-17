/*#pragma once

#include "BasicTuple.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
****************************************************************************** /
namespace eon
{
	/**************************************************************************
	  MetaData class - eon::MetaData

	  A meta data object is a dynamic tuple for which all attributes are read-
	  only and cannot be removed.
	************************************************************************** /
	class MetaData : public type::BasicTuple
	{
		/**********************************************************************
		  Construction
		********************************************************************** /
	public:

		// Construct an empty dynamic tuple
		MetaData() : type::BasicTuple( name_meta, source::Ref() ) {}

		// Construct dynamic tuple of unnamed attributes based on values only
		inline MetaData( std::initializer_list<Object*> values, source::Ref source )
			: type::BasicTuple( name_meta, values, source ) {}

		// Construct dynamic tuple for the specified attribute elements
		inline MetaData( std::initializer_list<type::Attribute> attributes, source::Ref source )
			: type::BasicTuple( name_meta, attributes, source ) {}

		// Copy construct at specified (stack) memory, or if nullptr, in
		// heap memory!
		inline MetaData( const MetaData& other ) : type::BasicTuple( other ) {}

		// Take ownership of other tuple
		inline MetaData( MetaData&& other ) noexcept : type::BasicTuple( std::move( other ) ) {}
		inline MetaData( type::BasicTuple&& other ) : type::BasicTuple( std::move( other ) ) { finalize(); }

		// Destruct tuple
		virtual ~MetaData() = default;



		// Delete 'this' object
		virtual void die() override { delete this; }


		// Copy
		inline type::Object* copy() override { return new MetaData( *this ); }
		inline MetaData& operator=( const MetaData& other ) {
			*static_cast<type::BasicTuple*>( this ) = other; return *this; }

		// Transfer ownership
		inline MetaData& operator=( MetaData&& other ) noexcept {
			*static_cast<type::BasicTuple*>( this ) = std::move( other ); return *this; }


		void callDestructor() override { this->~MetaData(); }




		/**********************************************************************
		  Basic Access
		********************************************************************** /
	public:

		static inline name_t tupleType() noexcept { return name_meta; }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	protected:

		// Output standard parenthesized prefix and posfix for tuple
		inline void standardPrefix( type::Stringifier& str ) const override {
			if( str.inTuple( name_meta ) ) str.addWord( "(" ); else str.addWord( "M(" ); }
		inline void standardPostfix( type::Stringifier& str ) const override { str.addWord( ")" ); }

		// Output explicit parenthesized prefix and postfix for tuple
		inline void explicitPrefix( type::Stringifier& str ) const override { return standardPrefix( str ); }
	};
}
*/