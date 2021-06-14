#pragma once

#include "Value.h"
#include "Path.h"
#include "ReferenceValue.h"
#include <unordered_map>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;

	EONEXCEPT( Incompatible );




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

		struct Attribute;
		using AttributePtr = std::shared_ptr<Attribute>;




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		tuple() = default;
		inline tuple( form tuple_form ) { Form = tuple_form; }
		tuple( form tuple_form, tup::variables& vars,
			std::initializer_list<name_t> name_attributes );
		inline tuple( const tuple& other ) { *this = other; }
		inline tuple( tuple&& other ) noexcept {
			*this = std::move( other ); }

		~tuple() { clear(); }



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
				? Attributes[ pos ]->Value : tup::valueptr(); }
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
				? found->second->Value : tup::valueptr(); }

		//* Check if this tuple contains a value matching the specified value
		bool containsUnnamedValue( name_t value, const tup::variables& vars )
			const noexcept;
		bool containsUnnamedValue( const tup::valueptr& value,
			const tup::variables& vars ) const noexcept;

		//* Get document (the top most parent that still has a parent)
		//* Returns nullptr if 'this' has not parent.
		inline const tuple* document() const noexcept {
			return Parent == nullptr || Parent->Parent == nullptr
				? Parent : Parent->document(); }

		//* Get full path to a named attribute of this tuple
		//* Returns 'false' if 'attribute_name' isn't an attribute.
		tup::path path( const tup::variables& vars,
			const tuple* subtuple = nullptr ) const noexcept;


		//* Stringify the tuple
		inline string str() const noexcept {
			size_t pos_on_line = 0; return str( pos_on_line, 0,
				Parent == nullptr || Parent->Parent == nullptr
					? tup::perm::allow_multiliner : tup::perm::allow_oneliner
				| tup::perm::allow_multiliner ); }
		string str( size_t& pos_on_line, size_t indentation_level,
			tup::perm permissions ) const noexcept;


		// Run internal validation (using meta data associated with named
		// attributes)
		void validate( tup::variables& vars ) const;

		//* Pattern tuples can validate other tuples according to the pattern
		//* definition.
		void validate( const tuple& other, tup::variables& vars ) const;




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		tuple& operator=( const tuple& other );
		inline tuple& operator=( tuple&& other ) noexcept {
			Attributes = std::move( other.Attributes ); Named = std::move(
				other.Named ); Form = other.Form; other.Form = form::plain;
			return *this; }

		//* Set attribute value by position
		//* Throws [eon::tup::WrongType] if meta data defines a different
		//* type, or if the attribute value already exists with a different
		//* type - or if the 'attribute_pos' is more then 1 past the last
		//* existing attribute.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		void set( size_t attribute_pos, const tup::valueptr& value,
			tup::variables& vars );

		//* Append a new attribute
		//* Throws [eon::tup::DuplicateName] if another attribute already
		//* exists with the same name.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		void append( const tup::valueptr& value, tup::variables& vars,
			name_t name = no_name, const tupleptr& metadata = tupleptr() );

		//* Copy and append all attributes of the other tuple
		//* Will not copy named attributes that already exists!
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		//* Throws [eon::Incompatible] if the tuples cannot be joined (such as
		//* meta data joined with regular tuple).
		void append( const tuple& other, tup::variables& vars );

		//* Clear all attributes
		inline void clear() noexcept { Attributes.clear(); Named.clear(); }




		/**********************************************************************
		  Searching

		  NOTE: Finding an attribute that is a reference will result in the
		        target being found, not the reference itself. To access the
				reference and not the target, find the parent tuple of the
				reference attribute and then use the 'attribute' method to
				access the reference value.
		**********************************************************************/
	public:

		//* Find attribute at specified path
		//* Returns 'false' if the path does not lead to an existing attribute.
		inline tup::valueptr find( const tup::path& path ) const noexcept {
			auto found = _find( path, 0, false, nullptr ); return found
				&& found->isRef() ? dynamic_cast<tup::refval*>(
					&*found )->target() : found; }
		inline tup::valueptr find( std::initializer_list<name_t> path )
			const noexcept { return find( tup::path( path ) ); }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		//* Two tuple objects are 'equal' if all attributes have the
		//* same names, types and values.
		bool equal( const tuple& other, const tup::variables& vars ) const noexcept;

		//* The 'other' tuple object is compatible with 'this' if all
		//* the named attributes have the same type and all the unnamed
		//* attributes have the same type for the same position.
		bool compatible( const tuple& other ) const noexcept;


		//* Disable comparison operators
		bool operator==( const tuple& ) = delete;
		bool operator!=( const tuple& ) = delete;
		bool operator<( const tuple& ) = delete;
		bool operator<=( const tuple& ) = delete;
		bool operator>( const tuple& ) = delete;
		bool operator>=( const tuple& ) = delete;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		//* Have all references include a pointer to their targets - if not
		//* already
		void _resolveReferences( const tup::variables& vars );

		//* Get a target attribute from a path of attribute names
		//* The 'pos' argument is the position of the attribute to get for
		//* 'this' tuple.
		//* Returns 'false' if the path does not lead to an existing
		//* attribute.
		tup::valueptr _find( const tup::path& path, size_t pos,
			bool resolve_refs, tuple** parent ) const noexcept;


		void _validate( const tuple& metadata, const tuple* parent, const AttributePtr& value, tup::variables& vars ) const;

		void _validateType( tup::valueptr meta, const tuple* location, const tup::valueptr& value, const tup::variables& vars ) const;

		void _validateMaxDepth( tup::valueptr meta, const tuple* location, const tuple& value, const tup::variables& vars ) const;

		void _validateMinLength( tup::valueptr meta, const tuple* location, const hex& value, tup::variables& vars ) const;
		void _validateMinLength( tup::valueptr meta, const tuple* location, const string& value, tup::variables& vars ) const;
		void _validateMinLength( tup::valueptr meta, const tuple* location, const tuple& value, tup::variables& vars ) const;

		void _validateMaxLength( tup::valueptr meta, const tuple* location, const hex& value, tup::variables& vars ) const;
		void _validateMaxLength( tup::valueptr meta, const tuple* location, const string& value, tup::variables& vars ) const;
		void _validateMaxLength( tup::valueptr meta, const tuple* location, const tuple& value, tup::variables& vars ) const;

		void _validateMin( tup::valueptr meta, const tuple* location, int64_t value, tup::variables& vars ) const;
		void _validateMin( tup::valueptr meta, const tuple* location, double value, tup::variables& vars ) const;

		void _validateMax( tup::valueptr meta, const tuple* location, int64_t value, tup::variables& vars ) const;
		void _validateMax( tup::valueptr meta, const tuple* location, double value, tup::variables& vars ) const;

		void _validateFormat( tup::valueptr meta, const tuple* location, const string& value, tup::variables& vars ) const;
		void _validateFormat( tup::valueptr meta, const tuple* location, const std::vector<string>& value, tup::variables& vars ) const;

		void _validateFlags( tup::valueptr meta, const tuple* location, const tuple& value, const tup::variables& vars ) const;

		void _validateOptions( tup::valueptr meta, const tuple* location, name_t value, tup::variables& vars ) const;

		void _mismatchingMeta( name_t attribute_name, const string& info, const tup::variables& vars ) const;




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

	private:
		std::vector<AttributePtr> Attributes;
		std::unordered_map<name_t, AttributePtr> Named;
		form Form{ form::plain };
		tuple* Parent{ nullptr };

		friend class etf;
	};
}
