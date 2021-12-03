#include "Name.h"


namespace eon
{
	static name NameObj;




	name_t name::get( string&& source ) noexcept
	{
		if( !valid( source ) )
			return no_name;
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		auto found = NameObj.NameLookup.find( &source );
		if( found != NameObj.NameLookup.end() )
		{
			source.clear();	// For consistency!
			return *found;
		}
		NameObj.Names.push_back( new string( std::move( source ) ) );
		auto id = static_cast<name_t>(
			NameObj.Names[ NameObj.Names.size() - 1 ] );
		NameObj.NameLookup.insert( id );
		return id;
	}
	bool name::valid( const string& source ) noexcept
	{
		if( source.empty() )
			return false;
		size_t non_numerals = 0;
		for( auto c : source )
		{
			if( string::isLetter( c ) || c == '_' )
				++non_numerals;
			else if( !string::isNumber( c ) )
				return false;
		}
		return non_numerals > 0;
	}

	name_t name::compilerGet( string&& source ) noexcept
	{
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		auto found = NameObj.NameLookup.find( &source );
		if( found != NameObj.NameLookup.end() )
		{
			source.clear();	// For consistency!
			return *found;
		}
		NameObj.Names.push_back( new string( std::move( source ) ) );
		auto id = static_cast<name_t>(
			NameObj.Names[ NameObj.Names.size() - 1 ] );
		NameObj.NameLookup.insert( id );
		return id;
	}




	name::~name()
	{
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		for( auto elm : Names )
		{
			if( elm )
				delete elm;
		}
	}




