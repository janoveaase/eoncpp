#pragma once

#include "TypeSystem.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"
#include "Name.h"
#include "Handle.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Primitive types
	**************************************************************************/

	//* Register all primitives in the global scope
	void registerPrimitives();

	//* Check if a named type is a primitive
	bool isPrimitive( name_t type_name ) noexcept;
}
