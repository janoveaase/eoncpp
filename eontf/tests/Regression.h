#pragma once

#include <eontest/Test.h>
#include <eontf/Etf.h>


namespace eon
{
	class EtfTest : public eontest::EonTest {};

	class BoolTest : public eontest::EonTest {};
	class CharTest : public eontest::EonTest {};
	class IntTest : public eontest::EonTest {};
	class FloatTest : public eontest::EonTest {};
	class NameTest : public eontest::EonTest {};
	class StringTest : public eontest::EonTest {};
	class BinaryTest : public eontest::EonTest {};
	class RawTest : public eontest::EonTest {};
	class RegexTest : public eontest::EonTest {};
	class RefTest : public eontest::EonTest {};

	class TupleTest : public eontest::EonTest {};
	class MetaTest : public eontest::EonTest {};

	class ValidateTest : public eontest::EonTest {};
}
