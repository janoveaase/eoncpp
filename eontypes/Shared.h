#pragma once

#include "TypeSystem.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon shared object
	**************************************************************************/
	class shared
	{
	public:
		shared() = default;
		inline shared( type::Object* value ) noexcept { Value = value; _inc(); }
		inline shared( shared& other ) noexcept { Refs = other.Refs; Value = other.Value; if( Refs ) _inc(); }
		inline ~shared() { _dec(); }

		inline shared& operator=( const shared& other ) noexcept {
			_dec(); Refs = other.Refs; Value = other.Value; if( Refs ) _inc(); return *this; }

		inline void reset() noexcept { _dec(); Refs = nullptr; Value = nullptr; }

		inline type::Object* value() noexcept { return Value; }
		inline type::Object* operator->() noexcept { return Value; }
		inline size_t refs() const noexcept { return Refs ? *Refs : 0; }

	private:
		inline void _inc() noexcept { if( !Refs ) Refs = new size_t{ 1 }; else ++Refs; }
		inline void _dec() noexcept {
			if( Refs ) { if( --Refs == 0 ) { Value->die(); Value = nullptr; delete Refs; Refs = nullptr; } } }

	private:
		size_t* Refs{ nullptr };
		type::Object* Value{ nullptr };
	};




	class Shared : public type::Object
	{
	public:
		Shared() = delete;
		Shared( shared value ) noexcept : type::Object( value->type(), value->source() ) { Value = value; }
		Shared( const Shared& other ) noexcept : Object( other ) { Value = other.Value; }
		virtual ~Shared() = default;

		Shared& operator=( const Shared& other ) noexcept {
			static_cast<Object&>( *this ) = other; Value = other.Value; return *this; }


		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline name_t generalType() const noexcept override { return name_shared; }
		inline void die() { Value.reset(); }
		inline void callDestructor() override { delete this; }
		inline Object* copy() override { return new Shared( Value ); }
		inline void str( Stringifier& str ) const override { str.word( "str" );  }

	private:
		shared Value;
	};
}
