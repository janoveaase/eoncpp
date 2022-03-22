#include "IndexActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Floatingpt.h"


namespace eon
{
	void registerIndexActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::IndexConstruct() );
		scope.addAction( name_constructor, new actions::IndexCopyConstruct() );
		scope.addAction( name_constructor, new actions::IndexBoolConstruct() );
		scope.addAction( name_constructor, new actions::IndexByteConstruct() );
		scope.addAction( name_constructor, new actions::IndexCharConstruct() );
		scope.addAction( name_constructor, new actions::IndexIntConstruct() );
		scope.addAction( name_constructor, new actions::IndexShortConstruct() );
		scope.addAction( name_constructor, new actions::IndexLongConstruct() );
		scope.addAction( name_constructor, new actions::IndexFloatConstruct() );
		scope.addAction( name_constructor, new actions::IndexLowConstruct() );
		scope.addAction( name_constructor, new actions::IndexHighConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::IndexCmp() );
		scope.addOperator( type::operators::code::plus, new actions::IndexPlus() );
		scope.addOperator( type::operators::code::minus, new actions::IndexMinus() );
		scope.addOperator( type::operators::code::multiply, new actions::IndexMultiply() );
		scope.addOperator( type::operators::code::divide, new actions::IndexDivide() );
		scope.addOperator( type::operators::code::element, new actions::IndexElementIndex() );
		scope.addOperator( type::operators::code::element, new actions::IndexElementInt() );
		scope.addOperator( type::operators::code::slice, new actions::IndexSlice() );
		scope.addOperator( type::operators::code::bit_not, new actions::IndexBitNot() );
		scope.addOperator( type::operators::code::bit_and, new actions::IndexBitAnd() );
		scope.addOperator( type::operators::code::bit_or, new actions::IndexBitOr() );
		scope.addOperator( type::operators::code::bit_xor, new actions::IndexBitXor() );
		scope.addOperator( type::operators::code::bit_lsh, new actions::IndexLShift() );
		scope.addOperator( type::operators::code::bit_rsh, new actions::IndexRShift() );
	}




	namespace actions
	{
		type::Object* IndexConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new IndexInstance(); }
		type::Object* IndexCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}

		type::Object* IndexBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<int_t>>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<short_t>>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<long_t>>( scope, action_node, 0 );
			return new IndexInstance( arg1->value(), arg1->source() );
		}
		type::Object* IndexFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<flt_t>>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ), arg1->source() );
		}
		type::Object* IndexLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<low_t>>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ), arg1->source() );
		}
		type::Object* IndexHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<high_t>>( scope, action_node, 0 );
			return new IndexInstance( static_cast<index_t>( arg1->value() ), arg1->source() );
		}

		type::Object* IndexCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
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
			auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
			// TODO: Verify that arg2 is in range!
			auto rval = new IndexInstance();
			rval->value( arg1->value() & ( static_cast<index_t>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
			return rval;
		}

		type::Object* IndexSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
			auto arg3 = _operand<IntegerInstance<int_t>>( scope, action_node, 2 );
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
			auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() << arg2->value() );
			return rval;
		}
		type::Object* IndexRShift::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
			auto rval = new IndexInstance();
			rval->value( arg1->value() >> arg2->value() );
			return rval;
		}
	}
}
