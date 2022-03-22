#pragma once

#include "TypeSystem.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerByte( scope::Global& scope );


	/**************************************************************************
	  Eon byte type
	**************************************************************************/
	class ByteType : public type::TypeDef
	{
	public:
		ByteType() : TypeDef( name_byte, source::Ref() ) {}
		~ByteType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "byte" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<byte_t>( 0 ) ); }
		type::Instance* instantiate( byte_t value ) const;
		inline type::Instance* instantiate( char value ) const { return instantiate( static_cast<byte_t>( value ) ); }
	};


	/**************************************************************************
	  Eon byte instance
	**************************************************************************/
	class ByteInstance : public type::Instance
	{
	public:
		ByteInstance() : Instance( EonType( name_byte ), source::Ref() ) {}
		ByteInstance( byte_t value, source::Ref source ) : Instance( EonType( name_byte ), source ) { Value = value; }

		inline void die() override { delete this; }
		inline void callDestructor() override {}
		inline Object* copy() override { return new ByteInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( byte_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		void str( type::Stringifier& str ) const override;
		inline Instance* copy() const override { return new ByteInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const ByteInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const byte_t& value() const noexcept { return Value; }
		inline void value( byte_t val ) noexcept { Value = val; }

	private:
		byte_t Value{ 0 };
	};
}
