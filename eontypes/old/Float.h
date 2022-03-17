#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerFloat( scope::Global& scope );


	/**************************************************************************
	  Eon float type
	**************************************************************************/
	class FloatType : public type::TypeDef
	{
	public:
		FloatType() : TypeDef( name_float ) {}
		~FloatType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "float" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( 0.0 ); }
		type::Instance* instantiate( flt_t value ) const;
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<flt_t>( value ) ); }
	};


	/**************************************************************************
	  Eon float instance
	**************************************************************************/
	class FloatInstance : public type::Instance
	{
	public:
		FloatInstance() : Instance( name_float ) {}
		FloatInstance( flt_t value ) : Instance( name_float ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new FloatInstance( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( flt_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new FloatInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const FloatInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const flt_t& value() const noexcept { return Value; }
		inline void value( flt_t val ) noexcept { Value = val; }

	private:
		flt_t Value{ 0.0 };
	};
}
