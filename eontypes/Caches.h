#pragma once

#include "BasicTuple.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::cache' namespace encloses all cache objects
	**************************************************************************/
	namespace cache
	{
		/**********************************************************************
		  A cache is just a (basic) tuple with the state set explicitly.
		**********************************************************************/
		class Cache : public type::BasicTuple
		{
		public:
			Cache( name_t type, source::Ref source ) : BasicTuple( type, source ) {}

			void formalPrefix( Stringifier& str ) const override {}
			void formalPostfix( Stringifier& str ) const override {}
		};

		class Locked : public Cache
		{
		public:
			Locked( source::Ref source ) : Cache( name_locked, source ) {}
			type::Object* copy() override { return new Locked( source() ); }
		};

		class Static : public Cache
		{
		public:
			Static( source::Ref source ) : Cache( name_static, source ) {}
			type::Object* copy() override { return new Static( source() ); }
		};

		class Protected : public Cache
		{
		public:
			Protected( source::Ref source ) : Cache( name_protected, source ) {}
			type::Object* copy() override { return new Protected( source() ); }
		};

		class Restricted : public Cache
		{
		public:
			Restricted( source::Ref source ) : Cache( name_restricted, source ) {}
			type::Object* copy() override { return new Restricted( source() ); }
		};

		class Open : public Cache
		{
		public:
			Open( source::Ref source ) : Cache( name_open, source ) {}
			type::Object* copy() override { return new Open( source() ); }
		};
	}
}
