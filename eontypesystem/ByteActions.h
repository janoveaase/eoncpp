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
		struct ByteCopy : public Action {
			ByteCopy() : Action(
				typetuple::newAction(
					name_byte, name_copy, name_operator ).returns(
						name_byte ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<byte_t>() = args.values().top().value<byte_t>();
				args.values().pop();
				return sig_t::_normal; } };

		struct ByteTake : public Action {
			ByteTake() : Action(
				typetuple::newAction(
					name_byte, name_take, name_operator ).returns(
						name_byte ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<byte_t>() = args.values().top().value<byte_t>();
				args.values().pop();
				return sig_t::_normal; } };


		struct ByteConstruct : public Action {
			ByteConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns( name_byte ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( byte_t( 0 ), name_byte, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteCopyConstruct : public Action {
			ByteCopyConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( a1, name_byte, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteBoolConstruct : public Action {
			ByteBoolConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<byte_t>( a1 ? 1 : 0 ), type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteCharConstruct : public Action {
			ByteCharConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteIntConstruct : public Action {
			ByteIntConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteShortConstruct : public Action {
			ByteShortConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteLongConstruct : public Action {
			ByteLongConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteFloatConstruct : public Action {
			ByteFloatConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteLowConstruct : public Action {
			ByteLowConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteHighConstruct : public Action {
			ByteHighConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct ByteIndexConstruct : public Action {
			ByteIndexConstruct() : Action(
				typetuple::newAction(
					name_byte, name_constructor, name_constructor ).returns(
						name_byte ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (byte_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };


		struct ByteCompare : public Action {
			ByteCompare() : Action(
				typetuple::newAction(
					name_byte, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct ByteReset : public Action {
			ByteReset() : Action(
				typetuple::newAction( name_byte, name_reset, name_operator ).returns( name_byte ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<byte_t>();
				a1 = 0;
				return sig_t::_normal; } };


		void registerByte();
	}
}
