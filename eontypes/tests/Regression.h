#pragma once

#include <eontest/Test.h>
#include <eontypes/Tuple.h>


namespace eon
{
	class BoolTest : public eontest::EonTest {};
	class ByteTest : public eontest::EonTest {};
	class CharTest : public eontest::EonTest {};
	class IntTest : public eontest::EonTest {};
	class ShortTest : public eontest::EonTest {};
	class LongTest : public eontest::EonTest {};
	class FloatTest : public eontest::EonTest {};
	class LowTest : public eontest::EonTest {};
	class HighTest : public eontest::EonTest {};
	class IndexTest : public eontest::EonTest {};
	class NameTest : public eontest::EonTest {};
	class HandleTest : public eontest::EonTest {};

	class BytesTest : public eontest::EonTest {};
	class StringTest : public eontest::EonTest {};
	class RegexTest : public eontest::EonTest {};
	class PathTest : public eontest::EonTest {};
	class NamePathTest : public eontest::EonTest {};

	class TupleTest : public eontest::EonTest {};
	class TupleStrTest : public eontest::EonTest {};

	class TypeTest : public eontest::EonTest {};
	
	class GlobalTest : public eontest::EonTest {};
}
