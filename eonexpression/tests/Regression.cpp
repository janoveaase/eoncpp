#include "Regression.h"
#include <eontuple/IntValue.h>



namespace eon
{
	TEST( ExpressionTest, binary1 )
	{
		expression expr;
		tup::variables vars;
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
		tup::variables vars;
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
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(4+3)*(2-1)", vars ) );
		WANT_EQ( "4 3 + 2 1 - *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 4 + 3 ) * ( 2 - 1 )",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, unary1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not 1", vars ) );
		WANT_EQ( "1 not",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "not 1",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, unary2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 and not 1 or not 0", vars ) );
		WANT_EQ( "1 1 not and 0 not or",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "1 and not 1 or not 0",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, ternary1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2", vars ) );
		WANT_EQ( "0 1 2 if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0 if 1 else 2 == 3", vars ) );
		WANT_EQ( "0 1 2 3 == if_else",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "0 if 1 else 2 == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
	TEST( ExpressionTest, ternary3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(0 if 1 else 2) == 3", vars ) );
		WANT_EQ( "0 1 2 if_else 3 ==",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ( "( 0 if 1 else 2 ) == 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}

	TEST( ExpressionTest, and1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true and true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true and false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false and true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, and4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false and false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, or1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true or false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 or 0", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, or3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"0\" or \"\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, not1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, not3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "not 4.3", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, if_else1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if true else 6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isChar() ) << "Wrong value type";
		WANT_TRUE( val->hardChar() == 'T' ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' if false else 6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->hardInt() == 6 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if true else (6+4)", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->hardInt() == 2 ) << "Wrong value";
	}
	TEST( ExpressionTest, if_else4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "(6-4) if false else (6+4)", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_TRUE( val->hardInt() == 10 ) << "Wrong value";
	}

	TEST( ExpressionTest, eq1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true == 8", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == 0", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq6 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "0.000 == \"0.0\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, eq7 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"\" == 1", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ne1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false != true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != 4.001", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4.001 != 4", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ne4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "4 != 4.0", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, lt1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false < true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true < false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < 2", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 < 1.1", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha\" < \"alphA\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, lt6 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphA\" < \"alpha\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, le1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false <= false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true <= false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= 1", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, le6 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1.1 <= \"1.10000\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, gt1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true > false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true > true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false > true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > 4", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "5 > 5.9999", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, gt6 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "'T' > 't'", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, ge1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= false", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "true >= true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "false >= true", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "99.9 >= 100", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alphb\" >= \"alpha\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge6 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= 'A'", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, ge7 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" >= 'a'", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}

	TEST( ExpressionTest, plus1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + 'A'", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "AA", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"A\" + 6.6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "A6.6", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 + \"1.1\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.6 + 1.1, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "1 + 2.9", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 3, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, plus5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2.9 + 1", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.9, val->hardFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, minus1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "10-6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 4, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6-10", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( -4, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "\"alpha beta\" - 'a'", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isString() ) << "Wrong value type";
		WANT_EQ( "lph bet", val->string_value().stdstr() ) << "Wrong value";
	}
	TEST( ExpressionTest, minus4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "99.99 - \"000.9900\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 99.99 - 0.99, val->hardFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, multiply1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3 * 6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 18, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3 * 6.0", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 18, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3.0 * 6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 18.0, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, multiply4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2.2 * \"3.3\"", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 2.2 * 3.3, val->hardFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, divide1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 3", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6.6 / 3.3", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.6 / 3.3, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, divide3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 / 0", vars ) );
		tup::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), eon::tup::DivisionByZero );
	}

	TEST( ExpressionTest, pow1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 pow 3", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 216.0, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "3.3 pow 3", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 35.937, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, pow3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "10 pow -2", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 0.01, val->hardFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, mod1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 5", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 6", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 0, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "2 mod 5", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 2, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod4 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6 mod 5.5", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isInt() ) << "Wrong value type";
		WANT_EQ( 1, val->hardInt() ) << "Wrong value";
	}
	TEST( ExpressionTest, mod5 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "6.9 mod 5", vars ) );
		tup::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), tup::UnsupportedOperand );
	}

	TEST( ExpressionTest, sqrt1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 9", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 3.0, val->hardFloat() ) << "Wrong value";
	}
	TEST( ExpressionTest, sqrt2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "sqrt 39.9424", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( val->isFloat() ) << "Wrong value type";
		WANT_EQ( 6.32, val->hardFloat() ) << "Wrong value";
	}

	TEST( ExpressionTest, match1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+ha/", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		WANT_TRUE( val->hardBool() ) << "Wrong value";
		auto match = vars.get( name::get( "complete" ) );
		REQUIRE_TRUE( match && match->isString() )
			<< "Didn't get capture group";
		WANT_EQ( "alpha", match->string_value().stdstr() ) << "Wrong capture";
	}
	TEST( ExpressionTest, match2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+haa/", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate() ) << "Failed to evaluate";
		WANT_FALSE( val->hardBool() ) << "Wrong value";
	}
	TEST( ExpressionTest, match3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT(
			expr = expression( "\"alpha beta\" match ?/.+@<grp1>(ha)/", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->hardBool() ) << "Wrong value";
		auto grp = vars.get( name::get( "grp1" ) );
		REQUIRE_TRUE( grp && grp->isString() ) << "Didn't get capture group";
		WANT_EQ( "ha", grp->string_value().stdstr() ) << "Wrong capture";
	}

	TEST( ExpressionTest, assign1 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = 99.9", vars ) );
		tup::valueptr val;
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVar() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isFloat() ) << "Didn't get variable";
		WANT_EQ( 99.9, var->hardFloat() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign2 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b + 321", vars ) );
		tup::valueptr val;
		vars.set( name::get( "b" ), tup::valueptr( new tup::intval( 123 ) ) );
		REQUIRE_NO_EXCEPT( val = expr.evaluate( vars ) )
			<< "Failed to evaluate";
		REQUIRE_TRUE( val->isVar() ) << "Wrong value type";
		auto var = vars.get( name::get( "a" ) );
		REQUIRE_TRUE( var && var->isInt() ) << "Didn't get variable";
		WANT_EQ( 444, var->hardInt() ) << "Wrong variable value";
	}
	TEST( ExpressionTest, assign3 )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "a = b", vars ) );
		tup::valueptr val;
		REQUIRE_EXCEPT( val = expr.evaluate(), tup::NotFound );
	}

	TEST( ExpressionTest, double_parenthesis )
	{
		expression expr;
		tup::variables vars;
		REQUIRE_NO_EXCEPT( expr = expression( "((5-1)/(4-2))*3", vars ) );
		WANT_EQ( "5 1 - 4 2 - / 3 *",
			expr.postfixStr().stdstr() ) << "Bad postfix expression";
		WANT_EQ(
			"( 5 - 1 ) / ( 4 - 2 ) * 3",
			expr.infixStr().stdstr() ) << "Bad infix expression";
	}
}
