#include "BinaryValue.h"


namespace eon
{
	namespace vars
	{
		string binaryval::str( size_t& pos_on_line, size_t indentation_level ) const noexcept
		{
			std::string s{ "%" };
			++pos_on_line;
			if( pos_on_line + Val.size() <= 79 )
			{
				s += Val.value();
				pos_on_line += Val.size();
				return s;
			}
			s += " ";
			++pos_on_line;
			auto indent = pos_on_line;
			auto linew = 79 - pos_on_line;
			if( linew % 2 != 0 )
				--linew;
			auto remaining = Val.size();
			for( size_t i = 0; remaining > 0; )
			{
				if( s.size() > 2 )
					s += "\n" + std::string( indent, ' ' );
				auto sz = remaining >= linew ? linew : remaining;
				s += Val.value().substr( i, sz );
				i += sz;
				remaining -= sz;
			}
			return s;
		}
	}
}
