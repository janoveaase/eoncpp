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
		struct NameCopy : public Action {
			NameCopy() : Action(
				TypeTuple::action(
					name_name, name_copy, name_operator, name_name, TypeTuple::args( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<name_t>() = args.values().top().value<name_t>();
				args.values().pop();
				return sig_t::norm; } };

		struct NameTake : public Action {
			NameTake() : Action(
				TypeTuple::action(
					name_name, name_take, name_operator, name_name, TypeTuple::args( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<name_t>() = args.values().top().value<name_t>();
				args.values().pop();
				return sig_t::norm; } };


		struct NameConstruct : public Action {
			NameConstruct() : Action(
				TypeTuple::action( name_name, name_constructor, name_constructor, name_name ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( no_name, Qualifier::none ) );
				return sig_t::norm; } };

		struct NameCopyConstruct : public Action {
			NameCopyConstruct() : Action(
				TypeTuple::action(
					name_name, name_constructor, name_constructor, name_name, TypeTuple::args( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<name_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit<name_t>( a1, name_name, Qualifier::none ) );
				return sig_t::norm; } };


		struct NameCompare : public Action {
			NameCompare() : Action(
				TypeTuple::action(
					name_name, symbol_cmp, name_operator, name_name, TypeTuple::args( { no_name, name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct NameReset : public Action {
			NameReset() : Action(
				TypeTuple::action( name_name, name_reset, name_operator, name_name ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<name_t>();
				a1 = no_name;
				return sig_t::norm; } };


		void registerName();
	}
}
