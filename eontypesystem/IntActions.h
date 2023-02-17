#pragma once

#include "Action.h"
#include "TupleFactory.h"
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
		template<typename T>
		struct IntCopy : public Action {
			IntCopy() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_copy, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<T>() = args.values().top().value<T>();
				args.values().pop();
				return sig_t::_normal; } };

		template<typename T>
		struct IntTake : public Action {
			IntTake() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_take, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().at( 1 ).value<T>() = args.values().top().value<T>();
				args.values().pop();
				return sig_t::_normal; } };

		template<typename T>
		struct IntConstruct : public Action {
			IntConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				args.values().push( static_cast<T>( 0 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntCopyConstruct : public Action {
			IntCopyConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				const auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( a1 );
				return sig_t::_normal; } };

		template<typename T>
		struct IntBoolConstruct : public Action {
			IntBoolConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_bool } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<bool>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 ? 1 : 0 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntByteConstruct : public Action {
			IntByteConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_byte } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<byte_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };

		template<typename T>
		struct IntCharConstruct : public Action {
			IntCharConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_char } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<char_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };

		struct IntShortConstruct : public Action {
			IntShortConstruct() : Action(
				typetuple::newAction(
					name_int, name_constructor, name_constructor ).returns(
						name_int ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (int_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };
		struct IntLongConstruct : public Action {
			IntLongConstruct() : Action(
				typetuple::newAction(
					name_int, name_constructor, name_constructor ).returns(
						name_int ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (int_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };
		struct ShortIntConstruct : public Action {
			ShortIntConstruct() : Action(
				typetuple::newAction(
					name_short, name_constructor, name_constructor ).returns(
						name_short ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (short_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };
		struct ShortLongConstruct : public Action {
			ShortLongConstruct() : Action(
				typetuple::newAction(
					name_short, name_constructor, name_constructor ).returns(
						name_short ).arguments( typetuple::convert( { name_long } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<long_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (short_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };
		struct LongIntConstruct : public Action {
			LongIntConstruct() : Action(
				typetuple::newAction(
					name_long, name_constructor, name_constructor ).returns(
						name_long ).arguments( typetuple::convert( { name_int } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (long_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };
		struct LongShortConstruct : public Action {
			LongShortConstruct() : Action(
				typetuple::newAction(
					name_long, name_constructor, name_constructor ).returns(
						name_long ).arguments( typetuple::convert( { name_short } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<short_t>();
				args.values().pop();
				args.values().push( Attribute::newImplicit( (long_t)a1, type::Qualifier::_none ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntFloatConstruct : public Action {
			IntFloatConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_float } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<flt_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };

		template<typename T>
		struct IntLowConstruct : public Action {
			IntLowConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_low } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<low_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };

		template<typename T>
		struct IntHighConstruct : public Action {
			IntHighConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_high } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<high_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };

		template<typename T>
		struct IntIndexConstruct : public Action {
			IntIndexConstruct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_index } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<index_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; } };


		template<typename T>
		struct IntB8Construct : public Action {
			IntB8Construct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_b8 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b8_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; }
		};
		template<typename T>
		struct IntB16Construct : public Action {
			IntB16Construct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_b16 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b16_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; }
		};
		template<typename T>
		struct IntB32Construct : public Action {
			IntB32Construct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_b32 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b32_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; }
		};
		template<typename T>
		struct IntB64Construct : public Action {
			IntB64Construct() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_constructor, name_constructor ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments( typetuple::convert( { name_b64 } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<b64_t>();
				args.values().pop();
				args.values().push( (T)a1 );
				return sig_t::_normal; }
		};


		template<typename T>
		struct IntCompare : public Action {
			IntCompare() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_cmp, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntLt : public Action {
			IntLt() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_lt, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 < a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntLe : public Action {
			IntLe() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_le, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 <= a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntGt : public Action {
			IntGt() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_gt, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 > a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntGe : public Action {
			IntGe() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_ge, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 >= a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntEq : public Action {
			IntEq() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_eq, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 == a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntNe : public Action {
			IntNe() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_ne, name_operator ).returns(
						name_int ).arguments( typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<bool>( a1 != a2 ) );
				return sig_t::_normal; } };


		template<typename T>
		struct IntUnaryMinus : public Action {
			IntUnaryMinus() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_unary_minus, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( -a1 );
				return sig_t::_normal; } };


		template<typename T>
		struct IntPlusAssign : public Action {
			IntPlusAssign() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_plus_assign, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() += a2;
				return sig_t::_normal; } };

		template<typename T>
		struct IntMinusAssign : public Action {
			IntMinusAssign() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_minus_assign, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() -= a2;
				return sig_t::_normal; } };

		template<typename T>
		struct IntMultiplyAssign : public Action {
			IntMultiplyAssign() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_multiply_assign, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto& a1 = args.values().top();
				a1.value<T>() *= a2;
				return sig_t::_normal; } };

		template<typename T>
		struct IntDivideAssign : public Action {
			IntDivideAssign() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_divide_assign, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( name( "division_by_zero" ) );
					return sig_t::_raise;
				}
				auto& a1 = args.values().top();
				a1.value<T>() /= a2;
				return sig_t::_normal; } };

		template<typename T>
		struct IntPlus : public Action {
			IntPlus() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_plus, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 + a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntMinus : public Action {
			IntMinus() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_minus, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 - a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntMultiply : public Action {
			IntMultiply() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_multiply, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 * a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntDivide : public Action {
			IntDivide() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), symbol_divide, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( name( "division_by_zero" ) );
					return sig_t::_raise;
				}
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 / a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntMod: public Action {
			IntMod() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_mod, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				if( a2 == 0 )
				{
					args.values().push( name( "division_by_zero" ) );
					return sig_t::_raise;
				}
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( a1 % a2 ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntPow : public Action {
			IntPow() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_pow, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ).arguments(
							typetuple::convert( { type::mapCppType( CPPTYPE( T ) ) } ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto a2 = args.values().top().value<T>();
				args.values().pop();
				auto a1 = args.values().top().value<T>();
				args.values().pop();
				args.values().push( static_cast<T>( std::pow( a1, a2 ) ) );
				return sig_t::_normal; } };

		template<typename T>
		struct IntReset : public Action {
			IntReset() : Action(
				typetuple::newAction(
					type::mapCppType( CPPTYPE( T ) ), name_reset, name_operator ).returns(
						type::mapCppType( CPPTYPE( T ) ) ) ) {}
			sig_t operator()( ActionExeArgs& args ) const override {
				auto& a1 = args.values().top().value<T>();
				a1 = 0;
				return sig_t::_normal; } };


		template<typename T>
		void registerInt()
		{
			scope::global().registerType( type::mapCppType( CPPTYPE( T ) ) );
			scope::global().registerAction( new IntCopy<T>() );
			scope::global().registerAction( new IntTake<T>() );
			scope::global().registerAction( new IntConstruct<T>() );
			scope::global().registerAction( new IntCopyConstruct<T>() );
			scope::global().registerAction( new IntBoolConstruct<T>() );
			scope::global().registerAction( new IntByteConstruct<T>() );
			scope::global().registerAction( new IntCharConstruct<T>() );
			if( typeid( T ) == typeid( int_t ) )
			{
				scope::global().registerAction( new IntShortConstruct() );
				scope::global().registerAction( new IntLongConstruct() );
			}
			else if( typeid( T ) == typeid( short_t ) )
			{
				scope::global().registerAction( new ShortIntConstruct() );
				scope::global().registerAction( new ShortLongConstruct() );
			}
			else if( typeid( T ) == typeid( long_t ) )
			{
				scope::global().registerAction( new LongIntConstruct() );
				scope::global().registerAction( new LongShortConstruct() );
			}
			scope::global().registerAction( new IntFloatConstruct<T>() );
			scope::global().registerAction( new IntLowConstruct<T>() );
			scope::global().registerAction( new IntHighConstruct<T>() );
			scope::global().registerAction( new IntIndexConstruct<T>() );
			scope::global().registerAction( new IntB8Construct<T>() );
			scope::global().registerAction( new IntB16Construct<T>() );
			scope::global().registerAction( new IntB32Construct<T>() );
			scope::global().registerAction( new IntB64Construct<T>() );
			scope::global().registerAction( new IntCompare<T>() );
			scope::global().registerAction( new IntLt<T>() );
			scope::global().registerAction( new IntLe<T>() );
			scope::global().registerAction( new IntGt<T>() );
			scope::global().registerAction( new IntGe<T>() );
			scope::global().registerAction( new IntEq<T>() );
			scope::global().registerAction( new IntNe<T>() );
			scope::global().registerAction( new IntUnaryMinus<T>() );
			scope::global().registerAction( new IntPlusAssign<T>() );
			scope::global().registerAction( new IntMinusAssign<T>() );
			scope::global().registerAction( new IntMultiplyAssign<T>() );
			scope::global().registerAction( new IntDivideAssign<T>() );
			scope::global().registerAction( new IntPlus<T>() );
			scope::global().registerAction( new IntMinus<T>() );
			scope::global().registerAction( new IntMultiply<T>() );
			scope::global().registerAction( new IntDivide<T>() );
			scope::global().registerAction( new IntMod<T>() );
			scope::global().registerAction( new IntPow<T>() );
			scope::global().registerAction( new IntReset<T>() );
		}
	}
}
