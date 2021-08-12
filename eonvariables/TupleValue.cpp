#include "TupleValue.h"


namespace eon
{
	namespace vars
	{
		string tupleval::str( size_t& pos_on_line, size_t indentation_level ) const noexcept
		{
			if( Val.empty() )
			{
				pos_on_line += 2;
				return "{}";
			}
			else
			{
				if( Val.braced() )
					++pos_on_line;
				auto s = Val.str( pos_on_line, indentation_level + 1 );
				if( Val.braced() )
					return "{" + s + "}";
				else
					return s;
			}
		}


		int tupleval::actualCompare( const valueptr& other ) const
		{
			if( other->isTuple() )
			{
				auto& o = other->actualTuple();
				if( Val.equal( o ) )
					return 0;

				// NOTE: We can only compare equal and not equal, relational
				//       comparison has an unpredictable result
				//       We use object address!
				return &Val < &o ? -1 : 1;
			}
			else
				return value::actualCompare( other );
		}
	}
}
