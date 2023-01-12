#pragma once

#include <eontest/Test.h>
#include <eonsource/String.h>
#include <eonfilesys/FileSys.h>
#include <eonedoc/Edoc.h>
#include <eonedoc/ToHtml.h>
#include <eontypesystem/TupleFactory.h>
#include <eontypesystem/Expression.h>
#include <eontypesystem/Bool.h>
#include <eontypesystem/Byte.h>
#include <eontypesystem/Char.h>
#include <eontypesystem/IntTypes.h>
#include <eontypesystem/FloatTypes.h>
#include <eontypesystem/Index.h>
#include <eontypesystem/Name.h>
#include <eontypesystem/Handle.h>
#include <eontypesystem/BytesActions.h>
#include <eontypesystem/StringActions.h>
#include <eontypesystem/BitsTypes.h>
#include <eontypesystem/TupleActions.h>

namespace eon
{
	class Prep
	{
	public:
		void prep()
		{
			scope::global().clear();
			type::registerBool();
			type::registerByte();
			type::registerChar();
			type::registerIndex();
			type::registerName();
			type::registerHandle();
			type::registerInt<int_t>();
			type::registerInt<short_t>();
			type::registerInt<long_t>();
			type::registerFloat<flt_t>();
			type::registerFloat<low_t>();
			type::registerFloat<high_t>();
			type::registerBits<b8_t>();
			type::registerBits<b16_t>();
			type::registerBits<b32_t>();
			type::registerBits<b64_t>();
			type::registerBytes();
			type::registerString();
			type::registerTupleActions();
		}
	};

	class EdocTest : public eontest::EonTest, public Prep
	{
	public:

		void prepare() override
		{
			prep();
		}
	};
	class ToHtmlTest : public eontest::EonTest, public Prep
	{
	public:

		void prepare() override
		{
			prep();
		}
	};
	class ToHtmlFileTest : public eontest::EonTestSandbox, public Prep
	{
	public:
		ToHtmlFileTest() : eontest::EonTestSandbox( "edoc", "tohtml" ) {}

		void prepare() override
		{
			prep();
		}
	};
}
