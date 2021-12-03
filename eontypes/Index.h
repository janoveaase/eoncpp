#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register index/size type and actions in the global scope
	void registerIndex( scope::Global& scope );


	/**************************************************************************
	  Eon index type
	**************************************************************************/
	class IndexType : public type::TypeDef
	{
	public:
		IndexType() : TypeDef( name_index ) {}
		~IndexType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "index" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override {
			return instantiate( static_cast<index_t>( 0 ) ); }
		type::Instance* instantiate( index_t value ) const;
		inline type::Instance* instantiate( int value ) const { return instantiate( static_cast<index_t>( value ) ); }
	};


	/**************************************************************************
	  Eon index instance
	**************************************************************************/
	class IndexInstance : public type::Instance
	{
	public:
		IndexInstance() : Instance( name_index ) {}
		IndexInstance( index_t value ) : Instance( name_index ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (IndexType*)scope.find( name_index ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( index_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( string( Value ) ); }
		inline Instance* copy() const override { return new IndexInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const IndexInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const index_t& value() const noexcept { return Value; }
		inline void value( index_t val ) noexcept { Value = val; }

	private:
		index_t Value{ 0 };
	};
}
