#pragma once

#include "Attribute.h"
#include "TypeSystem.h"
#include <typeinfo>
#include <unordered_set>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{	
	/**************************************************************************
	  The 'eon::type' namespace encloses all hidden type functionality
	**************************************************************************/
	namespace type
	{
		/**********************************************************************
		  BasicTuple class - eon::type::BasicTuple

		  This is the super-class for all the other tuples types (except for
		  [eon::EonType]).
		**********************************************************************/
		class BasicTuple : public type::Object
		{
			/******************************************************************
			  Definitions
			******************************************************************/
		public:

			//* Tuple permissions
			//* Permissions apply once the tuple has been finalized!
			enum class Permission
			{
				none		= 0x00,
				add_var		= 0x01,	// New variables can be added
				del_var		= 0x02,	// Variables can be deleted
				mod_var		= 0x04,	// Variables can be modified
				add_cache	= 0x08,	// Caches can be added
				add_act		= 0x10,	// Actions can be added
				add_grp		= 0x20,	// Groups can be aded

				open		= add_var | del_var | mod_var,
				all			= open | add_cache | add_act | add_grp
			};
			inline friend bool operator&( Permission a, Permission b ) noexcept {
				return static_cast<bool>( static_cast<int>( a ) & static_cast<int>( b ) ); }
			inline friend Permission operator|( Permission a, Permission b ) noexcept {
				return static_cast<Permission>( static_cast<int>( a ) | static_cast<int>( b ) ); }




			/******************************************************************
			  Construction
			******************************************************************/
		public:

			// Construct an empty tuple
			inline BasicTuple( name_t tuple_type, source::Ref source )
				: type::Object( EonType( tuple_type, source ), source ) {}

			// Construct tuple with unnamed attributes based on values only
			inline BasicTuple( name_t tuple_type, std::initializer_list<Object*> values, source::Ref source )
				: Object( EonType( tuple_type, source ), source ) { for( auto value : values ) *this += value; finalize(); }

			// Construct tuple for the specified attribute elements
			inline BasicTuple( name_t tuple_type, std::initializer_list<type::Attribute> attributes, source::Ref source )
				: Object( EonType( tuple_type, source ), source ) {
				for( auto& attribute : attributes ) *this += std::move( *(Attribute*)&attribute ); finalize(); }

			// Copy other tuple
			inline BasicTuple( const BasicTuple& other ) : Object( other.type(), other.source() ) {
				Perm = other.Perm; *this = other; }

			// Take ownership of other tuple
			inline BasicTuple( BasicTuple&& other ) noexcept : type::Object( other.type(), other.source() ) {
				_assertValidForMe( other ); Perm = other.Perm; *this = std::move( other ); }

			// Destruct tuple
			virtual ~BasicTuple() = default;



			// Finalize tuple (when construction is complete and it is ready for use)
			void finalize() noexcept;


			// Delete 'this' object
			virtual void die() override { delete this; }


			// Copy
			BasicTuple& operator=( const BasicTuple& other );

			// Transfer ownership
			inline BasicTuple& operator=( BasicTuple&& other ) noexcept {
				Finalized = other.Finalized; other.Finalized = false; Perm = other.Perm; other.Perm = Permission::open;
				Attributes = std::move( other.Attributes ); NamedAttribs = std::move( other.NamedAttribs );
				*static_cast<Object*>( this ) = std::move( other ); return *this; }


			virtual void callDestructor() override { this->~BasicTuple(); }




			/**********************************************************************
			  Basic Access
			**********************************************************************/
		public:

			//* Get the C++ type
			inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

			//* Identify this as a tuple
			virtual name_t generalType() const noexcept override { return name_tuple; }

			//* Check if this tuple is compatible with another object
			bool compatibleWith( const Object& other ) const noexcept override;

			//* Identify tuple type
			inline bool isPlain() const noexcept { return Type.name() == name_plain; }
			inline bool isDynamic() const noexcept { return Type.name() == name_dynamic; }
			inline bool isData() const noexcept { return Type.name() == name_data; }

			//* Check if the tuple has been finalized
			inline bool finalized() const noexcept { return Finalized; }

			//* Create a copy of 'this'
			virtual type::Object* copy() override { return new BasicTuple( *this ); }

			//* Get object as string representation
			virtual void str( type::Stringifier& str ) const override;




			/**********************************************************************
			  Attributes
			**********************************************************************/
		public:

			//* Check if there are any attributes at all
			inline bool empty() const noexcept { return Attributes.empty(); }
			inline operator bool() const noexcept { return !Attributes.empty(); }

			//* Get number of attributes in tuple
			inline index_t numAttributes() const noexcept { return Attributes.size(); }


			//* Get position of named attribute - returns no_index if not found
			inline index_t pos( name_t name ) const noexcept {
				auto found = NamedAttribs.find( name );
				return found != NamedAttribs.end() ? found->second : no_index; }

			//* Get name of attribute at position - returns no_name if not named.
			inline name_t name( index_t pos ) const noexcept {
				return pos < numAttributes() ? Attributes[ pos ].name() : no_name; }

			//* Get type of attribute at position
			//* Throws [eon::type::NotFound] if invalid position
			inline EonType attributeType( index_t pos ) const {
				if( pos < numAttributes() ) return _type( pos ); throw type::NotFound(); }

			//* Check if attribute exists
			inline bool exists( index_t pos ) const noexcept { return pos < numAttributes(); }
			inline bool exists( name_t name ) const noexcept { return NamedAttribs.find( name ) != NamedAttribs.end(); }


			//* Access attributes as read-only
			//* Throws [eon::type::NotFound] if invalid position
			inline const type::Attribute& at( index_t pos ) const {
				if( pos < numAttributes() ) return Attributes[ pos ]; throw type::NotFound(); }
			inline const type::Attribute& at( name_t name ) const {
				auto p = pos( name ); if( p != no_index ) return Attributes[ p ]; throw type::NotFound(); }
			inline const type::Attribute& operator[]( index_t pos ) const { return at( pos ); }
			inline const type::Attribute& operator[]( int pos ) const {
				return at( pos < 0 ? Attributes.size() - static_cast<index_t>( -pos ) : static_cast<index_t>( pos ) ); }
			inline const type::Attribute& operator[]( name_t name ) const { return at( name ); }

			//* Access modifiable attributes
			//* Throws [eon::type::NotFound] if invalid position!
			//* Throws [eon::type::AccessDenied] if modify is not permitted!
			inline type::Attribute& at( index_t pos ) {
				_assertCanModify( pos ); if( pos >= numAttributes() ) throw type::NotFound(); return Attributes[ pos ]; }
			inline type::Attribute& at( name_t name ) { auto p = pos( name );
				if( p == no_index ) throw type::NotFound(); _assertCanModify( p ); return Attributes[ p ]; }
			inline type::Attribute& operator[]( index_t pos ) { return at( pos ); }
			inline type::Attribute& operator[]( int pos ) {
				return at( pos < 0 ? Attributes.size() - static_cast<index_t>( -pos ) : static_cast<index_t>( pos ) ); }
			inline type::Attribute& operator[]( name_t name ) { return at( name ); }


			//* Get read-only reference to attribute value at specified position
			//* Throws [eon::type::NotFound] if the position is > numAttributes()!
			//* Throws [eon::type::WrongType] if the type requested doesn't match the actual attribute type!
			template<typename T>
			const T& value( index_t pos ) const
			{
				auto& attribute = at( pos );
				if( attribute.type().name() == name_syntax ) {
					if( typeid( T ) == typeid( name_t ) ) return *(T*)&_nameValue( pos ); else throw type::WrongType(); }
				auto object = (const Object*)attribute.value();
				if( object )
				{
					if( object->generalType() == name_instance )
					{
						auto instance = (const type::Instance*)object;
						if( std::type_index( typeid( T ) ) == instance->rawType() )
							return *(const T*)instance->rawValue();
					}
					else if( object->generalType() == name_tuple )
					{
						auto tuple = (BasicTuple*)object;
						if( tuple->type().name() == name_plain && std::type_info( typeid( T ) ) == tupleType() )
							return *(const T*)object;
						else if( tuple->type().name() == name_dynamic && isDynamicTuple( typeid( T ) ) )
							return *(const T*)object;
						else if( tuple->type().name() == name_data && isDataTuple( typeid( T ) ) )
							return *(const T*)object;
						else if( tuple->type().name() == name_lambda && isLambda( typeid( T ) ) )
							return *(const T*)object;
					}
				}
				throw type::WrongType();
			}
			template<typename T>
			const T& value( int pos ) const { return value<T>( pos < 0 ? Attributes.size() - static_cast<index_t>( -pos )
				: static_cast<index_t>( pos ) ); }
			template<typename T>
			const T& value( name_t name ) const { return value<T>( pos( name ) ); }
			template<typename T>
			const T& operator[]( index_t pos ) const { return value<T>( pos ); }
			template<typename T>
			const T& operator[]( int pos ) const { return value<T>( pos < 0
				? Attributes.size() - static_cast<index_t>( -pos ) : static_cast<index_t>( pos ) ); }

			//* Get modifiable reference to attribute value at specified position
			//* Throws [eon::type::NotFound] if the position is > numAttributes()!
			//* Throws [eon::AccessDenied] if modify is not permitted!
			//* Throws [eon::type::WrongType] if the type requested doesn't match the
			//* actual attribute type or isn't an instance type nor a (legal) tuple!
			template<typename T>
			T& value( index_t pos )
			{
				auto& attribute = at( pos );
				if( attribute.type().name() == name_syntax ) {
					if( typeid( T ) == typeid( name_t ) ) return *(T*)&_nameValue( pos ); else throw type::WrongType(); }
				auto object = (Object*)attribute.value();
				if( object )
				{
					if( object->generalType() == name_instance )
					{
						auto instance = (type::Instance*)object;
						if( std::type_index( typeid( T ) ) == instance->rawType() )
							return *(T*)instance->rawValue();
					}
					else if( object->generalType() == name_tuple )
					{
						auto tuple = (BasicTuple*)object;
						if( tuple->isPlain() && std::type_info( typeid( T ) ) == tupleType() )
							return *(T*)object;
						else if( tuple->isDynamic() && isDynamicTuple( typeid( T ) ) )
							return *(T*)object;
						else if( tuple->isData() && isDataTuple( typeid( T ) ) )
							return *(T*)object;
//						else if( tuple->isLambda() && isLambda( typeid( T ) ) )
//							return *(T*)object;
					}
				}
				throw type::WrongType();
			}
			template<typename T>
			T& value( int pos ) { return value<T>( pos < 0 ? Attributes.size() - static_cast<index_t>( -pos )
				: static_cast<index_t>( pos ) ); }
			template<typename T>
			T& value( name_t name ) { return value<T>( pos( name ) ); }
			template<typename T>
			T& operator[]( index_t pos ) { return value<T>( pos ); }
			template<typename T>
			T& operator[]( int pos ) { return value<T>( pos < 0 ? Attributes.size() - static_cast<index_t>( -pos )
				: static_cast<index_t>( pos ) ); }


			//* Add a new named attribute
			//* Throws [eon::type::AccessDenied] if not adding to data tuple!
			inline BasicTuple& add( name_t name, Object* value ) { return *this += Attribute( name, value ); }

			BasicTuple& addName( name_t name, name_t value );
			BasicTuple& addName( name_t value );

			BasicTuple& addRaw( name_t name, const char* str ) { return addRaw( name, string( str ) ); }
			template<typename T>
			BasicTuple& addRaw( name_t name, const T& value )
			{
				auto tp = type::Handler::mapType<T>();
				if( isData() )
					tp = type::Handler::mapToLegalForDataTuple( tp );
				*this += type::Attribute( name, EonType( tp, name ), isData() ? type::Handler::copyConstructData( value )
					: type::Handler::copyConstruct( value ) );
				return *this;
			}
			template<typename T>
			BasicTuple& addRaw( name_t name, T&& value )
			{
				auto tp = type::Handler::mapType<T>();
				if( isData() )
					tp = type::Handler::mapToLegalForDataTuple( tp );
				*this += type::Attribute( name, EonType( tp, name ), isData() ? type::Handler::moveConstructData(
					std::move( value ), source() ) : type::Handler::moveConstruct( std::move( value ), source() ) );
				return *this;
			}


			//* Add a new unnamed attribute
			//* Throws [eon::type::AccessDenied] if not adding to data tuple!
			inline BasicTuple& add( Object* value ) { return *this += Attribute( value ); }
			inline BasicTuple& add( Object&& value ) { return *this += Attribute( value.consume() ); }

			inline BasicTuple& addRaw( const char* str ) { return addRaw( string( str ) ); }
			template<typename T>
			BasicTuple& addRaw( const T& value, source::Ref source )
			{
				auto tp = type::Handler::mapType<T>();
				if( isData() )
					tp = type::Handler::mapToLegalForDataTuple( tp );
				*this += type::Attribute( EonType( tp ), isData() ? type::Handler::copyConstructData( value, source )
					: type::Handler::copyConstruct( value, source ) );
				return *this;
			}
			template<typename T>
			BasicTuple& addRaw( T&& value )
			{
				auto tp = type::Handler::mapType<T>();
				if( isData() )
					tp = type::Handler::mapToLegalForDataTuple( tp );
				*this += type::Attribute( EonType( tp ), isData() ? type::Handler::moveConstructData( std::move( value ), source() )
					: type::Handler::moveConstruct( std::move( value ), source() ) );
				return *this;
			}

			//* Add a tuple as attribute, return pointer to it
			//* This is only possible when tuple is not finalized!
			//* Throws [eon::type::AccessDenied] if finalized!
			BasicTuple* addPlainTuple( name_t name = no_name, source::Ref source = source::Ref() );
			BasicTuple* addDynamicTuple( name_t name = no_name, source::Ref source = source::Ref() );
			BasicTuple* addDataTuple( name_t name = no_name, source::Ref source = source::Ref() );

			//* Add a new detailed attribute
			//* Throws [eon::type::AccessDenied] if adding is not permitted!
			//* Throws [eon::type::DuplicateName] if named and name already exists!
			BasicTuple& operator+=( type::Attribute&& attribute );

			//* Add multiple attributes
			//* Throws [eon::type::AccessDenied] if adding is not permitted!
			//* Throws [eon::type::DuplicateName] if named and name already exists!
			inline BasicTuple& operator+=( std::initializer_list<type::Attribute> attributes ) {
				for( auto& attribute : attributes ) *this += std::move( *(Attribute*)&attribute ); return *this; }

			//* Add another tuple
			//* Throws [eon::type::DuplicateName] if the other tuple contains one
			//* or more named attributes with names already existing in this!
			//* Throws [eon::type::AccessDenied] if adding is not permitted!
			BasicTuple& operator+=( const BasicTuple& other );

			//* Remove elements from another tuple
			//* The other duple must be compatible!
			//* Throws [eon::type::AccessDenied] if removing is not permitted or other tuple is incompatible!
			BasicTuple& operator-=( const BasicTuple& other );

			//* Remove a named attribute
			//* Throws [eon::type::NotFound] if no such attribute!
			//* Throws [eon::type::AccessDenied] if removing is not permitted!
			BasicTuple& remove( name_t name );

			//* Remove attribute by position, negative value counts from the end
			//* Throws [eon::type::NotFound] if no such attribute!
			//* Throws [eon::type::AccessDenied] if removing is not permitted!
			BasicTuple& remove( size_t pos );
			inline BasicTuple& remove( int_t pos ) {
				return remove( static_cast<size_t>( pos < 0 ? Attributes.size() + pos : pos ) ); }


			// Compare with another tuple
			int compare( const BasicTuple& other ) const noexcept;

			inline bool operator<( const BasicTuple& other ) const noexcept { return compare( other ) < 0; }
			inline bool operator<=( const BasicTuple& other ) const noexcept { return compare( other ) <= 0; }
			inline bool operator>( const BasicTuple& other ) const noexcept { return compare( other ) > 0; }
			inline bool operator>=( const BasicTuple& other ) const noexcept { return compare( other ) >= 0; }
			inline bool operator==( const BasicTuple& other ) const noexcept { return compare( other ) == 0; }
			inline bool operator!=( const BasicTuple& other ) const noexcept { return compare( other ) != 0; }




			/**********************************************************************
			  Iteration
			**********************************************************************/
		public:

			using iterator = std::vector<type::Attribute>::iterator;
			using const_iterator = std::vector<type::Attribute>::const_iterator;

			inline iterator begin() noexcept { return Attributes.begin(); }
			inline iterator end() noexcept { return Attributes.end(); }
			inline const_iterator begin() const noexcept { return Attributes.begin(); }
			inline const_iterator end() const noexcept { return Attributes.end(); }
			inline const_iterator cbegin() const noexcept { return Attributes.cbegin(); }
			inline const_iterator cend() const noexcept { return Attributes.cend(); }




			///////////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		protected:

			void _generateTupleID();

			inline bool _canAddVar() const noexcept { return Perm & Permission::add_var; }
			inline bool _canModify( size_t attribute_no ) const noexcept {
				return Perm & Permission::mod_var && attribute_no < Attributes.size()
					&& Attributes[ attribute_no ].value()->generalType() != name_action; }
			inline bool _canDelVar() const noexcept { return Perm & Permission::del_var; }
			inline bool _canAddCache() const noexcept { return Perm & Permission::add_cache; }
			inline bool _canAddAct() const noexcept { return Perm & Permission::add_act; }

			inline void _assertCanAddVar() const { if( !_canAddVar() ) throw AccessDenied(); }
			void _assertCanAddVar( const EonType& type );
			inline void _assertCanModify( size_t attribute_no ) const {
				if( !_canModify( attribute_no ) ) throw AccessDenied(); }
			inline void _assertCanDelVar() const { if( !_canDelVar() ) throw AccessDenied(); }

			void _assertValidForMe( const BasicTuple& other );

			// Get attribute name value
			// WARNING: Assumes 'pos' is valid and identifies a name value!
			const name_t& _nameValue( index_t pos ) const noexcept;

			// Get type based on position.
			// Assumes 'pos' is < NumAttributes!
			inline EonType _type( index_t pos ) const noexcept { return Attributes[ pos ].type(); }

			// Get the C++ type of eon::Tuple
			std::type_index tupleType() const noexcept;

			void _str( type::Stringifier& str ) const;

			// Output standard parenthesized prefix and posfix for tuple
			virtual void standardPrefix( type::Stringifier& str ) const {}
			virtual void standardPostfix( type::Stringifier& str ) const {}

			// Output explicit parenthesized prefix and postfix for tuple
			virtual void explicitPrefix( type::Stringifier& str ) const { str.addWord( "(" ); }
			virtual void explicitPostfix( type::Stringifier& str ) const { str.addWord( ")" ); }

			// Check if the specified typeid matches
			bool isDynamicTuple( const std::type_info& type ) const;
			bool isDataTuple( const std::type_info& type ) const;
			bool isLambda( const std::type_info& type ) const;




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:

			bool Finalized{ false };
			Permission Perm{ Permission::all };
			std::vector<type::Attribute> Attributes;
			std::unordered_map<name_t, index_t> NamedAttribs;
		};
	}
}
