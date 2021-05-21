#include "Value.h"


namespace eon
{
	namespace tup
	{
		name_t mapBasicType( basic_type type )
		{
			switch( type )
			{
				case basic_type::bool_t:
					return name_bool;
				case basic_type::char_t:
					return name_char;
				case basic_type::int_t:
					return name_int;
				case basic_type::float_t:
					return name_float;
				case basic_type::name_t:
					return name_name;
				case basic_type::string_t:
					return name_string;
				case basic_type::binary_t:
					return name_binary;
				case basic_type::raw_t:
					return name_raw;
				case basic_type::regex_t:
					return name_regex;
				case basic_type::ref_t:
					return name_reference;
				case basic_type::tuple_t:
					return name_tuple;
				case basic_type::meta_t:
					return name_meta;
				default:
					return name_undef;
			}
		}
	}
}
