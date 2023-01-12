#pragma once

#include "Action.h"
#include "Operators.h"
#include "Tuple.h"
#include "TypeTuple.h"



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
				TypeTuple::action( name_handle, name_constructor, name_operator, name_handle ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( handle_t( nullptr ), Qualifier::none ) );
				return sig_t::norm; } };

		struct HandleCopyConstruct : public Action {
			HandleCopyConstruct() : Action(
				TypeTuple::action(
					name_handle,
					name_constructor,
					name_operator,
					name_handle,
					{ { no_name, name_handle } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<handle_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct HandleCompare : public Action {
			HandleCompare() : Action(
				TypeTuple::action( name_handle, symbol_cmp, name_operator, name_handle, { { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerHandle();
	}
}
