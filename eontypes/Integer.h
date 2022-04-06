#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <typeinfo>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	template<typename int_type> class IntegerType;
	template<typename int_type> class IntegerInstance;
	
	void registerIntegerActions( scope::Global& scope, const std::type_info& type_id );
	
	
	//* Register integer type and actions in the global scope
	template<typename int_type>
	void registerInteger( scope::Global& scope )
	{
		scope.add( type::Handler::mapType<int_type>(), new IntegerType<int_type>() );
		registerIntegerActions( scope, typeid( int_type ) );
	}


	/**************************************************************************
	  Eon integer type
	**************************************************************************/
	template<typename int_type>
	class IntegerType : public type::TypeDef
	{
	public:
		IntegerType() : TypeDef( type::Handler::mapType<int_type>(), source::Ref() ) {}
		~IntegerType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( type::Handler::mapType<int_type>() ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<int_type>( 0 ) ); }
		inline type::Instance* instantiate( int_type value ) const {
			return new IntegerInstance<int_type>( value, source::Ref() ); }
	};


	/**************************************************************************
	  Eon integer instance
	**************************************************************************/
	template<typename int_type>
	class IntegerInstance : public type::Instance
	{
	public:
		IntegerInstance() : Instance( type::Handler::mapType<int_type>(), source::Ref() ) {}
		IntegerInstance( int_type value, source::Ref source ) : Instance( type::Handler::mapType<int_type>(), source ) {
			Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new IntegerInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( int_type ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new IntegerInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const IntegerInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const int_type& value() const noexcept { return Value; }
		inline void value( int_type val ) noexcept { Value = val; }

	private:
		int_type Value{ 0 };
	};
}
