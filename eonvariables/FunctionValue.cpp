#include "FunctionValue.h"
#include "ControlValue.h"


namespace eon
{
	namespace vars
	{
		valueptr functionval::execute( variables& varcache, const tuple& args ) const
		{
			valueptr return_value;
			varcache.push();
			auto params = this->args();
			if( params )
			{
				for( auto arg : *params )
				{
					auto param_name = arg->name();
					valueptr val;
					if( args.exists( param_name ) )
						val = args.at( param_name );
					else if( arg->pos() < args.numAttributes() )
						val = args.at( arg->pos() );
					else
						throw NotFound( "Missing '" + *param_name + "' argument in function call!" );
					varcache.setTop( param_name, val->target( varcache, val ) );
				}
			}
			for( auto& expr : Val )
			{
				auto result = expr->value()->actualExpression().evaluate( varcache );
				if( result && result->isControl() )
				{
					auto& ctrl = *dynamic_cast<controlval*>( &*result );
					if( ctrl.code() == ctrl_t::_return )
					{
						return_value = ctrl.val();
						break;
					}
				}
			}
			varcache.pop();
			return return_value;
		}
	}
}
