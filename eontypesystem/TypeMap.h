#pragma once

#include "Definitions.h"
#include <eonexcept/Exception.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	EONEXCEPT( BadCppMapping );
	
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'type' namespace enclosing all background type functionality
	//
	namespace type
	{
		// Some types can have different names in Eon but be the same C++ type.
		// To ensure correct identification, these types need a hint.
		enum class hint
		{
			none,
			bits,	// The type should be one of b8, b16, b32, or b64.
			byte,	// On some platforms, byte_t and b8 can be the same.
			index,	// On some platforms, index_t and b32 or b64 can be the same.
		};

		// Map from C++ type to name_t
		// Returns eon::no_name if not a valid C++ type (for Eon)
		// Throws BadCppMapping if multiple Æon names for same C++ type and there is no (helpful) hint.
		// (The hint should be one of the following names: bits, byte, or index!)
		name_t mapCppType( std::type_index cpp_type, hint type_hint = hint::none );

		// Get size of C++ type given a name_t
		// Returns zero if not a valid C++ type (for Eon)
		index_t sizeOf( name_t cpp_type ) noexcept;
	}
}
