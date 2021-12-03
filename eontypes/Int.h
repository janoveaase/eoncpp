#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register 32 bit integer type and actions in the global scope
	void registerInt( scope::Global& scope );


	/**************************************************************************
	  Eon integer type
	**************************************************************************/
	class IntType : public type::TypeDef
	{
	public:
		IntType() : TypeDef( name_int ) {}
		~IntType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "int" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<int_t>( 0 ) ); }
		type::Instance* instantiate( int_t value ) const;
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<int_t>( value ) ); }
	};


	/**************************************************************************
	  Eon integer instance
	**************************************************************************/
	class IntInstance : public type::Instance
	{
	public:
		IntInstance() : Instance( name_int ) {}
		IntInstance( int_t value ) : Instance( name_int ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (IntType*)scope.find( type().asName() ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( int_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override {
			return new IntInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const IntInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const int_t& value() const noexcept { return Value; }
		inline void value( int_t val ) noexcept { Value = val; }

	private:
		int_t Value{ 0 };
	};
}
