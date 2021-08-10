#include "OperandNode.h"
#include <eonvariables/ReferenceValue.h>


namespace eon
{
	namespace expr
	{
		void operandnode::setContext( vars::valueptr context )
		{
			if( Value->isReference() )
				dynamic_cast<vars::refval*>( &*Value )->context( context );
			else if( Value->isExpression() )
				Value->expression_value().setContext( context );
		}




		string operandnode::str() const
		{
			if( Value )
			{
				size_t pos{ 0 };
				return Value->str( pos, 0 );
			}
			else
				return "N/A";
		}

		vars::valueptr operandnode::evaluateAccurate( vars::variables& vars ) const
		{
			auto value = Value;
			while( value )
			{
				if( value->isVariable() )
					value = vars.getActual( value->actualVariable() );
				else if( value->isReference() )
					value = vars.getActual( value->actualReference(), dynamic_cast<vars::refval*>( &*value )->context() );
				else if( value->isExpression() )
					value = value->actualExpression().evaluate( vars );
				else
					break;
			}
			return value;
		}

		bool operandnode::equal( const nodeptr& other ) const noexcept
		{
			if( !other || other->isOperator() )
				return false;
			auto oval = dynamic_cast<operandnode*>( &*other )->Value;
			if( !Value && !oval )
				return true;
			if( !Value || !oval )
				return false;
			return Value->actualCompare( oval );
		}
	}
}
