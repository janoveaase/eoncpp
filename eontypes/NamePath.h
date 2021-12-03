#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>
#include <eonname/NameRef.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerStringActions( scope::Global& scope );


	/**************************************************************************
	 Eon NamePath type
	**************************************************************************/
	class NamePathType : public type::TypeDef
	{
	public:
		NamePathType() : TypeDef( name_namepath ) {}
		~NamePathType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "namepath" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( nameref() ); }
		type::Instance* instantiate( const nameref& value ) const;
		type::Instance* instantiate( nameref&& value ) const;
	};


	/**************************************************************************
	 Eon NamePath instance
	**************************************************************************/
	class NamePathInstance : public type::Instance
	{
	public:
		NamePathInstance() : Instance( name_namepath ) {}
		NamePathInstance( const nameref& value ) : Instance( name_namepath ) { Value = value; }
		NamePathInstance( nameref&& value ) : Instance( name_namepath ) { Value = std::move( value ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (NamePathType*)scope.find( name_namepath ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( nameref ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( Value.str() ); }
		inline Instance* copy() const override { return new NamePathInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const NamePathInstance*)&other; return Value.compare( o.Value ); }

		inline const nameref& value() const noexcept { return Value; }
		inline void value( const nameref& val ) noexcept { Value = val; }
		inline void value( nameref&& val ) noexcept { Value = std::move( val ); }

	private:
		nameref Value;
	};
}
