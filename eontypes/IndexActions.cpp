#include "IndexActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"


namespace eon
{
	void registerIndexActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::IndexConstruct() );
		scope.add( name_construct, new actions::IndexCopyConstruct( scope ) );
		scope.add( name_construct, new actions::IndexBoolConstruct( scope ) );
		scope.add( name_construct, new actions::IndexByteConstruct( scope ) );
		scope.add( name_construct, new actions::IndexCharConstruct( scope ) );
		scope.add( name_construct, new actions::IndexIntConstruct( scope ) );
		scope.add( name_construct, new actions::IndexShortConstruct( scope ) );
		scope.add( name_construct, new actions::IndexLongConstruct( scope ) );
		scope.add( name_construct, new actions::IndexFloatConstruct( scope ) );
		scope.add( name_construct, new actions::IndexLowConstruct( scope ) );
		scope.add( name_construct, new actions::IndexHighConstruct( scope ) );
		scope.add( name_cmp, new actions::IndexCmp( scope ) );
		scope.add( name_plus, new actions::IndexPlus( scope ) );
		scope.add( name_minus, new actions::IndexMinus( scope ) );
		scope.add( name_multiply, new actions::IndexMultiply( scope ) );
		scope.add( name_divide, new actions::IndexDivide( scope ) );
		scope.add( name_element, new actions::IndexElementIndex( scope ) );
		scope.add( name_element, new actions::IndexElementInt( scope ) );
		scope.add( name_slice, new actions::IndexSlice( scope ) );
		scope.add( name_bitnot, new actions::IndexBitNot() );
		scope.add( name_bitand, new actions::IndexBitAnd( scope ) );
		scope.add( name_bitor, new actions::IndexBitOr( scope ) );
		scope.add( name_bitxor, new actions::IndexBitXor( scope ) );
		scope.add( name_lshift, new actions::IndexLShift( scope ) );
		scope.add( name_rshift, new actions::IndexRShift( scope ) );
	}




	namespace actions
	{
		type::Object* IndexConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new IndexInstance(); }
		type::Object* IndexCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}

		type::Object* IndexBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value() );
		}
		type::Object* IndexFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ) );
		}
		type::Object* IndexLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ) );
		}
		type::Object* IndexHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ) );
		}

		type::Object* IndexCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* IndexPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* IndexMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* IndexMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* IndexDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			if( arg2->value() == 0 )
				throw DivisionByZero();
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}

		type::Object* IndexElementIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new IndexInstance();
			rval->value( arg1->value() & ( static_cast<index_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}
		type::Object* IndexElementInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new IndexInstance();
			rval->value( arg1->value() & ( static_cast<index_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}

		type::Object* IndexSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto arg3 = _operand<IntInstance>( scope, action_node, 2 );
			auto rval = new IndexInstance();
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

		type::Object* IndexBitNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto rval = new IndexInstance();
			rval->value( ~arg1->value() );
			return rval;
		}
		type::Object* IndexBitAnd::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() & arg2->value() );
			return rval;
		}
		type::Object* IndexBitOr::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() | arg2->value() );
			return rval;
		}
		type::Object* IndexBitXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() ^ arg2->value() );
			return rval;
		}

		type::Object* IndexLShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() << arg2->value() );
			return rval;
		}
		type::Object* IndexRShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() >> arg2->value() );
			return rval;
		}
	}
}
