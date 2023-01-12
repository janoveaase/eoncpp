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
		struct CharCopy : public Action {
			CharCopy() : Action(
				TypeTuple::action(
					name_char, name_copy, name_operator, name_char, TypeTuple::args( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<char_t>() = args.values().top().value<char_t>();
				args.values().pop();
				return sig_t::norm; } };

		struct CharTake : public Action {
			CharTake() : Action(
				TypeTuple::action(
					name_char, name_take, name_operator, name_char, TypeTuple::args( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<char_t>() = args.values().top().value<char_t>();
				args.values().pop();
				return sig_t::norm; } };


		struct CharConstruct : public Action {
			CharConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( char_t( 0 ), Qualifier::none ) );
				return sig_t::norm; } };

		struct CharCopyConstruct : public Action {
			CharCopyConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharBoolConstruct : public Action {
			CharBoolConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<char_t>( a1 ? 1 : 0 ), Qualifier::none ) );
				return sig_t::norm; } };

		struct CharByteConstruct : public Action {
			CharByteConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharIntConstruct : public Action {
			CharIntConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharShortConstruct : public Action {
			CharShortConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharLongConstruct : public Action {
			CharLongConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharFloatConstruct : public Action {
			CharFloatConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharLowConstruct : public Action {
			CharLowConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharHighConstruct : public Action {
			CharHighConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };

		struct CharIndexConstruct : public Action {
			CharIndexConstruct() : Action(
				TypeTuple::action( name_char, name_constructor, name_constructor, name_char, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (char_t)a1, Qualifier::none ) );
				return sig_t::norm; } };


		struct CharCompare : public Action {
			CharCompare() : Action(
				TypeTuple::action( name_char, symbol_cmp, name_operator, name_int, { { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct CharReset : public Action {
			CharReset() : Action( TypeTuple::action( name_char, name_reset, name_operator, name_char, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<char_t>();
				a1 = 0;
				return sig_t::norm; } };


		void registerChar();
	}
}
