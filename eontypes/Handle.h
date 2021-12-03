#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerHandle( scope::Global& scope );


	/**************************************************************************
	  Eon handle type
	**************************************************************************/
	class HandleType : public type::TypeDef
	{
	public:
		HandleType() : TypeDef( name_handle ) {}
		~HandleType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "handle" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<handle_t>( 0 ) ); }
		type::Instance* instantiate( handle_t value ) const;
	};


	/**************************************************************************
	  Eon handle instance
	**************************************************************************/
	class HandleInstance : public type::Instance
	{
	public:
		HandleInstance() : Instance( name_handle ) {}
		HandleInstance( handle_t value ) : Instance( name_handle ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (HandleType*)scope.find( name_handle ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( handle_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new HandleInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const HandleInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const handle_t& value() const noexcept { return Value; }
		inline void value( handle_t val ) noexcept { Value = val; }

	private:
		handle_t Value{ 0 };
	};
}
