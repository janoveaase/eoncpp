#include "Table.h"


namespace eon
{
	namespace doc
	{
		void table::add( const elementptr& element, size_t col, size_t row )
		{
			auto elm_pos = Elms.size();
			Elms.push_back( element );
			Cells[ col ][ row ] = Cell( elm_pos, col, row );
			while( Cols.size() <= col )
				Cols.push_back( Column() );
			while( Rows.size() <= row )
				Rows.push_back( Row() );
		}
	}
}
