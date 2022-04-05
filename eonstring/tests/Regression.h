#pragma once

#include <eontest/Test.h>
#include <eonstring/String.h>
#include <eonstring/Name.h>
#include <eonstring/NamePath.h>
#include <eonstring/Hex.h>
#include <eonstring/ByteSerializer.h>
#include <chrono>


namespace eon
{
	class Substring : public eontest::EonTest {};
	class StaticString : public eontest::EonTest {};
	class String : public eontest::EonTest {};
	class Name : public eontest::EonTest {};
	class HexTest : public eontest::EonTest {};
	class ByteSerializerTest : public eontest::EonTest {};
}
