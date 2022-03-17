#include "StringActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Floatingpt.h"
#include "Index.h"


namespace eon
{
	void registerStringActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::StringConstruct() );
		scope.addAction( name_constructor, new actions::StringCopyConstruct() );
		scope.addAction( name_constructor, new actions::StringBoolConstruct() );
		scope.addAction( name_constructor, new actions::StringByteConstruct() );
		scope.addAction( name_constructor, new actions::StringCharConstruct() );
		scope.addAction( name_constructor, new actions::StringIntConstruct() );
		scope.addAction( name_constructor, new actions::StringShortConstruct() );
		scope.addAction( name_constructor, new actions::StringLongConstruct() );
		scope.addAction( name_constructor, new actions::StringFloatConstruct() );
		scope.addAction( name_constructor, new actions::StringLowConstruct() );
		scope.addAction( name_constructor, new actions::StringHighConstruct() );
		scope.addAction( name_constructor, new actions::StringIndexConstruct() );
		scope.addAction( name_constructor, new actions::StringNameConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::StringCmp() );
		scope.addOperator( type::operators::code::plus, new actions::StringPlus() );
		scope.addOperator( type::operators::code::minus, new actions::StringMinusString() );
		scope.addOperator( type::operators::code::multiply, new actions::StringMultiplyInt() );
		scope.addOperator( type::operators::code::multiply, new actions::StringMultiplyIndex() );
	}




	namespace actions
	{
		type::Object* StringConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new StringInstance(); }
		type::Object* StringCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new StringInstance( arg1->value(), arg1->source() );
		}

		type::Object* StringBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<int_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<short_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<long_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<flt_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<low_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<high_t>>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ), arg1->source() );
		}
		type::Object* StringNameConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NameInstance>( scope, action_node, 0 );
			return new StringInstance( eon::str( arg1->value() ), arg1->source() );
		}

		type::Object* StringCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<StringInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* StringPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<StringInstance>( scope, action_node, 1 );
			auto rval = new StringInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* StringMinusString::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<StringInstance>( scope, action_node, 1 );
			auto rval = new StringInstance();
			rval->value( arg1->value().remove( arg2->value() ) );
			return rval;
		}
		type::Object* StringMultiplyInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
			auto rval = new StringInstance();
			if( arg2->value() > 1 )
				rval->value( string( static_cast<size_t>( arg2->value() ), arg1->value() ) );
			return rval;
		}
		type::Object* StringMultiplyIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			auto rval = new StringInstance();
			if( arg2->value() > 1 )
				rval->value( string( arg2->value(), arg1->value() ) );
			return rval;
		}
	}
}
