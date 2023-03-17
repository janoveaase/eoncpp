#include "TupleParserBase.h"


namespace eon
{
	namespace parser
	{
		name_t TupleParserBase::_parseAttributeName( State& state )
		{
			name_t attrib_name = no_name;
			if( state.Tokens.viewed().is( name_name )
				&& state.Tokens.exists()
				&& state.Tokens.peekAhead().type() == symbol_assign )
			{
				attrib_name = eon::name( state.Tokens.viewed().str() );
				state.Tokens.forward( 2 );
			}
			return attrib_name;
		}
	}
}