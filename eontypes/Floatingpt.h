#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <typeinfo>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	template<typename float_type> class FloatingptType;
	template<typename float_type> class FloatingptInstance;
	
	void registerFloatingptActions( scope::Global& scope, const std::type_info& type_id );
	
	
	//* Register type and actions in the global scope
	template<typename float_type>
	void registerFloatintpt( scope::Global& scope )
	{
		scope.add( type::Handler::mapType<float_type>(), new FloatingptType<float_type>() );
		registerFloatingptActions( scope, typeid( float_type ) );
	}


	/**************************************************************************
	  Eon floating point type
	**************************************************************************/
	template<typename float_type>
	class FloatingptType : public type::TypeDef
	{
	public:
		FloatingptType() : TypeDef( name_float, source::Ref() ) {}
		~FloatingptType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override {
			str.pushWord( eon::str( type::Handler::mapType<float_type>() ) ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<float_type>( 0.0 ) ); }
		inline type::Instance* instantiate( float_type value ) const {
			return new FloatingptInstance<float_type>( value, source::Ref() ); }
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<float_type>( value ) ); }
	};


	/**************************************************************************
	  Eon floatint point instance
	**************************************************************************/
	template<typename float_type>
	class FloatingptInstance : public type::Instance
	{
	public:
		FloatingptInstance() : Instance( type::Handler::mapType<float_type>(), source::Ref() ) {}
		FloatingptInstance( float_type value, source::Ref source )
			: Instance( type::Handler::mapType<float_type>(), source ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new FloatingptInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( float_type ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override {
			str.pushWord( string( Value ) ); if( rawType() == typeid( low_t ) ) str.pushAppend( "L" ); else if( rawType()
				== typeid( high_t ) ) str.pushAppend( "H" ); }
		inline Instance* copy() const override { return new FloatingptInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const FloatingptInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const float_type& value() const noexcept { return Value; }
		inline void value( float_type val ) noexcept { Value = val; }

	private:
		float_type Value{ static_cast<float_type>( 0.0 ) };
	};
}
