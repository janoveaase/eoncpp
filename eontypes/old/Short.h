#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register 16 bit integer type and actions in the global scope
	void registerShort( scope::Global& scope );


	/**************************************************************************
	  Eon short type
	**************************************************************************/
	class ShortType : public type::TypeDef
	{
	public:
		ShortType() : TypeDef( name_short ) {}
		~ShortType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "short" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<short_t>( 0 ) ); }
		type::Instance* instantiate( short_t value ) const;
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<short_t>( value ) ); }
	};


	/**************************************************************************
	  Eon short instance
	**************************************************************************/
	class ShortInstance : public type::Instance
	{
	public:
		ShortInstance() : Instance( name_short ) {}
		ShortInstance( short_t value ) : Instance( name_short ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new ShortInstance( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( short ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new ShortInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const ShortInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const short_t& value() const noexcept { return Value; }
		inline void value( short_t val ) noexcept { Value = val; }

	private:
		short_t Value{ 0 };
	};
}
