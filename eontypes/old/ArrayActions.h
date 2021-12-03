#pragma once
#include "Action.h"
#include "Scope.h"
#include "Instance.h"
#include <vector>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		// Helpers
		inline size_t arrayPos( const array_t& value, int_t pos ) noexcept {
			return pos >= 0 ?static_cast<size_t>( pos ) : value.size() - static_cast<size_t>( -pos ); }




		/**********************************************************************
		  Eon Array Actions Classes - eon::tup::arrayact_<name>

		  Bindings for std::vector methods
		**********************************************************************/

		struct arrayact_size : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				return new instance( *scope, me->arrayValue()->size(), 1 ); }
		};
		struct arrayact_at_position : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				return new instance( *scope, me->arrayValue()->at( args->at( 0 )->positionValue() ), 1 ); }
		};
		struct arrayact_at_int : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				return new instance( *scope, me->arrayValue()->at(
					arrayPos( *me->arrayValue(), args->at( 0 )->intValue() ) ) ); }
		};
		struct arrayact_append : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				me->arrayValue()->push_back( args->at( 0 ) ); return me; }
		};
		struct arrayact_insert_pos : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				me->arrayValue()->insert( me->arrayValue()->begin() + args->at( 0 )->positionValue(), args->at( 1 ) );
				return me; }
		};
		struct arrayact_insert_int : public action
		{
			inline instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override {
				me->arrayValue()->insert( me->arrayValue()->begin() + arrayPos(
					*me->arrayValue(), args->at( 0 )->intValue() ), args->at( 1 ) ); return me; }
		};


		void registerArrayActions( varscope& scope );
	}
}
