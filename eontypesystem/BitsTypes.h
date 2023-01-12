#pragma once

#include "Action.h"
#include "TupleFactory.h"



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
		template<typename T>
		struct BitsConstruct : public Action {
			BitsConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push(
					Attribute::newImplicit(
						static_cast<T>( 0 ), type::Qualifier::none, type::hint::bits ) ); return sig_t::norm; } };

		template<typename T>
		struct BitsCopyConstruct : public Action {
			BitsCopyConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsBoolConstruct : public Action {
			BitsBoolConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 ? 1 : 0 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsByteConstruct : public Action {
			BitsByteConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit<T>( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsCharConstruct : public Action {
			BitsCharConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsIntConstruct : public Action {
			BitsIntConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsShortConstruct : public Action {
			BitsShortConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsLongConstruct : public Action {
			BitsLongConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsFloatConstruct : public Action {
			BitsFloatConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsLowConstruct : public Action {
			BitsLowConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsHighConstruct : public Action {
			BitsHighConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsIndexConstruct : public Action {
			BitsIndexConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit<T>( (T)a1, type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };


		template<typename T>
		struct BitsCompare : public Action {
			BitsCompare() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_cmp,
					name_operator,
					name_int,
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit(
						static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ),
						type::Qualifier::none,
						type::hint::bits ) );
				return sig_t::norm; } };


		template<typename T>
		struct BitsPlusAssign : public Action {
			BitsPlusAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_plus_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T, type::hint::bits>() += a2;
				return sig_t::norm; } };

		template<typename T>
		struct BitsMinusAssign : public Action {
			BitsMinusAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_minus_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T, type::hint::bits>() -= a2;
				return sig_t::norm; } };

		template<typename T>
		struct BitsMultiplyAssign : public Action {
			BitsMultiplyAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_multiply_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T, type::hint::bits>() *= a2;
				return sig_t::norm; } };

		template<typename T>
		struct BitsDivideAssign : public Action {
			BitsDivideAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_divide_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( Attribute::newName( name( "division_by_zero" ), type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto& a1 = args.values().top();
				a1.value<T, type::hint::bits>() /= a2;
				return sig_t::norm; } };

		template<typename T>
		struct BitsPlus : public Action {
			BitsPlus() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_plus,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 + a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsMinus : public Action {
			BitsMinus() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_minus,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 - a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsMultiply : public Action {
			BitsMultiply() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_multiply,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 * a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsDivide : public Action {
			BitsDivide() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_divide,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( Attribute::newName( name( "division_by_zero" ), type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 / a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsMod: public Action {
			BitsMod() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_mod,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( Attribute::newName( name( "division_by_zero" ), type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 % a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsPow : public Action {
			BitsPow() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_pow,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit(
						static_cast<T>( std::pow( a1, a2 ) ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsAtIndex : public Action {
			BitsAtIndex() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_element,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<index_t>();
				args.values().pop();
				if( a2 >= sizeof( T ) )
				{
					args.values().push( Attribute::newName( name_not_found, type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit(
						static_cast<T>( a1 & (T)( (T)1 << a2 ) ? 1 : 0 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsAtInt : public Action {
			BitsAtInt() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_element,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<int_t>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				int_t sz_t = static_cast<int_t>( sizeof( T ) * 8 );
				if( a2 < 0 )
					a2 = sz_t + a2;
				if( a2 < 0 || a2 >= sz_t )
				{
					args.values().push( Attribute::newName( name_not_found, type::Qualifier::none ) );
					return sig_t::raise;
				}
				args.values().push(
					Attribute::newImplicit(
						static_cast<T>( a1 & (T)( (T)1 << a2 ) ? 1 : 0 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsSlice : public Action {
			BitsSlice() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_slice,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_int }, { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a3 = args.values().top().value<int_t>();
				args.values().pop();
				auto a2 = args.values().top().value<int_t>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>(); args.values().pop();
				int_t sz_t = static_cast<int_t>( sizeof( T ) * 8 );
				if( a2 < 0 )
					a2 = sz_t + a2;
				if( a3 < 0 )
					a3 = sz_t + a3;
				if( a2 < 0 || a2 >= sz_t || a3 < 0 || a3 >= sz_t )
				{
					args.values().push( Attribute::newName( name_not_found, type::Qualifier::none ) );
					return sig_t::raise;
				}
				if( a2 > a3 )
				{
					auto tmp = a2;
					a2 = a3;
					a3 = a2;
				}
				a3 = sz_t - a3;
				args.values().push(
					Attribute::newImplicit(
						(T)( (T)( (T)( a1 << a3 ) >> a3 ) >> a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsNot : public Action {
			BitsNot() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_bitnot,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( ~a1 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsAnd : public Action {
			BitsAnd() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_bitand,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 & a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsOr : public Action {
			BitsOr() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_bitor,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 | a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsXor : public Action {
			BitsXor() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_bitxor,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, type::mapCppType( CPPTYPE( T ), type::hint::bits ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 ^ a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsLShift : public Action {
			BitsLShift() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_lshift,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<int_t>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( (T)( a1 << a2 ) ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsRShift : public Action {
			BitsRShift() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					symbol_rshift,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<int_t>();
				args.values().pop();
				auto a1 = args.values().top().value<T, type::hint::bits>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<T>( a1 >> a2 ), type::Qualifier::none, type::hint::bits ) );
				return sig_t::norm; } };

		template<typename T>
		struct BitsReset : public Action {
			inline BitsReset() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ), type::hint::bits ),
					name_reset,
					name_operator,
					type::mapCppType( CPPTYPE( T ), type::hint::bits ), {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<T, type::hint::bits>();
				a1 = 0;
				return sig_t::norm; } };


		template<typename T>
		void registerBits()
		{
			scope::global().registerType( type::mapCppType( CPPTYPE( T ), type::hint::bits ) );
			scope::global().registerAction( new BitsConstruct<T>() );
			scope::global().registerAction( new BitsCopyConstruct<T>() );
			scope::global().registerAction( new BitsBoolConstruct<T>() );
			scope::global().registerAction( new BitsByteConstruct<T>() );
			scope::global().registerAction( new BitsCharConstruct<T>() );
			scope::global().registerAction( new BitsIntConstruct<T>() );
			scope::global().registerAction( new BitsShortConstruct<T>() );
			scope::global().registerAction( new BitsLongConstruct<T>() );;
			scope::global().registerAction( new BitsFloatConstruct<T>() );
			scope::global().registerAction( new BitsLowConstruct<T>() );
			scope::global().registerAction( new BitsHighConstruct<T>() );
			scope::global().registerAction( new BitsIndexConstruct<T>() );
			scope::global().registerAction( new BitsCompare<T>() );
			scope::global().registerAction( new BitsPlusAssign<T>() );
			scope::global().registerAction( new BitsMinusAssign<T>() );
			scope::global().registerAction( new BitsMultiplyAssign<T>() );
			scope::global().registerAction( new BitsDivideAssign<T>() );
			scope::global().registerAction( new BitsPlus<T>() );
			scope::global().registerAction( new BitsMinus<T>() );
			scope::global().registerAction( new BitsMultiply<T>() );
			scope::global().registerAction( new BitsDivide<T>() );
			scope::global().registerAction( new BitsMod<T>() );
			scope::global().registerAction( new BitsPow<T>() );
			scope::global().registerAction( new BitsAtIndex<T>() );
			scope::global().registerAction( new BitsAtInt<T>() );
			scope::global().registerAction( new BitsSlice<T>() );
			scope::global().registerAction( new BitsNot<T>() );
			scope::global().registerAction( new BitsAnd<T>() );
			scope::global().registerAction( new BitsOr<T>() );
			scope::global().registerAction( new BitsXor<T>() );
			scope::global().registerAction( new BitsLShift<T>() );
			scope::global().registerAction( new BitsRShift<T>() );
			scope::global().registerAction( new BitsReset<T>() );
		}
	}
}
