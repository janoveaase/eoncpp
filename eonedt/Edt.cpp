#include "Edt.h"


namespace eon
{
	DataTuple edt::fromStr( const string& str )
	{
		return DataTuple();
	}

	bool edt::validate( const DataTuple& target, const DataTuple& source, DataTuple* issues )
	{
		return false;
	}




	void edt::_toStr( const DataTuple& tuple, string& str, int_t indentation_level, bool unnamed_tuple )
	{
		string indentation( indentation_level * 2, ' ' );
		for( index_t i = 0; i < tuple.numAttributes(); ++i )
		{
			auto& attribute = tuple[ i ];
			if( !unnamed_tuple )
				str << indentation;
			if( attribute.name() != no_name )
			{
				str << eon::str( attribute.name() );
				if( attribute.type().name() == name_data )
					str << ":\n";
				else
					str << " = ";
			}
			if( attribute.type().name() == name_data )
			{
				if( attribute.name() == no_name )
				{
					str << "(";
					_toStr( *(DataTuple*)attribute.value(), str, indentation_level + 1, true );
					str << ")\n";
				}
				else
					_toStr( *(DataTuple*)attribute.value(), str, indentation_level + 1, false );
			}
			else
			{
				type::Stringifier strf;
				attribute.value()->str( strf );
				str << strf.output();
				if( unnamed_tuple )
				{
					if( i < tuple.numAttributes() - 1 )
						str << ", ";
				}
				else
					str << "\n";
			}
		}
	}
}
