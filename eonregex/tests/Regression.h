#pragma once

#include <eontest/Test.h>
#include <eonregex/RegEx.h>


namespace eon
{
	class RegExTest : public eontest::EonTest {};
	class MatchOrTest : public eontest::EonTest {};
	class MiscTests : public eontest::EonTest {};
	class OptimizeTests : public eontest::EonTest
	{
	public:
		void optimizeTest( regex& plain, regex& optimized, string& good_str, string& bad_str, int iterations = 100000 );
	};
	class SpeedCmp : public eontest::EonTest {};
	class TestRegex : public eontest::EonTest {};
}
