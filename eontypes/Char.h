#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerChar( scope::Global& scope );


	/**************************************************************************
	  Eon character type
	**************************************************************************/
	class CharType : public type::TypeDef
	{
	public:
		CharType() : TypeDef( name_char, source::Ref() ) {}
		~CharType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( Stringifier& str ) const override { str.pushWord( "char" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<char_t>( 0 ) ); }
		type::Instance* instantiate( char_t value ) const;
	};


	/**************************************************************************
	  Eon character instance
	**************************************************************************/
	class CharInstance : public type::Instance
	{
	public:
		CharInstance() : Instance( EonType( name_char ), source::Ref() ) {}
		CharInstance( char_t value, source::Ref source ) : Instance( EonType( name_char ), source ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new CharInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( char_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		void str( Stringifier& str ) const override;
		inline Instance* copy() const override { return new CharInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const CharInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const char_t& value() const noexcept { return Value; }
		inline void value( char_t val ) noexcept { Value = val; }

	private:
		char_t Value{ 0 };
	};
}
