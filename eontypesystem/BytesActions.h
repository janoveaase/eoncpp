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
		struct BytesCopy : public Action {
			BytesCopy() : Action(
				typetuple::newAction(
					name_bytes, name_copy, name_operator ).returns(
						name_bytes ).arguments( typetuple::convert( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<std::string>() = args.values().top().value<std::string>();
				args.values().pop();
				return sig_t::_normal; } };

		struct BytesTake : public Action {
			BytesTake() : Action(
				typetuple::newAction(
					name_bytes, name_take, name_operator ).returns(
						name_bytes ).arguments( typetuple::convert( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<std::string>() = std::move( args.values().top().value<std::string>() );
				args.values().pop();
				return sig_t::_normal; } };


		struct BytesConstruct : public Action {
			BytesConstruct() : Action(
				typetuple::newAction( name_bytes, name_constructor, name_constructor ).returns( name_bytes ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( std::string(), name_bytes, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct BytesCopyConstruct : public Action {
			BytesCopyConstruct() : Action(
				typetuple::newAction( name_bytes, name_constructor, name_constructor ).returns(
					name_bytes ).arguments( typetuple::convert( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesByteConstruct : public Action {
			BytesByteConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesCharConstruct : public Action {
			BytesCharConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesIntConstruct : public Action {
			BytesIntConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesShortConstruct : public Action {
			BytesShortConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesLongConstruct : public Action {
			BytesLongConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments(	typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesFloatConstruct : public Action {
			BytesFloatConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesLowConstruct : public Action {
			BytesLowConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesHighConstruct : public Action {
			BytesHighConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesIndexConstruct : public Action {
			BytesIndexConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesNameConstruct : public Action {
			BytesNameConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct BytesStringConstruct : public Action {
			BytesStringConstruct() : Action(
				typetuple::newAction(
					name_bytes, name_constructor, name_constructor ).returns(
						name_bytes ).arguments( typetuple::convert( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BytesCompare : public Action {
			BytesCompare() : Action(
				typetuple::newAction(
					name_bytes, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override;
		};


		struct BytesReset : public Action {
			BytesReset() : Action(
				typetuple::newAction( name_bytes, name_reset, name_operator ).returns( name_bytes ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct BytesLength : public Action {
			BytesLength() : Action(
				typetuple::newAction( name_bytes, name_length, name_firstarg ).returns( name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerBytes();
	}
}
