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
		struct ByteCopy : public Action {
			ByteCopy() : Action(
				TypeTuple::action(
					name_byte, name_copy, name_operator, name_byte, TypeTuple::args( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<byte_t>() = args.values().top().value<byte_t>();
				args.values().pop();
				return sig_t::norm; } };

		struct ByteTake : public Action {
			ByteTake() : Action(
				TypeTuple::action(
					name_byte, name_take, name_operator, name_byte, TypeTuple::args( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<byte_t>() = args.values().top().value<byte_t>();
				args.values().pop();
				return sig_t::norm; } };


		struct ByteConstruct : public Action {
			ByteConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( byte_t( 0 ), name_byte, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteCopyConstruct : public Action {
			ByteCopyConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1, name_byte, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteBoolConstruct : public Action {
			ByteBoolConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<byte_t>( a1 ? 1 : 0 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteCharConstruct : public Action {
			ByteCharConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteIntConstruct : public Action {
			ByteIntConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteShortConstruct : public Action {
			ByteShortConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteLongConstruct : public Action {
			ByteLongConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteFloatConstruct : public Action {
			ByteFloatConstruct() : Action(
				TypeTuple::action( name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteLowConstruct : public Action {
			ByteLowConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteHighConstruct : public Action {
			ByteHighConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct ByteIndexConstruct : public Action {
			ByteIndexConstruct() : Action(
				TypeTuple::action(
					name_byte, name_constructor, name_constructor, name_byte, { { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::none ) );
				return sig_t::norm; } };


		struct ByteCompare : public Action {
			ByteCompare() : Action(
				TypeTuple::action(
					name_byte, symbol_cmp, name_operator, name_int, { { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct ByteReset : public Action {
			ByteReset() : Action(
				TypeTuple::action( name_byte, name_reset, name_operator, name_byte, {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<byte_t>();
				a1 = 0;
				return sig_t::norm; } };


		void registerByte();
	}
}
