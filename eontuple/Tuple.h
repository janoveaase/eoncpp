#pragma once

#include <unordered_map>
#include <memory>
#include <eonexcept/Exception.h>
#include <eonname/NameRef.h>
#include <eonhex/Hex.h>
#include "Attribute.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;
	namespace vars
	{
		class refval;
		class variables;
		class value;
		using valueptr = std::shared_ptr<value>;
	}




	/**************************************************************************
	  Eon Tuple Class - eon::tuple

	  Holds an actual tuple.
	**************************************************************************/
	class tuple
	{
/*	public:
		using key_type = name_t;
		using mapped_type = vars::valueptr;
		using value_type = std::pair<const name_t, name_t>;
		using allocator_type = typename _Mybase::allocator_type;
		using size_type = typename _Mybase::size_type;
		using difference_type = typename _Mybase::difference_type;
		using pointer = typename _Mybase::pointer;
		using const_pointer = typename _Mybase::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = typename _Mybase::iterator;
		using const_iterator = typename _Mybase::const_iterator;
		using reverse_iterator = typename _Mybase::reverse_iterator;
		using const_reverse_iterator = typename _Mybase::const_reverse_iterator;*/




		/**********************************************************************
		  Defintions
		**********************************************************************/
	public:

		//* Tuples can exist in multiple forms
		enum class form
		{
			plain,
			braced,
			metadata,
			function
		};

		EONEXCEPT( Incompatible );
		EONEXCEPT( Invalid );
		EONEXCEPT( DuplicateName );
		EONEXCEPT( CircularReferencing );




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		tuple() = default;
		inline tuple( form tuple_form ) { Form = tuple_form; }
		tuple( form tuple_form, vars::variables& varcache, std::initializer_list<name_t> name_attributes );
		tuple( std::initializer_list<std::pair<name_t, vars::valueptr>> attributes );
		inline tuple( const tuple& other ) { *this = other; }
		inline tuple( tuple&& other ) noexcept { *this = std::move( other ); }

		~tuple() { clear(); }



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if there are any attributes at all
		inline bool empty() const noexcept { return Attributes.empty(); }

		//* Get number of attributes (Primitives will have only one.)
		inline size_t numAttributes() const noexcept { return Attributes.size(); }

		//* A 'false' tuple object is an empty one
		inline operator bool() const noexcept { return !empty(); }

		//* Get a const attribute by position
		//* Returns 'false' if invalid position.
		inline vars::valueptr at( size_t pos ) const noexcept {
			return pos < Attributes.size() ? Attributes[ pos ]->value() : vars::valueptr(); }
		inline vars::valueptr at( int pos ) const noexcept {
			return at( static_cast<size_t>( pos ) ); }

		//* Check if the attribute at a given position is named
		//* Returns false if not named or position is invalid.
		inline bool isNamed( size_t pos ) const noexcept { return name( pos ) != no_name; }

		//* Get name of attribute identified by position
		//* Returns no_name if unnamed or position is invalid.
		inline name_t name( size_t pos ) const noexcept {
			return pos < Attributes.size() ? Attributes[ pos ]->name() : no_name; }

		//* Get meta data for a named attribute
		//* Returns 'false' tuple if no metadata
		tupleptr metadata( name_t name ) const noexcept {
			auto found = Named.find( name ); return found != Named.end() ? found->second->meta() : tupleptr(); }

		//* Check if a named attribute exists
		inline bool exists( name_t name ) const noexcept { return Named.find( name ) != Named.end(); }

		//* Get a const attribute by name
		//* Returns 'false' if no attribute with that name.
		inline vars::valueptr at( name_t name ) const noexcept {
			auto found = Named.find( name ); return found != Named.end() ? found->second->value() : vars::valueptr(); }

		//* Check if this tuple contains a value matching the specified value
		bool containsUnnamedValue( name_t value ) const noexcept;
		bool containsUnnamedValue( const vars::valueptr& value ) const noexcept;

		//* Get document
		//* Documents are attributes under the Eof documents tuple
		//* Returns nullptr if 'this' has not parent.
		inline const tuple* document() const noexcept {
			return Parent == nullptr || Parent->Parent == nullptr
				? nullptr : Parent->Parent->Parent == nullptr
				? Parent : Parent->document(); }

		//* Get full path to a named attribute of this tuple
		//* Returns 'false' if 'attribute_name' isn't an attribute.
		nameref path( const vars::variables& varcache, const tuple* subtuple = nullptr ) const noexcept;


		//* Stringify the tuple
		inline string str() const { size_t pos_on_line = 0; return str( pos_on_line, 0 ); }
		string str( size_t& pos_on_line, size_t indentation_level ) const;


		//* Check if the tuple is braced
		inline bool braced() const noexcept { return Form == form::braced; }

		//* Check if this is a metadata tuple
		inline bool metadata() const noexcept { return Form == form::metadata; }

		//* Check if th is is a function tuple
		inline bool function() const noexcept { return Form == form::function; }


		// Run internal validation (using meta data associated with named
		// attributes)
		void validate( vars::variables& varcache ) const;

		//* Pattern tuples can validate other tuples according to the pattern
		//* definition.
		void validate( const tuple& other, vars::variables& varcache ) const;




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		tuple& operator=( const tuple& other );
		tuple& operator=( tuple&& other ) noexcept;

		//* Set attribute value by position
		//* Throws [eon::vars::WrongType] if meta data defines a different
		//* type, or if the attribute value already exists with a different
		//* type - or if the 'attribute_pos' is more then 1 past the last
		//* existing attribute.
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		void set( size_t attribute_pos, const vars::valueptr& value, vars::variables& varcache );

		//* Append a new attribute
		//* Throws [eon::vars::DuplicateName] if another attribute already
		//* exists with the same name.
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		attributeptr append( const vars::valueptr& value, name_t name = no_name, const tupleptr& metadata = tupleptr() );
		void append( const vars::valueptr& value, vars::variables& varcache, name_t name = no_name,
			const tupleptr& metadata = tupleptr() );

		//* Add a new attribute - if one exists with the same name, replace it
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		void addReplace( const vars::valueptr& value, vars::variables& varcache, name_t name,
			const tupleptr& metadata = tupleptr() );

		//* Add a new attribute - if one exists with the same name, merge with
		//* it, replacing only attributes that are unmergeable
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		void addMerge( const vars::valueptr& value, vars::variables& varcache, name_t name,
			const tupleptr& metadata = tupleptr() );

		//* Copy and append all attributes of the other tuple
		//* Will not copy named attributes that already exists!
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		//* Throws [eon::Incompatible] if the tuples cannot be joined (such as
		//* meta data joined with regular tuple).
		void append( const tuple& other, vars::variables& varcache );

		//* Merge another tuple with this one
		//* Named attributes existing in both will be overewritten in 'this'!
		//* Throws [eon::vars::CircularReferencing] if circular referencing is
		//* detected!
		//* Throws [eon::Incompatible] if the tuples cannot be merged (such as
		//* meta data joined with regular tuple).
		void merge( const tuple& other, vars::variables& varcache );

		//* Set context of attributes (valueptr to this)
		void setAttributeContext( vars::valueptr context );

		//* Clear all attributes
		inline void clear() noexcept { Attributes.clear(); Named.clear(); }

		//* Have all references include a pointer to their targets - if not
		//* already.
		//* References without a valid target will be accepted.
		inline void resolveReferences( const vars::variables& varcache ) { _resolveReferences( varcache, false ); }

		//* Resolve all references, throw [eon::vars::NotFound] if a reference
		//* target cannot be found.
		inline void resolveAllReferences( const vars::variables& varcache ) { _resolveReferences( varcache, true ); }




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
		vars::valueptr find( const nameref& path ) const noexcept;
		inline vars::valueptr find( std::initializer_list<name_t> path ) const noexcept {
			return find( nameref( path ) ); }




		/**********************************************************************
		  Attribute iteration
		**********************************************************************/
	public:

		using iterator = std::vector<attributeptr>::iterator;
		using const_iterator = std::vector<attributeptr>::const_iterator;

		inline iterator begin() noexcept { return Attributes.begin(); }
		inline iterator end() noexcept { return Attributes.end(); }

		inline const_iterator begin() const noexcept { return Attributes.begin(); }
		inline const_iterator end() const noexcept { return Attributes.end(); }




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

		//* Get a target attribute from a path of attribute names
		//* The 'pos' argument is the position of the attribute to get for
		//* 'this' tuple.
		//* Returns 'false' if the path does not lead to an existing
		//* attribute.
		vars::valueptr _find( const nameref& path, size_t pos, bool resolve_refs, tuple** parent ) const noexcept;

		void _resolveReferences( const vars::variables& vars, bool all );


		void _validate( const tuple& metadata, const tuple* parent, const attributeptr& value, vars::variables& vars ) const;

		void _validateType( vars::valueptr meta, const tuple* location,
			const vars::valueptr& value, const vars::variables& vars ) const;

		void _validateMaxDepth( vars::valueptr meta, const tuple* location,
			const tuple& value, const vars::variables& vars ) const;

		void _validateMinLength( vars::valueptr meta, const tuple* location,
			const hex& value, vars::variables& vars ) const;
		void _validateMinLength( vars::valueptr meta, const tuple* location,
			const string& value, vars::variables& vars ) const;
		void _validateMinLength( vars::valueptr meta, const tuple* location,
			const tuple& value, vars::variables& vars ) const;

		void _validateMaxLength( vars::valueptr meta, const tuple* location,
			const hex& value, vars::variables& vars ) const;
		void _validateMaxLength( vars::valueptr meta, const tuple* location,
			const string& value, vars::variables& vars ) const;
		void _validateMaxLength( vars::valueptr meta, const tuple* location,
			const tuple& value, vars::variables& vars ) const;

		void _validateMin( vars::valueptr meta, const tuple* location, int64_t value, vars::variables& vars ) const;
		void _validateMin( vars::valueptr meta, const tuple* location, double value, vars::variables& vars ) const;

		void _validateMax( vars::valueptr meta, const tuple* location, int64_t value, vars::variables& vars ) const;
		void _validateMax( vars::valueptr meta, const tuple* location, double value, vars::variables& vars ) const;

		void _validateFormat( vars::valueptr meta, const tuple* location,
			const string& value, vars::variables& vars ) const;
		void _validateFormat( vars::valueptr meta, const tuple* location,
			const std::vector<string>& value, vars::variables& vars ) const;

		void _validateFlags( vars::valueptr meta, const tuple* location, const tuple& value,
			const vars::variables& vars ) const;

		void _validateOptions( vars::valueptr meta, const tuple* location, name_t value, vars::variables& vars ) const;

		void _mismatchingMeta( name_t attribute_name, const string& info, const vars::variables& vars ) const;


		string _strCompact( size_t& pos_on_line, size_t indentation_level ) const;
		string _strMultiline( size_t& pos_on_line, size_t indentation_level ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::vector<attributeptr> Attributes;
		std::unordered_map<name_t, attributeptr> Named;
		form Form{ form::plain };
		tuple* Parent{ nullptr };

		friend class etf;
	};
}
