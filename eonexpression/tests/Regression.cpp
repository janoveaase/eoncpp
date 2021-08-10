#include "Regression.h"
#include <eonvariables/IntValue.h>
#include <eonvariables/FloatValue.h>
#include <eonvariables/StringValue.h>



namespace eon
{
	TEST( ExpressionTest, binary1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2 pow ( 3 + 1 )", vars ) );
		WANT_EQ( "2 3 1 + pow",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"2 pow ( 3 + 1 )",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, binary2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression(
			"1 - 2 pow 3 pow 3 - ( 4 + 5 * 6 ) * 7", vars ) );
		WANT_EQ( "1 2 3 3 pow pow - 4 5 6 * + 7 * -",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"1 - 2 pow 3 pow 3 - ( 4 + 5 * 6 ) * 7",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, binary3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(4+3)*(2-1)", vars ) );
		WANT_EQ( "4 3 + 2 1 - *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 4 + 3 ) * ( 2 - 1 )",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, unary1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not 1", vars ) );
		WANT_EQ( "1 not",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "not 1",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, unary2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 and not 1 or not 0", vars ) );
		WANT_EQ( "1 1 not and 0 not or",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "1 and not 1 or not 0",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, ternary1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2", vars ) );
		WANT_EQ( "0 1 2 if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2 == 3", vars ) );
		WANT_EQ( "0 1 2 3 == if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2 == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(0 if 1 else 2) == 3", vars ) );
		WANT_EQ( "0 1 2 if_else 3 ==",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 0 if 1 else 2 ) == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, and1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true and true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true and false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false and true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false and false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, or1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true or false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 > 0 or 0 > 0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "tobool \"0\" or tobool \"\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, not1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not tobool 4.3", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, if_else1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if true else 6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isChar() ) << "Wrong value type";
		WANT_TRUE( val->actualChar() == 'T' ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if false else 6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 6 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if true else (6+4)", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 2 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if false else (6+4)", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 10 ) << "Wrong value";
	}

	TEST( ExpressionTest, eq1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == tobool 8", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat 0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq6 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat \"0.0\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq7 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"\" == tostring 1", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ne1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false != true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != toint 4.001", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4.001 != tofloat 4", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != toint 4.0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, lt1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false < true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true < false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < 2", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < toint 1.1", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha\" < \"alphA\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt6 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphA\" < \"alpha\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, le1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true <= false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= tofloat 1", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le6 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= tofloat \"1.10000\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, gt1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true > false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true > true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false > true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > 4", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > toint 5.9999", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt6 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' > 't'", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ge1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= false", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false >= true", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "99.9 >= tofloat 100", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphb\" >= \"alpha\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge6 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= tostring 'A'", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge7 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= tostring 'a'", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, plus1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + tostring 'A'", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "AA", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + tostring 6.6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "A6.6", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 + tofloat \"1.1\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.6 + 1.1, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "tofloat 1 + 2.9", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.9, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2.9 + tofloat 1", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.9, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, minus1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "10-6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 4, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6-10", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( -4, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha beta\" - tostring 'a'", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "lph bet", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "99.99 - tofloat \"000.9900\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 99.99 - 0.99, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, multiply1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3 * 6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 18, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "tofloat 3 * 6.0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 18, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3.0 * tofloat 6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 18.0, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2.2 * tofloat \"3.3\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 2.2 * 3.3, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, divide1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 3", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 / 3.3", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 2, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 0", vars ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), eon::expr::DivisionByZero );
	}

