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
		struct NameCopy : public Action {
			NameCopy() : Action(
				typetuple::newAction(
					name_name, name_copy, name_operator ).returns(
						name_name ).arguments( typetuple::convert( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<name_t>() = args.values().top().value<name_t>();
				args.values().pop();
				return sig_t::_normal; } };

		struct NameTake : public Action {
			NameTake() : Action(
				typetuple::newAction(
					name_name, name_take, name_operator ).returns(
						name_name ).arguments( typetuple::convert( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<name_t>() = args.values().top().value<name_t>();
				args.values().pop();
				return sig_t::_normal; } };


		struct NameConstruct : public Action {
			NameConstruct() : Action(
				typetuple::newAction( name_name, name_constructor, name_constructor ).returns( name_name ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( no_name, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct NameCopyConstruct : public Action {
			NameCopyConstruct() : Action(
				typetuple::newAction(
					name_name, name_constructor, name_constructor ).returns(
						name_name ).arguments( typetuple::convert( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<name_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit<name_t>( a1, name_name, Qualifier::_none ) );
				return sig_t::_normal; } };


		struct NameCompare : public Action {
			NameCompare() : Action(
				typetuple::newAction(
					name_name, symbol_cmp, name_operator ).returns(
						name_name ).arguments( typetuple::convert( { no_name, name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct NameReset : public Action {
			NameReset() : Action(
				typetuple::newAction( name_name, name_reset, name_operator ).returns( name_name ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<name_t>();
				a1 = no_name;
				return sig_t::_normal; } };


		void registerName();
	}
}
