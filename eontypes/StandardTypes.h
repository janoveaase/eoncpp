#pragma once

#include "TypeDefinitions.h"
#include "Scope.h"
#include "Bytes.h"
#include "String.h"
//#include "Chars.h"
//#include "Real.h"
//#include "Complex.h"
#include "Regex.h"
#include "NamePath.h"
#include "Path.h"
#include "ExpressionObj.h"
//#include "Array.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	void registerStandardTypes( scope::Global& scope );
	bool isStandard( name_t type_name );
}
