#pragma once
#include "Source.h"

/*
	A StringSource is a string with eon code
*/

class StringSource : public Source
{
public:
	SOURCE_SETUP( StringSource );
	StringSource( String&& code );
};
