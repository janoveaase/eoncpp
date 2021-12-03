#include "ArrayActions.h"


namespace eon
{
	namespace tup
	{
		void registerArrayActions( varscope& scope )
		{
			actionspec size;
			size.returnType( scope.tuple( tuplespec( { scope.primitive( name_position ) } ) ) );
			scope.registerAction( name_array, std::move( size ), new arrayact_size() );

			actionspec at_pos;
		}
	}
}
