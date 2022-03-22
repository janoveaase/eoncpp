#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include "Expression.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerExpression( scope::Global& scope );


	/**************************************************************************
	 Eon Expression type
	**************************************************************************/
	class ExprType : public type::TypeDef
	{
	public:
		ExprType() : TypeDef( name_regex, source::Ref() ) {}
		~ExprType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "regex" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( type::Expression() ); }
		type::Instance* instantiate( const type::Expression& value ) const;
		type::Instance* instantiate( type::Expression&& value ) const;
	};


	/**************************************************************************
	 Eon Expression instance
	**************************************************************************/
	class ExprInstance : public type::Instance
	{
	public:
		ExprInstance() : Instance( name_regex, source::Ref() ) {}
		ExprInstance( const type::Expression& value, source::Ref source ) : Instance( name_regex, source ) { Value = value; }
		ExprInstance( type::Expression&& value, source::Ref source ) : Instance( name_regex, source ) { Value = std::move( value ); }
		ExprInstance( const string& value, source::Ref source ) : Instance( name_regex, source ) {
			Value = type::Expression( value.substr() ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new ExprInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( type::Expression ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { Value.str( str ); }
		inline Instance* copy() const override { return new ExprInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const ExprInstance*)&other; return Value.compare( o.Value ); }

		inline const type::Expression& value() const noexcept { return Value; }
		inline void value( const type::Expression& val ) noexcept { Value = val; }
		inline void value( type::Expression&& val ) noexcept { Value = std::move( val ); }

	private:
		type::Expression Value;
	};
}
