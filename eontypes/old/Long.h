#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register 64 bit integer type and actions in the global scope
	void registerLong( scope::Global& scope );


	/**************************************************************************
	  Eon long type
	**************************************************************************/
	class LongType : public type::TypeDef
	{
	public:
		LongType() : TypeDef( name_long ) {}
		~LongType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "long" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<long_t>( 0 ) ); }
		type::Instance* instantiate( long_t value ) const;
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<long_t>( value ) ); }
	};


	/**************************************************************************
	  Eon long instance
	**************************************************************************/
	class LongInstance : public type::Instance
	{
	public:
		LongInstance() : Instance( name_long ) {}
		LongInstance( long_t value ) : Instance( name_long ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new LongInstance( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( long_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new LongInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const LongInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const long_t& value() const noexcept { return Value; }
		inline void value( long_t val ) noexcept { Value = val; }

	private:
		long_t Value{ 0 };
	};
}
