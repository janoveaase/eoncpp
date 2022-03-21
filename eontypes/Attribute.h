#pragma once

#include "TypeSystem.h"
#include "TypeHandler.h"
#include <typeinfo>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
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
			inline Attribute( const EonType& type, Object* value = nullptr ) { Type = type; assign( value ); }
			inline Attribute( name_t name, Object* value = nullptr ) {
				Name = name; if( value ) Type = EonType( value->type(), name ); assign( value ); }
			inline Attribute( name_t name, const EonType& type, Object* value = nullptr ) {
				Name = name; Type = EonType( type, name ); assign( value ); }

			Attribute( const Attribute& other ) { *this = other; }
			Attribute( Attribute&& other ) noexcept { *this = std::move( other ); }

			~Attribute() = default;

			Attribute& operator=( const Attribute& other );
			Attribute& operator=( Attribute&& other ) noexcept;

			inline name_t name() const noexcept { return Name; }
			inline const EonType& type() const noexcept { return Type; }
			inline Object* value() const noexcept { return Value; }

			virtual void assign( Object* value ) { if( value ) {
				if( value->type() == Type ) Value = value; else throw IncompatibleType(); } }


		private:
			name_t Name{ no_name };
			EonType Type;
			Object* Value{ nullptr };
		};
	}
}
