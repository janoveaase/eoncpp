#include "Locale.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	thread_local locale locale::CurLocale;

#ifdef EON_TEST_LOCALE_NO
#	define NO_NB "nb_NO" _UTF8
#endif


	EON_TEST( locale, get, basic,
		EON_EQ( "en_US" _UTF8, locale::get().name() ) );

#ifdef EON_TEST_LOCALE_NO
	EON_TEST_2STEP( locale, set, basic,
		locale::set( locale( NO_NB ) ),
		EON_EQ( NO_NB, locale::get().name() ) );
#endif

	EON_TEST( locale, decimalSep, standard,
		EON_EQ( char_t( '.' ), locale::get().decimalSep() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, decimalSep, no_NB,
		EON_EQ( char_t( ',' ), locale( NO_NB ).decimalSep() ) );
#endif

	EON_TEST( locale, thousandsSep, standard,
		EON_EQ( char_t( ',' ), locale::get().thousandsSep() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, thousandsSep, no_NB,
		EON_EQ( NBSpaceChr, locale( NO_NB ).thousandsSep() ) );
#endif

	EON_TEST( locale, grouping, standard,
		EON_EQ( '\3', locale::get().grouping().at( 0 ) ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, grouping, no_NB,
		EON_EQ( '\3', locale( NO_NB ).grouping().at( 0 ) ) );
#endif




	EON_TEST( locale, toUpper, standard_1,
		EON_EQ( L'A', locale::get().toUpper( L'a' ) ) );
	EON_TEST( locale, toUpper, standard_2,
		EON_EQ( L'Á', locale::get().toUpper( L'á' ) ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, toUpper, no_NB_1,
		EON_EQ( L'A', locale::get().toUpper( L'a' ) ) );
	EON_TEST( locale, toUpper, no_NB_2,
		EON_EQ( L'Å', locale::get().toUpper( L'å' ) ) );
#endif

	EON_TEST( locale, toLower, standard_1,
		EON_EQ( L'a', locale::get().toLower( L'A' ) ) );
	EON_TEST( locale, toLower, standard_2,
		EON_EQ( L'á', locale::get().toLower( L'Á' ) ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, toLower, no_NB_1,
		EON_EQ( L'a', locale::get().toLower( L'A' ) ) );
	EON_TEST( locale, toLower, no_NB_2,
		EON_EQ( L'å', locale::get().toLower( L'Å' ) ) );
#endif




	EON_TEST( locale, decimalSepMoney, standard,
		EON_EQ( char_t( '.' ), locale::get().decimalSepMoney() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, decimalSepMoney, no_NB,
		EON_EQ( char_t( ',' ), locale( NO_NB ).decimalSepMoney() ) );
#endif

	EON_TEST( locale, thousandsSepMoney, standard,
		EON_EQ( char_t( ',' ), locale::get().thousandsSepMoney() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, thousandsSepMoney, no_NB,
		EON_EQ( NBSpaceChr, locale( NO_NB ).thousandsSepMoney() ) );
#endif

	EON_TEST( locale, groupingMoney, standard,
		EON_EQ( '\3', locale::get().groupingMoney().at( 0 ) ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, groupingMoney, no_NB,
		EON_EQ( '\3', locale( NO_NB ).groupingMoney().at( 0 ) ) );
#endif

	EON_TEST( local, currencySymbol, standard,
		EON_EQ( L"$", locale::get().currencySymbol() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, currencySymbol, no_NB,
		EON_EQ( string( "kr" ), locale( NO_NB ).currencySymbol() ) );
#endif

	EON_TEST( locale, positiveSign, standard,
		EON_EQ( L"", locale::get().positiveSign() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, positiveSign, no_NB,
		EON_EQ( L"", locale( NO_NB ).positiveSign() ) );
#endif

	EON_TEST( locale, negativeSign, standard,
		EON_EQ( L"-", locale::get().negativeSign() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, negativeSign, no_NB,
		EON_EQ( L"-", locale( NO_NB ).negativeSign() ) );
#endif

	EON_TEST( locale, fractionalDigits, standard,
		EON_EQ( 2, locale::get().fractionalDigits() ) );
#ifdef EON_TEST_LOCALE_NO
	EON_TEST( locale, fractionalDigits, no_NB,
		EON_EQ( 2, locale( NO_NB ).fractionalDigits() ) );
#endif




	void locale::_set( std::string name )
	{
		Loc = std::locale( name.c_str() );		// Issues with G++ using std::string directly!
		Coll = &std::use_facet<std::collate<wchar_t>>( Loc );
		Num = &std::use_facet<std::numpunct<wchar_t>>( Loc );
		CType = &std::use_facet<std::ctype<wchar_t>>( Loc );
		Money = &std::use_facet<std::moneypunct<wchar_t>>( Loc );
	}
}
