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
#ifdef EON_UNIX
#	include <cmath>
#endif


namespace eon
{
	namespace tup
	{
		name_t mapBasicType( basic_type type )
		{
			switch( type )
			{
				case basic_type::bool_t:
					return name_bool;
				case basic_type::char_t:
					return name_char;
				case basic_type::int_t:
					return name_int;
				case basic_type::float_t:
					return name_float;
				case basic_type::name_t:
					return name_name;
				case basic_type::string_t:
					return name_string;
				case basic_type::binary_t:
					return name_binary;
				case basic_type::raw_t:
					return name_raw;
				case basic_type::regex_t:
					return name_regex;
				case basic_type::ref_t:
					return name_reference;
				case basic_type::var_t:
					return name_variable;
				case basic_type::expr_t:
					return name_expression;
				case basic_type::tuple_t:
					return name_tuple;
				case basic_type::meta_t:
					return name_meta;
				default:
					return name_undef;
			}
		}




		basic_type value::softType( tup::variables& vars ) const
		{
			if( isVar() )
			{
				auto val = vars.get( hardVar() );
				if( !val )
					throw NotFound( "No such variable: " + *hardVar() );
				return val->type();
			}
			else if( isRef() )
			{
				return Type;
			}
			else
				return Type;
		}


		valueptr value::evaluate( operators::code op_code, variables& vars )
			const
		{
			auto type = softType( vars );
			switch( op_code )
			{
				case operators::code::_not:
					return valueptr( new boolval( !softBool( vars ) ) );
				case operators::code::sqrt:
					return valueptr( new floatval(
						std::sqrt( softFloat( vars ) ) ) );
				case operators::code::unary_minus:
					if( type == tup::basic_type::int_t )
						return valueptr( new intval( -softInt( vars ) ) );
					else if( type == tup::basic_type::float_t )
						return valueptr( new floatval(
							-softFloat( vars ) ) );
					break;
				default:
					break;
			}
			throw UnsupportedOperand( "Unary operator '"
				+ operators::mapCode( op_code )
				+ "' not supported for " + *mapBasicType( type )
				+  " operand" );
		}
		valueptr value::evaluate( operators::code op_code,
			const valueptr& arg2, variables& vars ) const
		{
			auto type1 = Type;
			if( isVar() && vars.get( hardVar() ) )
				type1 = softType( vars );
			auto type2 = arg2->softType( vars );
			switch( op_code )
			{
				case operators::code::_and:
					return valueptr( new boolval( softBool( vars )
						&& arg2->softBool( vars ) ) );
				case operators::code::_or:
					return valueptr( new boolval( softBool( vars )
						|| arg2->softBool( vars ) ) );
				case operators::code::eq:
					return valueptr( new boolval(
						softCompare( arg2, vars ) == 0 ) );
				case operators::code::ne:
					return valueptr( new boolval(
						softCompare( arg2, vars ) != 0 ) );
				case operators::code::lt:
					return valueptr( new boolval(
						softCompare( arg2, vars ) < 0 ) );
				case operators::code::le:
					return valueptr( new boolval(
						softCompare( arg2, vars ) <= 0 ) );
				case operators::code::gt:
					return valueptr( new boolval(
						softCompare( arg2, vars ) > 0 ) );
				case operators::code::ge:
					return valueptr( new boolval(
						softCompare( arg2, vars ) >= 0 ) );
				case operators::code::plus:
				{
					if( type1 == basic_type::int_t )
						return valueptr( new intval(
							softInt( vars ) + arg2->softInt( vars ) ) );
					else if( type1 == basic_type::float_t )
						return valueptr( new floatval(
							softFloat( vars ) + arg2->softFloat( vars ) ) );
					else if( type1 == basic_type::string_t )
						return valueptr( new stringval(
							softString( vars ) + arg2->softString( vars ) ) );
					else if( type1 == basic_type::binary_t )
						return valueptr( new binaryval(
							softBinary( vars ) + arg2->softBinary( vars ) ) );
					else if( type1 == basic_type::raw_t )
					{
						auto a = softRaw( vars );
						auto& b = arg2->softRaw( vars );
						a.insert( a.end(), b.begin(), b.end() );
						return valueptr( new rawval( a ) );
					}
					else if( type1 == basic_type::tuple_t )
					{
						auto a = softTuple( vars );
						a.append( arg2->softTuple( vars ), vars );
						return valueptr( new tupleval( std::move( a ) ) );
					}
					else if( type1 == basic_type::meta_t )
					{
						auto a = softMeta( vars );
						a.append( arg2->softMeta( vars ), vars );
						return valueptr( new metaval( std::move( a ) ) );
					}
					break;
				}
				case operators::code::minus:
					if( type1 == tup::basic_type::int_t )
						return valueptr( new intval( softInt( vars )
							- arg2->softInt( vars ) ) );
					else if( type1 == tup::basic_type::float_t )
						return valueptr( new floatval( softFloat( vars )
							- arg2->softFloat( vars ) ) );
					else if( type1 == tup::basic_type::string_t )
						return valueptr(
							new stringval( softString( vars ).remove(
								arg2->softString( vars ) ) ) );
					break;
				case operators::code::multiply:
					if( type1 == tup::basic_type::int_t )
						return valueptr( new intval( softInt( vars )
							* arg2->softInt( vars ) ) );
					else if( type1 == tup::basic_type::float_t )
						return valueptr( new floatval( softFloat( vars )
							* arg2->softFloat( vars ) ) );
					break;
				case operators::code::divide:
					if( type1 == tup::basic_type::int_t )
					{
						auto b = arg2->softInt( vars );
						if( b == 0 )
							throw DivisionByZero();
						return valueptr( new intval( softInt( vars ) / b ) );
					}
					else if( type1 == tup::basic_type::float_t )
					{
						auto b = arg2->softFloat( vars );
						if( b == 0.0 )
							throw DivisionByZero();
						return valueptr( new floatval(
							softFloat( vars ) / b ) );
					}
					break;
				case operators::code::pow:
					if( type1 == tup::basic_type::int_t )
						return valueptr( new floatval(
							std::pow( softInt( vars ),
								arg2->softInt( vars ) ) ) );
					else if( type1 == tup::basic_type::float_t )
						return valueptr( new floatval(
							std::pow( softFloat( vars ),
								arg2->softFloat( vars ) ) ) );
					break;
				case operators::code::mod:
					if( type1 == tup::basic_type::int_t )
						return valueptr( new intval( softInt( vars )
							% arg2->softInt( vars ) ) );
					break;
				case operators::code::match:
					if( type1 == tup::basic_type::string_t
						&& type2 == tup::basic_type::regex_t )
					{
						auto trg = softString( vars );
						auto match = arg2->softRegex( vars ).match( trg );
						if( match )
						{
							for( auto& capt : match )
								vars.set( capt.first,
									valueptr( new stringval( capt.second ) ) );
							return valueptr( new boolval( true ) );
						}
						return valueptr( new boolval( false ) );
					}
					break;
				case operators::code::assign:
				{
					auto name = no_name;
					if( isName() ) name = hardName();
						else if( isVar() ) name = hardVar();
						else break;
					if( arg2->isVar() )
					{
						auto val = vars.get( name );
						if( !val )
							throw NotFound( "No such variable: " + *name );
						vars.set( name, val );
					}
					else
						vars.set( name, arg2 );
					return valueptr( new variableval( name ) );
				}
				default:
					break;
			}
			throw UnsupportedOperand( "Binary operator '"
				+ operators::mapCode( op_code )
				+ "' not supported for " + *mapBasicType( Type )
				+ " operand" );
		}
		valueptr value::evaluate( operators::code op_code,
			const valueptr& arg1, const valueptr& arg3, variables& vars ) const
		{
			switch( op_code )
			{
				case operators::code::if_else:
					return softBool( vars ) ? arg1 : arg3;
				default:
					break;
			}
			throw UnsupportedOperand( "Unary operator '"
				+ operators::mapCode( op_code )
				+ "' not supported for " + *mapBasicType( Type )
				+ " operand" );
		}
	}
}
