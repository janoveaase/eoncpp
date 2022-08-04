#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <eonstring/NamePath.h>


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
		NamePathType() : TypeDef( name_namepath, source::Ref() ) {}
		~NamePathType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.word( "namepath" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( namepath() ); }
		type::Instance* instantiate( const namepath& value ) const;
		type::Instance* instantiate( namepath&& value ) const;
	};


	/**************************************************************************
	 Eon NamePath instance
	**************************************************************************/
	class NamePathInstance : public type::Instance
	{
	public:
		NamePathInstance() : Instance( EonType( name_namepath ), source::Ref() ) {}
		NamePathInstance( const namepath& value, source::Ref source ) : Instance( EonType( name_namepath ), source ) {
			Value = value; }
		NamePathInstance( namepath&& value, source::Ref source ) : Instance( EonType( name_namepath ), source ) {
			Value = std::move( value ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new NamePathInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( namepath ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override { str.word( Value.str() ); }
		inline Instance* copy() const override { return new NamePathInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const NamePathInstance*)&other; return Value.compare( o.Value ); }

		inline const namepath& value() const noexcept { return Value; }
		inline void value( const namepath& val ) noexcept { Value = val; }
		inline void value( namepath&& val ) noexcept { Value = std::move( val ); }

	private:
		namepath Value;
	};
}
