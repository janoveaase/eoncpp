#include "BinaryValue.h"


namespace eon
{
	namespace tup
	{
		string binaryval::str( size_t& pos_on_line, size_t indentation_level,
			perm format ) const noexcept
		{
			bool oneliner = static_cast<bool>( format & perm::allow_oneliner );
			bool multiliner = static_cast<bool>(
				format & perm::allow_multiliner );
			std::string s{ "%" };
			++pos_on_line;
			if( !multiliner || ( Val.size() <= 79 - pos_on_line ) )
				return s + Val.value();
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
