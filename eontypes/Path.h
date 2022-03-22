#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <eonfilesys/Path.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerStringActions( scope::Global& scope );


	/**************************************************************************
	 Eon Path type
	**************************************************************************/
	class PathType : public type::TypeDef
	{
	public:
		PathType() : TypeDef( name_path, source::Ref() ) {}
		~PathType() = default;

		void die() override {}
		void callDestructor() override {}
		Object* copy() override { throw type::AccessDenied( "Cannot copy type object!" ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }
		inline void str( type::Stringifier& str ) const override { str.addWord( "path" ); }

		inline type::Instance* instantiate( type::Node* args = nullptr ) const override { return instantiate( path() ); }
		type::Instance* instantiate( const path& value ) const;
		type::Instance* instantiate( path&& value ) const;
	};


	/**************************************************************************
	 Eon Path instance
	**************************************************************************/
	class PathInstance : public type::Instance
	{
	public:
		PathInstance() : Instance( EonType( name_path ), source::Ref() ) {}
		PathInstance( const path& value, source::Ref source ) : Instance( EonType( name_path ), source ) {
			Value = value; }
		PathInstance( path&& value, source::Ref source ) : Instance( EonType( name_path ), source ) {
			Value = std::move( value ); }

		inline void die() override { delete this; }
		void callDestructor() override {}
		inline Object* copy() override { return new PathInstance( Value, source() ); }
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( path ) ); }
		inline void* rawValue() const noexcept override { return (void*)&Value; }
		inline void str( type::Stringifier& str ) const override {
			str.addRaw( "p\"" ); str.addWord( Value.str() ); str.addRaw( "\"" ); }
		inline Instance* copy() const override { return new PathInstance( Value, source() ); }
		inline int compare( const Instance& other ) const noexcept override {
			auto& o = *(const PathInstance*)&other; return Value.compare( o.Value ); }

		inline const path& value() const noexcept { return Value; }
		inline void value( const path& val ) noexcept { Value = val; }
		inline void value( path&& val ) noexcept { Value = std::move( val ); }

	private:
		path Value;
	};
}
