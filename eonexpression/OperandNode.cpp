#include "OperandNode.h"


namespace eon
{
	namespace expr
	{
		string operandnode::str() const
		{
			if( Value )
			{
				size_t pos{ 0 };
				return Value->str( pos, 0, tup::perm::allow_oneliner );
			}
			else
				return "N/A";
		}

		tup::valueptr operandnode::evaluate( tup::variables& vars ) const
		{
			auto value = Value;
			while( value && value->isVar() )
				value = vars.get( value->hardVar() );;
			return value;
		}

		bool operandnode::equal( const nodeptr& other,
			const tup::variables& vars ) const noexcept
		{
			if( !other || other->isOperator() )
				return false;
			auto oval = dynamic_cast<operandnode*>( &*other )->Value;
			if( !Value && !oval )
				return true;
			if( !Value || !oval )
				return false;
			return Value->hardCompare( oval );
		}
	}
}
