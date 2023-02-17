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
		struct IndexCopy : public Action {
			IndexCopy() : Action(
				typetuple::newAction(
					name_index, name_copy, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<index_t>() = args.values().top().value<index_t>();
				args.values().pop();
				return sig_t::_normal; } };

		struct IndexTake : public Action {
			IndexTake() : Action(
				typetuple::newAction(
					name_index, name_take, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<index_t>() = args.values().top().value<index_t>();
				args.values().pop();
				return sig_t::_normal; } };


		struct IndexConstruct : public Action {
			IndexConstruct() : Action(
				typetuple::newAction( name_index, name_constructor, name_constructor ).returns( name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( index_t( 0 ), Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexCopyConstruct : public Action {
			IndexCopyConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexBoolConstruct : public Action {
			IndexBoolConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<index_t>( a1 ? 1 : 0 ), Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexByteConstruct : public Action {
			IndexByteConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexCharConstruct : public Action {
			IndexCharConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexIntConstruct : public Action {
			IndexIntConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexShortConstruct : public Action {
			IndexShortConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexLongConstruct : public Action {
			IndexLongConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexFloatConstruct : public Action {
			IndexFloatConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexLowConstruct : public Action {
			IndexLowConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexHighConstruct : public Action {
			IndexHighConstruct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexB8Construct : public Action {
			IndexB8Construct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_b8 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b8_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexB16Construct : public Action {
			IndexB16Construct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_b16 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b16_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (index_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexB32Construct : public Action {
			IndexB32Construct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_b32 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b32_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit<b32_t>( (b32_t)a1, name_b32, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct IndexB64Construct : public Action {
			IndexB64Construct() : Action(
				typetuple::newAction(
					name_index, name_constructor, name_constructor ).returns(
						name_index ).arguments( typetuple::convert( { name_b64 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b64_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit<b64_t>( (b64_t)a1, name_b64, Qualifier::_none ) );
				return sig_t::_normal; } };


		struct IndexCompare : public Action {
			IndexCompare() : Action(
				typetuple::newAction(
					name_index, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexPlusAssign : public Action {
			IndexPlusAssign() : Action(
				typetuple::newAction(
					name_index, symbol_plus_assign, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMinusAssign : public Action {
			IndexMinusAssign() : Action(
				typetuple::newAction(
					name_index, symbol_minus_assign, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMultiplyAssign : public Action {
			IndexMultiplyAssign() : Action(
				typetuple::newAction(
					name_index, symbol_multiply_assign, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexDivideAssign : public Action {
			IndexDivideAssign() : Action(
				typetuple::newAction(
					name_index, symbol_divide_assign, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexPlus : public Action {
			IndexPlus() : Action(
				typetuple::newAction(
					name_index, symbol_plus, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMinus : public Action {
			IndexMinus() : Action(
				typetuple::newAction(
					name_index, symbol_minus, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexMultiply : public Action {
			IndexMultiply() : Action(
				typetuple::newAction(
					name_index, symbol_multiply, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct IndexDivide : public Action {
			IndexDivide() : Action(
				typetuple::newAction(
					name_index, symbol_divide, name_operator ).returns(
						name_index ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct IndexReset : public Action {
			IndexReset() : Action(
				typetuple::newAction(
					name_index, name_reset, name_operator ).returns( name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<index_t>();
				a1 = 0;
				return sig_t::_normal; } };


		void registerIndex();
	}
}
