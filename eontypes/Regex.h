#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerStringActions( scope::Global& scope );


	/**************************************************************************
	 Eon Regex type
	**************************************************************************/
	class RegexType : public type::TypeDef
	{
	public:
		RegexType() : TypeDef( name_regex ) {}
		~RegexType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "regex" ); }

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
		RegexInstance() : Instance( name_regex ) {}
		RegexInstance( const regex& value ) : Instance( name_regex ) { Value = value; }
		RegexInstance( regex&& value ) : Instance( name_regex ) { Value = std::move( value ); }
		RegexInstance( const string& value ) : Instance( name_regex ) { Value = regex( value.substr() ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (RegexType*)scope.find( name_regex ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( regex ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( Value.str() ); }
		inline Instance* copy() const override { return new RegexInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const RegexInstance*)&other; return Value.str().compare( o.Value.str() ); }

		inline const regex& value() const noexcept { return Value; }
		inline void value( const regex& val ) noexcept { Value = val; }
		inline void value( regex&& val ) noexcept { Value = std::move( val ); }

	private:
		regex Value;
	};
}
