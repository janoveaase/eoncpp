#pragma once

#include "TypeSystem.h"
#include "TypeHandler.h"
#include <typeinfo>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
//	class MetaData;

	namespace type
	{
		class Object;


		/**********************************************************************
		  Literal attribute for normal tuples
		**********************************************************************/
		class Attribute
		{
		public:
			Attribute() = default;
			inline Attribute( Object* value ) { if( value ) Type = value->type(); assign( value ); }
			inline Attribute( const TypeTuple& type, Object* value = nullptr ) { Type = type; assign( value ); }
			inline Attribute( name_t name, Object* value = nullptr ) {
				Name = name; if( value ) Type = value->type(); assign( value ); }
			inline Attribute( name_t name, const TypeTuple& type, Object* value = nullptr ) {
				Name = name; Type = type; assign( value ); }
			inline Attribute( name_t name, name_t type, Object* value = nullptr ) {
				Name = name; Type = TypeTuple::name( type ); assign( value ); }

			Attribute( const Attribute& other ) { *this = other; }
			Attribute( Attribute&& other ) noexcept { *this = std::move( other ); }

			~Attribute() = default;

			Attribute& operator=( const Attribute& other );
			Attribute& operator=( Attribute&& other ) noexcept;

			inline name_t name() const noexcept { return Name; }
			inline const TypeTuple& type() const noexcept { return Type; }
			inline Object* value() const noexcept { return Value; }

			virtual void assign( Object* value ) { if( value ) {
				if( value->type() == Type ) Value = value; else throw IncompatibleType(); } }


		private:
			name_t Name{ no_name };
			TypeTuple Type;
			Object* Value{ nullptr };
		};
	}
}
