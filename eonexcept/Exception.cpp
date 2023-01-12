#include "Exception.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	EON_TEST( exception, exception, bad_name,
		EON_RAISE( exception( 0 ), BadName ) );
	EON_TEST( exception, exception, good_name,
		EON_NO_X( eon::exception( name( "test" ) ) ) );




	EON_TEST_3STEP( exception, info, set,
		exception obj( name( "test" ), "" ),
		obj.info( "got info" ),
		EON_EQ( "got info", obj.info() ) );
	EON_TEST_3STEP( exception, info, add,
		exception obj( name( "test" ), "got info" ),
		obj.info( "more info" ),
		EON_EQ( "got infomore info", obj.info() ) );




	const char* exception::what() const noexcept {
		return Info.empty() ? str( Name ).c_str() : Info.c_str(); }
	EON_TEST_2STEP( exception, what, with_info,
		exception obj( name( "test" ), "some info" ),
		EON_EQ( "some info", obj.what() ) );
	EON_TEST_2STEP( exception, what, without_info,
		exception obj( name( "test" ) ),
		EON_EQ( "test", obj.what() ) );

	string exception::details() const {
		return !Info.empty() ? str( Name ) + ": " + Info : str( Name ); }
	EON_TEST_2STEP( exception, details, with_info,
		exception obj( name( "test" ), "some info" ),
		EON_EQ( "test: some info", obj.details() ) );
	EON_TEST_2STEP( exception, details, without_info,
		exception obj( name( "test" ) ),
		EON_EQ( "test", obj.details() ) );

	EON_TEST_2STEP( exception, info, empty,
		exception obj( name( "test" ) ),
		EON_EQ( "", obj.info() ) );
	EON_TEST_2STEP( exception, info, not_empty,
		exception obj( name( "test" ), "some info" ),
		EON_EQ( "some info", obj.info() ) );
}
