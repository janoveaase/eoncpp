#include "Value.h"
#include "BoolValue.h"
#include "CharValue.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "NameValue.h"
#include "StringValue.h"
#include "BinaryValue.h"
#include "RawValue.h"
#include "RegexValue.h"
#include "ReferenceValue.h"
#include "VariableValue.h"
#include "ExpressionValue.h"
#include "TupleValue.h"
#include "MetaValue.h"
#include "FunctionValue.h"
#ifdef EON_UNIX
#	include <cmath>
#endif
#include <eonfilesys/FileSys.h>


namespace eon
{
	namespace vars
	{
		name_t mapTypeCode( type_code code)
		{
			switch( code )
			{
				case type_code::bool_t:
					return name_bool;
				case type_code::char_t:
					return name_char;
				case type_code::int_t:
					return name_int;
				case type_code::float_t:
					return name_float;
				case type_code::name_t:
					return name_name;
				case type_code::string_t:
					return name_string;
				case type_code::binary_t:
					return name_binary;
				case type_code::raw_t:
					return name_raw;
				case type_code::regex_t:
					return name_regex;
				case type_code::ref_t:
					return name_reference;
				case type_code::var_t:
					return name_variable;
				case type_code::expr_t:
					return name_expression;
				case type_code::tuple_t:
					return name_tuple;
				case type_code::meta_t:
					return name_meta;
				case type_code::function_t:
					return name_function;
				default:
					return name_undef;
			}
		}




		type_code value::targetType( vars::variables& vars ) const
		{
			if( isVariable() )
			{
				auto val = vars.get( actualVariable() );
				if( !val )
					throw NotFound( "No such variable: " + *actualVariable() );
				return val->targetType( vars );
			}
			else if( isReference() )
			{
				auto val = vars.get( actualReference(), dynamic_cast<const refval*>( this )->context() );
				if( !val )
					throw NotFound( "No such reference: " + actualReference().str() );
				return val->targetType( vars );
			}
			return Type;
		}



		bool value::convertTargetBool( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertBool() : convertBool();
		}
		char_t value::convertTargetChar( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertChar() : convertChar();
		}
		int64_t value::convertTargetInt( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertInt() : convertInt();
		}
		double value::convertTargetFloat( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertFloat() : convertFloat();
		}

		name_t value::convertTargetName( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertName() : convertName();
		}
		string value::convertTargetString( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertString() : convertString();
		}
		nameref value::convertTargetReference( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertReference() : convertReference();
		}
		name_t value::convertTargetVariable( variables& vars ) const
		{
			auto trg = target( vars, valueptr() );
			return trg ? trg->convertVariable() : convertVariable();
		}




		void value::setTarget( valueptr new_value, variables& vars )
		{
			auto trg = target( vars, valueptr() );
			if( trg )
				trg->setActual( new_value );
			else
			{
				size_t pos{ 0 };
				throw NotFound( "Cannot set target of " + *mapTypeCode( Type )
					+ " (" + str( pos, 0 ) + ") - it does not exist!" );
			}
		}
	}
}
