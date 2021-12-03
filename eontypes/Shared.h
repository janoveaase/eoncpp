#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register name type and actions in the global scope
	void registerName( scope::Global& scope );


	/**************************************************************************
	  Eon shared object
	**************************************************************************/
	class shared
	{
	public:
		shared() = default;
		inline shared( type::Object* value ) noexcept { Value = value; _inc(); }
		inline shared( shared& other ) noexcept { Refs = other.Refs; Value = other.Value; if( Refs ) _inc(); }
		inline ~shared() { _dec(); }

		inline shared& operator=( shared& other ) noexcept {
			_dec(); Refs = other.Refs; Value = other.Value; if( Refs ) _inc(); return *this; }

		inline void reset() noexcept { _dec(); Refs = nullptr; Value = nullptr; }

		inline type::Object* value() noexcept { return Value; }
		inline size_t refs() const noexcept { return Refs ? *Refs : 0; }

	private:
		inline void _inc() noexcept { if( !Refs ) Refs = new size_t{ 1 }; else ++Refs; }
		inline void _dec() noexcept {
			if( Refs ) { if( --Refs == 0 ) { Value->die(); Value = nullptr; delete Refs; Refs = nullptr; } } }

	private:
		size_t* Refs{ nullptr };
		type::Object* Value{ nullptr };
	};


	/**************************************************************************
	  Eon shared type
	**************************************************************************/
	class SharedType : public type::TypeDef
	{
	public:
		SharedType() : TypeDef( name_shared ) {}
		~SharedType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy( scope::Scope& scope ) override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "shared" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( shared() ); }
		type::Instance* instantiate( shared value ) const;
	};


	/**************************************************************************
	  Eon shared instance
	**************************************************************************/
	class SharedInstance : public type::Instance
	{
	public:
		SharedInstance() : Instance( name_shared ) {}
		SharedInstance( shared value ) : Instance( name_shared ) { Value = value; }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy( scope::Scope& scope ) override {
			return ( (SharedType*)scope.find( type().asName() ) )->instantiate( Value ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( name_t ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override { str.addWord( Value ); }
		inline Instance* copy() const override { return new SharedInstance( Value ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const SharedInstance*)&other; return Value < o.Value ? -1 : o.Value < Value ? 1 : 0; }

		inline const shared& value() const noexcept { return Value; }
		inline void value( const shared& val ) noexcept { Value = val; }

	private:
		shared Value;
	};
}
