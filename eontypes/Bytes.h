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
	 Eon bytes type
	**************************************************************************/
	class BytesType : public type::TypeDef
	{
	public:
		BytesType() : TypeDef( name_bytes ) {}
		~BytesType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "bytes" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( std::string() ); }
		type::Instance* instantiate( const std::string& value ) const;
		type::Instance* instantiate( std::string&& value ) const;
	};


	/**************************************************************************
	 Eon bytes instance
	**************************************************************************/
	class BytesInstance : public type::Instance
	{
	public:
		BytesInstance() : Instance( name_bytes ) {}
		BytesInstance( const std::string& value ) : Instance( name_bytes ) { Value = value; }
		BytesInstance( std::string&& value ) : Instance( name_bytes ) { Value = std::move( value ); }
		BytesInstance( const string& value ) : Instance( name_bytes ) { Value = value.stdstr(); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (BytesType*)scope.find( name_bytes ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( std::string ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( Value ); } // .escapeAll(); }
		inline Instance* copy() const override { return new BytesInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const BytesInstance*)&other; return Value.compare( o.Value ); }

		inline const std::string& value() const noexcept { return Value; }
		inline void value( const std::string& val ) noexcept { Value = val; }
		inline void value( std::string&& val ) noexcept { Value = std::move( val ); }

	private:
		std::string Value;
	};
}
