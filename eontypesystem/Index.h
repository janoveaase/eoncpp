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
		struct IndexCopy : public Action {
			IndexCopy() : Action(
				TypeTuple::action(
					name_index, name_copy, name_operator, name_index, TypeTuple::args( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<index_t>() = args.values().top().value<index_t>();
				args.values().pop();
				return sig_t::norm; } };

		struct IndexTake : public Action {
			IndexTake() : Action(
				TypeTuple::action(
					name_index, name_take, name_operator, name_index, TypeTuple::args( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<index_t>() = args.values().top().value<index_t>();
				args.values().pop();
				return sig_t::norm; } };


		struct IndexConstruct : public Action {
			IndexConstruct() : Action(
				TypeTuple::action( name_index, name_constructor, name_constructor, name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( index_t( 0 ), Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexCopyConstruct : public Action {
			IndexCopyConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexBoolConstruct : public Action {
			IndexBoolConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<index_t>( a1 ? 1 : 0 ), Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexByteConstruct : public Action {
			IndexByteConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexCharConstruct : public Action {
			IndexCharConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexIntConstruct : public Action {
			IndexIntConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexShortConstruct : public Action {
			IndexShortConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexLongConstruct : public Action {
			IndexLongConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexFloatConstruct : public Action {
			IndexFloatConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexLowConstruct : public Action {
			IndexLowConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexHighConstruct : public Action {
			IndexHighConstruct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexB8Construct : public Action {
			IndexB8Construct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_b8 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b8_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexB16Construct : public Action {
			IndexB16Construct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_b16 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b16_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexB32Construct : public Action {
			IndexB32Construct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_b32 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b32_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct IndexB64Construct : public Action {
			IndexB64Construct() : Action(
				TypeTuple::action(
					name_index,
					name_constructor,
					name_constructor,
					name_index,
					{ { no_name, name_b64 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b64_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit<index_t>( (index_t)a1, name_index, Qualifier::none ) );
				return sig_t::norm; } };


		struct IndexCompare : public Action {
			IndexCompare() : Action(
				TypeTuple::action( name_index, symbol_cmp, name_operator, name_int, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexPlusAssign : public Action {
			IndexPlusAssign() : Action(
				TypeTuple::action(
					name_index,
					symbol_plus_assign,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMinusAssign : public Action {
			IndexMinusAssign() : Action(
				TypeTuple::action(
					name_index,
					symbol_minus_assign,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMultiplyAssign : public Action {
			IndexMultiplyAssign() : Action(
				TypeTuple::action(
					name_index,
					symbol_multiply_assign,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexDivideAssign : public Action {
			IndexDivideAssign() : Action(
				TypeTuple::action(
					name_index,
					symbol_divide_assign,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexPlus : public Action {
			IndexPlus() : Action(
				TypeTuple::action(
					name_index,
					symbol_plus,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMinus : public Action {
			IndexMinus() : Action(
				TypeTuple::action( name_index, symbol_minus, name_operator, name_index, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMultiply : public Action {
			IndexMultiply() : Action(
				TypeTuple::action(
					name_index,
					symbol_multiply,
					name_operator,
					name_index,
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexDivide : public Action {
			IndexDivide() : Action(
				TypeTuple::action( name_index, symbol_divide, name_operator, name_index, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexReset : public Action {
			IndexReset() : Action(
				TypeTuple::action( name_index, name_reset, name_operator, name_index, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<index_t>();
				a1 = 0;
				return sig_t::norm; } };


		void registerIndex();
	}
}
