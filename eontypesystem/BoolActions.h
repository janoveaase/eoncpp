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
		struct BoolCopy : public Action {
			BoolCopy() : Action(
				typetuple::newAction(
					name_bool, name_copy, name_operator ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<bool>() = args.values().top().value<bool>();
				args.values().pop();
				return sig_t::_normal; } };

		struct BoolTake : public Action {
			BoolTake() : Action(
				typetuple::newAction(
					name_bool, name_take, name_operator ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<bool>() = args.values().top().value<bool>();
				args.values().pop();
				return sig_t::_normal; } };


		struct BoolConstruct : public Action {
			BoolConstruct() : Action(
				typetuple::newAction( name_bool, name_constructor, name_constructor ).returns( name_bool ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( false, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolCopyConstruct : public Action {
			BoolCopyConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const bool a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolByteConstruct : public Action {
			BoolByteConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolCharConstruct : public Action {
			BoolCharConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolIntConstruct : public Action {
			BoolIntConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolShortConstruct : public Action {
			BoolShortConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolLongConstruct : public Action {
			BoolLongConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolFloatConstruct : public Action {
			BoolFloatConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolLowConstruct : public Action {
			BoolLowConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolHighConstruct : public Action {
			BoolHighConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolIndexConstruct : public Action {
			BoolIndexConstruct() : Action(
				typetuple::newAction(
					name_bool, name_constructor, name_constructor ).returns(
						name_bool ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1 != 0, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };


		struct BoolCompare : public Action {
			BoolCompare() : Action(
				typetuple::newAction(
					name_bool, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BoolNot : public Action {
			BoolNot() : Action(
				typetuple::newAction(
					name_bool, name_not, name_operator).returns( name_bool ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( !a1, name_bool, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BoolAnd : public Action {
			BoolAnd() : Action(
				typetuple::newAction(
					name_bool, name_and, name_operator ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BoolOr : public Action {
			BoolOr() : Action(
				typetuple::newAction(
					name_bool, name_or, name_operator ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BoolXor : public Action {
			BoolXor() : Action(
				typetuple::newAction(
					name_bool, name_xor, name_operator ).returns(
						name_bool ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BoolReset : public Action {
			BoolReset() : Action( typetuple::newAction( name_bool, name_reset, name_operator ).returns( name_bool ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<bool>();
				a1 = false;
				return sig_t::_normal; } };


		void registerBool();
	}
}
