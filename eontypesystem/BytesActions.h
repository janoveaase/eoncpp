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
		struct BytesCopy : public Action {
			BytesCopy() : Action(
				TypeTuple::action(
					name_bytes, name_copy, name_operator, name_bytes, TypeTuple::args( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<std::string>() = args.values().top().value<std::string>();
				args.values().pop();
				return sig_t::norm; } };

		struct BytesTake : public Action {
			BytesTake() : Action(
				TypeTuple::action(
					name_bytes, name_take, name_operator, name_bytes, TypeTuple::args( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<std::string>() = std::move( args.values().top().value<std::string>() );
				args.values().pop();
				return sig_t::norm; } };


		struct BytesConstruct : public Action {
			BytesConstruct() : Action(
				TypeTuple::action( name_bytes, name_constructor, name_constructor, name_bytes ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( std::string(), name_bytes, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct BytesCopyConstruct : public Action {
			BytesCopyConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesByteConstruct : public Action {
			BytesByteConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesCharConstruct : public Action {
			BytesCharConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesIntConstruct : public Action {
			BytesIntConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesShortConstruct : public Action {
			BytesShortConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesLongConstruct : public Action {
			BytesLongConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesFloatConstruct : public Action {
			BytesFloatConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesLowConstruct : public Action {
			BytesLowConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesHighConstruct : public Action {
			BytesHighConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesIndexConstruct : public Action {
			BytesIndexConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesNameConstruct : public Action {
			BytesNameConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesStringConstruct : public Action {
			BytesStringConstruct() : Action(
				TypeTuple::action(
					name_bytes,
					name_constructor,
					name_constructor,
					name_bytes,
					TypeTuple::args( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BytesCompare : public Action {
			BytesCompare() : Action(
				TypeTuple::action(
					name_bytes,
					symbol_cmp,
					name_operator,
					name_int,
					TypeTuple::args( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override;
		};


		struct BytesReset : public Action {
			BytesReset() : Action(
				TypeTuple::action( name_bytes, name_reset, name_operator, name_bytes ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BytesLength : public Action {
			BytesLength() : Action(
				TypeTuple::action( name_bytes, name_length, name_firstarg, name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerBytes();
	}
}
