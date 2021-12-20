#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


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

		inline shared& operator=( shared& other ) noexcept {
			_dec(); Refs = other.Refs; Value = other.Value; if( Refs ) _inc(); return *this; }

		inline void reset() noexcept { _dec(); Refs = nullptr; Value = nullptr; }

		inline type::Object* value() noexcept { return Value; }
		inline size_t refs() const noexcept { return Refs ? *Refs : 0; }

	private:
		inline void _inc() noexcept { if( !Refs ) Refs = new size_t{ 1 }; else ++Refs; }
		inline void _dec() noexcept {
			if( Refs ) { if( --Refs == 0 ) { Value->die(); Value = nullptr; delete Refs; Refs = nullptr; } } }

	private:
		size_t* Refs{ nullptr };
		type::Object* Value{ nullptr };
	};
}
