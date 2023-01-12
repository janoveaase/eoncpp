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
		struct FloatCopy : public Action {
			FloatCopy() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_copy,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					TypeTuple::args( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<T>() = args.values().top().value<T>();
				args.values().pop();
				return sig_t::norm; } };

		template<typename T>
		struct FloatTake : public Action {
			FloatTake() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_take,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					TypeTuple::args( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<T>() = args.values().top().value<T>();
				args.values().pop();
				return sig_t::norm; } };

		template<typename T>
		struct FloatConstruct : public Action {
			FloatConstruct() : Action(
				TypeTuple::action( type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( Attribute::newImplicit( static_cast<T>( 0 ), type::Qualifier::none ) );
				return sig_t::norm; } };
		 
		template<typename T>
		struct FloatCopyConstruct : public Action {
			FloatCopyConstruct() : Action(
				TypeTuple::action( type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatBoolConstruct : public Action {
			FloatBoolConstruct() : Action(
				TypeTuple::action( type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_bool } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<T>( a1 ? 1.0 : 0.0 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatByteConstruct : public Action {
			FloatByteConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_byte } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatCharConstruct : public Action {
			FloatCharConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_char } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatIntConstruct : public Action {
			FloatIntConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_int } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<int_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatShortConstruct : public Action {
			FloatShortConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_short } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatLongConstruct : public Action {
			FloatLongConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_long } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		struct FloatLowConstruct : public Action {
			FloatLowConstruct() : Action(
				TypeTuple::action(
					name_float,
					name_constructor,
					name_constructor,
					name_float,
					{ { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (flt_t)a1, name_float, type::Qualifier::none ) );
				return sig_t::norm; } };
		struct FloatHighConstruct : public Action {
			FloatHighConstruct() : Action(
				TypeTuple::action(
					name_float, name_constructor, name_constructor, name_float, { { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (flt_t)a1, name_float, type::Qualifier::none ) );
				return sig_t::norm; } };
		struct LowFloatConstruct : public Action {
			LowFloatConstruct() : Action(
				TypeTuple::action(
					name_low, name_constructor, name_constructor, name_low, { { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (low_t)a1, name_low, type::Qualifier::none ) );
				return sig_t::norm; } };
		struct LowHighConstruct : public Action {
			LowHighConstruct() : Action(
				TypeTuple::action(
					name_low, name_constructor, name_constructor, name_low, { { no_name, name_high } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (low_t)a1, name_low, type::Qualifier::none ) );
				return sig_t::norm; } };
		struct HighFloatConstruct : public Action {
			HighFloatConstruct() : Action(
				TypeTuple::action(
					name_high, name_constructor, name_constructor, name_high, { { no_name, name_float } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (high_t)a1, name_high, type::Qualifier::none ) );
				return sig_t::norm; } };
		struct HighLowConstruct : public Action {
			HighLowConstruct() : Action(
				TypeTuple::action(
					name_high, name_constructor, name_constructor, name_high, { { no_name, name_low } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( Attribute::newExplicit( (high_t)a1, name_high, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatIndexConstruct : public Action {
			FloatIndexConstruct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_index } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatB8Construct : public Action {
			FloatB8Construct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ), { { no_name, name_b8 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b8_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; }
		};
		template<typename T>
		struct FloatB16Construct : public Action {
			FloatB16Construct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_b16 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b16_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; }
		};
		template<typename T>
		struct FloatB32Construct : public Action {
			FloatB32Construct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_b32 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b32_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; }
		};
		template<typename T>
		struct FloatB64Construct : public Action {
			FloatB64Construct() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_constructor,
					name_constructor,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, name_b64 } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b64_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (T)a1, type::Qualifier::none ) );
				return sig_t::norm; }
		};

		template<typename T>
		struct FloatCompare : public Action {
			FloatCompare() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_cmp,
					name_operator,
					name_int,
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push(
					Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), type::Qualifier::none ) );
				return sig_t::norm; } };
		

		template<typename T>
		struct FloatUnaryMinus : public Action {
			FloatUnaryMinus() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_unary_minus,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ), {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( -a1, type::Qualifier::none ) );
				return sig_t::norm; } };


		template<typename T>
		struct FloatPlusAssign : public Action {
			FloatPlusAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_plus_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() += a2;
				return sig_t::norm; } };

		template<typename T>
		struct FloatMinusAssign : public Action {
			FloatMinusAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_minus_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() -= a2;
				return sig_t::norm; } };

		template<typename T>
		struct FloatMultiplyAssign : public Action {
			FloatMultiplyAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_multiply_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() *= a2;
				return sig_t::norm; } };

		template<typename T>
		struct FloatDivideAssign : public Action {
			FloatDivideAssign() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_divide_assign,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				if( a2 == 0.0 )
				{
					args.values().push(
						Attribute::newExplicit( name( "division_by_zero" ), name_name, type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto& a1 = args.values().top();
				a1.value<T>() /= a2;
				return sig_t::norm; } };

		template<typename T>
		struct FloatPlus : public Action {
			FloatPlus() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_plus,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<T>( a1 + a2 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatMinus : public Action {
			FloatMinus() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_minus,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<T>( a1 - a2 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatMultiply : public Action {
			FloatMultiply() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_multiply,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<T>( a1 * a2 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatDivide : public Action {
			FloatDivide() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					symbol_divide,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ),
					{ { no_name, type::mapCppType( CPPTYPE( T ) ) } } ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				if( a2 == 0.0 )
				{
					args.values().push(
						Attribute::newExplicit( name( "division_by_zero" ), name_name, type::Qualifier::none ) );
					return sig_t::raise;
				}
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( static_cast<T>( a1 / a2 ), type::Qualifier::none ) );
				return sig_t::norm; } };

		template<typename T>
		struct FloatReset : public Action {
			FloatReset() : Action(
				TypeTuple::action(
					type::mapCppType( CPPTYPE( T ) ),
					name_reset,
					name_operator,
					type::mapCppType( CPPTYPE( T ) ), {} ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<T>();
				a1 = 0.0;
				return sig_t::norm; } };


		template<typename T>
		void registerFloat()
		{
			scope::global().registerType( type::mapCppType( CPPTYPE( T ) ) );
			scope::global().registerAction( new FloatCopy<T>() );
			scope::global().registerAction( new FloatTake<T>() );
			scope::global().registerAction( new FloatConstruct<T>() );
			scope::global().registerAction( new FloatCopyConstruct<T>() );
			scope::global().registerAction( new FloatBoolConstruct<T>() );
			scope::global().registerAction( new FloatByteConstruct<T>() );
			scope::global().registerAction( new FloatCharConstruct<T>() );
			scope::global().registerAction( new FloatIntConstruct<T>() );
			scope::global().registerAction( new FloatShortConstruct<T>() );
			scope::global().registerAction( new FloatLongConstruct<T>() );
			if( typeid( T ) == typeid( flt_t ) )
			{
				scope::global().registerAction( new FloatLowConstruct() );
				scope::global().registerAction( new FloatHighConstruct() );
			}
			else if( typeid( T ) == typeid( low_t ) )
			{
				scope::global().registerAction( new LowFloatConstruct() );
				scope::global().registerAction( new LowHighConstruct() );
			}
			else if( typeid( T ) == typeid( high_t ) )
			{
				scope::global().registerAction( new HighFloatConstruct() );
				scope::global().registerAction( new HighLowConstruct() );
			}
			scope::global().registerAction( new FloatIndexConstruct<T>() );
			scope::global().registerAction( new FloatB8Construct<T>() );
			scope::global().registerAction( new FloatB16Construct<T>() );
			scope::global().registerAction( new FloatB32Construct<T>() );
			scope::global().registerAction( new FloatB64Construct<T>() );
			scope::global().registerAction( new FloatCompare<T>() );
			scope::global().registerAction( new FloatUnaryMinus<T>() );
			scope::global().registerAction( new FloatPlusAssign<T>() );
			scope::global().registerAction( new FloatMinusAssign<T>() );
			scope::global().registerAction( new FloatMultiplyAssign<T>() );
			scope::global().registerAction( new FloatDivideAssign<T>() );
			scope::global().registerAction( new FloatPlus<T>() );
			scope::global().registerAction( new FloatMinus<T>() );
			scope::global().registerAction( new FloatMultiply<T>() );
			scope::global().registerAction( new FloatDivide<T>() );
			scope::global().registerAction( new FloatReset<T>() );
		}
	}
}
