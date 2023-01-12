#pragma once

#include "Definitions.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Type Tuple class - eon::TypeTuple
	//
	// A type object identifies the type of an Eon object. A type is either simple
	// (single-name) or complex (a tree structure). For complex types, the first
	// TypeTuple attribute will be a general type name (such as 'tuple', 'action',
	// 'lambda', etc.), while the subsequent attributes specifies the type further.
	//
	// The rule of "First Name":
	// When getting the TypeTuple (main) name value, we get the single name for
	// single-name (simple) types and the name of the first attribute for complex
	// types.
	//
	// Type tuples are expressed literally with 'T' followed by '(' followed by
	// single name or attributes, and then ended by ')'.
	//
	// Primitives, basic, and custom types have the following type format: T(<name>).
	// Examples: T(bool), T(byte), T(string), etc. I.e., single name types.
	//
	// Tuples have the following type format:
	//   T(tuple, type=<tuple type name>, details=(<details>))
	// Example: T(tuple, type=static, details=((tuple, name=plain, details=(int, int, bool)), (short, short)))
	//
	// Actions have the following type format:
	//   T(action, type=<type-object's name>, category=<category>, name=<name>, return=<return-type>, args=(<arguments>))
	// Example: T(action, type=int, name=add, return=int, args=(a=int, b=int))
	//
	class TypeTuple
	{
	public:

		// Default constructor for place-holder object
		TypeTuple() = default;

		// Construct a single-name type
		inline TypeTuple( name_t type_name, type::Qualifier qualifier = type::Qualifier::none ) noexcept {
			NameValue = type_name; Qual = qualifier; }

		// Copy another type
		inline TypeTuple( const TypeTuple& other ) { *this = other; }

		// Take ownership of another type's details
		inline TypeTuple( TypeTuple&& other ) noexcept { *this = std::move( other ); }

		virtual ~TypeTuple() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Factory methods
		//
	public:

		// Create type for a tuple.
		static TypeTuple tuple( name_t tuple_type );

		// Create type for a tuple with name attributes.
		static TypeTuple tupleOfNames( std::initializer_list<name_t> names );

		// Create type for a tuple with named attributes.
		// NOTE: Unnamed attributes can be included by specifying eon::no_name as name!
		static TypeTuple tupleOfNamedAttributes( std::initializer_list<std::pair<name_t, TypeTuple>> attributes );


		// Create type for an expression.
		static TypeTuple expression( TypeTuple&& return_type );


		// Create type for an action.
		static TypeTuple action(
			name_t type_name,
			name_t action_name,
			name_t category,
			TypeTuple&& return_type,
			TypeTuple&& arguments );

		// Create type for an action.
		static TypeTuple action(
			name_t type_name,
			name_t action_name,
			name_t category,
			TypeTuple&& return_type,
			const TypeTuple& arguments );

		// Create type for an action.
		static TypeTuple action(
			name_t type_name,
			name_t action_name,
			name_t category,
			const TypeTuple& return_type,
			const TypeTuple& arguments );

		// Create type for an action.
		static TypeTuple action(
			name_t type_name,
			name_t action_name,
			name_t category,
			TypeTuple&& return_type,
			std::initializer_list<std::pair<name_t, TypeTuple>> arguments
				= std::initializer_list<std::pair<name_t, TypeTuple>>() );


		// Create type for a list of unnamed action arguments.
		static TypeTuple args( std::initializer_list<TypeTuple> attributes );

		// Create type for a list of named action arguments.
		static TypeTuple args( std::initializer_list<std::pair<name_t, TypeTuple>> attributes );




		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Discard all details and copy those of another type.
		inline TypeTuple& operator=( const TypeTuple& other ) {
			NameValue = other.NameValue; Qual = other.Qual; TupleValue = other.TupleValue;
			NamedAttributes = other.NamedAttributes; return *this; }

		// Discard all details and take ownership of those of another type.
		TypeTuple& operator=( TypeTuple&& other ) noexcept;


		// Add an unnamed attribute type specification in the form of a single type name.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( name_t type_name ) { _ensureSubTuple(); TupleValue.push_back( type_name ); return *this; }

		// Add a named attribute type specification in the form of a single type name.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( name_t name, name_t type_name ) {
			if( name != no_name ) NamedAttributes[ name ] = TupleValue.size(); return add( type_name ); }

		// Add an unnamed attribute type specification in the form of another TypeTuple.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( TypeTuple&& sub_tuple ) {
			_ensureSubTuple(); TupleValue.push_back( std::move( sub_tuple ) ); return *this; }

		// Add an unnamed attribute type specification in the form of another TypeTuple.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( const TypeTuple& sub_tuple ) {
			_ensureSubTuple(); TupleValue.push_back( sub_tuple ); return *this; }

		// Add a named attribute type specification in the form of another TypeTuple.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( name_t name, TypeTuple&& sub_tuple ) {
			_ensureSubTuple(); if( name != no_name ) NamedAttributes[ name ] = TupleValue.size();
			return add( std::move( sub_tuple ) ); }

		// Add a named attribute type specification in the form of another TypeTuple.
		// NOTE: If 'this' is a single-name type, it will lose that name when attributes are added!
		inline TypeTuple& add( name_t name, const TypeTuple& sub_tuple ) {
			_ensureSubTuple(); if( name != no_name ) NamedAttributes[ name ] = TupleValue.size(); return add( sub_tuple); }




		///////////////////////////////////////////////////////////////////////
		//
		// Qualifiers
		//
		// These are included to help the Æon compiler and the Expression class!
		//
	public:

		// Mark as literal value.
		// NOTE: This will also mark as rvalue!
		inline void markLiteral() noexcept { Qual |= type::Qualifier::literl | type::Qualifier::rvalue; }

		// Check if literal.
		inline bool literal() const noexcept { return Qual & type::Qualifier::literl; }

		// Mark as rvalue.
		inline void markRvalue() noexcept { Qual |= type::Qualifier::rvalue; }

		// Check if rvalue.
		inline bool rvalue() const noexcept { return Qual & type::Qualifier::rvalue; }

		// Mark as reference
		inline void markReference() noexcept { Qual |= type::Qualifier::reference; }

		// Check if reference
		inline bool reference() const noexcept { return Qual & type::Qualifier::reference; }

		// Mark as link
		inline void markLink() noexcept { Qual |= type::Qualifier::link; }

		// Check if link
		inline bool link() const noexcept { return Qual & type::Qualifier::link; }

		// Mark as optional
		inline void markOptional() noexcept { Qual |= type::Qualifier::optional; }

		// Check if optional
		inline bool optional() const noexcept { return Qual & type::Qualifier::optional; }

		// Set qualifier
		inline void qualifier( type::Qualifier qualifier ) noexcept { Qual = qualifier; }

		// Get qualifier
		inline type::Qualifier qualifier() const noexcept { return Qual; }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if TypeTuple specifies a type (any type) or nothing (is void/false).
		inline operator bool() const noexcept { return NameValue != no_name; }

		
		// Get number of attributes.
		inline index_t numAttributes() const noexcept { return isName() ? 1 : TupleValue.size(); }


		// Check if 'this' is a single-name type.
		inline bool isName() const noexcept { return NameValue != no_name && NameValue != name_ttypetuple; }

		// Check if 'this' is a complex type.
		inline bool isComplex() const noexcept { return NameValue == name_ttypetuple; }
		

		// Get type name (eon::name_ttypetuple if complex type, eon::no_name if not a type at all).
		inline name_t nameValue() const noexcept { return NameValue; }

		// Get type name (see rule of "First Name").
		inline name_t name() const noexcept { return NameValue != name_ttypetuple ? NameValue : TupleValue[ 0 ].NameValue; }


		// Check if this is a primitive type.
		inline bool isPrimitive() const noexcept { return isName() && isPrimitive( NameValue ); }

		// Check if this is a tuple of any kind.
		inline bool isTuple() const noexcept { return isComplex() && at( 0 ).nameValue() == name_tuple; }

		// Check if this is an action signature.
		inline bool isAction() const noexcept { return isComplex() && at( 0 ).nameValue() == name_action; }

		// Check if this is a basic type.
		inline bool isBasic() const noexcept {
			_populateBasics(); return isName() && BasicTypes.find( NameValue ) != BasicTypes.end(); }


		// Check if an attribute exists at the specified position.
		inline bool exists( index_t attribute_num ) const noexcept {
			return ( attribute_num == 0 && isName() ) || ( isTuple() && attribute_num < TupleValue.size() ); }

		// Check if an attribute exists with the specified name.
		inline bool exists( name_t attribute_name ) const noexcept {
			return NamedAttributes.find( attribute_name ) != NamedAttributes.end(); }


		// Get position of named attribute.
		// Throws eon::type::NotFound if no attribute with that name!
		index_t indexOf( name_t attribute_name ) const;


		// Check if attribute at specified position is a single-name type.
		// NOTE: Will also return false if no attribute at that position!
		inline bool isName( index_t attribute_num ) const noexcept {
			return exists( attribute_num ) && TupleValue[ attribute_num ] != name_ttypetuple; }

		// Check if attribute with specified name is a single-name type.
		// NOTE: Will also return false if no attribute with that name!
		inline bool isName( name_t attribute_name ) const noexcept {
			return exists( attribute_name ) && TupleValue[ indexOf( attribute_name ) ] != name_ttypetuple; }

		// Check if attribute at specified position is a complex type.
		// NOTE: Will also return false if no attribute at that position!
		inline bool isSubTuple( index_t attribute_num ) const noexcept {
			return exists( attribute_num ) && TupleValue[ attribute_num ] == name_ttypetuple; }

		// Check if attribute with specified name is a complex type.
		// NOTE: Will also return false if no attribute with that name!
		inline bool isSubTuple( name_t attribute_name ) const noexcept {
			return exists( attribute_name ) && TupleValue[ indexOf( attribute_name ) ] == name_ttypetuple; }


		// Get full attribute type for specified position.
		// Throws eon::type::NotFound if no attribute at that position!
		inline const TypeTuple& at( index_t attribute_num ) const {
			_assertInRange( attribute_num ); return TupleValue[ attribute_num ]; }

		// Get full attribute type for specified position.
		// Throws eon::type::NotFound if no attribute at that position!
		inline const TypeTuple& at( int attribute_num ) const { return at( static_cast<index_t>( attribute_num ) ); }

		// Get full attribute type for specified name.
		// Throws eon::type::NotFound if no attribute with that position!
		inline const TypeTuple& at( name_t attribute_name ) const {
			_assertInRange( indexOf( attribute_name ) ); return TupleValue[ indexOf( attribute_name ) ]; }


		// Describe type tuple in string format, written into the given Stringifier with 'T'-prefix.
		void str( Stringifier& strf ) const;

		// Describe type tuple in string format.
		// Uses str(Stringifier&) to generate the string value!
		inline string str() const { Stringifier strf; str( strf ); return strf.generateString(); }


		//// Get the size necessary to allocate an object of this type
		//index_t sizeOfObject() const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		// Compare with another TypeTuple, return -1 if this is less than other, 0 if equal, and 1 if greater.
		int compare( const TypeTuple& other ) const noexcept;

		// Check if 'this' sorts before 'other'.
		inline bool operator<( const TypeTuple& other ) const noexcept { return compare( other ) < 0; }

		// Check if 'this' sorts before or same as 'other'.
		inline bool operator<=( const TypeTuple& other ) const noexcept { return compare( other ) <= 0; }

		// Check if 'this' sorts after 'other'.
		inline bool operator>( const TypeTuple& other ) const noexcept { return compare( other ) > 0; }

		// Check if 'this' sorts after or same as 'other'.
		inline bool operator>=( const TypeTuple& other ) const noexcept { return compare( other ) >= 0; }

		// Check if 'this' sorts same as 'other'.
		inline bool operator==( const TypeTuple& other ) const noexcept { return compare( other ) == 0; }

		// Check if 'this' sorts before or after 'other'.
		inline bool operator!=( const TypeTuple& other ) const noexcept { return compare( other ) != 0; }


		// Compare with a plain name (according to rule of "First Name").
		// Returns -1 if this is less than other, 0 if equal, and 1 if greater
		inline int compare( name_t other ) const noexcept { auto n = name(); return n < other ? -1 : n > other ? 1 : 0; }

		// Check if 'this' sorts before 'other' (according to rule of "First Name").
		inline bool operator<( name_t other ) const noexcept { return name() < other; }

		// Check if 'this' sorts before or same as 'other' (according to rule of "First Name").
		inline bool operator<=( name_t other ) const noexcept { return name() <= other; }

		// Check if 'this' sorts after 'other' (according to rule of "First Name").
		inline bool operator>( name_t other ) const noexcept { return name() > other; }

		// Check if 'this' sorts after or same as 'other' (according to rule of "First Name").
		inline bool operator>=( name_t other ) const noexcept { return name() >= other; }

		// Check if 'this' sorts same as 'other' (according to rule of "First Name").
		inline bool operator==( name_t other ) const noexcept { return name() == other; }

		// Check if 'this' sorts before or after 'other' (according to rule of "First Name").
		inline bool operator!=( name_t other ) const noexcept { return name() != other; }


		// Check if 'a' sorts before 'b' (according to rule of "First Name").
		inline friend bool operator<( name_t a, const TypeTuple& b ) noexcept { return b >= a; }

		// Check if 'a' sorts before or same as 'b' (according to rule of "First Name").
		inline friend bool operator<=( name_t a, const TypeTuple& b ) noexcept { return b > a; }

		// Check if 'a' sorts after 'b' (according to rule of "First Name").
		inline friend bool operator>( name_t a, const TypeTuple& b ) noexcept { return b <= a; }

		// Check if 'a' sorts after or same as 'b' (according to rule of "First Name").
		inline friend bool operator>=( name_t a, const TypeTuple& b ) noexcept { return b < a; }

		// Check if 'a' sorts same as 'b' (according to rule of "First Name").
		inline friend bool operator==( name_t a, const TypeTuple& b ) noexcept { return b != a; }

		// Check if 'a' sorts before or after 'b' (according to rule of "First Name").
		inline friend bool operator!=( name_t a, const TypeTuple& b ) noexcept { return b == a; }


		// Check if this type is compatible with the other.
		// (If it is, then tuple of 'this' type can have its attributes copied to tuple of 'other' type.)
		// If neither are tuples then same as compare!
		// If other is a tuple and this is not, then this must compare equal with the first attribute of other.
		// This tuple is compatible with the other if:
		//   1. All named attributes match on type
		//   2. All unnamed attributes match on position and type
		bool compatibleWith( const TypeTuple& other ) const noexcept;


		// Get TypeTuple hash value.
		size_t hash() const noexcept { size_t val = NameHasher( NameValue ); for( auto& sub : TupleValue )
			val = val * 2734561 + sub.hash(); return val; }




		///////////////////////////////////////////////////////////////////////
		//
		// Static methods
		//
	public:

		// Check if specified name is for a primitive type.
		static inline bool isPrimitive( name_t type_name ) {
			_populatePrims(); return PrimTypes.find( type_name ) != PrimTypes.end(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		inline void _ensureSubTuple() noexcept { if( NameValue != name_ttypetuple ) NameValue = name_ttypetuple; }

		inline void _assertInRange( index_t attribute_num ) const {
			if( attribute_num >= TupleValue.size() ) throw type::NotFound(); }

		void _toStr( Stringifier& strf, bool top_level ) const;
		inline void _writePrefix( Stringifier& strf, bool top_level ) const {
			if( top_level || isComplex() ) strf.start_grp2( "(" ); }
		inline void _writePostfix( Stringifier& strf, bool top_level ) const {
			if( top_level || isComplex() ) strf.end_grp2( ")" ); }
		void _writeComplex( Stringifier& strf ) const;
		void _writeComplex( Stringifier& strf, const std::unordered_map<index_t, name_t>& names, index_t attrib_no ) const;
		void _writeNamedAttribName(
			Stringifier& strf, const std::unordered_map<index_t, name_t>& names, index_t attrib_no ) const;

		std::unordered_map<index_t, name_t> _reverseNameIndexMap() const;

		static void _populatePrims();
		static void _populateBasics();

		int _attributesCompare( const TypeTuple& other ) const noexcept;
		bool _equalOnNamedArgs( const TypeTuple& other, std::unordered_set<index_t>& compared_by_name ) const noexcept;
		bool _equalOnUnnamedArgs(
			const TypeTuple& other, const std::unordered_set<index_t>& compared_by_name ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:

		static std::hash<name_t> NameHasher;

		// A TypeTuple's value can be a single name or a sub-tuple (not both!).
		// If the latter, the name value will be "#typetuple".
		// If no value at all, the name value will be eon::no_name.
		name_t NameValue{ no_name };

		// A type may have qualifiers attached.
		type::Qualifier Qual{ type::Qualifier::none };

		// If sub-tuple, then we have a set of attributes = TypeTuples, and
		// these can be both unnamed and named.
		std::vector<TypeTuple> TupleValue;
		std::unordered_map<name_t, index_t> NamedAttributes;

		static std::unordered_set<name_t> PrimTypes, BasicTypes;
	};

	struct TypeTupleLt {
		inline bool operator()( const TypeTuple* lhs, const TypeTuple* rhs ) const noexcept { return *lhs < *rhs; } };
}

namespace std
{
	// Allow implicit use of [eon::TypeTuple] as key/value when used in
	// containers such as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct hash<::eon::TypeTuple> {
		inline size_t operator()( const ::eon::TypeTuple& obj ) const { return obj.hash(); } };
	template<>
	struct equal_to<::eon::TypeTuple> {
		inline bool operator()( const ::eon::TypeTuple& a, const ::eon::TypeTuple& b ) const { return a == b; } };
}
