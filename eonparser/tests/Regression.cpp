﻿#include "Regression.h"



namespace eon
{
	TEST( ExpressionParserTest, minimal )
	{
		// The smallest expression is one containing only a single operand
		string str{ "9.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		REQUIRE_TRUE( result.type() == name_float );
		WANT_EQ( 9.9, result.value<flt_t>() );
	}

	TEST( ExpressionParserTest, literal_bool_true )
	{
		string str{ "true" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, literal_bool_false)
	{
		string str{ "false" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, literal_byte )
	{
		string str{ "B'x'" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_byte, error ) ) << error;
		WANT_EQ( 'x', (result.value<byte_t, type::hint::byte>() ) );
	}

	TEST( ExpressionParserTest, literal_char )
	{
		string str{ "'X'" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_char, error ) ) << error;
		WANT_EQ( 'X', result.value<char_t>() );
	}

	TEST( ExpressionParserTest, literal_int_positive )
	{
		string str{ "55" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 55, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, literal_int_negative )
	{
		string str{ "-99" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( -99, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, literal_float_positive )
	{
		string str{ "3.22" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_float, error ) ) << error;
		WANT_EQ( 3.22, result.value<flt_t>() );
	}

	TEST( ExpressionParserTest, literal_float_negative )
	{
		string str{ "-12.3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_float, error ) ) << error;
		WANT_EQ( -12.3, result.value<flt_t>() );
	}

	TEST( ExpressionParserTest, literal_names )
	{
		string str{ "operator" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_name, error ) ) << error;
		WANT_EQ( name_operator, ( result.value<name_t>() ) );
	}

	TEST( ExpressionParserTest, literal_names_explicit )
	{
		string str{ "#a" };
		scope::global().add( name( "a" ), 99 );
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_name, error ) ) << error;
		WANT_EQ( name( "a" ), ( result.value<name_t>() ) );
	}

	TEST( ExpressionParserTest, literal_bytes )
	{
		string str{ "B\"bytes\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bytes, error ) ) << error;
		WANT_EQ( "bytes", result.value<std::string>() );
	}

	TEST( ExpressionParserTest, literal_string )
	{
		string str{ "\"a string\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_string, error ) ) << error;
		WANT_EQ( "a string", result.value<string>() );
	}

	TEST( ExpressionParserTest, literal_regex )
	{
		string str{ "r\"\\d+\\w+\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_regex, error ) ) << error;
		WANT_EQ( "\\d+\\w+", result.value<regex>().str() );
	}

	TEST( ExpressionParserTest, literal_namepath )
	{
		string str{ "@one/two/three" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_namepath, error ) ) << error;
		WANT_EQ( "@one/two/three", result.value<namepath>().str() );
	}

	TEST( ExpressionParserTest, literal_path )
	{
		string str{ "p\"C:/Program Files/eon\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_path, error ) ) << error;
		WANT_EQ( "C:/Program Files/eon", result.value<path>().str() );
	}


	TEST( ExpressionParserTest, literal_statictuple )
	{
		string str{ "static( '2' , x  = \"x\" )" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_tuple, error ) ) << error;
		WANT_EQ( "static('2', x=\"x\")", result.value<Tuple>().str() );
	}

	TEST( ExpressionParserTest, literal_typetuple )
	{
		string str{ "T((int, int),named=(int, int))" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_typetuple, error ) ) << error;
		WANT_EQ(
			"T(tuple, type=static, value=((tuple, type=static, value=(int, int)), named=(tuple, type=static, "
			"value=(int, int))))", result.value<TypeTuple>().str() );
	}

	TEST( ExpressionParserTest, literal_expression )
	{
		string str{ "ex(1.1  +  2.2)" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		REQUIRE_EQ( "1.1 + 2.2", expr.str() );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_float, error ) ) << error;
		WANT_EQ( 3.3, result.value<flt_t>() );
	}

	TEST( ExpressionParserTest, basic_plus )
	{
		string str{ "1 + 2" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 3, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, basic_minus )
	{
		string str{ "3 - 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( -1, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, basic_multiply )
	{
		string str{ "5 * 6" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 30, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, basic_divide )
	{
		string str{ "8 / 2" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 4, result.value<int_t>() );
	}


	TEST( ExpressionParserTest, basic_mod )
	{
		string str{ "5 mod 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 2, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, basic_pow )
	{
		string str{ "3^4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 81, result.value<int_t>() );
	}


	TEST( ExpressionParserTest, basic_var_rhs )
	{
		string str{ "3 + a" };
		scope::global().add( name( "a" ), int_t( 4 ) );
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 7, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, basic_var_lhs )
	{
		// Variables on left side of arithmetic assignment
		string str{ "a += 2" };
		scope::global().add( name( "a" ), int_t( 4 ) );
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 6, result.value<int_t>() );
	}


	TEST( ExpressionParserTest, precedence_override )
	{
		string str{ "(1 + 2) * 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 9, result.value<int_t>() );
	}


	TEST( ExpressionParserTest, int_cmp_lt )
	{
		string str{ "3 <=> 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( -1, result.value<int_t>() );
	}
	TEST( ExpressionParserTest, int_cmp_eq )
	{
		string str{ "3 <=> 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 0, result.value<int_t>() );
	}
	TEST( ExpressionParserTest, int_cmp_gt )
	{
		string str{ "4 <=> 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_int, error ) ) << error;
		WANT_EQ( 1, result.value<int_t>() );
	}

	TEST( ExpressionParserTest, int_lt_lt )
	{
		string str{ "3 < 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_lt_eq )
	{
		string str{ "4 < 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_lt_gt )
	{
		string str{ "4 < 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, int_le_lt )
	{
		string str{ "3 <= 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_le_eq )
	{
		string str{ "3 <= 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_gt )
	{
		string str{ "4 <= 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, int_gt_lt )
	{
		string str{ "3 > 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_gt_eq )
	{
		string str{ "3 > 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_gt_gt )
	{
		string str{ "4 > 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, int_ge_lt )
	{
		string str{ "3 >= 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_ge_eq )
	{
		string str{ "3 >= 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_ge_gt )
	{
		string str{ "4 >= 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, int_eq_lt )
	{
		string str{ "3 == 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_eq_eq )
	{
		string str{ "3 == 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_eq_gt )
	{
		string str{ "4 == 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, int_ne_lt )
	{
		string str{ "3 != 4" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_ne_eq )
	{
		string str{ "3 != 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, int_ne_gt )
	{
		string str{ "4 != 3" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_lt_lt )
	{
		string str{ "3.9 < 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_lt_eq )
	{
		string str{ "3.9 < 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_lt_gt )
	{
		string str{ "4.1 < 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_le_lt )
	{
		string str{ "3.9 <= 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_le_eq )
	{
		string str{ "3.9 <= 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_le_gt )
	{
		string str{ "4.1 <= 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_gt_lt )
	{
		string str{ "3.9 > 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_gt_eq )
	{
		string str{ "3.9 > 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_gt_gt )
	{
		string str{ "4.1 > 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_ge_lt )
	{
		string str{ "3.9 >= 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_ge_eq )
	{
		string str{ "3.9 >= 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_ge_gt )
	{
		string str{ "4.1 >= 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_eq_lt )
	{
		string str{ "3.9 == 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_eq_eq )
	{
		string str{ "3.9 == 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_eq_gt )
	{
		string str{ "4.1 == 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, float_ne_lt )
	{
		string str{ "3.9 != 4.1" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_ne_eq )
	{
		string str{ "3.9 != 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_FALSE( result.value<bool>() );
	}
	TEST( ExpressionParserTest, float_ne_gt )
	{
		string str{ "4.1 != 3.9" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_bool, error ) ) << error;
		WANT_TRUE( result.value<bool>() );
	}

	TEST( ExpressionParserTest, bits_element )
	{
		string str{ "b8(3)[1]" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_b8, error ) ) << error;
		WANT_EQ( 1, ( result.value<b8_t, type::hint::bits>() ) );
	}

	TEST( ExpressionParserTest, int_slice )
	{
		string str{ "b8(85)[2:-2]" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		string error;
		REQUIRE_TRUE( validateResult( result, name_b8, error ) ) << error;
		WANT_EQ( 5, (result.value<b8_t, type::hint::bits>() ) );
	}

	TEST( ExpressionParserTest, int_reset )
	{
		string str{ "reset a" };
		scope::global().add( name( "a" ), int_t( 4 ) );
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<int_t>( name( "a" ) );
		WANT_EQ( 0, value );
	}

	TEST( ExpressionParserTest, string_reset )
	{
		string str{ "reset a" };
		scope::global().add( name( "a" ), string( "Value" ) );
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<string>( name( "a" ) );
		WANT_EQ( "", value );
	}


	TEST( ExpressionParserTest, bool_assign )
	{
		scope::global().add( name( "a" ), static_cast<bool>( false ) );
		string str{ "a = true" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<bool>( name( "a" ) );
		WANT_TRUE( value );
	}
	TEST( ExpressionParserTest, byte_assign )
	{
		scope::global().add( name( "a" ), static_cast<byte_t>( '0' ) );
		string str{ "a = B'Y'" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<byte_t>( name( "a" ) );
		WANT_EQ( 'Y', value );
	}
	TEST( ExpressionParserTest, char_assign )
	{
		scope::global().add( name( "a" ), static_cast<char_t>( 'c' ) );
		string str{ "a = 'C'" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<char_t>( name( "a" ) );
		WANT_EQ( 'C', value );
	}
	TEST( ExpressionParserTest, int_assign )
	{
		scope::global().add( name( "a" ), static_cast<int_t>( 3 ) );
		string str{ "a = 6" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<int_t>( name( "a" ) );
		WANT_EQ( 6, value );
	}
	TEST( ExpressionParserTest, float_assign )
	{
		scope::global().add( name( "a" ), static_cast<flt_t>( 3.3 ) );
		string str{ "a = 6.6" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<flt_t>( name( "a" ) );
		WANT_EQ( 6.6, value );
	}
	TEST( ExpressionParserTest, index_assign )
	{
		scope::global().add( name( "a" ), static_cast<index_t>( 99 ) );
		string str{ "a = 75" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<index_t>( name( "a" ) );
		WANT_EQ( 75, value );
	}
	TEST( ExpressionParserTest, name_assign )
	{
		scope::global().add( name( "a" ), Attribute::newName( eon::name_true, type::Qualifier::_none ) );
		string str{ "a = #false" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<name_t>( name( "a" ) );
		WANT_EQ( name_false, value );
	}
	TEST( ExpressionParserTest, bytes_assign )
	{
		scope::global().add( name( "a" ), std::string( "x" ) );
		string str{ "a = B\"some bytes\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<std::string>( name( "a" ) );
		WANT_EQ( "some bytes", value );
	}
	TEST( ExpressionParserTest, string_assign )
	{
		scope::global().add( name( "a" ), string( "y" ) );
		string str{ "a = \"a string\"" };
		Expression expr;
		parser::State state( std::move( str ), Report );
		REQUIRE_NO_EXCEPT( expr = parser::ExpressionParser().parse( state, scope::global() ) );
		Attribute result;
		REQUIRE_NO_EXCEPT( result = expr.execute( scope::global() ) );
		auto value = scope::global().get<string>( name( "a" ) );
		WANT_EQ( "a string", value );
	}
	// TODO: Add regex_assign, namepath_assign, path_assign!*/


	TEST( EdfParserTest, basic )
	{
		string str{
			"name=\"Donald Duck\"\n"
			"occupation=\"unemployed\"\n"
			"nephews:\n"
			"  - \"Hewey Duck\"\n"
			"  - \"Dewey Duck\"\n"
			"  - \"Louie Duck\"\n"
			"uncles=(\"Schrooge McDuck\")\n"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(name=\"Donald Duck\", occupation=\"unemployed\", nephews=(\"Hewey Duck\", \"Dewey Duck\", "
			"\"Louie Duck\"), uncles=(\"Schrooge McDuck\"))" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, list_of_named )
	{
		string str{
			"- id=alpha\n"
			"  num=1\n"
			"- id=beta\n"
			"  num=2\n"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data((id=alpha, num=1), (id=beta, num=2))" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, list_of_named_and_unnamed )
	{
		string str{
			"- id=alpha\n"
			"  num=1\n"
			"- one\n"
			"- id=beta\n"
			"  num=2\n"
			"- two"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data((id=alpha, num=1), one, (id=beta, num=2), two)" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, list_of_unnamed_tuples )
	{
		string str{
			"- (1, 2, 3)\n"
			"- (4, 5)\n"
			"- (6)\n"
			"- 7"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data((1, 2, 3), (4, 5), (6), 7)" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, list_of_named_subtuples )
	{
		string str{
			"- One:\n"
			"  - a\n"
			"  - b\n"
			"- Two:\n"
			"  - x\n"
			"  - y"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data((One=(a, b)), (Two=(x, y)))" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, comma_separation )
	{
		string str{
			"- One:\n"
			"  - a,\n"
			"  - b\n"
			"x=1,\n"
			"y=2,"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data((One=(a, b)), x=1, y=2)" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, split_bytes1 )
	{
		string str{ "B\"This string value \"  B\"is split!\"" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(B\"This string value is split!\")" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, split_bytes2 )
	{
		string str{
			"B\"This bytes value \"\n"
			"  B\"is split!\""
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(B\"This bytes value is split!\")" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, split_bytes3 )
	{
		string str{
			"B\"This bytes \"\n"
			"  B\"value \"\n"
			"    B\"is split!\""
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(B\"This bytes value is split!\")" };
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, split_string1 )
	{
		string str{ "\"This string value \"  \"is split!\"" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(\"This string value is split!\")" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, split_string2 )
	{
		string str{
			"\"This string value \"\n"
			"  \"is split!\""
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(\"This string value is split!\")" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, split_string3 )
	{
		string str{
			"\"This string \"\n"
			"  \"value \"\n"
			"    \"is split!\""
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.str();
		string exp{ "data(\"This string value is split!\")" };
		WANT_EQ( exp, act );
	}


	TEST( EdfParserTest, small_edf1 )
	{
		string str{
			"one:\n"
			"  some_name\n"
			"  value=\"string\"\n"
			"  - item1\n"
			"  - item2"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{
			"one:\n"
			"  - some_name\n"
			"  value=\"string\"\n"
			"  - item1\n"
			"  - item2"
		};
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, small_edf2 )
	{
		string str{
			"item3\n"
			"- item4:\n"
			"  'c'\n"
			"  - B'b'"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{
			"- item3\n"
			"- item4:\n"
			"  - 'c'\n"
			"  - B'b'"
		};
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, small_edf3 )
	{
		string str{
			"item5:\n"
			"  - item6\n"
			"  - item7:\n"
			"    type=test\n"
			"    value=B\"bytes\"\n"
			"    elements:\n"
			"      (one, two)"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{
			"item5:\n"
			"  - item6\n"
			"  - item7:\n"
			"    type=test\n"
			"    value=B\"bytes\"\n"
			"    elements:\n"
			"      - - one\n"
			"        - two"
		};
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, small_edf4 )
	{
		string str{
			"- item5:\n"
			"  - item6\n"
			"  item7:\n"
			"    type=test\n"
			"    value=B\"bytes\"\n"
			"    elements:\n"
			"      (one, two)\n"
			"  item8=55\n"
			"- item9"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{
			"- item5:\n"
			"  - item6\n"
			"  item7:\n"
			"    type=test\n"
			"    value=B\"bytes\"\n"
			"    elements:\n"
			"      - - one\n"
			"        - two\n"
			"  item8=55\n"
			"- item9"
		};
		WANT_EQ( exp, act );
	}

	TEST( EdfParserTest, large_edf )
	{
		string str{
			"one:\n"
			"  some_name\n"
			"  value=\"string\"\n"
			"  - item1\n"
			"  - item2\n"
			"  item3\n"
			"  - item4:\n"
			"    'c'\n"
			"    - B'b'\n"
			"  - item5:\n"
			"    - item6\n"
			"    item7:\n"
			"      type=test\n"
			"      value=B\"bytes\"\n"
			"      elements:\n"
			"        (one, two)"
		};
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{
			"one:\n"
			"  - some_name\n"
			"  value=\"string\"\n"
			"  - item1\n"
			"  - item2\n"
			"  - item3\n"
			"  - item4:\n"
			"    - 'c'\n"
			"    - B'b'\n"
			"  - item5:\n"
			"    - item6\n"
			"    item7:\n"
			"      type=test\n"
			"      value=B\"bytes\"\n"
			"      elements:\n"
			"        - - one\n"
			"          - two"
		};
		WANT_EQ( exp, act );
	}


	// EDF Rule 1 - Attribute separation:
	// Standard: Each attribute should appear on a separate line without comma-separation.
	// Legal: Each attribute should appear on a separate line with comma-separation.
	// Legal: Each attribute can appear on the same line with or without comma-separation.
	TEST( EdfParserTest, rule1_standard )
	{
		string str{ "value1\nvalue2\nvalue3\nvalue4" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- value1\n- value2\n- value3\n- value4" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule1_legal1 )
	{
		string str{ "value1,\nvalue2,\nvalue3,\nvalue4" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- value1\n- value2\n- value3\n- value4" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule1_legal2_A )
	{
		string str{ "value1, value2,value3,  value4" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- value1\n- value2\n- value3\n- value4" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule1_legal2_B )
	{
		string str{ "value1 value2   value3 value4" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- value1\n- value2\n- value3\n- value4" };
		WANT_EQ( exp, act );
	}

	// EDF Rule 2 - Named attributes with tuple values:
	// Standard: Name + 'colon':' on first line, values on subsequent indented lines.
	TEST( EdfParserTest, rule2_standard )
	{
		string str{ "name:\n  value1\n  value2\n  value3" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - value1\n  - value2\n  - value3" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule2_illegal_A )
	{
		string str{ "name: value" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		WANT_TRUE( act.empty() );
	}
	TEST( EdfParserTest, rule2_illegal_B )
	{
		string str{ "name:\n\n  value" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		WANT_TRUE( act.empty() );
	}

	// EDF Rule 3 - Named attributes with non-tuple values:
	// Standard: Name + 'equals'=' + value on the same line.
	// Legal: Tuple-value enclosed in parenthesis counts as non-tuple value.
	// Legal: Name + equals, value on subsequent indented line.
	TEST( EdfParserTest, rule3_standard )
	{
		string str{ "name=value1" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name=value1" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule3_legal_1 )
	{
		string str{ "name=(value1, value2)" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - value1\n  - value2" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule3_legal_2 )
	{
		string str{ "name=\n  value1" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name=value1" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule3_illegal_1 )
	{
		string str{ "name=value1, value2" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		WANT_TRUE( act.empty() );
	}
	TEST( EdfParserTest, rule3_illegal_2 )
	{
		string str{ "name=\n\n  value1" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		WANT_TRUE( act.empty() );
	}

	// EDF Rule 4 - Unnamed tuple attributes:
	// Standard: A dash followed by attribute value.
	// Legal: Dash can be omitted if the value is not a tuple.
	TEST( EdfParserTest, rule4_standard )
	{
		string str{ "name:\n  - value1\n  - value2" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - value1\n  - value2" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule4_legal_A )
	{
		string str{ "name:\n  value1\n  value2" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - value1\n  - value2" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule4_legal_B )
	{
		string str{ "name:\n  value1 value2" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - value1\n  - value2" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule4_legal_C )
	{
		string str{ "name:\n  - (value1, value2)" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "name:\n  - - value1\n    - value2" };
		WANT_EQ( exp, act );
	}

	// EDF Rule 5 - Bytes and String values:
	// Standard: Full value on the same line, enclosed in double quotes( bytes prefixed by 'B' ).
	// Legal : Leading part of value on first line, fully enclosed in double quotes(bytes prefixed by 'B'),
	//         remaining parts on subsequent indented lines, each part fully enclosed in double quotes
	//         (bytes prefixed by 'B').
	// Legal : Parts of value on same or subsequent indended lines, each part enclosed in double quotes
	//         (bytes prefixed by 'B').
	TEST( EdfParserTest, rule5_standard )
	{
		string str{ "B\"one two three\"\n\"alpha beta gamma\"" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- B\"one two three\"\n- \"alpha beta gamma\"" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule5_legal_A )
	{
		string str{ "B\"one \"\n  B\"two \"\n  B\"three\"\n\"alpha \"\n  \"beta \"\n  \"gamma\"" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- B\"one two three\"\n- \"alpha beta gamma\"" };
		WANT_EQ( exp, act );
	}
	TEST( EdfParserTest, rule5_legal_B )
	{
		string str{ "B\"one \" B\"two \"   B\"three\"\n\"alpha \" \"beta \"   \"gamma\"" };
		parser::State state( std::move( str ), Report );
		auto data = parser::EdfParser().parseRaw( state, scope::global() );
		auto act = data.edf();
		string exp{ "- B\"one two three\"\n- \"alpha beta gamma\"" };
		WANT_EQ( exp, act );
	}
}
