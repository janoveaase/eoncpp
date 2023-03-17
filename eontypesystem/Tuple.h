#pragma once

#include "Definitions.h"
#include "TypeMap.h"
#include "Action.h"
#include "TupleDefs.h"
#include "TypeTupleFactory.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Tuple class - eon::Tuple
	//
	// Generic tuple class
	//
	class Tuple
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct a void tuple - a place-holder.
		inline Tuple() { _populateData(); }

		// Construct as a copy of another tuple.
		inline Tuple( const Tuple& other ) { *this = other; }

		// Construct by taking ownership of another tuple's contents.
		inline Tuple( Tuple&& other ) noexcept { *this = std::move( other ); }




		///////////////////////////////////////////////////////////////////////
		//
		// General modifiers
		//
	public:

		// Discard contents and copy that of another tuple.
		Tuple& operator=( const Tuple& other );

		// Discard contents and take ownership of that of another tuple.
		Tuple& operator=( Tuple&& other ) noexcept;

		// Copy attributes from 'other', named attributes are copied by name refernece, unnamed by position.
		// NOTE: 'other' must be compatible with 'this'!
		// WARNING: No checking is done for compatibility!
		Tuple& copyAttributes( const Tuple& other );


		// Discard contents. (Attributes - and stored actions.)
		// Throws [eon::type::AccessDenied] if no 'remove' permission!
		void clear() noexcept {
			_assertRemPerm(); Attributes.clear(); NamedAttributes.clear(); Actions.clear(); ActionsByName.clear(); }




		///////////////////////////////////////////////////////////////////////
		//
		// General information
		//
	public:

		// A tuple is 'true' if it contains something, 'false' if not.
		inline operator bool() const noexcept { return !empty(); }

		// Check if tuple is empty (has no attributes and no actions).
		inline bool empty() const noexcept { return Attributes.empty() && Actions.empty(); }


		// Get tuple type.
		inline const TypeTuple& type() const noexcept { return Type; }


		// Get "parent" tuple, nullptr if none.
		inline Tuple* parent() noexcept { return Parent; }


		// Check if attributes can be added to tuple.
		inline bool canAdd() const noexcept { return Permissions & TuplePerm::add; }

		// Check if attributes can be removed from tuple.
		inline bool canRemove() const noexcept { return Permissions & TuplePerm::remove; }

		// Check if attributes in tuple can be modified.
		inline bool canModify() const noexcept { return Permissions & TuplePerm::modify; }

		// Check if tuple can contain actions.
		inline bool canHaveActions() const noexcept { return Permissions & TuplePerm::actions; }


		// Describe tuple in string format, written into the given Stringifier.
		// Rules:
		//   1. '(' immediately after tuple type identifier, no newline. Example: "data("
		//   2. Value of tuple-attribute with name shall follow indented on next line. Example: "value:\n  (a, b)"
		//   3. ')' to be added after last item without newlines. Example: "data(sub:\n  (a, b))"
		//   4. Long lines shall be split as follows:
		//      1. Split after ',' for highest possible attribute level
		//      2. Split after '='
		//      3. Split bytes/string values on space with a backslash at the end of the split line
		inline void str( Stringifier& strf ) const { _str( ToStrData::newRoot( strf, *this ) ); }

		// Describe tuple in string format.
		// Uses str(Stringifier&) to generate the string value!
		inline string str() const { Stringifier strf; str( strf ); return strf.generateString(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Adding and removing attributes
		//
	public:

		// Add a void unnamed attribute of specified type.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		Tuple& addType( const TypeTuple& type );

		// Add a void named attribute of specified type.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		// Throws eon::type::DuplicateName if name has already been used!
		Tuple& addType( name_t name, const TypeTuple& type );

		// Add an unnamed tuple attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		inline Tuple& addTuple(
			const Tuple& value, type::Qualifier qualifier = type::Qualifier::_none, source::Ref source = source::Ref() ) {
			return addTuple( Tuple( value ), qualifier, source ); }

		// Add an unnamed tuple attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		inline Tuple& addTuple(
			Tuple&& value, type::Qualifier qualifier = type::Qualifier::_none, source::Ref source = source::Ref() ) {
			return add( Attribute::newTuple( std::move( value ), qualifier, source ) ); }

		// Add a named tuple attribute
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		// Throws eon::type::DuplicateName if name has already been used!
		inline Tuple& addTuple(
			name_t name,
			const Tuple& value,
			type::Qualifier qualifier = type::Qualifier::_none,
			source::Ref source = source::Ref() ) {
			return addTuple( name, Tuple( value ), qualifier, source ); }

		// Add a named tuple attribute
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		// Throws eon::type::DuplicateName if name has already been used!
		inline Tuple& addTuple(
			name_t name,
			Tuple&& value,
			type::Qualifier qualifier = type::Qualifier::_none,
			source::Ref source = source::Ref() ) {
			return add( name, Attribute::newTuple( std::move( value ), qualifier, source ) ); }

		// Add an unnamed name attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		inline Tuple& addName( name_t value ) { return add( Attribute::newName( value, type::Qualifier::_none ) ); }

		// Add a named name attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		// Throws eon::type::DuplicateName if name has already been used!
		inline Tuple& addName( name_t name, name_t value ) {
			return add( name, Attribute::newName( value, type::Qualifier::_none ) ); }

		// Add an unnamed attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		inline Tuple& add( Attribute&& value ) { return add( no_name, std::move( value ) ); }

		// Add a named attribute.
		// Throws eon::type::AccessDenied if no 'add' permission or value not legal for data tuple!
		// Throws eon::type::DuplicateName if name has already been used!
		Tuple& add( name_t name, Attribute&& value );

		// Remove an attribute by position.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::AccessDenied if no 'remove' permission!
		Tuple& remove( index_t attribute_num );

		// Remove an attribute by position, negative counts from end (-1 = last, etc.).
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::AccessDenied if no 'remove' permission!
		Tuple& remove( int_t attribute_num );

		// Remove an attribute by name
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::AccessDenied if no 'remove' permission!
		inline Tuple& remove( name_t attribute_name ) { return remove( index( attribute_name ) ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Accessing attributes
		//
	public:

		// Get number of attributes.
		inline index_t numAttributes() const noexcept { return Attributes.size(); }


		// Check if an attribute exists at the specified position.
		inline bool exists( index_t attribute_num ) const noexcept { return attribute_num < Attributes.size(); }

		// Check if an attribute with the specified name exists.
		inline bool exists( name_t attribute_name ) const noexcept {
			return NamedAttributes.findOne( attribute_name ) != NamedAttributes.endOne(); }

		// Check if an attribute with the specified name exists, search parent tuple if not found locally.
		inline bool existsIncludeParent( name_t attribute_name ) const noexcept {
			return exists( attribute_name ) ? true : Parent ? Parent->existsIncludeParent( attribute_name ) : false; }


		// Get type of attribute at specified position.
		// Returns an empty/false TypeTuple if no such attribute!
		TypeTuple attributeType( index_t attribute_num ) const noexcept {
			return exists( attribute_num ) ? Attributes[ attribute_num ].type() : TypeTuple(); }

		// Get type of attribute with specified name, void empty/false type tuple if no such attribute!
		TypeTuple attributeType( name_t attribute_name ) const noexcept {
			return exists( attribute_name ) ? attributeType( index( attribute_name ) ) : TypeTuple();}


		// Get attribute value by position, mutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::AccessDenied if no 'modify' permission!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline T& get( index_t attribute_num ) { _assertModPerm(); _assertInRange( attribute_num ); return
			Attributes[ attribute_num ].value<T, type_hint>(); }

		// Get attribute value by name, mutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::AccessDenied if no 'modify' permission!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline T& get( name_t attribute_name ) { _assertModPerm(); auto pos = index( attribute_name ); _assertInRange(
			pos ); return Attributes[ pos ].value<T, type_hint>(); }

		// Get attribute value by position, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& get( index_t attribute_num ) const {
			_assertInRange( attribute_num ); return Attributes[ attribute_num ].value<T, type_hint>(); }

		// Get attribute value by position, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& read( index_t attribute_num ) const { return get<T, type_hint>( attribute_num ); }

		// Get attribute value by position, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& read( int attribute_num ) const {
			return get<T, type_hint>( static_cast<index_t>( attribute_num ) ); }

		// Get attribute value by name, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& get( name_t attribute_name ) const {
			auto pos = index( attribute_name ); _assertInRange( pos ); return Attributes[ pos ].value<T, type_hint>(); }

		// Get attribute value by name, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		// Throws eon::type::WrongType if attribute has a different type!
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& read( name_t attribute_name ) const { return get<T, type_hint>( attribute_name ); }


		// Get full attribute at specified position, mutable access.
		// Throws eon::type::NotFound if no such attribute!
		inline Attribute& attribute( index_t attribute_num ) {
			_assertInRange( attribute_num ); return Attributes[ attribute_num ]; }

		// Get full attribute with specified name, mutable access.
		// Throws eon::type::NotFound if no such attribute!
		inline Attribute& attribute( name_t attribute_name ) {
			auto pos = index( attribute_name ); _assertInRange( pos ); return Attributes[ pos ]; }

		// Get full attribute with specified name, search parent tuple if not found locally, mutable access.
		// Returns nullptr if not found.
		Attribute* findAttributeIncludeParent( name_t attribute_name ) noexcept;

		// Get full attribute at specified position, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		inline const Attribute& attribute( index_t attribute_num ) const {
			_assertInRange( attribute_num ); return Attributes[ attribute_num ]; }

		// Get full attribute with specified name, immutable access.
		// Throws eon::type::NotFound if no such attribute!
		inline const Attribute& attribute( name_t attribute_name ) const {
			auto pos = index( attribute_name ); _assertInRange( pos ); return Attributes[ pos ]; }

		// Get full attribute with specified name, search parent tuple if not found locally, immutable access.
		// Returns nullptr if not found.
		const Attribute* findAttributeIncludeParent( name_t attribute_name ) const noexcept;


		// Check if an unnamed name attribute (flag) with the specified value exists.
		bool flag( name_t name ) const noexcept;


		// Get index for an attribute name, eon::no_index if no such attribute!
		inline index_t index( name_t attribute_name ) const noexcept {
			auto found = NamedAttributes.findOne( attribute_name );
			return found != NamedAttributes.endOne() ? found->second : no_index; }

		// Get attribute name given an index, eon::no_name if no such attribute or unnamed attribute!
		inline name_t name( index_t attribute_num ) const noexcept {
			auto found = NamedAttributes.findTwo( attribute_num );
			return found != NamedAttributes.endTwo() ? found->second : no_name; }


		// Attribute iterator for mutable access.
		using iterator = std::vector<Attribute>::iterator;

		// Attribute iterator for immutable access.
		using const_iterator = std::vector<Attribute>::const_iterator;

		// Get iterator for first attribute, mutable access.
		inline iterator begin() noexcept { return Attributes.begin(); }

		// Get iterator for end attribute (one past last), mutable access.
		inline iterator end() noexcept { return Attributes.end(); }

		// Get iterator for first attribute, immutable access.
		inline const_iterator begin() const noexcept { return Attributes.begin(); }

		// Get iterator for end attribute (one past last), immutable access.
		inline const_iterator end() const noexcept { return Attributes.end(); }

		// Get iterator for first attribute, immutable access.
		inline const_iterator cbegin() const noexcept { return Attributes.begin(); }

		// Get iterator for end attribute (one past last), immutable access.
		inline const_iterator cend() const noexcept { return Attributes.end(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Types
		//
	public:

		// Register a type.
		// Throws eon::type::AccessDenied if no 'types' permission!
		// Throws eon::type::DuplicateName if type has already been registered!
		Tuple& registerType( name_t type );

		// Check if specified name is a registered type
		bool isType( name_t name ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Actions
		//
		// A tuple can store actions if it has the 'actions' permission. This
		// is usually only for the global scope!
		//
		// NOTE: Calls to get an action will be redirected to Parent tuple (if
		//       there is one)!
		//
	public:

		// Add a reference to an action to this tuple.
		// Throws eon::type::AccessDenied if no 'actions' permission!
		// Throws eon::type::DuplicateName if action has already been registered!
		Tuple& registerAction( type::Action* action );


		// Get an action based on signature.
		// Returns nullptr if not found.
		inline const type::Action* action( const TypeTuple& signature ) const noexcept {
			if( Parent ) return Parent->action( signature );
			auto found = Actions.find( signature ); return found != Actions.end() ? found->second : nullptr; }

		// Get signatures of all actions with the specified name, for any type and arguments
		const std::set<TypeTuple>& signatures( name_t action_name ) const;

		// Get signatures of all actions with the specified name, implemented for the specified type.
		std::set<TypeTuple> signatures( name_t action_name, name_t type_name ) const;

		// Get signatures of all actions with the specified name, implemented for the specified type
		// and with the specified arguments.
		std::set<TypeTuple> signatures( name_t action_name, name_t type_name, const TypeTuple& args ) const;

		// Get signatures of all actions with the specified name of specified category,
		// implemented for the specified type and with the specified arguments.
		std::set<TypeTuple> signatures(
			name_t action_name, name_t category, name_t type_name, const TypeTuple& args ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Constructors available only to tuple factory classes.
		//
	private:

		// Construct an empty tuple.
		// If tuple type is 'static', "optional", 'dynamic', or 'data' then the permissions
		// argument is ignored and permissions set to default for that tuple type!
		Tuple( name_t tuple_type_name, TuplePerm permissions = TuplePerm::all_normal, Tuple* parent = nullptr );

		// Construct tuple with named and unnamed attributes.
		// If tuple type is 'plain', 'dynamic', or 'data' then the permissions
		// argument is ignored and permissions set to default for that tuple type!
		Tuple( name_t tuple_type_name, std::initializer_list<AttributePair> attributes,
			TuplePerm permissions = TuplePerm::all_normal, Tuple* parent = nullptr );

		// Construct tuple with named and unnamed attributes.
		// If tuple type is 'plain', 'dynamic', or 'data' then the permissions
		// argument is ignored and permissions set to default for that tuple type!
		Tuple( name_t tuple_type_name, std::vector<AttributePair> attributes,
			TuplePerm permissions = TuplePerm::all_normal, Tuple* parent = nullptr );

		// Construct optional tuple.
		Tuple( TypeTuple optional_tuple_attributes );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _perm( name_t tuple_type, TuplePerm permissions );

		void _assertAddPerm( name_t tuple_type, const TypeTuple& type ) const;
		void _assertRemPerm() const;
		void _assertModPerm() const;
		void _assertTypPerm() const;
		void _assertActPerm() const;

		void _assertInRange( index_t attribute_num ) const;

		void _add( name_t tuple_type, Attribute&& value );
		void _add( name_t tuple_type, name_t attribute_name, Attribute&& value );

		class ToStrData
		{
		private:
			ToStrData( Stringifier& strf, const Tuple& level ) { Strf = &strf; Level = &level; }
			ToStrData( ToStrData& parent, const Tuple& tuple, bool named_tuple )
			{
				Strf = parent.Strf;
				Parent = &parent;
				Level = &tuple;
				NamedTuple = named_tuple;
				if( NamedTuple )
				{
					IndentedBlock = true;
					IndentedByAttribute = -1;
				}
			}
		public:

			static ToStrData newRoot( Stringifier& strf, const Tuple& level ) { return ToStrData( strf, level ); }
			static ToStrData newSub( ToStrData& parent, const Tuple& tuple, bool named_tuple ) {
				return ToStrData( parent, tuple, named_tuple ); }

			inline bool isRoot() const noexcept { return Parent == nullptr; }
			inline const ToStrData& root() const noexcept { return Parent ? Parent->root() : *this; }
			inline const ToStrData* parent() const noexcept { return Parent; }
			inline const Tuple& tuple() const noexcept { return *Level; }
			inline bool namedTuple() const noexcept { return NamedTuple; }

			inline Stringifier& strf() noexcept { return *Strf; }

		private:
			Stringifier* Strf{ nullptr };
			ToStrData* Parent{ nullptr };
			const Tuple* Level{ nullptr };
			bool NamedTuple{ false };
		public:
			bool IndentedBlock{ false };
			int IndentedByAttribute{ 0 };
			bool EndWithCloseParen{ false };
			bool PrevWasTuple{ false };
			index_t AttribNo{ 0 };
			const Attribute* Attrib{ nullptr };
			bool NamedAttribute{ false };
			bool First{ true };
		};

		void _str( ToStrData data ) const;
		void _writeTupleStart( ToStrData& data ) const;
		bool _needFormalPrefix( ToStrData& data ) const;
		void _writeFormalTuplePrefix( ToStrData& data ) const;
		void _writeInformalTuplePrefix( ToStrData& data ) const;
		void _writeTupleAttributes( ToStrData& data ) const;
		void _writeTupleEnd( ToStrData& data ) const;
		void _writeAttribute( ToStrData& data ) const;
		void _separateAttributes( ToStrData& data ) const;
		void _writeAttributeTuple( ToStrData& data ) const;
		//void _formatTupleAttributeLine( ToStrData& data ) const;
		//void _writeNamedTupleAttributeName( ToStrData& data ) const;
		void _writeNormalAttribute( ToStrData& data ) const;
		//void _formatNormalAttributeLine( ToStrData& data ) const;
		void _writeNamedNormalAttributeName( ToStrData& data ) const;
		void _writeValue( Stringifier& strf, const Attribute& attribute ) const;

		static inline void _populateData() { _populateLegalForDataTuple(); _populateStrPrefixPostfixForTypes(); }
		static void _populateLegalForDataTuple();
		static void _populateStrPrefixPostfixForTypes();

		template<typename T>
		void _constructStatic( T attributes )
		{
			TypeTuple static_type;
			for( auto& value : attributes )
			{
				if( value.Name == no_name )
					static_type.add( value.Value.type() );
				else
					static_type.set( value.Name, value.Value.type() );
				_add( name_static, value.Name, std::move( *(Attribute*)&value.Value ) );
			}
			Type = typetuple::newStatic( std::move( static_type ) );
		}

		template<typename T>
		void _constructNonstatic( name_t tuple_type_name, T attributes )
		{
			for( auto& value : attributes )
				_add( tuple_type_name, value.Name, std::move( *(Attribute*)&value.Value ) );
			Type = typetuple::_new( tuple_type_name );
		}

		void _replaceCopyAttributes( const Tuple& other );
		void _replaceMoveAttributes( Tuple& other );

		void _addAttributeName( name_t name, const TypeTuple& type );
		void _addAttributeValue( Attribute&& value );

		std::set<TypeTuple> _signatures(
			name_t action_name,
			const std::vector<std::pair<name_t, name_t>>& name_fields,
			const TypeTuple* args = nullptr ) const;
		bool _matchingSignature( const TypeTuple& signature,
			const std::vector<std::pair<name_t, name_t>>& name_fields,
			const TypeTuple* args = nullptr ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:

		TypeTuple Type{ no_name };
		TuplePerm Permissions{ TuplePerm::all_normal };

		Tuple* Parent{ nullptr };

		std::vector<Attribute> Attributes;
		reflexive_map<name_t, index_t> NamedAttributes;

		std::unordered_map<TypeTuple, type::Action*> Actions;
		std::unordered_map<name_t, std::set<TypeTuple>> ActionsByName;

		static std::unordered_set<name_t> LegalForDataTuple;
		static std::unordered_map<name_t, std::pair<string, string>> StrPrefixPostfixForTypes;

		friend class scope;
		friend class cache;
		friend class unit;
		friend class tuple;
	};
}
