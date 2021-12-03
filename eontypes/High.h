#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerHigh( scope::Global& scope );


	/**************************************************************************
	  Eon high type
	**************************************************************************/
	class HighType : public type::TypeDef
	{
	public:
		HighType() : TypeDef( name_high ) {}
		~HighType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "high" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( 0.0 ); }
		type::Instance* instantiate( high_t value ) const;
		inline type::Instance* instantiate( double value ) const { return instantiate( static_cast<high_t>( value ) ); }
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<high_t>( value ) ); }
	};


	/**************************************************************************
	  Eon high instance
	**************************************************************************/
	class HighInstance : public type::Instance
	{
	public:
		HighInstance() : Instance( name_high ) {}
		HighInstance( high_t value ) : Instance( name_high ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (HighType*)scope.find( name_high ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( high_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new HighInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const HighInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const high_t& value() const noexcept { return Value; }
		inline void value( high_t val ) noexcept { Value = val; }

	private:
		high_t Value{ 0.0 };
	};
}
