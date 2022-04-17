#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBytes( scope::Global& scope );


	/**************************************************************************
	 Eon bytes type
	**************************************************************************/
	class BytesType : public type::TypeDef
	{
	public:
		BytesType() : TypeDef( name_bytes, source::Ref() ) {}
		~BytesType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.pushWord( "bytes" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( std::string() ); }
		type::Instance* instantiate( const std::string& value ) const;
		type::Instance* instantiate( std::string&& value ) const;
	};


	/**************************************************************************
	 Eon bytes instance
	**************************************************************************/
	class BytesInstance : public type::Instance
	{
	public:
		BytesInstance() : Instance( EonType( name_bytes ), source::Ref() ) {}
		BytesInstance( const std::string& value, source::Ref source ) : Instance( EonType( name_bytes ), source ) {
			Value = value; }
		BytesInstance( const EonType& type, std::string&& value, source::Ref source ) : Instance( type, source ) {
			Value = std::move( value ); }
		BytesInstance( const EonType& type, const string& value, source::Ref source ) : Instance( type, source ) {
			Value = value.stdstr(); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new BytesInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( std::string ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( Stringifier& str ) const override {
			str.pushPrefix( "b\"" ).pushWord( Value ).pushAppend( "\"" ); } // .escapeAll(); }
		inline Instance* copy() const override { return new BytesInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const BytesInstance*)&other; return Value.compare( o.Value ); }

		inline const std::string& value() const noexcept { return Value; }
		inline void value( const std::string& val ) noexcept { Value = val; }
		inline void value( std::string&& val ) noexcept { Value = std::move( val ); }

	private:
		std::string Value;
	};
}
