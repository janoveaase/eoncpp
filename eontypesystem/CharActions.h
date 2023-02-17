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
		struct CharCopy : public Action {
			CharCopy() : Action(
				typetuple::newAction(
					name_char, name_copy, name_operator ).returns(
						name_char ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<char_t>() = args.values().top().value<char_t>();
				args.values().pop();
				return sig_t::_normal; } };

		struct CharTake : public Action {
			CharTake() : Action(
				typetuple::newAction(
					name_char, name_take, name_operator ).returns(
						name_char ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<char_t>() = args.values().top().value<char_t>();
				args.values().pop();
				return sig_t::_normal; } };


		struct CharConstruct : public Action {
			CharConstruct() : Action(
				typetuple::newAction( name_char, name_constructor, name_constructor ).returns( name_char ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( char_t( 0 ), Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharCopyConstruct : public Action {
			CharCopyConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharBoolConstruct : public Action {
			CharBoolConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<char_t>( a1 ? 1 : 0 ), Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharByteConstruct : public Action {
			CharByteConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharIntConstruct : public Action {
			CharIntConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharShortConstruct : public Action {
			CharShortConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharLongConstruct : public Action {
			CharLongConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharFloatConstruct : public Action {
			CharFloatConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharLowConstruct : public Action {
			CharLowConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharHighConstruct : public Action {
			CharHighConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };

		struct CharIndexConstruct : public Action {
			CharIndexConstruct() : Action(
				typetuple::newAction(
					name_char, name_constructor, name_constructor ).returns(
						name_char ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::_none ) );
				return sig_t::_normal; } };


		struct CharCompare : public Action {
			CharCompare() : Action(
				typetuple::newAction(
					name_char, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct CharReset : public Action {
			CharReset() : Action(
				typetuple::newAction( name_char, name_reset, name_operator ).returns( name_char ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<char_t>();
				a1 = 0;
				return sig_t::_normal; } };


		void registerChar();
	}
}
