#include "Insert.h"


namespace eon
{
	namespace doc
	{
		string mapInsType( instype type )
		{
			switch( type )
			{
				case instype::example_t:
					return "example";
				case instype::code_t:
					return "code";
				case instype::quote_t:
					return "quote";
				case instype::insert_t:
					return "insert";
				default:
					return "undef";
			}
		}




		string insert::str( size_t max_line_len, size_t indent ) const
		{
			string s{ "  --" };
			s += mapInsType( InsType );
			if( !TypeInfo.empty() )
				s += " " + TypeInfo;
			if( !Heading.empty() )
				s += ", " + Heading;
			s += anchor( " ", "" );
			s += "-->";
			if( Lines.size() == 1
				&& Lines[ 0 ].numChars() + s.numChars() + 3 <= 79 )
				s += Lines[ 0 ] + "<--\n";
			else
			{
				for( auto& line : Lines )
				{
					if( line.empty() )
						s += "\n";
					else
						s += "\n  " + line;
				}
				if( !s.endsWith( NewlineChr ) )
					s += "\n";
				s += "  <--\n";
			}
			return s;
		}
	}
}
