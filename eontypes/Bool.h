#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBool( scope::Global& scope );


	/**************************************************************************
	  Eon boolean type
	**************************************************************************/
	class BoolType : public type::TypeDef
	{
	public:
		BoolType() : TypeDef( name_bool, source::Ref() ) {}
		~BoolType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.pushWord( "bool" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( false ); }
		type::Instance* instantiate( bool value ) const;
	};


	/**************************************************************************
	  Eon boolean instance
	**************************************************************************/
	class BoolInstance : public type::Instance
	{
	public:
		BoolInstance() : Instance( EonType( name_bool ), source::Ref() ) {}
		BoolInstance( bool value, source::Ref source ) : Instance( EonType( name_bool ), source ) { Value = value; }

		inline void die() override { delete this; }
		inline void callDestructor() override {}
		inline Object* copy() override { return new BoolInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( bool ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override { str.pushWord( Value ? "true" : "false" ); }
		inline Instance* copy() const override { return new BoolInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const BoolInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const bool& value() const noexcept { return Value; }
		inline void value( bool val ) noexcept { Value = val; }

	private:
		bool Value{ false };
	};
}
