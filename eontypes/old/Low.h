#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register low presicion lowing-point and actions in the global scope
	void registerLow( scope::Global& scope );


	/**************************************************************************
	  Eon low type
	**************************************************************************/
	class LowType : public type::TypeDef
	{
	public:
		LowType() : TypeDef( name_low ) {}
		~LowType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "low" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( 0.0 ); }
		type::Instance* instantiate( low_t value ) const;
		inline type::Instance* instantiate( double value ) const { return instantiate( static_cast<low_t>( value ) ); }
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<low_t>( value ) ); }
	};


	/**************************************************************************
	  Eon low instance
	**************************************************************************/
	class LowInstance : public type::Instance
	{
	public:
		LowInstance() : Instance( name_low ) {}
		LowInstance( low_t value ) : Instance( name_low ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new LowInstance( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( low_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new LowInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const LowInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0;
		}

		inline const low_t& value() const noexcept { return Value; }
		inline void value( low_t val ) noexcept { Value = val; }

	private:
		low_t Value{ 0.0 };
	};
}
