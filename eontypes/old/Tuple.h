#pragma once

#include "Types.h"
#include <unordered_map>
#include <eonexcept/Exception.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	EONEXCEPT( NoSuchName );


	/**************************************************************************
	  Eon Tuple Class - eon::Tuple
	**************************************************************************/
	class Tuple : public tup::Object
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Construct a place-holder instance
		Tuple() = default;

		//* Copy/move
		inline Tuple( const Tuple& other ) { *this = other; }
		inline Tuple( Tuple&& other ) noexcept { *this = std::move( other ); }

		inline ~Tuple() { _reset(); }




		/**********************************************************************
		  General Read-only Methods
		**********************************************************************/
	public:

		//* Get the tuple's type
		//* WARNING: This will construct a new tuple of names, for each
		//*          attribute. If you need the type more than once, store it!
		Tuple type() const;

		//* Get a copy of the tuple
		Tuple* copy() const;

		//* Get a string representation
		//* NOTE: Braces will not be included - the caller must handle that!
		string str() const;




		/**********************************************************************
		  Attribute Methods
		**********************************************************************/
	public:

		//* Check if there are any attributes at all
		inline bool empty() const noexcept { return Attributes.empty(); }

		//* Get number of attributes (Primitives will have only one.)
		inline size_t numAttributes() const noexcept { return Attributes.size(); }

		//* Add an attribute without name or meta data
		inline void add( tup::Instance* attribute ) { Attributes.push_back( attribute ); }

		//* Add a named attribute, with or without meta data
		inline void add( name_t name, tup::Instance* attribute, Tuple* metadata = nullptr ) {
			_addName( name, metadata ); Attributes.push_back( attribute ); }


		//* Get attribute by position
		//* Throws std::out_of_range if invalid position!
		inline const tup::Instance* at( size_t pos ) const { return Attributes.at( pos ); }
		inline tup::Instance* at( size_t pos ) { return Attributes.at( pos ); }
		inline const tup::Instance* at( int pos ) const { return at( static_cast<pos_t>( pos ) ); }
		inline tup::Instance* at( int pos ) { return at( static_cast<pos_t>( pos ) ); }

		//* Get attribute by name
		//* Throws eon::NoSuchName if not found!
		inline const tup::Instance* at( name_t name ) const { if( Names ) {
			auto found = Names->find( name ); if( found != Names->end() ) return Attributes[ found->second.first ]; }
			throw NoSuchName( *name ); }
		inline tup::Instance* at( name_t name ) { if( Names ) {
			auto found = Names->find( name ); if( found != Names->end() ) return Attributes[ found->second.first ]; }
			throw NoSuchName( *name ); }

		//* Get meta data for a named attribute
		//* Returns nullptr if no meta data or no such name!
		inline const eon::Tuple* metadata( name_t name ) const noexcept { if( Names ) {
			auto found = Names->find( name ); if( found != Names->end() ) return found->second.second; }
			return nullptr; }
		inline eon::Tuple* metadata( name_t name ) noexcept { if( Names ) {
			auto found = Names->find( name ); if( found != Names->end() ) return found->second.second; }
			return nullptr; }




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		Tuple& operator=( const Tuple& other );
		inline Tuple& operator=( Tuple&& other ) noexcept {
			_reset(); Attributes = std::move( other.Attributes );
			if( other.Names ) { Names = other.Names; other.Names = nullptr; } return *this; }




		/**********************************************************************
		  Attribute iteration
		**********************************************************************/
	public:

		using iterator = std::vector<tup::Instance*>::iterator;
		using const_iterator = std::vector<tup::Instance*>::const_iterator;

		inline iterator begin() noexcept { return Attributes.begin(); }
		inline iterator end() noexcept { return Attributes.end(); }

		inline const_iterator begin() const noexcept { return Attributes.begin(); }
		inline const_iterator end() const noexcept { return Attributes.end(); }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		//* NOTE: This assumes that 'other' has the same type as this!
		//*       (I.e. compare types first!)
		int compare( const Tuple& other ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _reset();

		inline void _addName( name_t name, Tuple* metadata = nullptr ) {
			if( !Names )
				Names = new std::unordered_map<name_t, std::pair<pos_t, Tuple*>>();
			Names->insert( std::make_pair( name, std::make_pair( Attributes.size(), metadata ) ) ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		// Attributes part of this tuple
		std::vector<tup::Instance*> Attributes;

		// Named attributes - with optional meta data
		std::unordered_map<name_t, std::pair<pos_t, Tuple*>>* Names{ nullptr };
	};
}
