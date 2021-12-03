#include "StringActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerStringActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::StringConstruct() );
		scope.add( name_copyconstruct, new actions::StringCopyConstruct( scope ) );
		scope.add( name_construct, new actions::StringBoolConstruct( scope ) );
		scope.add( name_construct, new actions::StringByteConstruct( scope ) );
		scope.add( name_construct, new actions::StringCharConstruct( scope ) );
		scope.add( name_construct, new actions::StringIntConstruct( scope ) );
		scope.add( name_construct, new actions::StringShortConstruct( scope ) );
		scope.add( name_construct, new actions::StringLongConstruct( scope ) );
		scope.add( name_construct, new actions::StringFloatConstruct( scope ) );
		scope.add( name_construct, new actions::StringLowConstruct( scope ) );
		scope.add( name_construct, new actions::StringHighConstruct( scope ) );
		scope.add( name_construct, new actions::StringIndexConstruct( scope ) );
		scope.add( name_cmp, new actions::StringCmp( scope ) );
		scope.add( name_construct, new actions::StringPlus( scope ) );
		scope.add( name_construct, new actions::StringMinusString( scope ) );
		scope.add( name_construct, new actions::StringMultiplyInt( scope ) );
		scope.add( name_construct, new actions::StringMultiplyIndex( scope ) );
	}




	namespace actions
	{
		type::Object* StringConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new StringInstance(); }
		type::Object* StringCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new StringInstance( arg1->value() );
		}

		type::Object* StringBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}
		type::Object* StringIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new StringInstance( string( arg1->value() ) );
		}

		type::Object* StringCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			auto arg2 = _operand<StringInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
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
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
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
