#include "ShortActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerShortActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::ShortConstruct() );
		scope.add( name_construct, new actions::ShortCopyConstruct() );
		scope.add( name_construct, new actions::ShortBoolConstruct() );
		scope.add( name_construct, new actions::ShortByteConstruct() );
		scope.add( name_construct, new actions::ShortCharConstruct() );
		scope.add( name_construct, new actions::ShortIntConstruct() );
		scope.add( name_construct, new actions::ShortLongConstruct() );
		scope.add( name_construct, new actions::ShortFloatConstruct() );
		scope.add( name_construct, new actions::ShortLowConstruct() );
		scope.add( name_construct, new actions::ShortHighConstruct() );
		scope.add( name_construct, new actions::ShortIndexConstruct() );
		scope.add( name_cmp, new actions::ShortCmp() );
		scope.add( name_plus, new actions::ShortPlus() );
		scope.add( name_minus, new actions::ShortMinus() );
		scope.add( name_multiply, new actions::ShortMultiply() );
		scope.add( name_divide, new actions::ShortDivide() );
		scope.add( name_element, new actions::ShortElementIndex() );
		scope.add( name_element, new actions::ShortElementInt() );
		scope.add( name_slice, new actions::ShortSlice() );
		scope.add( name_bitnot, new actions::ShortBitNot() );
		scope.add( name_bitand, new actions::ShortBitAnd() );
		scope.add( name_bitor, new actions::ShortBitOr() );
		scope.add( name_bitxor, new actions::ShortBitXor() );
		scope.add( name_lshift, new actions::ShortLShift() );
		scope.add( name_rshift, new actions::ShortRShift() );
	}




	namespace actions
	{
		type::Object* ShortConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new ShortInstance(); }
		type::Object* ShortCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new ShortInstance( arg1->value() );
		}

		type::Object* ShortBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new ShortInstance( arg1->value() );
		}
		type::Object* ShortByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new ShortInstance( arg1->value() );
		}
		type::Object* ShortCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}
		type::Object* ShortIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new ShortInstance( static_cast<short_t>( arg1->value() ) );
		}

		type::Object* ShortCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* ShortPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* ShortMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* ShortMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* ShortDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			if( arg2->value() == 0 )
				throw DivisionByZero();
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}

		type::Object* ShortElementIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new ShortInstance();
			rval->value( arg1->value() & ( static_cast<short_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}
		type::Object* ShortElementInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new ShortInstance();
			rval->value( arg1->value() & ( static_cast<short_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}

		type::Object* ShortSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto arg3 = _operand<IntInstance>( scope, action_node, 2 );
			auto rval = new ShortInstance();
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

		type::Object* ShortBitNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto rval = new ShortInstance();
			rval->value( ~arg1->value() );
			return rval;
		}
		type::Object* ShortBitAnd::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() & arg2->value() );
			return rval;
		}
		type::Object* ShortBitOr::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() | arg2->value() );
			return rval;
		}
		type::Object* ShortBitXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ShortInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() ^ arg2->value() );
			return rval;
		}

		type::Object* ShortLShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() << arg2->value() );
			return rval;
		}
		type::Object* ShortRShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new ShortInstance();
			rval->value( arg1->value() >> arg2->value() );
			return rval;
		}
	}
}
