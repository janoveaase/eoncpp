#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerString( scope::Global& scope );


	/**************************************************************************
	 Eon string type
	**************************************************************************/
	class StringType : public type::TypeDef
	{
	public:
		StringType() : TypeDef( name_string, source::Ref() ) {}
		~StringType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.word( "string" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( string() ); }
		type::Instance* instantiate( const string& value ) const;
		type::Instance* instantiate( string&& value ) const;
	};


	/**************************************************************************
	 Eon string instance
	**************************************************************************/
	class StringInstance : public type::Instance
	{
	public:
		StringInstance() : Instance( EonType( name_string ), source::Ref() ) {}
		StringInstance( const string& value, source::Ref source ) : Instance( EonType( name_string ), source ) {
			Value = value; }
		StringInstance( const EonType& type, string&& value, source::Ref source ) : Instance( type, source ) {
			Value = std::move( value ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new StringInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( string ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override { str.doubleq( Value ); }
		inline Instance* copy() const override { return new StringInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const StringInstance*)&other; return Value.compare( o.Value ); }

		inline const string& value() const noexcept { return Value; }
		inline void value( const string& val ) noexcept { Value = val; }
		inline void value( string&& val ) noexcept { Value = std::move( val ); }

	private:
		string Value;
	};
}
