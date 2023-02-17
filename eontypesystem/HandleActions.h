#pragma once

#include "Action.h"
#include "Operators.h"
#include "Tuple.h"
#include "TypeTupleFactory.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'type' namespace enclosing all background type functionality
	//
	namespace type
	{
		struct HandleConstruct : public Action {
			HandleConstruct() : Action(
				typetuple::newAction( name_handle, name_constructor, name_operator ).returns( name_handle ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( handle_t( nullptr ), Qualifier::_none ) );
				return sig_t::_normal; } };

		struct HandleCopyConstruct : public Action {
			HandleCopyConstruct() : Action(
				typetuple::newAction(
					name_handle, name_constructor, name_operator ).returns(
						name_handle ).arguments( typetuple::convert( { name_handle } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<handle_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct HandleCompare : public Action {
			HandleCompare() : Action(
				typetuple::newAction( name_handle, symbol_cmp, name_operator ).returns(
					name_handle ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerHandle();
	}
}
