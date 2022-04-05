#include "Edt.h"


namespace eon
{
	void edt::save( const DataTuple& tuple, const path& file )
	{
	}

	DataTuple edt::load( const path& file )
	{
		return DataTuple();
	}

	void edt::toStr( const DataTuple& tuple, std::string& str )
	{
	}
	DataTuple edt::fromStr( const std::string& str )
	{
		return DataTuple();
	}

	bool edt::validate( const DataTuple& target, const DataTuple& source, DataTuple* issues )
	{
		return false;
	}
}