	const name_t name_undef = name::get( "undef" );
	const name_t name_bool = name::get( "bool" );
	const name_t name_byte = name::get( "byte" );
	const name_t name_char = name::get( "char" );
	const name_t name_int = name::get( "int" );
	const name_t name_short = name::get( "short" );
	const name_t name_long = name::get( "long" );
	const name_t name_float = name::get( "float" );
	const name_t name_low = name::get( "low" );
	const name_t name_high = name::get( "high" );
	const name_t name_index = name::get( "index" );
	const name_t name_name = name::get( "name" );
	const name_t name_handle = name::get( "handle" );
	const name_t name_type = name::get( "type" );
	const name_t name_tuple = name::get( "tuple" );
	const name_t name_basic = name::get( "basic" );
	const name_t name_plain = name::get( "plain" );
	const name_t name_dynamic = name::get( "dynamic" );
	const name_t name_bytes = name::get( "bytes" );
	const name_t name_string = name::get( "string" );
	const name_t name_binary = name::get( "binary" );
	const name_t name_regex = name::get( "regex" );
	const name_t name_namepath = name::get( "namepath" );
	const name_t name_path = name::get( "path" );
	const name_t name_array = name::get( "array" );
	const name_t name_list = name::get( "list" );
	const name_t name_set = name::get( "set" );
	const name_t name_heap = name::get( "heap" );
	const name_t name_me = name::get( "me" );
	const name_t name_true = name::get( "true" );
	const name_t name_false = name::get( "false" );
	const name_t name_complete = name::get( "complete" );
	const name_t name_key = name::get( "key" );
	const name_t name_value = name::get( "value" );
	const name_t name_raw = name::get( "raw" );
	const name_t name_meta = name::get( "meta" );
	const name_t name_typetuple = name::get( "typetuple" );
	const name_t name_function = name::get( "function" );
	const name_t name_functions = name::get( "functions" );
	const name_t name_reference = name::get( "reference" );
	const name_t name_variable = name::get( "variable" );
	const name_t name_var = name::get( "var" );
	const name_t name_expression = name::get( "expression" );
	const name_t name_operator = name::get( "operator" );
	const name_t name_operand = name::get( "operand" );
	const name_t name_syntax = name::get( "syntax" );
	const name_t name_action = name::get( "action" );
	const name_t name_docs = name::get( "docs" );
	const name_t name_test = name::get( "test" );
	const name_t name_title = name::get( "title" );
	const name_t name_pattern = name::get( "pattern" );
	const name_t name_optional = name::get( "optional" );
	const name_t name_yes = name::get( "yes" );
	const name_t name_no = name::get( "no" );
	const name_t name_on = name::get( "on" );
	const name_t name_off = name::get( "off" );
	const name_t name_positive = name::get( "positive" );
	const name_t name_negative = name::get( "negative" );
	const name_t name_ok = name::get( "ok" );
	const name_t name_cancel = name::get( "cancel" );
	const name_t name_active = name::get( "active" );
	const name_t name_inactive = name::get( "inactive" );
	const name_t name_success = name::get( "success" );
	const name_t name_failure = name::get( "failure" );
	const name_t name_error = name::get( "error" );
	const name_t name_abort = name::get( "abort" );
	const name_t name_output = name::get( "output" );
	const name_t name_token = name::get( "token" );
	const name_t name_scan = name::get( "scan" );
	const name_t name_while = name::get( "while" );
	const name_t name_loop = name::get( "loop" );
	const name_t name_match = name::get( "match" );
	const name_t name_condition = name::get( "condition" );
	const name_t name_id = name::get( "id" );
	const name_t name_include = name::get( "include" );
	const name_t name_structure = name::get( "structure" );
	const name_t name_locate = name::get( "locate" );
	const name_t name_section = name::get( "section" );
	const name_t name_if = name::get( "if" );
	const name_t name_skip = name::get( "skip" );
	const name_t name_space = name::get( "space" );
	const name_t name_tab = name::get( "tab" );
	const name_t name_letters = name::get( "letters" );
	const name_t name_digits = name::get( "digits" );
	const name_t name_source = name::get( "source" );
	const name_t name_target = name::get( "target" );
	const name_t name_args = name::get( "args" );
	const name_t name_global = name::get( "global" );
	const name_t name_newline = name::get( "newline" );
	const name_t name_instance = name::get( "instance" );
	const name_t name_definition = name::get( "definition" );
	const name_t name_enum = name::get( "enum" );
	const name_t name_cmp = name::get( "cmp" );
	const name_t name_not = name::get( "not" );
	const name_t name_and = name::get( "and" );
	const name_t name_or = name::get( "or" );
	const name_t name_xor = name::get( "xor" );
	const name_t name_plus = name::get( "plus" );
	const name_t name_minus = name::get( "minus" );
	const name_t name_multiply = name::get( "multiply" );
	const name_t name_divide = name::get( "divide" );
	const name_t name_element = name::get( "element" );
	const name_t name_slice = name::get( "slice" );
	const name_t name_bitnot = name::get( "bitnot" );
	const name_t name_bitand = name::get( "bitand" );
	const name_t name_bitor = name::get( "bitor" );
	const name_t name_bitxor = name::get( "bitxor" );
	const name_t name_lshift = name::get( "lshift" );
	const name_t name_rshift = name::get( "rshift" );
	const name_t name_construct = name::get( "construct" );
	const name_t name_copyconstruct = name::get( "copyconstruct" );
	const name_t name_moveconstruct = name::get( "moveconstruct" );
	const name_t name_destruct = name::get( "destruct" );
	const name_t name_other = name::get( "other" );
	const name_t name_memory_denied = name::get( "memory_denied" );
	const name_t name_pos = name::get( "pos" );
	const name_t name_underscore = name::get( "underscore" );
	const name_t name_open = name::get( "open" );
	const name_t name_close = name::get( "close" );
	const name_t name_open_square = name::get( "open_square" );
	const name_t name_close_square = name::get( "close_square" );
	const name_t name_open_curly = name::get( "open_curly" );
	const name_t name_close_curly = name::get( "close_curly" );
	const name_t name_open_angle = name::get( "open_angle" );
	const name_t name_close_angle = name::get( "close_angle" );
	const name_t name_backslash = name::get( "backslash" );
	const name_t name_member = name::get( "member" );
	const name_t name_doublequote = name::get( "doublequote" );
	const name_t name_singlequote = name::get( "singlequote" );
	const name_t name_point = name::get( "point" );
	const name_t name_symbol = name::get( "symbol" );
	const name_t name_questionmark = name::get( "questionmark" );
	const name_t name_hash = name::get( "hash" );
	const name_t name_at = name::get( "at" );
	const name_t name_indentation = name::get( "indentation" );
	const name_t name_group = name::get( "group" );
	const name_t name_lambda = name::get( "lambda" );
	const name_t name_state = name::get( "state" );
	const name_t name_return = name::get( "return" );
	const name_t name_data = name::get( "data" );
	const name_t name_shared = name::get( "shared" );
}
