#include "Regression.h"
#include <eonvariables/IntValue.h>
#include <eonvariables/FloatValue.h>
#include <eonvariables/StringValue.h>
#include <eonvariables/RawValue.h>
#include <eonvariables/BinaryValue.h>
#include <eonvariables/TupleValue.h>



namespace eon
{
	TEST( ExpressionTest, binary1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "2 pow ( 3 + 1 )", varcache ) );
		WANT_EQ( "2 3 1 + pow",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"2 pow ( 3 + 1 )",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, binary2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression(
			"1 - 2 pow 3 pow 3 - ( 4 + 5 * 6 ) * 7", varcache ) );
		WANT_EQ( "1 2 3 3 pow pow - 4 5 6 * + 7 * -",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"1 - 2 pow 3 pow 3 - ( 4 + 5 * 6 ) * 7",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, binary3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "(4+3)*(2-1)", varcache ) );
		WANT_EQ( "4 3 + 2 1 - *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 4 + 3 ) * ( 2 - 1 )",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, unary1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "not 1", varcache ) );
		WANT_EQ( "1 not",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "not 1",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, unary2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1 and not 1 or not 0", varcache ) );
		WANT_EQ( "1 1 not and 0 not or",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "1 and not 1 or not 0",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, ternary1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2", varcache ) );
		WANT_EQ( "0 1 2 if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2 == 3", varcache ) );
		WANT_EQ( "0 1 2 3 == if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2 == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "(0 if 1 else 2) == 3", varcache ) );
		WANT_EQ( "0 1 2 if_else 3 ==",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 0 if 1 else 2 ) == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, and1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true and true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true and false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false and true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false and false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, or1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true or false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1 > 0 or 0 > 0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "tobool \"0\" or tobool \"\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, not1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "not true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "not false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "not tobool 4.3", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, if_else1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if true else 6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isChar() ) << "Wrong value type";
		WANT_TRUE( val->actualChar() == 'T' ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if false else 6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 6 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if true else (6+4)", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 2 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if false else (6+4)", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->actualInt() == 10 ) << "Wrong value";
	}

	TEST( ExpressionTest, eq1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true == true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true == false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true == tobool 8", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat 0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq6 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == tofloat \"0.0\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq7 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"\" == tostring 1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ne1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false != true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != toint 4.001", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "4.001 != tofloat 4", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != toint 4.0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, lt1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false < true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true < false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < 2", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < toint 1.1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha\" < \"alphA\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt6 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphA\" < \"alpha\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, le1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true <= false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= tofloat 1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le6 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= tofloat \"1.10000\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, gt1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true > false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true > true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false > true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > 4", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > toint 5.9999", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt6 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' > 't'", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ge1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= false", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "false >= true", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "99.9 >= tofloat 100", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphb\" >= \"alpha\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge6 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= tostring 'A'", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge7 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= tostring 'a'", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, plus1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + tostring 'A'", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "AA", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + tostring 6.6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "A6.6", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 + tofloat \"1.1\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.6 + 1.1, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "tofloat 1 + 2.9", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.9, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "2.9 + tofloat 1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.9, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, minus1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "10-6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 4, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6-10", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( -4, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha beta\" - tostring 'a'", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "lph bet", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "99.99 - tofloat \"000.9900\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 99.99 - 0.99, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, multiply1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "3 * 6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 18, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "tofloat 3 * 6.0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 18, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "3.0 * tofloat 6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 18.0, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "2.2 * tofloat \"3.3\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 2.2 * 3.3, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, divide1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 3", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 / 3.3", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 2, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 0", varcache ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), eon::expr::DivisionByZero );
	}

	TEST( ExpressionTest, pow1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 pow 3", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 216, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "3.3 pow tofloat 3", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 35.937, val->actualFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "10.0 pow -2.0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 0.01, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, mod1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 5", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 6", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 0, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "2 mod 5", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod4 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod toint 5.5", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod5 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "{9, 5} mod 5", varcache ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), vars::UnsupportedOperand );
	}

	TEST( ExpressionTest, sqrt1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 9", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 3, val->actualInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, sqrt2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 39.9424", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.32, val->actualFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, match1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+ha/", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		WANT_TRUE( val->actualBool() ) << "Wrong value";
		auto match = varcache.get( name::get( "complete" ) );
		REQUIRE_TRUE( match && match->isString() )
			<< "Didn't get capture group";
		WANT_EQ( "alpha", match->string_value().stdstr() ) << "Wrong capture";
	}
	TEST( ExpressionTest, match2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+haa/", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->actualBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, match3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+@<grp1>(ha)/", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->actualBool() ) << "Wrong value";
		auto grp = varcache.get( name::get( "grp1" ) );
		REQUIRE_TRUE( grp && grp->isString() ) << "Didn't get capture group";
		WANT_EQ( "ha", grp->string_value().stdstr() ) << "Wrong capture";
	}

	TEST( ExpressionTest, assign1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 99.9", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isFloat() ) << "Didn't get variable";
		WANT_EQ( 99.9, var->actualFloat() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b + 321", varcache ) );
		vars::valueptr val;
		varcache.set( name::get( "b" ), vars::intval::create( 123 ) );
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 444, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign3 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b", varcache ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate( varcache ), vars::NotFound );
	}

	TEST( ReferenceTest, reference_get_nonexisting )
	{
		expression expr;
		vars::variables varcache( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "a = @one/two", varcache ) );
		vars::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate( varcache ), vars::NotFound );
	}
	TEST( ReferenceTest, reference_get_existing )
	{
		_set( nameref( "one/two" ), vars::valueptr(), vars::stringval::create( "Hello" ) );
		expression expr;
		vars::variables varcache( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "a = @one/two", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) );
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "Hello", var->actualString().stdstr() ) << "Wrong variable value";
	}
	TEST( ReferenceTest, reference_set_new )
	{
		expression expr;
		vars::variables varcache( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "@one/two = 123", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) );
		auto result = _get( nameref( "one/two" ), vars::valueptr() );
		REQUIRE_TRUE( result && result->isInt() ) << "Didn't set variable";
		WANT_EQ( 123, result->actualInt() ) << "Wrong variable value";
	}
	TEST( ReferenceTest, reference_set_existing )
	{
		_set( nameref( "one/two" ), vars::valueptr(), vars::stringval::create( "Hello" ) );
		expression expr;
		vars::variables varcache( *this );
		REQUIRE_NO_EXCEPT( expr = expression( "@one/two = 123", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) );
		auto result = _get( nameref( "one/two" ), vars::valueptr() );
		REQUIRE_TRUE( result && result->isInt() ) << "Didn't set variable";
		WANT_EQ( 123, result->actualInt() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, exists_yes )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 1 if exists \""
			+ string( _TestExe ).replace( '\\', '/' ) + "\" else 0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 1, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, exists_no )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 1 if exists \""
			"somethingimpossiblewhateverblahblah\" else 0", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 0, var->actualInt() ) << "Wrong variable value";
	}
	
	TEST( FileTest, save_load_delete )
	{
		vars::variables varcache;
		vars::valueptr result;
		auto name_str = name::get( "str" );
		file myfile{ testdir.dpath() + "myfile.txt" };

		REQUIRE_NO_EXCEPT( testdir.ensureExists() );
		
		expression save{ "\"Hello World!\" saveto \"" + myfile.fpath().str() + "\"", varcache };
		REQUIRE_NO_EXCEPT( result = save.evaluate( varcache ) ) << "Failed to save!";
		REQUIRE_TRUE( result->actualTuple().at( name_success )->actualBool() ) << "Failed to save!";
		WANT_TRUE( myfile.exists() ) << "File not created!";

		expression load{ "str = loadstr \"" + myfile.fpath().str() + "\"", varcache };
		REQUIRE_NO_EXCEPT( load.evaluate( varcache ) ) << "Failed to load!";
		WANT_EQ( "Hello World!", varcache.get( name_str )->actualString().stdstr() ) << "Wrong value loaded!";

		expression del{ "delete \"" + myfile.fpath().str() + "\"", varcache };
		WANT_NO_EXCEPT( result = del.evaluate( varcache ) ) << "Failed to delete!";
		WANT_FALSE( myfile.exists() ) << "File not deleted!";
	}

	TEST( ExpressionTest, str_elm )
	{
		vars::variables varcache;
		expression expr{ "x = \"Hello World!\"[4]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( 'o', static_cast<char>( varcache.get( name::get( "x" ) )->actualChar() ) );
	}
	TEST( ExpressionTest, str_negpos_elm )
	{
		vars::variables varcache;
		expression expr{ "x = \"Hello World!\"[-4]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( 'r', static_cast<char>( varcache.get( name::get( "x" ) )->actualChar() ) );
	}
	TEST( ExpressionTest, raw_elm )
	{
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::valueptr( new vars::rawval( { "one", "two", "three" } ) ) );
		expression expr{ "x = r[1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "two", varcache.get( name::get( "x" ) )->actualString().stdstr() );
	}
	TEST( ExpressionTest, raw_negpos_elm )
	{
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::valueptr( new vars::rawval( { "one", "two", "three" } ) ) );
		expression expr{ "x = r[-1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "three", varcache.get( name::get( "x" ) )->actualString().stdstr() );
	}
	TEST( ExpressionTest, bin_elm )
	{
		vars::variables varcache;
		varcache.set( name::get( "b" ), vars::valueptr( new vars::binaryval( hex( "abc" ) ) ) );
		expression expr{ "x = b[1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( 98, varcache.get( name::get( "x" ) )->actualInt() );
	}
	TEST( ExpressionTest, bin_negpos_elm )
	{
		vars::variables varcache;
		varcache.set( name::get( "b" ), vars::valueptr( new vars::binaryval( hex( "abc" ) ) ) );
		expression expr{ "x = b[-3]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( 97, varcache.get( name::get( "x" ) )->actualInt() );
	}
	TEST( ExpressionTest, tuple_pos_elm )
	{
		vars::variables varcache;
		expression expr{ "x = {1, a=test, three=3.0}[1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( name_test, varcache.get( name::get( "x" ) )->actualName() );
	}
	TEST( ExpressionTest, tuple_negpos_elm )
	{
		vars::variables varcache;
		expression expr{ "x = {1, a=test, three=3.0}[-1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( 3.0, varcache.get( name::get( "x" ) )->actualFloat() );
	}
	TEST( ExpressionTest, tuple_name_elm )
	{
		vars::variables varcache;
		expression expr{ "x = {1, a=test, three=3.0}[#a]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( name_test, varcache.get( name::get( "x" ) )->actualName() );
	}

	TEST( ExpressionTest, str_slice )
	{
		vars::variables varcache;
		expression expr{ "x = \"Hello World!\"[3:6]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "lo W", varcache.get( name::get( "x" ) )->actualString().stdstr() );
	}
	TEST( ExpressionTest, str_negpos_slice )
	{
		vars::variables varcache;
		expression expr{ "x = \"Hello World!\"[-8:-5]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "o Wo", varcache.get( name::get( "x" ) )->actualString().stdstr() );
	}
	TEST( ExpressionTest, raw_slice )
	{
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::valueptr( new vars::rawval( { "one", "two", "three" } ) ) );
		expression expr{ "x = r[1:2]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "two,three", string( "," ).join( varcache.get( name::get( "x" ) )->actualRaw() ).stdstr() );
	}
	TEST( ExpressionTest, raw_negpos_slice )
	{
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::valueptr( new vars::rawval( { "one", "two", "three" } ) ) );
		expression expr{ "x = r[-2:-1]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "two,three", string( "," ).join( varcache.get( name::get( "x" ) )->actualRaw() ).stdstr() );
	}
	TEST( ExpressionTest, raw_negpos_reverse_slice )
	{
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::valueptr( new vars::rawval( { "one", "two", "three" } ) ) );
		expression expr{ "x = r[-1:-2]", varcache };
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) );
		WANT_EQ( "three,two", string( "," ).join( varcache.get( name::get( "x" ) )->actualRaw() ).stdstr() );
	}

	TEST( ExpressionTest, trim )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "b = trim (a = trim \"  str  \")", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "str", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "str", varb->actualString().stdstr() ) << "Wrong variable value b";
	}
	TEST( ExpressionTest, rtrim )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "b = rtrim (a = rtrim \"  str  \")", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "  str", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "  str", varb->actualString().stdstr() ) << "Wrong variable value b";
	}
	TEST( ExpressionTest, ltrim )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "b = ltrim (a = ltrim \"  str  \")", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto vara = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( vara && vara->isString() ) << "Didn't get variable a";
		WANT_EQ( "str  ", vara->actualString().stdstr() ) << "Wrong variable value a";
		auto varb = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( varb && varb->isString() ) << "Didn't get variable b";
		WANT_EQ( "str  ", varb->actualString().stdstr() ) << "Wrong variable value b";
	}

	TEST( ExpressionTest, len_str )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len \"0123456789\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 10, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_raw )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len |\n  one\n  two\n  three", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_bin)
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len %0A417D", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, len_tuple )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = len {one two three}", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 3, var->actualInt() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, resize_str_grow )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"one\" resize 5", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "one  ", var->actualString().stdstr() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, resize_str_shrink )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"one\" resize 1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "o", var->actualString().stdstr() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, resize_raw_grow )
	{
		expression expr;
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::rawval::create( { "one", "two" } ) );
		REQUIRE_NO_EXCEPT( expr = expression( "a = r resize 5", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isRaw() ) << "Didn't get variable";
		WANT_EQ( "one,two,,,", string( "," ).join( var->actualRaw() ).stdstr() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, resize_raw_shrink )
	{
		expression expr;
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::rawval::create( { "one", "two" } ) );
		REQUIRE_NO_EXCEPT( expr = expression( "a = r resize 1", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isRaw() ) << "Didn't get variable";
		WANT_EQ( "one", string( "," ).join( var->actualRaw() ).stdstr() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, type1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "typeof \"str\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isName() );
		WANT_TRUE( val->actualName() == name_string );
	}
	TEST( ExpressionTest, type2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "typeof 3.14", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isName() );
		WANT_TRUE( val->actualName() == name_float );
	}

	TEST( ExpressionTest, isname1 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "isname \"012alpha_21\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isBool() );
		WANT_TRUE( val->actualBool() );
	}
	TEST( ExpressionTest, isname2 )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "isname \"012al pha_21\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isBool() );
		WANT_FALSE( val->actualBool() );
	}

	TEST( ExpressionTest, tovar )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "tovar(\"a\" + tostring 2) = 99.9", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "a2" ) );
		REQUIRE_TRUE( var && var->isFloat() ) << "Didn't get variable";
		WANT_EQ( 99.9, var->actualFloat() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, char_in_str )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 'b' in \"abc\"; b = 'b' in \"cde\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( varcache.exists( name::get( "a" ) ) );
		REQUIRE_TRUE( varcache.exists( name::get( "b" ) ) );
		auto a = varcache.get( name::get( "a" ) );
		auto b = varcache.get( name::get( "b" ) );
		REQUIRE_TRUE( a->isBool() );
		REQUIRE_TRUE( b->isBool() );
		WANT_TRUE( a->actualBool() );
		WANT_FALSE( b->actualBool() );
	}
	TEST( ExpressionTest, str_in_str )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"bc\" in \"abcde\"; b = \"bc\" in \"cdeb\"", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( varcache.exists( name::get( "a" ) ) );
		REQUIRE_TRUE( varcache.exists( name::get( "b" ) ) );
		auto a = varcache.get( name::get( "a" ) );
		auto b = varcache.get( name::get( "b" ) );
		REQUIRE_TRUE( a->isBool() );
		REQUIRE_TRUE( b->isBool() );
		WANT_TRUE( a->actualBool() );
		WANT_FALSE( b->actualBool() );
	}
	TEST( ExpressionTest, str_in_raw )
	{
		expression expr;
		vars::variables varcache;
		varcache.set( name::get( "r" ), vars::rawval::create( { "one", "two", "three" } ) );
		REQUIRE_NO_EXCEPT( expr = expression( "a = \"two\" in r; b = \"Two\" in r", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( varcache.exists( name::get( "a" ) ) );
		REQUIRE_TRUE( varcache.exists( name::get( "b" ) ) );
		auto a = varcache.get( name::get( "a" ) );
		auto b = varcache.get( name::get( "b" ) );
		REQUIRE_TRUE( a->isBool() );
		REQUIRE_TRUE( b->isBool() );
		WANT_TRUE( a->actualBool() );
		WANT_FALSE( b->actualBool() );
	}
	TEST( ExpressionTest, name_in_tuple )
	{
		expression expr;
		vars::variables varcache;
		tuple t;
		t.append( vars::intval::create( 33 ), varcache );
		t.append( vars::stringval::create( "Hello" ), varcache, name::get( "str" ) );
		t.append( vars::floatval::create( 3.14 ), varcache, name::get( "pi" ) );
		varcache.set( name::get( "t" ), vars::tupleval::create( t ) );

		REQUIRE_NO_EXCEPT( expr = expression( "a = #str in t; b = #PI in t", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) ) << "Failed to evaluate";
		REQUIRE_TRUE( varcache.exists( name::get( "a" ) ) );
		REQUIRE_TRUE( varcache.exists( name::get( "b" ) ) );
		auto a = varcache.get( name::get( "a" ) );
		auto b = varcache.get( name::get( "b" ) );
		REQUIRE_TRUE( a->isBool() );
		REQUIRE_TRUE( b->isBool() );
		WANT_TRUE( a->actualBool() );
		WANT_FALSE( b->actualBool() );
	}

	TEST( ExpressionTest, double_parenthesis )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "((5-1)/(4-2))*3", varcache ) );
		WANT_EQ( "5 1 - 4 2 - / 3 *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"( 5 - 1 ) / ( 4 - 2 ) * 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, complex_len )
	{
		expression expr;
		vars::variables varcache;
		varcache.set( name_token, vars::stringval::create( "    " ) );
		REQUIRE_NO_EXCEPT( expr = expression(
			"(indentation = len token if not tobool (ltrim token) else 0)", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "indentation" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 4, var->actualInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, complex_if_else )
	{
		expression expr;
		vars::variables varcache;
		varcache.set( name_name, vars::stringval::create( "one" ) );
		REQUIRE_NO_EXCEPT( expr = expression(
			"id=\"namespace__\" + ((parent_id + \"__\") if tobool parent_id else \"\") + name", varcache ) );
		vars::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( varcache ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVariable() ) << "Wrong value type";
		auto var = varcache.get( name::get( "id" ) );
		REQUIRE_TRUE( var && var->isString() ) << "Didn't get variable";
		WANT_EQ( "namespace__one", var->actualString().stdstr() ) << "Wrong variable value";
	}

	TEST( ExpressionTest, multiple )
	{
		expression expr;
		vars::variables varcache;
		REQUIRE_NO_EXCEPT( expr = expression( "x = 1 + 2; x > 2", varcache ) );
		WANT_EQ( "x 1 2 + =; x 2 >", expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "x = 1 + 2; x > 2", expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, stress )
	{
		expression expr;
		vars::variables varcache;
		eon::string str{ "tag = \"Even stephen\" if toint number mod 2 == 0 else \"N/A\"; "
			"ln = letter + \":\" + tostring number; "
			"sz = len letter; "
			"_match = m if letter match ?/^.*?@<m>(p.*i).*$/ else \"N/A\"" };
		try
		{
			expr = expression( std::move( str ), varcache );
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
			varcache.set( letter, letters[ l ] );
			varcache.set( number, numbers[ n ] );
			REQUIRE_NO_EXCEPT( expr.evaluate( varcache ) ) << "Failed to evaluate at round " << eon::string( i ).stdstr();
			result.push_back( varcache.get( tag )->targetString( varcache ) + ","
				+ varcache.get( ln )->convertTargetString( varcache ) + ","
				+ varcache.get( sz )->convertTargetString( varcache ) + ","
				+ varcache.get( match )->targetString( varcache ) );
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
