#include "Regression.h"
#include <eonsource/String.h>



namespace eon
{
	TEST( GlobalTest, cache )
	{
		type::Handler::init();
		scope::Global global;

		source::String testsrc( "test", "dummy text" );
		source::Ref src( testsrc );

		cache::Locked lock( src );
		lock.addRaw( name_test, "Locked!" );
		lock.finalize();
		global.add( name_locked, &lock );

		cache::Static stat( src );
		stat.addRaw( name_test, "Static!" );
		stat.finalize();
		global.add( name_static, &stat );

		cache::Protected prot( src );
		prot.addRaw( name_test, "Protected!" );
		prot.finalize();
		global.add( name_protected, &prot );

		cache::Restricted rest( src );
		rest.addRaw( name_test, "Restricted!" );
		rest.finalize();
		global.add( name_restricted, &rest );

		cache::Open open( src );
		open.addRaw( name_test, "Open!" );
		open.finalize();
		global.add( name_open, &open );

		REQUIRE_TRUE( global.find( name_locked ) );
		auto l = (const cache::Locked*)global.find( name_locked );
		REQUIRE_TRUE( l->exists( name_test ) );
		REQUIRE_EQ( name_string, l->at( name_test ).type().name() );
		WANT_EQ( "Locked!", ( (StringInstance*)l->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_static ) );
		auto s = (const cache::Static*)global.find( name_static );
		REQUIRE_TRUE( s->exists( name_test ) );
		REQUIRE_EQ( name_string, s->at( name_test ).type().name() );
		WANT_EQ( "Static!", ( (StringInstance*)s->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_protected ) );
		auto p = (const cache::Protected*)global.find( name_protected );
		REQUIRE_TRUE( p->exists( name_test ) );
		REQUIRE_EQ( name_string, p->at( name_test ).type().name() );
		WANT_EQ( "Protected!", ( (StringInstance*)p->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_restricted ) );
		auto r = (const cache::Restricted*)global.find( name_restricted );
		REQUIRE_TRUE( r->exists( name_test ) );
		REQUIRE_EQ( name_string, r->at( name_test ).type().name() );
		WANT_EQ( "Restricted!", ( (StringInstance*)r->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_open ) );
		auto o = (const cache::Open*)global.find( name_open );
		REQUIRE_TRUE( o->exists( name_test ) );
		REQUIRE_EQ( name_string, o->at( name_test ).type().name() );
		WANT_EQ( "Open!", ( (StringInstance*)o->at( name_test ).value() )->value() );
	}
}
