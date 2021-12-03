#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerStringActions( scope::Global& scope );


	/**************************************************************************
	 Eon string type
	**************************************************************************/
	class StringType : public type::TypeDef
	{
	public:
		StringType() : TypeDef( name_string ) {}
		~StringType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "string" ); }

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
		StringInstance() : Instance( name_string ) {}
		StringInstance( const string& value ) : Instance( name_string ) { Value = value; }
		StringInstance( string&& value ) : Instance( name_string ) { Value = std::move( value ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (StringType*)scope.find( name_string ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( string ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( "\"" + Value.escapeAll() + "\"" ); }
		inline Instance* copy() const override { return new StringInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const StringInstance*)&other; return Value.compare( o.Value ); }

		inline const string& value() const noexcept { return Value; }
		inline void value( const string& val ) noexcept { Value = val; }
		inline void value( string&& val ) noexcept { Value = std::move( val ); }

	private:
		string Value;
	};
}
