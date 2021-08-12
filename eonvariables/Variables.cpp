#include "Variables.h"
#include "BoolValue.h"
#include "Function.h"


namespace eon
{
	namespace vars
	{
		valueptr variables::get( name_t name ) const noexcept
		{
			auto val = getActual( name );
			return val ? val : valueptr( new boolval( false ) );
		}
		valueptr variables::getActual( name_t name ) const noexcept
		{
			for( auto& scope : VarStack )
			{
				auto found = scope.find( name );
				if( found != scope.end() )
					return found->second;
			}
			return valueptr();
		}

		void variables::set( name_t name, const valueptr& value )
		{
			for( auto& scope : VarStack )
			{
				auto found = scope.find( name );
				if( found != scope.end() )
				{
					found->second = value;
					return;
				}
			}
			setTop( name, value );
		}



		valueptr variables::get( const nameref& name, valueptr context ) const noexcept
		{
			auto val = getActual( name, context );
			return val ? val : valueptr( new boolval( false ) );
		}
		valueptr variables::getActual( const nameref& name, valueptr context ) const noexcept
		{
			if( RefSource != nullptr )
			{
				auto val = RefSource->_get( name, context );
				if( val )
					return val;
			}
			return valueptr();
		}




		function* variables::getFunction( const nameref& name )
		{
			auto found = Functions.find( name );
			return found != Functions.end() ? &*found->second : nullptr;
		}

		void variables::setFunction( const nameref& name, functionptr func ) {
			Functions.insert( std::make_pair( name, std::move( func ) ) ); }
	}
}
