#pragma once

#include "TypeSystem.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  A reference refers to another object

	  In the basic form, a reference is read-only!
	**************************************************************************/
	class Reference : public type::Object
	{
	public:
		Reference( source::Ref source ) : type::Object( EonType( name_reference, source ), source ) {}
		Reference( EonType type, source::Ref source ) noexcept : type::Object( type, source ) {}
		Reference( type::Object* value ) noexcept : type::Object( *value ) { Value = value; }
		Reference( const Reference& other ) noexcept : Object( other ) { Value = other.Value; }
		virtual ~Reference() = default;

		inline Reference* operator=( const Reference& other ) noexcept {
			static_cast<Object&>( *this ) = other; Value = other.Value; return this; }


		virtual inline bool canModify() const noexcept { return false; }
		const Object* value() const noexcept { return Value; }


		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		virtual name_t generalType() const noexcept override { return name_reference; }
		inline void die() override { Value = nullptr; }
		inline void callDestructor() override { delete this; }
		inline Object* copy() override { return new Reference( Value ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "str" );  }

	protected:
		Object* Value{ nullptr };
	};


	/**************************************************************************
	  Modifiable reference
	**************************************************************************/
	class Modifiable : public Reference
	{
	public:
		Modifiable( source::Ref source ) : Reference( source ) {}
		Modifiable( EonType type, source::Ref source ) noexcept : Reference( type, source ) {}
		Modifiable( type::Object* value ) noexcept : Reference( value ) {}
		Modifiable( const Modifiable& other ) noexcept : Reference( other ) {}
		virtual ~Modifiable() = default;

		Object* operator=( const Reference& other ) noexcept { static_cast<Reference&>( *this ) = other; return this; }


		virtual inline bool canModify() const noexcept override { return true; }
		Object* value() noexcept { return Value; }


		inline name_t generalType() const noexcept override { return name_modify; }
		inline void str( type::Stringifier& str ) const override { str.addWord( "str" ); }
	};
}
