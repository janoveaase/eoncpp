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
		struct StringCopy : public Action {
			StringCopy() : Action(
				typetuple::newAction(
					name_string, name_copy, name_operator ).returns(
						name_string ).arguments( typetuple::convert( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<string>() = args.values().top().value<string>();
				args.values().pop();
				return sig_t::_normal; } };

		struct StringTake : public Action {
			StringTake() : Action(
				typetuple::newAction(
					name_string, name_take, name_operator ).returns(
						name_string ).arguments( typetuple::convert( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<string>() = std::move( args.values().top().value<string>() );
				args.values().pop();
				return sig_t::_normal; } };


		struct StringConstruct : public Action {
			StringConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns( name_string ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newExplicit( string(), name_string, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		struct StringCopyConstruct : public Action {
			StringCopyConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringByteConstruct : public Action {
			StringByteConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringCharConstruct : public Action {
			StringCharConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringIntConstruct : public Action {
			StringIntConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringShortConstruct : public Action {
			StringShortConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLongConstruct : public Action {
			StringLongConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringFloatConstruct : public Action {
			StringFloatConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLowConstruct : public Action {
			StringLowConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringHighConstruct : public Action {
			StringHighConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringIndexConstruct : public Action {
			StringIndexConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringNameConstruct : public Action {
			StringNameConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_name } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringBytesConstruct : public Action {
			StringBytesConstruct() : Action(
				typetuple::newAction(
					name_string, name_constructor, name_constructor ).returns(
						name_string ).arguments( typetuple::convert( { name_bytes } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		struct StringCompare : public Action {
			StringCompare() : Action(
				typetuple::newAction(
					name_string, symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { name_string } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct StringReset : public Action {
			StringReset() : Action(
				typetuple::newAction( name_string, name_reset, name_operator ).returns( name_string ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };

		struct StringLength : public Action {
			StringLength() : Action(
				typetuple::newAction( name_string, name_length, name_firstarg ).returns( name_index ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override; };


		void registerString();
	}
}
