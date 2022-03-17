#include "LongActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Short.h"
#include "Int.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerLongActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::LongConstruct() );
		scope.add( name_construct, new actions::LongCopyConstruct() );
		scope.add( name_construct, new actions::LongBoolConstruct() );
		scope.add( name_construct, new actions::LongByteConstruct() );
		scope.add( name_construct, new actions::LongCharConstruct() );
		scope.add( name_construct, new actions::LongShortConstruct() );
		scope.add( name_construct, new actions::LongIntConstruct() );
		scope.add( name_construct, new actions::LongFloatConstruct() );
		scope.add( name_construct, new actions::LongLowConstruct() );
		scope.add( name_construct, new actions::LongHighConstruct() );
		scope.add( name_construct, new actions::LongIndexConstruct() );
		scope.add( name_cmp, new actions::LongCmp() );
		scope.add( name_plus, new actions::LongPlus() );
		scope.add( name_minus, new actions::LongMinus() );
		scope.add( name_multiply, new actions::LongMultiply() );
		scope.add( name_divide, new actions::LongDivide() );
		scope.add( name_element, new actions::LongElementIndex() );
		scope.add( name_element, new actions::LongElementInt() );
		scope.add( name_slice, new actions::LongSlice() );
		scope.add( name_bitnot, new actions::LongBitNot() );
		scope.add( name_bitand, new actions::LongBitAnd() );
		scope.add( name_bitor, new actions::LongBitOr() );
		scope.add( name_bitxor, new actions::LongBitXor() );
		scope.add( name_lshift, new actions::LongLShift() );
		scope.add( name_rshift, new actions::LongRShift() );
	}




	namespace actions
	{
		type::Object* LongConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new LongInstance(); }
		type::Object* LongCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}

		type::Object* LongBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}
		type::Object* LongByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}
		type::Object* LongCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}
		type::Object* LongShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}
		type::Object* LongIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}
		type::Object* LongFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new LongInstance( static_cast<long_t>( arg1->value() ) );
		}
		type::Object* LongLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new LongInstance( static_cast<long_t>( arg1->value() ) );
		}
		type::Object* LongHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new LongInstance( static_cast<long_t>( arg1->value() ) );
		}
		type::Object* LongIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new LongInstance( arg1->value() );
		}

		type::Object* LongCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* LongPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* LongMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* LongMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* LongDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			if( arg2->value() == 0 )
				throw DivisionByZero();
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto rval = new LongInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}

		type::Object* LongElementIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new LongInstance();
			rval->value( arg1->value() & ( static_cast<long_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}
		type::Object* LongElementInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new LongInstance();
			rval->value( arg1->value() & ( static_cast<long_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}

		type::Object* LongSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto arg3 = _operand<IntInstance>( scope, action_node, 2 );
			auto rval = new LongInstance();
			auto v1 = arg2->value(), v2 = arg3->value();
			if( v1 < 0 )
				v1 = 32 + v1;
			if( v2 < 0 )
				v2 = 32 + v2;
			auto low = v1 < v2 ? v1 : v2;
			auto high = v1 > v2 ? v1 : v2;
			// TODO: Verify that v1 and v2 are in range!
			if( low > 0 && high <= 32 )
				rval->value( ( arg1->value() << ( 32 - high ) ) >> low );
			return rval;
		}

		type::Object* LongBitNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto rval = new LongInstance();
			rval->value( ~arg1->value() );
			return rval;
		}
		type::Object* LongBitAnd::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() & arg2->value() );
			return rval;
		}
		type::Object* LongBitOr::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() | arg2->value() );
			return rval;
		}
		type::Object* LongBitXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LongInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() ^ arg2->value() );
			return rval;
		}

		type::Object* LongLShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() << arg2->value() );
			return rval;
		}
		type::Object* LongRShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new LongInstance();
			rval->value( arg1->value() >> arg2->value() );
			return rval;
		}
	}
}
