#pragma once

#include "TypeDefinitions.h"
#include "Scope.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Index.h"
#include "IntegerActions.h"
#include "Integer.h"
#include "FloatingptActions.h"
#include "Floatingpt.h"
#include "Handle.h"
#include "Name.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	void registerPrimitives( scope::Global& scope );
	bool isPrimitive( name_t type_name );
}
