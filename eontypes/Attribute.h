#pragma once

#include "TypeSystem.h"
#include "TypeHandler.h"
#include <typeinfo>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class MetaData;
	class Object;

	namespace type
	{
		/**********************************************************************
		  Attribute qualifier defintions - eon::Attribute::Qualifier
		**********************************************************************/
		enum class Qualifier : uint8_t
		{
			undef,

			read,			//* Value is immutable pointer to external memory
			modify,			//* Value is mutable pointer to external memory
			consume,		//* Value is mutable pointer to local memory
		};
		inline bool operator&( Qualifier a, Qualifier b ) noexcept {
			return ( static_cast<int>( a ) & static_cast<int>( b ) ) != 0; }
		extern Qualifier mapQualifier( string qual );
		extern string mapQualifier( Qualifier qual );


		/**********************************************************************
		  Literal attribute for normal tuples
		**********************************************************************/
		class Attribute
		{
		public:
			Attribute() = default;
			inline Attribute( Object* value ) { if( value ) Type = value->type(); assign( value ); }
			inline Attribute( const TypeTuple& type, Object* value = nullptr ) { Type = type; assign( value ); }
			inline Attribute( name_t name, Object* value = nullptr ) {
				Name = name; if( value ) Type = value->type(); assign( value ); }
			Attribute( name_t name, MetaData&& meta, Object* value = nullptr );
			inline Attribute( name_t name, const TypeTuple& type, Object* value = nullptr ) {
				Name = name; Type = type; assign( value ); }
			inline Attribute( name_t name, name_t type, Object* value = nullptr ) {
				Name = name; Type = TypeTuple( { type } ); assign( value ); }
			Attribute( name_t name, const TypeTuple& type, MetaData&& meta, Object* value = nullptr );
			Attribute( name_t name, name_t type, MetaData&& meta, Object* value = nullptr );
			inline Attribute( Qualifier qual, Object* value = nullptr ) {
				Qual = qual; if( value ) Type = value->type(); assign( value ); }
			inline Attribute( const TypeTuple& type, Qualifier qual, Object* value = nullptr ) {
				Type = type; Qual = qual; assign( value ); }
			inline Attribute( name_t name, Qualifier qual, Object* value = nullptr ) {
				Name = name; Qual = qual; if( value ) Type = value->type(); assign( value ); }
			Attribute( name_t name, MetaData&& meta, Qualifier qual, Object* value = nullptr );
			inline Attribute( name_t name, const TypeTuple& type, Qualifier qual, Object* value = nullptr ) {
				Name = name; Type = type; Qual = qual; assign( value ); }
			inline Attribute( name_t name, name_t type, Qualifier qual, Object* value = nullptr ) {
				Name = name; Type = TypeTuple( { type } ); Qual = qual; assign( value ); }
			Attribute( name_t name, const TypeTuple& type, MetaData&& meta, Qualifier qual, Object* value = nullptr );
			Attribute( name_t name, name_t type, MetaData&& meta, Qualifier qual, Object* value = nullptr );
			~Attribute();

			inline Qualifier qualifier() const noexcept { return Qual; }
			inline name_t name() const noexcept { return Name; }
			inline const MetaData* meta() const noexcept { return Meta; }
			inline const TypeTuple& type() const noexcept { return Type; }
			inline Object* value() const noexcept { return Value; }

			virtual void assign( Object* value ) { if( value ) {
				if( value->type() == Type ) Value = value; else throw IncompatibleType(); } }

		protected:
			Qualifier Qual{ Qualifier::read };
			name_t Name{ no_name };
			MetaData* Meta{ nullptr };
			TypeTuple Type;
			Object* Value{ nullptr };
		};
	}
}
