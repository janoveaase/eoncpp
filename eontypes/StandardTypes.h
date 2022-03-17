#pragma once

#include "TypeDefinitions.h"
#include <eonscopes/Scope.h>
#include "Bytes.h"
#include "String.h"
#include "ExpressionObj.h"
#include "Regex.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	void registerStandardTypes( scope::Global& scope );
	bool isStandard( name_t type_name );
}
