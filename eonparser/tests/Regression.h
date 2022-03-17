#pragma once

#include <eontest/Test.h>
#include <eonsource/String.h>
#include <eontypes/Primitives.h>
#include <eontypes/BasicTupleActions.h>
#include <eontypes/StandardTypes.h>
#include <eonparser/RawParser.h>
#include <eonparser/ParseExpression.h>

namespace eon
{
	class RawParserTest : public eontest::EonTest
	{
	public:
		string format( const std::vector<parser::Element>& elements );
	};

	class ParseExpressionTest : public eontest::EonTest {};
}
