#include "Index.h"
#include "IndexActions.h"


namespace eon
{
	void registerIndex( scope::Global& scope )
	{
		scope.add( name_index, new IndexType() );
		registerIndexActions( scope );
	}




	type::Instance* IndexType::instantiate( index_t value ) const { return new IndexInstance( value, source::Ref() ); }
}
