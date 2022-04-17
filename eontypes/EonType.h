#pragma once

#include <eonstring/Name.h>
#include <eonsource/SourceRef.h>
#include "TypeDefinitions.h"
#include <eonstring/Stringifier.h>
#include <vector>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Type class - eon::EonType

	  A type object identifies the type of an Eon object. It is usually a
	  single name, but in the case of plain tuples, it will be a tree
	  structure.
	**************************************************************************/
	class EonType
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		EonType() = default;

		inline EonType( const EonType& other ) { *this = other; }
		inline EonType( EonType&& other ) noexcept { *this = std::move( other ); }

		inline EonType( const EonType& other, name_t attribute_name ) { *this = other; AttributeName = attribute_name; }
		inline EonType( name_t name, source::Ref source = source::Ref() ) noexcept { Name = name; Source = source; }
		inline EonType( name_t name, name_t attribute_name, source::Ref source = source::Ref() ) noexcept {
			Name = name; AttributeName = attribute_name; Source = source; }
		EonType( std::initializer_list<name_t> names, source::Ref source = source::Ref() );
		EonType( std::initializer_list<EonType> types, source::Ref source = source::Ref() );

		virtual ~EonType() = default;




		/**********************************************************************
		  Modifiers
		**********************************************************************/
	public:

		inline EonType& operator=( const EonType& other ) {
			Name = other.Name; AttributeName = other.AttributeName; Attributes = other.Attributes;
			NamedAttributes = other.NamedAttributes; Source = other.Source; return *this; }
		inline EonType& operator=( EonType&& other ) noexcept {
			Name = other.Name; other.Name = no_name; AttributeName = other.AttributeName; other.AttributeName = no_name;
			Attributes = std::move( other.Attributes ); NamedAttributes = std::move( other.NamedAttributes );
			Source = other.Source; other.Source = source::Ref(); return *this; }

		inline EonType& operator<<( EonType&& attribute ) { Name = name_plain;
			if( attribute.AttributeName != no_name ) NamedAttributes[ attribute.AttributeName ] = Attributes.size();
			Attributes.push_back( std::move( attribute ) ); return *this; }

		//* Set/change source
		inline void source( source::Ref source ) noexcept { Source = std::move( source ); }




		/**********************************************************************
		  Basic Access
		**********************************************************************/
	public:

		//* Check if type is specified
		inline operator bool() const noexcept { return Name != no_name; }

		//* Get main type name (name_plain for plain tuples)
		inline name_t name() const noexcept { return Name; }

		//* Get main type attribute name
		inline name_t attributeName() const noexcept { return AttributeName; }

		//* Get attributes of plain tuple
		//* Will be empty for empty plain tuple (and if not a plain tuple)
		inline const std::vector<EonType> attributes() const noexcept { return Attributes; }

		//* Get named attributes of plain tuple, mapped to position
		//* Will be empty if no named attributes (and if not a plain tuple)
		inline const std::unordered_map<name_t, size_t>& namedAttributes() const noexcept { return NamedAttributes; }

		//* Get position of named attribute
		//* Returns eon::EonType::no_pos if no such name
		static const index_t no_pos{ SIZE_MAX };
		inline index_t pos( name_t attribute_name ) const noexcept { auto found = NamedAttributes.find( attribute_name );
			return found != NamedAttributes.end() ? found->second : no_pos; }

		//* Get name of attribute at specified position
		//* Returns no_name if invalid position or unnamed
		inline name_t name( index_t pos ) const noexcept {
			return pos < Attributes.size() ? Attributes[ pos ].Name : no_name; }

		//* Get type tuple as 'T'-prefixed string representation
		void str( Stringifier& str ) const;
		inline string str() const { Stringifier strf; str( strf ); return strf.str(); }

		//* Get source
		inline const source::Ref& source() const noexcept { return Source; }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		inline int compare( const EonType& other ) const noexcept {
			return Name < other.Name ? -1 : Name > other.Name ? 1 : _compareAttributes( other ); }
		inline bool operator<( const EonType& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const EonType& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const EonType& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const EonType& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const EonType& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const EonType& other ) const noexcept { return compare( other ) != 0; }


		//* Comparison with name_t
		inline int compare( name_t other ) const noexcept { return Name < other ? -1 : Name > other ? 1 : 0; }
		inline bool operator<( name_t other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( name_t other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( name_t other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( name_t other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( name_t other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( name_t other ) const noexcept { return compare( other ) != 0; }
		inline friend bool operator<( name_t a, const EonType& b ) noexcept { return b >= a; }
		inline friend bool operator<=( name_t a, const EonType& b ) noexcept { return b > a; }
		inline friend bool operator>( name_t a, const EonType& b ) noexcept { return b <= a; }
		inline friend bool operator>=( name_t a, const EonType& b ) noexcept { return b < a; }
		inline friend bool operator==( name_t a, const EonType& b ) noexcept { return b != a; }
		inline friend bool operator!=( name_t a, const EonType& b ) noexcept { return b == a; }


		//* Check if the 'other' type is compatible with 'this'
		//* If neither 'this' nor 'other' are tuples: Must be equal!
		//* If 'this' is dynamic tuple:
		//*   'other' can be any type but named tuple attributes must exist in
		//*   'this' with same name and type and unnamed attributes must match
		//*   on position and type!
		//* If 'this' is data tuple:
		//*   'other' can be any type supported by data tuple plus plain tuple (but
		//*   not dynamic tuple), named plain/data tuple attributes must exist in
		//*   'this' with same name and type and unnamed attributes must match on
		//*   position and type!
		//* If 'this' is plain tuple:
		//*   'other' must be a non-tuple with type equal to the first
		//*   attribute of 'this', or a plain tuple where named attributes must
		//*   exist in 'this' with same name and type and unnamed attributes
		//*   must match on position and type!
		bool compatibleWith( const EonType& other ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _toStr( Stringifier& str ) const;

		int _compareAttributes( const EonType& other ) const noexcept;
		bool _compatibleAttributes( const EonType& other ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:

		// Main type name
		name_t Name{ no_name };

		// Optional attribute name
		name_t AttributeName{ no_name };

		// Attributes are only for Name=name_plain
		std::vector<EonType> Attributes;
		std::unordered_map<name_t, size_t> NamedAttributes;

		// Where the type is defined
		source::Ref Source;
	};
}
