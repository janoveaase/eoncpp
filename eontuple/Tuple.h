#pragma once

#include "Value.h"
#include "Path.h"
#include <unordered_map>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;




	/**************************************************************************
	  Eon Tuple Class - eon::tuple

	  Holds an actual tuple.
	**************************************************************************/
	class tuple
	{
		/**********************************************************************
		  Defintions
		**********************************************************************/
	public:

		//* Tuples can exist in multiple forms
		enum class form
		{
			plain,
			metadata
		};




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		tuple() = default;
		inline tuple( form tuple_form ) { Form = tuple_form; }
		tuple( form tuple_form,
			std::initializer_list<name_t> name_attributes );
		inline tuple( const tuple& other ) { *this = other; }
		inline tuple( tuple&& other ) noexcept {
			*this = std::move( other ); }

		~tuple() { clear(); }


		tuple& operator=( const tuple& other );
		inline tuple& operator=( tuple&& other ) noexcept {
			Attributes = std::move( other.Attributes ); Named = std::move(
				other.Named ); Form = other.Form; other.Form = form::plain;
			return *this; }



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if there are any attributes at all
		inline bool empty() const noexcept { return Attributes.empty(); }

		//* Get number of attributes (Primitives will have only one.)
		inline size_t numAttributes() const noexcept {
			return Attributes.size(); }

		//* A 'false' tuple object is an empty one
		inline operator bool() const noexcept { return !empty(); }

		//* Get a const attribute by position
		//* Returns 'false' if invalid position.
		inline const tup::valueptr attribute( size_t pos ) const noexcept {
			return pos < Attributes.size()
				? Attributes[ pos ]->Value : nullptr; }
		inline const tup::valueptr attribute( int pos ) const noexcept {
			return attribute( static_cast<size_t>( pos ) ); }

		//* Check if the attribute at a given position is named
		//* Returns false if not named or position is invalid.
		inline bool isNamed( size_t pos ) const noexcept {
			return name( pos ) != no_name; }

		//* Get name of attribute identified by position
		//* Returns no_name if unnamed or position is invalid.
		inline name_t name( size_t pos ) const noexcept { return
			pos < Attributes.size() ? Attributes[ pos ]->Name : no_name; }

		//* Get a const attribute by name
		//* Returns 'false' if no attribute with that name.
		inline const tup::valueptr attribute( name_t name ) const noexcept {
			auto found = Named.find( name ); return found != Named.end()
				? found->second->Value : nullptr; }

		//* Check if this tuple contains a value matching the specified value
		bool contains( name_t value ) const noexcept;

		//* Get attribute at specified path
		//* Returns 'false' if the path does not lead to an existing attribute.
		inline const tup::valueptr get( const tup::path& path )
			const noexcept { return _get( path, 0 ); }

		//* Get a const attribute or sub-tuple's const attribute
		//* Specify path by listing attribute names.
		//* Returns 'false' if the path does not lead to an existing attribute.
		inline const tup::valueptr get( std::initializer_list<name_t> path )
			const noexcept { return _get( tup::path( path ), 0 ); }

		//* Get document (the top most parent that still has a parent)
		//* Returns nullptr if 'this' has not parent.
		inline const tuple* document() const noexcept {
			return Parent == nullptr || Parent->Parent == nullptr
				? Parent : Parent->document(); }

		//* Get full path to a named attribute of this tuple
		//* Returns 'false' if 'attribute_name' isn't an attribute.
		tup::path path( const tuple* subtuple = nullptr ) const noexcept;


		//* Stringify the tuple
		enum class perm
		{
			allow_oneliner = 0x01,
			allow_multiliner = 0x02
		};
		friend perm operator|( perm a, perm b ) noexcept {
			return static_cast<perm>( static_cast<int>( a )
				| static_cast<int>( b ) ); }
		friend perm operator&( perm a, perm b ) noexcept {
			return static_cast<perm>( static_cast<int>( a )
				& static_cast<int>( b ) ); }
		inline string str() const noexcept {
			size_t pos_on_line = 0; return str( pos_on_line, 0,
				Parent == nullptr || Parent->Parent == nullptr
					? perm::allow_multiliner : perm::allow_oneliner
				| perm::allow_multiliner ); }
		string str( size_t& pos_on_line, size_t indentation_level,
			perm permissions ) const noexcept;


		// Run internal validation (using meta data associated with named
		// attributes)
		void validate() const;

		//* Pattern tuples can validate other tuples according to the pattern
		//* definition.
		void validate( const tuple& other );




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		//* Set attribute value by position
		//* Throws [eon::tup::WrongType] if meta data defines a different
		//* type, or if the attribute value already exists with a different
		//* type - or if the 'attribute_pos' is more then 1 past the last
		//* existing attribute.
		void set( size_t attribute_pos, const tup::valueptr& value );

		//* Append a new attribute
		//* Throws [eon::tup::DuplicateName] if another attribute already
		//* exists with the same name.
		void append( const tup::valueptr& value, name_t name = no_name,
			const tupleptr& metadata = tupleptr ());

		//* Get a modifiable attribute by position
		//* Returns 'false' if invalid position.
		inline tup::valueptr attribute( size_t pos ) noexcept { return pos
			< Attributes.size() ? Attributes[ pos ]->Value : nullptr; }
		inline tup::valueptr attribute( int pos ) noexcept {
			return attribute( static_cast<size_t>( pos ) ); }

		//* Get a modifiable attribute by name
		//* Returns 'false' if no attribute with that name.
		inline tup::valueptr attribute( name_t name ) noexcept {
			auto found = Named.find( name ); return found != Named.end()
				? found->second->Value : nullptr; }

		//* Get a modifiable attribute or sub-tuple's attribute
		//* Returns 'false' if the path does not lead to an existing attribute.
		inline tup::valueptr get( const tup::path& path ) noexcept {
			return _get( path, 0 ); }

		//* Get a modifiable attribute or sub-tuple's attribute
		//* Specify path by listing attribute names.
		// Returns 'false' if the path does not lead to an existing attribute.
		inline tup::valueptr get( std::initializer_list<name_t> path )
			noexcept { return _get( tup::path( path ), 0 ); }

		//* Clear all attributes
		inline void clear() noexcept { Attributes.clear(); Named.clear(); }



		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		//* Two tuple objects are 'equal' if all attributes have the
		//* same names, types and values.
		bool equal( const tuple& other ) const noexcept;

		//* The 'other' tuple object is compatible with 'this' if all
		//* the named attributes have the same type and all the unnamed
		//* attributes have the same type for the same position.
		bool compatible( const tuple& other ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		//* Get a const attribute or sub-tuple's const attribute
		//* The 'path' argument must be a tuple of (unnamed) 'name'
		//* attributes, where each specifies an attribute name along the
		//* path.
		//* The 'pos' argument is the position of the attribute to get for
		//* 'this' tuple.
		//* Returns 'false' if the path does not lead to an existing
		//* attribute.
		const tup::valueptr _get( const tup::path& path, size_t pos )
			const noexcept;


		void _validateMaxDepth( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const;

		void _validateMinLength( tup::valueptr meta, const tuple* meta_owner, const hex& value ) const;
		void _validateMinLength( tup::valueptr meta, const tuple* meta_owner, const string& value ) const;
		void _validateMinLength( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const;

		void _validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const hex& value ) const;
		void _validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const string& value ) const;
		void _validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const;

		void _validateMin( tup::valueptr meta, const tuple* meta_owner, int64_t value ) const;
		void _validateMin( tup::valueptr meta, const tuple* meta_owner, double value ) const;

		void _validateMax( tup::valueptr meta, const tuple* meta_owner, int64_t value ) const;
		void _validateMax( tup::valueptr meta, const tuple* meta_owner, double value ) const;

		void _validateFormat( tup::valueptr meta, const tuple* meta_owner, const string& value ) const;
		void _validateFormat( tup::valueptr meta, const tuple* meta_owner, const std::vector<string>& value ) const;

		void _validateFlags( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const;

		void _validateOptions( tup::valueptr meta, const tuple* meta_owner, name_t value ) const;

		void _mismatchingMeta( name_t attribute_name, const string& info ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	public:
		struct Attribute
		{
			Attribute() = default;
			Attribute( const Attribute& other ) { *this = other; }
			inline Attribute( Attribute&& other ) noexcept {
				*this = std::move( other ); }
			~Attribute() = default;

			Attribute& operator=( const Attribute& other );
			Attribute& operator=( Attribute&& other ) noexcept;

			name_t Name{ no_name };
			tupleptr MetaData;
			size_t Pos{ 0 };
			tup::valueptr Value;
		};
		using AttributePtr = std::shared_ptr<Attribute>;

	private:
		std::vector<AttributePtr> Attributes;
		std::unordered_map<name_t, AttributePtr> Named;
		form Form{ form::plain };
		tuple* Parent{ nullptr };

		friend class etf;
	};
}
