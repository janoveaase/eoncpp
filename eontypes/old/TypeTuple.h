#pragma once

#include <eonstring/Name.h>
#include <eonsource/SourceRef.h>
#include "TypeDefinitions.h"
#include "Stringifier.h"
#include <vector>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class TypeElement
	{
	public:
		virtual bool isNameElement() const noexcept { return false; }
		virtual bool isTupleElement() const noexcept { return false; }
		virtual bool isEllipsisElement() const noexcept { return false; }

		virtual bool compatibleWith( const TypeElement& other ) const noexcept = 0;

		inline void name( name_t new_name ) noexcept { Name = new_name; }

	public:
		name_t Name{ no_name };
	};

	class NameElement : public TypeElement
	{
	public:
		NameElement() = default;
		inline NameElement( name_t value ) noexcept { Value = value; }
		inline NameElement( name_t name, name_t value ) noexcept { Name = name; Value = value; }
		inline NameElement( const NameElement& other ) noexcept { Name = other.Name; Value = other.Value; }
		~NameElement() = default;

		inline bool isNameElement() const noexcept override { return true; }
		bool compatibleWith( const TypeElement& other ) const noexcept override {
			return other.isNameElement() ? Value == ( (NameElement*)&other )->Value : false; }

	public:
		name_t Value{ no_name };
	};

	class EllipsisElement : public TypeElement
	{
	public:
		EllipsisElement() = default;
		inline EllipsisElement( name_t type ) { Type = type; }
		~EllipsisElement() = default;

		inline bool isEllipsisElement() const noexcept override { return true; }
		bool compatibleWith( const TypeElement& other ) const noexcept override { return false; }

	public:
		name_t Type{ no_name };
	};




	/**************************************************************************
	  TypeTuple class - eon::TypeTuple

	  A type tuple object is a tuple of name (and type tuple) attributes
	  identifying a type.
	**************************************************************************/
	class TypeTuple : public TypeElement
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		TypeTuple() = default;

		inline TypeTuple( const TypeTuple& other ) { *this = other; }
		inline TypeTuple( TypeTuple&& other ) noexcept { *this = std::move( other ); }

		enum class own_attributes
		{
			no,
			yes
		};
		inline TypeTuple( std::vector<TypeElement*> attributes, name_t tuple_type, own_attributes own = own_attributes::no ) {
			TupleType = tuple_type; for( auto elm : attributes ) _add( elm ); _makeName(); OwnElements = own; }

		inline TypeTuple( name_t type_name ) {
			if( type_name != no_name ) Elements.push_back( new NameElement( type_name ) ); _makeName(); }
		inline TypeTuple( std::initializer_list<name_t> names ) {
			for( auto name : names ) { if( name != no_name ) Elements.push_back( new NameElement( name ) ); } _makeName(); }

		inline TypeTuple( std::initializer_list<TypeTuple> args ) { for( auto& arg : args ) _add( new TypeTuple( arg ) ); }

		inline TypeTuple( const TypeTuple& other, name_t new_name ) { *this = other; Name = new_name; }

		inline TypeTuple( std::initializer_list<TypeElement*> elements ) {
			for( auto elm : elements ) _add( elm ); _makeName(); }
		inline TypeTuple( std::vector<TypeElement*> elements ) { for( auto elm : elements ) _add( elm ); _makeName(); }

		// Destruct tuple
		virtual ~TypeTuple() { if( OwnElements == own_attributes::yes ) { for( auto elm : Elements ) delete elm; } }




		/**********************************************************************
		  Factory Construction
		**********************************************************************/
	public:

		static inline TypeTuple tuple( name_t tuple_type ) { return TypeTuple( std::vector<TypeElement*>(), tuple_type ); }
		static inline TypeTuple tuple( name_t tuple_type, std::vector<TypeElement*> attributes ) {
			return TypeTuple( attributes, tuple_type ); }

		static inline TypeTuple name( name_t type_name ) {
			return type::isTupleType( type_name ) ? tuple( type_name ) : TypeTuple( type_name ); }
		static inline TypeTuple name( std::initializer_list<name_t> names ) { return TypeTuple( names ); }

		static inline TypeTuple args( std::initializer_list<TypeTuple> arg_types ) { return TypeTuple( arg_types ); }

		static inline TypeTuple rename( const TypeTuple& other, name_t new_name ) { return TypeTuple( other, new_name ); }

		static inline TypeTuple type( std::initializer_list<TypeElement*> elements ) { return TypeTuple( elements ); }
		static inline TypeTuple type( std::vector<TypeElement*> elements ) { return TypeTuple( elements ); }




		/**********************************************************************
		  Modifiers
		**********************************************************************/
	public:

		inline TypeTuple& operator=( name_t value ) {
			TupleType = no_name; Name = no_name; Elements.clear(); Elements.push_back( new NameElement( value ) );
			_makeName(); return *this; }
		inline TypeTuple& operator=( NameElement* element ) {
			TupleType = no_name; Name = no_name; Elements.clear(); _add( element ); _makeName(); return *this; }
		inline TypeTuple& operator=( std::initializer_list<TypeElement*> elements ) {
			TupleType = no_name; Name = no_name; for( auto& elm : elements ) _add( elm ); _makeName(); return *this; }
		inline TypeTuple& operator=( std::initializer_list<name_t> names ) {
			TupleType = no_name; Name = no_name; Elements.clear();
			for( auto name : names ) Elements.push_back( new NameElement( name ) ); _makeName(); return *this; }
		inline TypeTuple& operator=( const TypeTuple& other ) {
			TupleType = other.TupleType; Name = other.Name; Named = other.Named; Elements = other.Elements;
			AsName = other.AsName; return *this; }
		inline TypeTuple& operator=( TypeTuple&& other ) noexcept {
			TupleType = other.TupleType; other.TupleType = no_name; Name = other.Name; other.Name = no_name;
			Named = std::move( other.Named ); Elements = std::move( other.Elements ); AsName = other.AsName;
			other.AsName = no_name; return *this; }

		inline TypeTuple& operator<<( name_t value ) {
			Elements.push_back( new NameElement( value ) ); _makeName(); return *this; }
		inline TypeTuple& operator<<( NameElement* element ) { _add( element ); _makeName(); return *this; }
		inline TypeTuple& operator<<( const TypeTuple& other ) {
			for( auto& elm : other.Elements ) _add( elm ); _makeName(); return *this; }
		inline TypeTuple& operator<<( TypeElement* element ) { _add( element ); _makeName(); return *this; }




		/**********************************************************************
		  Basic Access
		**********************************************************************/
	public:

		//* Identify this as a type tuple
		inline bool isTupleElement() const noexcept override { return true; }

		//* Check if the type tuple is just a single name
		inline bool isName() const noexcept { return Elements.size() == 1 && Elements[ 0 ]->isNameElement(); }

		//* Check if the type tuple is just a single name, matching the one given
		inline bool isName( name_t name ) const noexcept {
			return isName() && ((NameElement*)( Elements[ 0 ] ))->Value == name; }

		//* Check if the type tuple is for a tuple (any type)
		inline bool isTuple() const noexcept { return TupleType != no_name; }

		//* Get the general tuple type that this type tuple is for
		//* Returns no_name if not for a tuple!
		inline name_t tupleType() const noexcept { return TupleType; }

		//* Get type tuple as name
		inline name_t asName() const noexcept { return AsName; }

		//* Check if this type has any elements
		inline operator bool() const noexcept { return !Elements.empty(); }

		//* Get type tuple as string representation
		void str( type::Stringifier& str ) const;
		inline string str() const { type::Stringifier strf; str( strf ); return strf.output(); }

		//* Get source
		inline const source::Ref& source() const noexcept { return Source; }

		//* Set source
		inline void source( source::Ref source ) noexcept { Source = std::move( source ); }




		/**********************************************************************
		  Iteration
		**********************************************************************/
	public:

		using iterator = std::vector<TypeElement*>::const_iterator;

		inline iterator begin() const noexcept { return Elements.begin(); }
		inline iterator end() const noexcept { return Elements.end(); }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		inline int compare( const TypeTuple& other ) const noexcept {
			return AsName < other.AsName ? -1 : other.AsName < AsName ? 1 : 0; }

		inline bool operator<( const TypeTuple& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const TypeTuple& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const TypeTuple& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const TypeTuple& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const TypeTuple& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const TypeTuple& other ) const noexcept { return compare( other ) != 0; }

		//* Check if types are compatible
		//* NOTE: Tuple types are not compared!
		bool compatibleWith( const TypeTuple& other ) const noexcept;
		bool compatibleWith( const TypeElement& other ) const noexcept override {
			if( isName() && other.isNameElement() )
				return asName() == ( (NameElement*)&other )->Value;
			return other.isTupleElement() ? compatibleWith( *( (TypeTuple*)&other ) ) : false; }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _makeName();
		string _makeNameStr() const;

		void _add( TypeElement* element );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:

		name_t TupleType{ no_name };
		std::vector<TypeElement*> Elements;
		own_attributes OwnElements{ own_attributes::no };
		std::unordered_map<name_t, size_t> Named;
		name_t AsName{ no_name };
		source::Ref Source;
	};
}
