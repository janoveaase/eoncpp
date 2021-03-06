#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register name type and actions in the global scope
	void registerName( scope::Global& scope );


	/**************************************************************************
	  Eon name type
	**************************************************************************/
	class NameType : public type::TypeDef
	{
	public:
		NameType() : TypeDef( name_name, source::Ref() ) {}
		~NameType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.pushWord( "name" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( no_name ); }
		type::Instance* instantiate( name_t value ) const;
	};


	/**************************************************************************
	  Eon name instance
	**************************************************************************/
	class NameInstance : public type::Instance
	{
	public:
		NameInstance() : Instance( EonType( name_name ), source::Ref() ) {}
		NameInstance( name_t value, source::Ref source ) : Instance( EonType( name_name ), source ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new NameInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( name_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override { str.pushWord( eon::str( Value ) ); }
		inline Instance* copy() const override { return new NameInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const NameInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const name_t& value() const noexcept { return Value; }
		inline void value( name_t val ) noexcept { Value = val; }

	private:
		name_t Value{ no_name };
	};
}
