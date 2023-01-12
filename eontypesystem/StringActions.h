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
		struct StringCopy : public Action {
			StringCopy() : Action(
				TypeTuple::action(
					name_string, name_copy, name_operator, name_string, TypeTuple::args( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<string>() = args.values().top().value<string>();
				args.values().pop();
				return sig_t::norm; } };

		struct StringTake : public Action {
			StringTake() : Action(
				TypeTuple::action(
					name_string, name_take, name_operator, name_string, TypeTuple::args( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<string>() = std::move( args.values().top().value<string>() );
				args.values().pop();
				return sig_t::norm; } };


		struct StringConstruct : public Action {
			StringConstruct() : Action(
				TypeTuple::action( name_string, name_constructor, name_constructor, name_string ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( string(), name_string, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct StringCopyConstruct : public Action {
			StringCopyConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringByteConstruct : public Action {
			StringByteConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringCharConstruct : public Action {
			StringCharConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringIntConstruct : public Action {
			StringIntConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringShortConstruct : public Action {
			StringShortConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLongConstruct : public Action {
			StringLongConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringFloatConstruct : public Action {
			StringFloatConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLowConstruct : public Action {
			StringLowConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringHighConstruct : public Action {
			StringHighConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringIndexConstruct : public Action {
			StringIndexConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringNameConstruct : public Action {
			StringNameConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringBytesConstruct : public Action {
			StringBytesConstruct() : Action(
				TypeTuple::action(
					name_string,
					name_constructor,
					name_constructor,
					name_string,
					TypeTuple::args( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct StringCompare : public Action {
			StringCompare() : Action(
				TypeTuple::action(
					name_string,
					symbol_cmp,
					name_operator,
					name_int,
					TypeTuple::args( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct StringReset : public Action {
			StringReset() : Action(
				TypeTuple::action( name_string, name_reset, name_operator, name_string, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLength : public Action {
			StringLength() : Action(
				TypeTuple::action( name_string, name_length, name_firstarg, name_index, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerString();
	}
}
