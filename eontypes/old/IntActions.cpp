#include "IntActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerIntActions( scope::Global& scope )
	{
		scope.addAction( name_construct, new actions::IntConstruct() );
		scope.addAction( name_construct, new actions::IntCopyConstruct() );
		scope.addAction( name_construct, new actions::IntBoolConstruct() );
		scope.addAction( name_construct, new actions::IntByteConstruct() );
		scope.addAction( name_construct, new actions::IntCharConstruct() );
		scope.addAction( name_construct, new actions::IntShortConstruct() );
		scope.addAction( name_construct, new actions::IntLongConstruct() );
		scope.addAction( name_construct, new actions::IntFloatConstruct() );
		scope.addAction( name_construct, new actions::IntLowConstruct() );
		scope.addAction( name_construct, new actions::IntHighConstruct() );
		scope.addAction( name_construct, new actions::IntIndexConstruct() );
		scope.addAction( name_cmp, new actions::IntCmp() );
		scope.addAction( name_plus, new actions::IntPlus() );
		scope.addAction( name_minus, new actions::IntMinus() );
		scope.addAction( name_multiply, new actions::IntMultiply() );
		scope.addAction( name_divide, new actions::IntDivide() );
		scope.addAction( name_element, new actions::IntElementIndex() );
		scope.addAction( name_element, new actions::IntElementInt() );
		scope.addAction( name_slice, new actions::IntSlice() );
		scope.addAction( name_bitnot, new actions::IntBitNot() );
		scope.addAction( name_bitand, new actions::IntBitAnd() );
		scope.addAction( name_bitor, new actions::IntBitOr() );
		scope.addAction( name_bitxor, new actions::IntBitXor() );
		scope.addAction( name_lshift, new actions::IntLShift() );
		scope.addAction( name_rshift, new actions::IntRShift() );
	}




	namespace actions
	{
		type::Object* IntConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new IntInstance(); }
		type::Object* IntCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new IntInstance( arg1->value() );
		}

		type::Object* IntBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new IntInstance( arg1->value() );
		}
		type::Object* IntByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new IntInstance( arg1->value() );
		}
		type::Object* IntCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new IntInstance( arg1->value() );
		}
		type::Object* IntShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new IntInstance( arg1->value() );
		}
		type::Object* IntLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new IntInstance( static_cast<int_t>( arg1->value() ) );
		}
		type::Object* IntFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new IntInstance( static_cast<int_t>( arg1->value() ) );
		}
		type::Object* IntLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new IntInstance( static_cast<int_t>( arg1->value() ) );
		}
		type::Object* IntHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new IntInstance( static_cast<int_t>( arg1->value() ) );
		}
		type::Object* IntIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new IntInstance( static_cast<int_t>( arg1->value() ) );
		}

		type::Object* IntCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* IntPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* IntMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* IntMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* IntDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			if( arg2->value() == 0 )
				throw DivisionByZero();
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto rval = new IntInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}

		type::Object* IntElementIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			if( arg2->value() >= sizeof( int_t ) )
				throw type::NotFound();
			auto rval = new IntInstance();
			rval->value( arg1->value() & ( 1 << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}
		type::Object* IntElementInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			index_t pos = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
				: sizeof( int_t ) - static_cast<index_t>( -arg2->value() );
			if( pos >= sizeof( int_t ) )
				throw type::NotFound();
			auto rval = new IntInstance();
			rval->value( arg1->value() & ( 1 << ( pos - 1 ) ) ? 1 : 0 );
			return rval;
		}

		type::Object* IntSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto arg3 = _operand<IntInstance>( scope, action_node, 2 );
			auto rval = new IntInstance();
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

		type::Object* IntBitNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto rval = new IntInstance();
			rval->value( ~arg1->value() );
			return rval;
		}
		type::Object* IntBitAnd::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() & arg2->value() );
			return rval;
		}
		type::Object* IntBitOr::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() | arg2->value() );
			return rval;
		}
		type::Object* IntBitXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() ^ arg2->value() );
			return rval;
		}

		type::Object* IntLShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() << arg2->value() );
			return rval;
		}
		type::Object* IntRShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() >> arg2->value() );
			return rval;
		}
	}
}