	TEST( ExpressionTest, pow1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 pow 3", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 216.0, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3.3 pow 3", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 35.937, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "10 pow -2", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 0.01, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, mod1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 5", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 6", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 0, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2 mod 5", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod4 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod toint 5.5", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod5 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "{9, 5} mod 5", vars ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), vars::UnsupportedOperand );
	}

	TEST( ExpressionTest, sqrt1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 9", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.0, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, sqrt2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 39.9424", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.32, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, match1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+ha/", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
		auto match = vars.get( name::get( "complete" ) );
		REQUIRE_TRUE( match && match->isString() )
			<< "Didn't get capture group";
		WANT_EQ( "alpha", match->string_value().stdstr() ) << "Wrong capture";
	}
	TEST( ExpressionTest, match2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+haa/", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, match3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+@<grp1>(ha)/", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->actualBool() ) << "Wrong value";
		auto grp = vars.get( name::get( "grp1" ) );
		REQUIRE_TRUE( grp && grp->isString() ) << "Didn't get capture group";
		WANT_EQ( "ha", grp->string_value().stdstr() ) << "Wrong capture";
	}

	TEST( ExpressionTest, assign1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 99.9", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isFloat() ) << "Didn't get variable";
		WANT_EQ( 99.9, var->actualFloat() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b + 321", vars ) );
		vars::valueptr val;
		vars.set( name::get( "b" ), vars::intval::create( 123 ) );
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 444, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign3 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b", vars ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate( vars ), vars::NotFound );
	}

	TEST( ReferenceTest, reference_get_nonexisting )
	{
		expression expr;
		vars::variables vars( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "a = @one/two", vars ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate( vars ), vars::NotFound );
	}
	TEST( ReferenceTest, reference_get_existing )
	{
		_set( nameref( "one/two" ), vars::stringval::create( "Hello" ) );
		expression expr;
		vars::variables vars( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "a = @one/two", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) );
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "Hello", var->actualString().stdstr() ) << "Wrong variable value";
	}
	TEST( ReferenceTest, reference_set_new )
	{
		expression expr;
		vars::variables vars( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "@one/two = 123", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) );
		auto result = _get( nameref( "one/two" ) );
		REQUIRE_TRUE( result && result->isInt() ) << "Didn't set variable";
		WANT_EQ( 123, result->actualInt() ) << "Wrong variable value";
	}
	TEST( ReferenceTest, reference_set_existing )
	{
		_set( nameref( "one/two" ), vars::stringval::create( "Hello" ) );
		expression expr;
		vars::variables vars( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "@one/two = 123", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) );
		auto result = _get( nameref( "one/two" ) );
		REQUIRE_TRUE( result && result->isInt() ) << "Didn't set variable";
		WANT_EQ( 123, result->actualInt() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, exists_yes )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 1 if exists \""
			+ string( _TestExe ).replace( '\\', '/' ) + "\" else 0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 1, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, exists_no )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 1 if exists \""
			"somethingimpossiblewhateverblahblah\" else 0", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 0, var->actualInt() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, trim )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "b = trim (a = trim \"  str  \")", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "str", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "str", varb->actualString().stdstr() ) << "Wrong variable value b";
	}
	TEST( ExpressionTest, rtrim )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "b = rtrim (a = rtrim \"  str  \")", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "  str", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "  str", varb->actualString().stdstr() ) << "Wrong variable value b";
	}
	TEST( ExpressionTest, ltrim )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "b = ltrim (a = ltrim \"  str  \")", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "str  ", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "str  ", varb->actualString().stdstr() ) << "Wrong variable value b";
	}

	TEST( ExpressionTest, len_str )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len \"0123456789\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 10, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_raw )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len |\n  one\n  two\n  three", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_bin)
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len %0A417D", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_tuple )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len {one two three}", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, resize_grow )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"one\" resize 5", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "one  ", var->actualString().stdstr() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, resize_shrink )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"one\" resize 1", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "o", var->actualString().stdstr() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, type1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "typeof \"str\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isName() );
		WANT_TRUE( val->actualName() == name_string );
	}
	TEST( ExpressionTest, type2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "typeof 3.14", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isName() );
		WANT_TRUE( val->actualName() == name_float );
	}

	TEST( ExpressionTest, isname1 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "isname \"012alpha_21\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isBool() );
		WANT_TRUE( val->actualBool() );
	}
	TEST( ExpressionTest, isname2 )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "isname \"012al pha_21\"", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isBool() );
		WANT_FALSE( val->actualBool() );
	}

	TEST( ExpressionTest, name )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "tovar(\"a\" + tostring 2) = 99.9", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "a2" ) );
		REQUIRE_TRUE( var && var->isFloat() ) << "Didn't get variable";
		WANT_EQ( 99.9, var->actualFloat() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, double_parenthesis )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "((5-1)/(4-2))*3", vars ) );
		WANT_EQ( "5 1 - 4 2 - / 3 *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"( 5 - 1 ) / ( 4 - 2 ) * 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, complex_len )
	{
		expression expr;
		vars::variables vars;
		vars.set( name_token, vars::stringval::create( "    " ) );
		REQUIRE_NO_EXCEPT( expr = expression(
			"(indentation = len token if not tobool (ltrim token) else 0)", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "indentation" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 4, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, complex_if_else )
	{
		expression expr;
		vars::variables vars;
		vars.set( name_name, vars::stringval::create( "one" ) );
		REQUIRE_NO_EXCEPT( expr = expression(
			"id=\"namespace__\" + ((parent_id + \"__\") if tobool parent_id else \"\") + name", vars ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = vars.get( name::get( "id" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "namespace__one", var->actualString().stdstr() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, multiple )
	{
		expression expr;
		vars::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "x = 1 + 2; x > 2", vars ) );
		WANT_EQ( "x 1 2 + =; x 2 >", expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "x = 1 + 2; x > 2", expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, stress )
	{
		expression expr;
		vars::variables vars;
		eon::string str{ "tag = \"Even stephen\" if toint number mod 2 == 0 else \"N/A\"; "
			"ln = letter + \":\" + tostring number; "
			"sz = len letter; "
			"_match = m if letter match ?/^.*?@<m>(p.*i).*$/ else \"N/A\"" };
		try
		{
			expr = expression( std::move( str ), vars );
		}
		catch( eon::exception& e )
		{
			FATAL_MESSAGE << e.details().stdstr() << "\n";
		}
		auto letter = name::get( "letter" );
		auto number = name::get( "number" );
		auto tag = name::get( "tag" );
		auto ln = name::get( "ln" );
		auto sz = name::get( "sz" );
		auto match = name::get( "_match" );

		std::vector<vars::valueptr> letters{
			vars::stringval::create( "alpha" ),
			vars::stringval::create( "beta" ),
			vars::stringval::create( "gamma" ),
			vars::stringval::create( "delta" ),
			vars::stringval::create( "epsilon" ),
			vars::stringval::create( "zeta" ),
			vars::stringval::create( "eta" ),
			vars::stringval::create( "theta" ),
			vars::stringval::create( "iota" ),
			vars::stringval::create( "kappa" ),
			vars::stringval::create( "lambda" ),
			vars::stringval::create( "mu" ),
			vars::stringval::create( "nu" ),
			vars::stringval::create( "xi" ),
			vars::stringval::create( "omicron" ),
			vars::stringval::create( "pi" ),
			vars::stringval::create( "rho" ),
			vars::stringval::create( "sigma" ),
			vars::stringval::create( "tau" ),
			vars::stringval::create( "upsilon" ),
			vars::stringval::create( "phi" ),
			vars::stringval::create( "chi" ),
			vars::stringval::create( "psi" ),
			vars::stringval::create( "omega" ) };
		std::vector<vars::valueptr> numbers;
		for( double i = 0.0; i < 100.0; i += 1.0 )
			numbers.push_back( vars::floatval::create( i ) );

		int rounds = 100000;
#ifdef _DEBUG
		rounds /= 10;
#endif

		std::list<eon::string> result;
		size_t l = 0, n = 0;
		std::chrono::steady_clock clock;
		auto start = clock.now();
		for( int i = 0; i < rounds; ++i )
		{
			vars.set( letter, letters[ l ] );
			vars.set( number, numbers[ n ] );
			REQUIRE_NO_EXCEPT( expr.evaluate( vars ) ) << "Failed to evaluate at round " << eon::string( i ).stdstr();
			result.push_back( vars.get( tag )->targetString( vars ) + ","
				+ vars.get( ln )->convertTargetString( vars ) + ","
				+ vars.get( sz )->convertTargetString( vars ) + ","
				+ vars.get( match )->targetString( vars ) );
			if( ++l == letters.size() )
				l = 0;
			if( ++n == numbers.size() )
				n = 0;
		}
		auto end = clock.now();
		auto time = end - start;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( time );
		std::cout << "\nRunning expression " << rounds << "/" << result.size()
			<< " iterations took: " << eon::string( ms.count() ) << "ms\n";
	}
}
