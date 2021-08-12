#pragma once

#include <unordered_map>
#include <memory>
#include <eonexcept/Exception.h>
#include <eonname/NameRef.h>
#include <eonhex/Hex.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;
	namespace vars
	{
		class refval;
		class variables;
		class value;
		using valueptr = std::shared_ptr<value>;
	}




	/**************************************************************************
	  Eon Attribute Class - eon::attribute

	  Holds a tuple attribute.
	**************************************************************************/
	class attribute
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		attribute() = default;
		inline attribute( const attribute& other ) { *this = other; }
		inline attribute( attribute&& other ) noexcept { *this = std::move( other ); }

		inline attribute( size_t pos, vars::valueptr value ) { Pos = pos; Value = value; }
		attribute( name_t name, size_t pos, vars::valueptr value );
		attribute( name_t name, size_t pos, tupleptr metadata, vars::valueptr value );

		virtual ~attribute() = default;


		attribute& operator=( const attribute& other );
		attribute& operator=( attribute&& other ) noexcept;



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Is the attribute named?
		inline bool named() const noexcept { return Name != no_name; }

		//* Get the name - no_name if not named
		inline name_t name() const noexcept { return Name; }

		//* Get the meta data (void pointer if not named)
		inline tupleptr meta() const noexcept { return MetaData; }

		//* Get the position of the attribute within its tuple
		inline size_t pos() const noexcept { return Pos; }

		//* Get the value
		inline vars::valueptr value() const noexcept { return Value; }




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		//* Set meta data
		inline void meta( tupleptr new_meta ) { MetaData = new_meta; }

		//* Set value
		inline void value( vars::valueptr new_value ) { Value = new_value; }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		name_t Name{ no_name };
		tupleptr MetaData;
		size_t Pos{ 0 };
		vars::valueptr Value;
	};
	using attributeptr = std::shared_ptr<attribute>;
}
