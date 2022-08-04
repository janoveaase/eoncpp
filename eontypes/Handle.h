#pragma once

#include "TypeSystem.h"
#include "Scope.h"


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
		HandleType() : TypeDef( name_handle, source::Ref() ) {}
		~HandleType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.word( "handle" ); }

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
		HandleInstance() : Instance( EonType( name_handle ), source::Ref() ) {}
		HandleInstance( handle_t value, source::Ref source ) : Instance( EonType( name_handle ), source ) {
			Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new HandleInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( handle_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override { str.word( string( Value ) ); }
		inline Instance* copy() const override { return new HandleInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const HandleInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const handle_t& value() const noexcept { return Value; }
		inline void value( handle_t val ) noexcept { Value = val; }

	private:
		handle_t Value{ 0 };
	};
}
