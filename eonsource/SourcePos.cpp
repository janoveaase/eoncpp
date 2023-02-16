#include "SourcePos.h"


namespace eon
{
	namespace source
	{
		EON_TEST_2STEP( Pos, Pos, basic,
			Pos obj( 14, 15, 4, 5 ),
			EON_EQ( "14,15,4,5,valid", obj.rawStr() ) );


		EON_TEST_2STEP( Pos, opeator_eq, copy,
			Pos obj = Pos( 14, 15, 4, 5 ),
			EON_EQ( "14,15,4,5,valid", obj.rawStr() ) );


		EON_TEST( Pos, isValid, invalid,
			EON_FALSE( Pos().isValid() ) );
		EON_TEST( Pos, isValid, valid,
			EON_TRUE( Pos( 13, 15, 4, 5 ).isValid() ) );


		EON_TEST( Pos, str, empty,
			EON_EQ( "", Pos().str() ) );
		EON_TEST( Pos, str, singleton,
			EON_EQ( "1:1", Pos( 0, 0, 0, 0 ).str() ) );
		EON_TEST( Pos, str, multiple,
			EON_EQ( "5:6", Pos( 14, 15, 4, 5 ).str() ) );


		EON_TEST( Pos, operator_lt, lt,
			EON_TRUE( Pos( 0, 0, 0, 0 ) < Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_lt, gt,
			EON_FALSE( Pos( 1, 1, 0, 1 ) < Pos( 0, 0, 0, 0 ) ) );

		EON_TEST( Pos, operator_le, lt,
			EON_TRUE( Pos( 0, 0, 0, 0 ) <= Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_le, le,
			EON_TRUE( Pos( 1, 1, 0, 1 ) <= Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_le, gt,
			EON_FALSE( Pos( 1, 1, 0, 1 ) <= Pos( 0, 0, 0, 0 ) ) );

		EON_TEST( Pos, operator_gt, gt,
			EON_TRUE( Pos( 1, 1, 0, 1 ) > Pos( 0, 0, 0, 0 ) ) );
		EON_TEST( Pos, operator_gt, lt,
			EON_FALSE( Pos( 0, 0, 0, 0 ) > Pos( 1, 1, 0, 1 ) ) );

		EON_TEST( Pos, operator_ge, gt,
			EON_TRUE( Pos( 1, 1, 0, 1 ) >= Pos( 0, 0, 0, 0 ) ) );
		EON_TEST( Pos, operator_ge, ge,
			EON_TRUE( Pos( 1, 1, 0, 1 ) >= Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_ge, lt,
			EON_FALSE( Pos( 0, 0, 0, 0 ) >= Pos( 1, 1, 0, 1 ) ) );

		EON_TEST( Pos, operator_eq, eq,
			EON_TRUE( Pos( 1, 1, 0, 1 ) == Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_eq, lt,
			EON_FALSE( Pos( 0, 0, 0, 0 ) == Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_eq, gt,
			EON_FALSE( Pos( 1, 1, 0, 1 ) == Pos( 0, 0, 0, 0 ) ) );

		EON_TEST( Pos, operator_ne, eq,
			EON_FALSE( Pos( 1, 1, 0, 1 ) != Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_ne, lt,
			EON_TRUE( Pos( 0, 0, 0, 0 ) != Pos( 1, 1, 0, 1 ) ) );
		EON_TEST( Pos, operator_ne, gt,
			EON_TRUE( Pos( 1, 1, 0, 1 ) != Pos( 0, 0, 0, 0 ) ) );
	}
}
