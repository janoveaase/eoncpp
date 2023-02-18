#pragma once

#include "Definitions.h"
#include "TypeTuple.h"
#include "TypeMap.h"
#include <eonsource/SourceRef.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	class Tuple;




	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Attribute class - eon::Attribute
	//
	// Tuple attribute.
	//
	class Attribute
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default constructor for place-holder instance object.
		inline Attribute() { _prep(); }

		// Construct for any supported value type by copying it.
		// Throws eon::type::AccessDenied if value is not legal for any tuple type!
		template<typename T>
		inline Attribute( const T& value, type::hint type_hint = type::hint::none ) {
			_construct( value, type_hint ); Value = new T( value ); }

		// Construct for any supported value type by taking over ownership of it.
		// Throws eon::type::AccessDenied if value is not legal for any tuple type!
		template<typename T>
		inline Attribute( T&& value, type::hint type_hint = type::hint::none ) {
			_construct( value, type_hint ); Value = new T( std::move( value ) ); }

		// Copy another attribute, if there is a value, it will be copied!
		Attribute( const Attribute& other ) { *this = other; }

		// Copy another attribute, if there is a value, it will be referenced!
		Attribute( Attribute& other ) { *this = other; }

		// Take ownership of another attribute's details
		Attribute( Attribute&& other ) noexcept { *this = std::move( other ); }

		inline virtual ~Attribute() { _clear(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Factory methods
		//
	public:

		// Create attribute for specified type without value.
		static inline Attribute newVoid( TypeTuple type, type::Qualifier qualifiers ) {
			Attribute a; a.Type = type; a.Qualifiers = qualifiers; return a; }

		// Create tuple attribute.
		static Attribute newTuple( Tuple&& value, type::Qualifier qualifiers, source::Ref source = source::Ref() );

		// Create name attribute.
		static Attribute newName( name_t value, type::Qualifier qualifiers, source::Ref source = source::Ref() );

		// Create type-tuple attribute.
		static Attribute newTypeTuple( TypeTuple&& value, type::Qualifier qualifiers, source::Ref source = source::Ref() );

		// Create type-tuple attribute.
		static Attribute newTypeTuple(
			const TypeTuple& value, type::Qualifier qualifiers, source::Ref source = source::Ref() );

		// Create attribute for value of specified type (no explicit type-deduction!).
		template<typename T>
		static Attribute newExplicit(
			T&& value, name_t value_type, type::Qualifier qualifiers, source::Ref source = source::Ref() )
		{
			Attribute a;
			a.Type = value_type;
			a.Qualifiers = qualifiers;
			a.Value = new T( std::move( value ) );
			a.Source = source;
			return a;
		}

		// Create attribute for value of specified type (no explicit type-deduction!).
		template<typename T>
		static Attribute newExplicit(
			const T& value, name_t value_type, type::Qualifier qualifiers, source::Ref source = source::Ref() )
		{
			Attribute a;
			a.Type = value_type;
			a.Qualifiers = qualifiers;
			a.Value = new T( value );
			a.Source = source;
			return a;
		}

		// Create attribute for value, deduce type.
		// Some types are different in Eon but the same in C++. (These can be byte_t vs. b8, index_t vs. b32 or b64.)
		// To ensure correct type, please provide a 'hint'.
		// Throws eon::type::AccessDenied if value is not legal for any tuple type!
		template<typename T>
		static Attribute newImplicit(
			T&& value,
			type::Qualifier qualifiers,
			type::hint type_hint = type::hint::none,
			source::Ref source = source::Ref() )
		{
			auto type_name = type::mapCppType( std::type_index( typeid( T ) ), type_hint );
			if( type_name == no_name )
				throw type::AccessDenied();
			Attribute a;
			a.Type = type_name;
			a.Qualifiers = qualifiers;
			a.Value = new T( std::move( value ) );
			a.Source = source;
			return a;
		}

		// Create attribute for value, deduce type.
		// Some types are different in Eon but the same in C++. (These can be byte_t vs. b8, index_t vs. b32 or b64.)
		// To ensure correct type, please provide a 'hint'.
		// Throws eon::type::AccessDenied if value is not legal for any tuple type!
		template<typename T>
		static Attribute newImplicit(
			const T& value,
			type::Qualifier qualifiers,
			type::hint type_hint = type::hint::none,
			source::Ref source = source::Ref() )
		{
			auto type_name = type::mapCppType( std::type_index( typeid( T ) ), type_hint );
			if( type_name == no_name )
				throw type::AccessDenied();
			Attribute a;
			a.Type = type_name;
			a.Qualifiers = qualifiers;
			a.Value = new T( value );
			a.Source = source;
			return a;
		}




		///////////////////////////////////////////////////////////////////////
		//
		// General modifiers
		//
	public:

		// Copy another attribute, if there is a value, it will be copied!
		Attribute& operator=( const Attribute& other );

		// Copy another attribute, if there is a value, it will be referenced!
		Attribute& operator=( Attribute& other );

		// Take ownership of another attribute's details.
		Attribute& operator=( Attribute&& other ) noexcept;

		// Attach a source to the attribute
		inline void source( eon::source::Ref src ) noexcept { Source = src; }




		///////////////////////////////////////////////////////////////////////
		//
		// Basic access
		//
	public:

		// Check that the attribute is not nothing.
		inline operator bool() const noexcept { return static_cast<bool>( Type ); }

		// Get attribute type
		inline const TypeTuple& type() const noexcept { return Type; }

		// Get attribute value
		// Throws eon::type::WrongType if value has a different type than requested
		template<typename T, type::hint type_hint = type::hint::none>
		T& value()
		{
			if( Type.nameValue() == name_syntax )
			{
				if( typeid( T ) == typeid( name_t ) )
					return *(T*)Value;
				else
					throw type::WrongType( "Trying to get \"" + eon::str( type::mapCppType( CPPTYPE( T ), type_hint ) )
						+ "\" value from attribute when it actually is a syntax element!" );
			}

			TypeTuple type( type::mapCppType( CPPTYPE( T ), type_hint ) );
			if( type.nameValue() == name_typetuple )
				return *(T*)Value;
			else if( Type.nameValue() == name_ttypetuple && type.name() == name_tuple )
				return *(T*)Value;
			else if( type != Type )
				throw type::WrongType( "Trying to get \"" + eon::str( type::mapCppType( CPPTYPE( T ), type_hint ) )
					+ "\" value from attribute when it actually is " + Type.str() + "!" );
			return *(T*)Value;
		}
		template<typename T, type::hint type_hint = type::hint::none>
		inline const T& value() const { return *(const T*)&( (Attribute*)this )->value<T, type_hint>(); }

		// Get source (if any).
		const source::Ref& source() const noexcept { return Source; }
		source::Ref& source() noexcept { return Source; }

		// Stringify value
		void str( Stringifier& strf ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Qualifiers
		//
		// These are included to help the Æon compiler and the Expression class!
		//
	public:

		// Mark as owned value.
		// NOTE: This will unmark borrowed, optional, and pointer qualifiers!
		inline void markOwned() noexcept { unmarkBorrowed(); unmarkOptional(); Qualifiers |= type::Qualifier::_owned; }

		// Check if owned value.
		inline bool isOwned() const noexcept { return Qualifiers && type::Qualifier::_owned; }

		// Unmark as owned value.
		inline void unmarkOwned() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_owned; }


		// Mark as borrowed value.
		// NOTE: This will unmark optional and pointer qualifiers but not owned!
		inline void markBorrowed() noexcept { unmarkOptional(); unmarkPointer(); Qualifiers |= type::Qualifier::_borrowed; }

		// Check if borrowed value.
		inline bool isBorrowed() const noexcept { return Qualifiers && type::Qualifier::_borrowed; }

		// Unmark as borrowed value.
		inline void unmarkBorrowed() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_borrowed; }


		// Mark as optional value.
		// NOTE: This will unmark owned, borrowed, and pointer qualifiers!
		inline void markOptional() noexcept {
			unmarkOwned(); unmarkBorrowed(); unmarkPointer(); Qualifiers |= type::Qualifier::_optional; }

		// Check if optional value.
		inline bool isOptional() const noexcept { return Qualifiers && type::Qualifier::_optional; }

		// Unmark as optional value.
		inline void unmarkOptional() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_optional; }


		// Mark as pointer value.
		// NOTE: This will unmark owned and borrowed qualifiers but not optional!
		inline void markPointer() noexcept { unmarkOwned(); unmarkBorrowed(); Qualifiers |= type::Qualifier::_pointer; }

		// Check if pointer value.
		inline bool isPointer() const noexcept { return Qualifiers && type::Qualifier::_pointer; }

		// Unmark as pointer value.
		inline void unmarkPointer() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_pointer; }


		// Mark as reference value.
		inline void markReference() noexcept { Qualifiers |= type::Qualifier::_reference; }

		// Check if reference value.
		inline bool isReference() const noexcept { return Qualifiers && type::Qualifier::_reference; }

		// Unmark as reference value.
		inline void unmarkReference() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_reference; }


		// Mark as literal value.
		// NOTE: This will also mark rvalue qualifier!
		inline void markLiteral() noexcept { Qualifiers |= type::Qualifier::_literal | type::Qualifier::_rvalue; }

		// Check if literal.
		inline bool isLiteral() const noexcept { return Qualifiers && type::Qualifier::_literal; }

		// Unmark as literal.
		// NOTE: This will not unmark rvalue qualifier!
		inline void unmarkLiteral() noexcept { Qualifiers = Qualifiers & ~type::Qualifier::_literal; }


		// Mark as rvalue.
		// NOTE: This will not unmark literal qualifier!
		inline void markRvalue() noexcept { Qualifiers |= type::Qualifier::_rvalue; }

		// Check if rvalue.
		inline bool isRvalue() const noexcept { return Qualifiers && type::Qualifier::_rvalue; }

		// Unmark as literal.
		// NOTE: This will also unmark literal qualifier!
		inline void unmarkRvalue() noexcept { unmarkLiteral();  Qualifiers = Qualifiers & ~type::Qualifier::_literal; }


		// Set qualifier flags directly.
		inline void qualifier( type::Qualifier qualifier ) noexcept { Qualifiers = qualifier; }

		// Get qualifier flags directly.
		inline type::Qualifier qualifier() const noexcept { return Qualifiers; }




		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	private:

		class TypeHandler
		{
		public:
			virtual void destroy( void* value ) noexcept = 0;
			virtual void* copy( const void* value ) = 0;
			virtual void str( Stringifier& strf, const void* value ) const = 0;
		};
		using TypeHandlerPtr = std::shared_ptr<TypeHandler>;

		class TypeHandlerStr : public TypeHandler
		{
		public:
			void destroy( void* value ) noexcept override { delete (string*)value; }
			void* copy( const void* value ) override { return new string( *(string*)value ); }
			void str( Stringifier& strf, const void* value ) const override { strf.word( *(string*)value ); }
		};

		class TypeHandlerName : public TypeHandler
		{
		public:
			void destroy( void* value ) noexcept override { delete (name_t*)value; }
			void* copy( const void* value ) override { return new name_t( *(name_t*)value ); }
			void str( Stringifier& strf, const void* value ) const override { strf.word( eon::str( *(name_t*)value ) ); }
		};

		template<typename T>
		class TypeHandlerStrMethod : public TypeHandler
		{
		public:
			void destroy( void* value ) noexcept override { delete (T*)value; }
			void* copy( const void* value ) override { return new T( *(T*)value ); }
			void str( Stringifier& strf, const void* value ) const override { strf.word( ((T*)value)->str() ); }
		};

		template<typename T>
		class TypeHandlerStrSupported : public TypeHandler
		{
		public:
			void destroy( void* value ) noexcept override { delete (T*)value; }
			void* copy( const void* value ) override { return new T( *(T*)value ); }
			void str( Stringifier& strf, const void* value ) const override { strf.word( string( *(T*)value ) ); }
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _prep();
		void _clear() noexcept;

		template<typename T>
		void _construct( const T& value, type::hint type_hint = type::hint::none )
		{
			_prep();
			auto type_name = type::mapCppType( CPPTYPE( T ), type_hint );
			if( type_name == no_name )
				throw type::AccessDenied();
			if( type_name == name_tuple )
				Type = _tupleType( (void*)&value );
			else
				Type = type_name;
		}

		TypeTuple _tupleType( void* value ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		TypeTuple Type;
		source::Ref Source;
		void* Value{ nullptr };
		type::Qualifier Qualifiers{ type::Qualifier::_owned };

		static std::unordered_map<name_t, TypeHandlerPtr> TypeHandlers;
	};
}
