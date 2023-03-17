#pragma once

#include <eontest/Test.h>
#include <eonparser/ExpressionParser.h>
#include <eontypesystem/TupleFactory.h>
#include <eontypesystem/BoolActions.h>
#include <eontypesystem/ByteActions.h>
#include <eontypesystem/CharActions.h>
#include <eontypesystem/IntActions.h>
#include <eontypesystem/FloatActions.h>
#include <eontypesystem/IndexActions.h>
#include <eontypesystem/NameActions.h>
#include <eontypesystem/HandleActions.h>
#include <eontypesystem/BytesActions.h>
#include <eontypesystem/StringActions.h>
#include <eontypesystem/BitsActions.h>
#include <eontypesystem/TupleActions.h>
#include <eonparser/EdfParser.h>


namespace eon
{
	class ParserTest : public eontest::EonTest
	{
	public:
		source::Reporter Report;
		void prepare() override
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
			Report.defaultTargets();
		}
	};

	class ExpressionParserTest : public ParserTest
	{
	public:
		bool validateResult( Attribute& result, name_t arg, string& error )
		{
			if( !result )
			{
				if( arg != no_name )
					error = "No value in result!";
			}
			else
			{
				if( arg == name_typetuple && result.type().nameValue() != name_ttypetuple && result.type() != arg )
					error += "Wrong type in result! Expected " + str( arg ) + ", got " + str( result.type().name() ) + ".";
			}
			return error.empty();
		}
	};

	class EdfParserTest : public ParserTest {};
}
