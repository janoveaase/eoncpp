#pragma once

#include <eontest/Test.h>
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
	class TypeTupleTest : public eontest::EonTest {};
	class TupleTest : public eontest::EonTest {};
	class ExpressionTest : public eontest::EonTest
	{
	public:
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
		}

		bool validateResult( const std::vector<Attribute>& result, std::initializer_list<name_t> args, string& error )
		{
			if( result.size() < args.size() )
			{
				error = "Too few values in result! Expected " + string( args.size() )
					+ ", got " + string( result.size() ) + ".";
			}
			else if( result.size() > args.size() )
			{
				error = "Too many values in result! Expected " + string( args.size() )
					+ ", got " + string( result.size() ) + ".";
			}
			else
			{
				index_t i = 0;
				for( auto& arg : args )
				{
					if( ( arg == name_typetuple && result[ i ].type().nameValue() != name_ttypetuple )
						&& result[ i ].type() != arg )
					{
						if( !error.empty() )
							error += "\n";
						error += "Argument #" + string( i + 1 ) + " has the wrong type! Expected " + str( arg )
							+ ", got " + str( result[ i ].type().name() ) + ".";
					}
					++i;
				}
			}
			return error.empty();
		}
	};
}
