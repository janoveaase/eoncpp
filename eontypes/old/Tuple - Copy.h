#pragma once

#include "TypeSpec.h"
#include "Scope.h"

#include <unordered_map>
#include <eonexcept/Exception.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace tup
	{
		class varscope;
	}

	class instance;




	/**************************************************************************
	  Eon Tuple Class - eon::tuple

	  Holds an actual tuple.
	**************************************************************************/
	class tuple
	{
		/**********************************************************************
		  Defintions
		**********************************************************************/
	public:

		EONEXCEPT( NotFound );
		EONEXCEPT( TypeAlreadyLocked );
		EONEXCEPT( DuplicateName );

		class attribute
		{
		public:
			attribute() = default;
			attribute( name_t name, eon::tuple* meta, instance* value );
			attribute( name_t name, instance* value );
			attribute( instance* value );

			~attribute() = default;

		private:
			name_t Name{ nullptr };
			eon::tuple* NameMeta{ nullptr };
			instance* Value{ nullptr };

			friend class tuple;
		};




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Construct a place-holder instance
		tuple() = default;

		//* Copy/move
		inline tuple( const tuple& other ) { *this = other; }
		inline tuple( tuple&& other ) noexcept { *this = std::move( other ); }

		//* Construct an empty tuple
		tuple( tup::varscope& scope );

		//* Construct a tuple with attributes
		tuple( tup::varscope& scope, std::initializer_list<attribute> attributes );

		~tuple();




		/**********************************************************************
		  General Read-only Methods
		**********************************************************************/
	public:

		//* Get the tuple's type
		inline const tup::tuplespec* type() const noexcept { return Type; }

		//* Get a copy of the tuple
		tuple* copy() const;

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


		//* Get attribute by position
		//* Throws NotFound if invalid position!
		inline const instance* at( size_t pos ) const {
			if( pos < Attributes.size() ) return Attributes[ pos ]; else throw NotFound(); }
		inline instance* at( size_t pos ) {
			if( pos < Attributes.size() ) return Attributes[ pos ]; else throw NotFound(); }
		inline const instance* at( int pos ) const { return at( static_cast<size_t>( pos ) ); }
		inline instance* at( int pos ) { return at( static_cast<size_t>( pos ) ); }

		//* Get meta data for a named attribute
		//* Returns nullptr if no meta data
		inline const eon::tuple* metadata( name_t name ) const noexcept {
			auto found = Meta.find( name ); return found != Meta.end() ? found->second : nullptr; }
		inline eon::tuple* metadata( name_t name ) {
			auto found = Meta.find( name ); return found != Meta.end() ? found->second : nullptr; }




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		inline tuple& operator=( const tuple& other ) {
			Type = other.Type; Attributes = other.Attributes; Meta = other.Meta; return *this; }
		inline tuple& operator=( tuple&& other ) noexcept { Type = other.Type; other.Type = nullptr; Attributes
			= std::move( other.Attributes ); Meta = std::move( other.Meta ); return *this; }




		/**********************************************************************
		  Attribute iteration
		**********************************************************************/
	public:

		using iterator = std::vector<instance*>::iterator;
		using const_iterator = std::vector<instance*>::const_iterator;

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
		int compare( const tuple& other ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		const tup::tuplespec* Type{ nullptr };
		std::vector<instance*> Attributes;
		std::unordered_map<name_t, eon::tuple*> Meta;
	};
}
