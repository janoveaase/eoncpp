#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerRegex( scope::Global& scope );


	/**************************************************************************
	 Eon Regex type
	**************************************************************************/
	class RegexType : public type::TypeDef
	{
	public:
		RegexType() : TypeDef( name_regex, source::Ref() ) {}
		~RegexType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.pushWord( "regex" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( regex() ); }
		type::Instance* instantiate( const regex& value ) const;
		type::Instance* instantiate( regex&& value ) const;
	};


	/**************************************************************************
	 Eon Regex instance
	**************************************************************************/
	class RegexInstance : public type::Instance
	{
	public:
		RegexInstance() : Instance( EonType( name_regex ), source::Ref() ) {}
		RegexInstance( const regex& value, source::Ref source ) : Instance( EonType( name_regex ), source ) {
			Value = value; }
		RegexInstance( regex&& value, source::Ref source ) : Instance( EonType( name_regex ), source ) {
			Value = std::move( value ); }
		RegexInstance( const string& value, source::Ref source ) : Instance( EonType( name_regex ), source ) {
			Value = regex( value.substr() ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new RegexInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( regex ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override {
			str.pushPrefix( "r\"" ).pushWord( Value.str() ).pushAppend( "\"" ); }
		inline Instance* copy() const override { return new RegexInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const RegexInstance*)&other; return Value.str().compare( o.Value.str() ); }

		inline const regex& value() const noexcept { return Value; }
		inline void value( const regex& val ) noexcept { Value = val; }
		inline void value( regex&& val ) noexcept { Value = std::move( val ); }

	private:
		regex Value;
	};
}
