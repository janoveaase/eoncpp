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
		struct BoolCopy : public Action {
			BoolCopy() : Action(
				TypeTuple::action(
					name_bool, name_copy, name_operator, name_bool, TypeTuple::args( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<bool>() = args.values().top().value<bool>();
				args.values().pop();
				return sig_t::norm; } };

		struct BoolTake : public Action {
			BoolTake() : Action(
				TypeTuple::action(
					name_bool, name_take, name_operator, name_bool, TypeTuple::args( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<bool>() = args.values().top().value<bool>();
				args.values().pop();
				return sig_t::norm; } };


		struct BoolConstruct : public Action {
			BoolConstruct() : Action(
				TypeTuple::action( name_bool, name_constructor, name_constructor, name_bool ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( false, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolCopyConstruct : public Action {
			BoolCopyConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const bool a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolByteConstruct : public Action {
			BoolByteConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolCharConstruct : public Action {
			BoolCharConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolIntConstruct : public Action {
			BoolIntConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolShortConstruct : public Action {
			BoolShortConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolLongConstruct : public Action {
			BoolLongConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolFloatConstruct : public Action {
			BoolFloatConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolLowConstruct : public Action {
			BoolLowConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolHighConstruct : public Action {
			BoolHighConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolIndexConstruct : public Action {
			BoolIndexConstruct() : Action(
				TypeTuple::action(
					name_bool, name_constructor, name_constructor, name_bool, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };


		struct BoolCompare : public Action {
			BoolCompare() : Action(
				TypeTuple::action(
					name_bool, symbol_cmp, name_operator, name_int, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BoolNot : public Action {
			BoolNot() : Action(
				TypeTuple::action(
					name_bool, name_not, name_operator, name_bool ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( !a1, name_bool, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BoolAnd : public Action {
			BoolAnd() : Action(
				TypeTuple::action(
					name_bool, name_and, name_operator, name_bool, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BoolOr : public Action {
			BoolOr() : Action(
				TypeTuple::action(
					name_bool, name_or, name_operator, name_bool, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BoolXor : public Action {
			BoolXor() : Action(
				TypeTuple::action(
					name_bool, name_xor, name_operator, name_bool, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BoolReset : public Action {
			BoolReset() : Action(
				TypeTuple::action(
					name_bool, name_reset, name_operator, name_bool, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<bool>();
				a1 = false;
				return sig_t::norm; } };


		void registerBool();
	}
}
