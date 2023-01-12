#pragma once

#include "TypeTuple.h"
#include "Attribute.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// Tuple permissions
	enum class TuplePerm : uint8_t
	{
		// No permissions
		none = 0x00,
		
		// Attributes can be added
		add = 0x01,

		// Attributes can be removed
		remove = 0x02,

		// Attributes can be modified
		modify = 0x04,

		// Tuple can register types
		types = 0x08,

		// Tuple can register actions
		actions = 0x10,

		// All normal tuple permissions
		all_normal = add | remove | modify,
	};
	inline bool operator&( TuplePerm a, TuplePerm b ) noexcept {
		return static_cast<bool>( static_cast<int>( a ) & static_cast<int>( b ) ); }
	inline TuplePerm operator|( TuplePerm a, TuplePerm b ) noexcept {
		return static_cast<TuplePerm>( static_cast<int>( a ) | static_cast<int>( b ) ); }



	// Tuple attribute pair
	class AttributePair
	{
	public:
		AttributePair() = default;
		inline AttributePair( name_t name, Attribute&& value ) noexcept { Name = name; Value = std::move( value ); }
		inline AttributePair( Attribute&& value ) noexcept { Value = std::move( value ); }
		inline AttributePair( const AttributePair& other ) noexcept { Name = other.Name, Value = other.Value; }
		inline AttributePair( AttributePair&& other ) noexcept {
			Name = other.Name; other.Name = no_name; Value = std::move( other.Value ); }
		~AttributePair() = default;

	public:
		name_t Name{ no_name };
		Attribute Value;
	};
}
