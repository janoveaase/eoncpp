#include "DefaultNames.h"
#include "Name.h"


namespace eon
{
	const name_t name_nn = compilerName( "$NN" );

	const name_t name_undef = name( "undef" );
	const name_t name_bool = name( "bool" );
	const name_t name_byte = name( "byte" );
	const name_t name_char = name( "char" );
	const name_t name_int = name( "int" );
	const name_t name_short = name( "short" );
	const name_t name_long = name( "long" );
	const name_t name_float = name( "float" );
	const name_t name_low = name( "low" );
	const name_t name_high = name( "high" );
	const name_t name_index = name( "index" );
	const name_t name_name = name( "name" );
	const name_t name_handle = name( "handle" );
	const name_t name_type = name( "type" );
	const name_t name_tuple = name( "tuple" );
	const name_t name_basic = name( "basic" );
	const name_t name_plain = name( "plain" );
	const name_t name_dynamic = name( "dynamic" );
	const name_t name_bits = name( "bits" );
	const name_t name_bytes = name( "bytes" );
	const name_t name_chars = name( "chars" );
	const name_t name_string = name( "string" );
	const name_t name_binary = name( "binary" );
	const name_t name_regex = name( "regex" );
	const name_t name_namepath = name( "namepath" );
	const name_t name_path = name( "path" );
	const name_t name_array = name( "array" );
	const name_t name_list = name( "list" );
	const name_t name_set = name( "set" );
	const name_t name_heap = name( "heap" );
	const name_t name_me = name( "me" );
	const name_t name_true = name( "true" );
	const name_t name_false = name( "false" );
	const name_t name_complete = name( "complete" );
	const name_t name_key = name( "key" );
	const name_t name_value = name( "value" );
	const name_t name_raw = name( "raw" );
	const name_t name_typetuple = name( "typetuple" );
	const name_t name_function = name( "function" );
	const name_t name_functions = name( "functions" );
	const name_t name_reference = name( "reference" );
	const name_t name_variable = name( "variable" );
	const name_t name_var = name( "var" );
	const name_t name_expression = name( "expression" );
	const name_t name_operator = name( "operator" );
	const name_t name_operand = name( "operand" );
	const name_t name_syntax = name( "syntax" );
	const name_t name_action = name( "action" );
	const name_t name_docs = name( "docs" );
	const name_t name_test = name( "test" );
	const name_t name_title = name( "title" );
	const name_t name_pattern = name( "pattern" );
	const name_t name_optional = name( "optional" );
	const name_t name_yes = name( "yes" );
	const name_t name_no = name( "no" );
	const name_t name_on = name( "on" );
	const name_t name_off = name( "off" );
	const name_t name_positive = name( "positive" );
	const name_t name_negative = name( "negative" );
	const name_t name_ok = name( "ok" );
	const name_t name_cancel = name( "cancel" );
	const name_t name_active = name( "active" );
	const name_t name_inactive = name( "inactive" );
	const name_t name_success = name( "success" );
	const name_t name_failure = name( "failure" );
	const name_t name_error = name( "error" );
	const name_t name_abort = name( "abort" );
	const name_t name_output = name( "output" );
	const name_t name_token = name( "token" );
	const name_t name_scan = name( "scan" );
	const name_t name_while = name( "while" );
	const name_t name_loop = name( "loop" );
	const name_t name_match = name( "match" );
	const name_t name_condition = name( "condition" );
	const name_t name_id = name( "id" );
	const name_t name_include = name( "include" );
	const name_t name_structure = name( "structure" );
	const name_t name_locate = name( "locate" );
	const name_t name_section = name( "section" );
	const name_t name_if = name( "if" );
	const name_t name_skip = name( "skip" );
	const name_t name_space = name( "space" );
	const name_t name_tab = name( "tab" );
	const name_t name_letters = name( "letters" );
	const name_t name_digits = name( "digits" );
	const name_t name_source = name( "source" );
	const name_t name_target = name( "target" );
	const name_t name_args = name( "args" );
	const name_t name_global = name( "global" );
	const name_t name_newline = name( "newline" );
	const name_t name_instance = name( "instance" );
	const name_t name_definition = name( "definition" );
	const name_t name_enum = name( "enum" );
	const name_t name_cmp = name( "cmp" );
	const name_t name_not = name( "not" );
	const name_t name_and = name( "and" );
	const name_t name_or = name( "or" );
	const name_t name_xor = name( "xor" );
	const name_t name_plus = name( "plus" );
	const name_t name_minus = name( "minus" );
	const name_t name_multiply = name( "multiply" );
	const name_t name_divide = name( "divide" );
	const name_t name_element = name( "element" );
	const name_t name_slice = name( "slice" );
	const name_t name_bitnot = name( "bitnot" );
	const name_t name_bitand = name( "bitand" );
	const name_t name_bitor = name( "bitor" );
	const name_t name_bitxor = name( "bitxor" );
	const name_t name_lshift = name( "lshift" );
	const name_t name_rshift = name( "rshift" );
	const name_t name_constructor = name( "constructor" );
	const name_t name_destructor = name( "destructor" );
	const name_t name_other = name( "other" );
	const name_t name_memory_denied = name( "memory_denied" );
	const name_t name_pos = name( "pos" );
	const name_t name_underscore = name( "underscore" );
	const name_t name_brace = name( "brace" );
	const name_t name_open = name( "open" );
	const name_t name_close = name( "close" );
	const name_t name_open_square = name( "open_square" );
	const name_t name_close_square = name( "close_square" );
	const name_t name_open_curly = name( "open_curly" );
	const name_t name_close_curly = name( "close_curly" );
	const name_t name_open_angle = name( "open_angle" );
	const name_t name_close_angle = name( "close_angle" );
	const name_t name_backslash = name( "backslash" );
	const name_t name_member = name( "member" );
	const name_t name_doublequote = name( "doublequote" );
	const name_t name_singlequote = name( "singlequote" );
	const name_t name_point = name( "point" );
	const name_t name_symbol = name( "symbol" );
	const name_t name_questionmark = name( "questionmark" );
	const name_t name_hash = name( "hash" );
	const name_t name_at = name( "at" );
	const name_t name_indentation = name( "indentation" );
	const name_t name_group = name( "group" );
	const name_t name_lambda = name( "lambda" );
	const name_t name_state = name( "state" );
	const name_t name_return = name( "return" );
	const name_t name_data = name( "data" );
	const name_t name_shared = name( "shared" );
	const name_t name_normal = name( "normal" );
	const name_t name_strong = name( "strong" );
	const name_t name_weak = name( "weak" );
	const name_t name_red = name( "red" );
	const name_t name_green = name( "green" );
	const name_t name_yellow = name( "yellow" );
	const name_t name_blue = name( "blue" );
	const name_t name_magenta = name( "magenta" );
	const name_t name_cyan = name( "cyan" );
	const name_t name_dark = name( "dark" );
	const name_t name_black = name( "black" );
	const name_t name_bright = name( "bright" );
	const name_t name_white = name( "white" );
	const name_t name_note = name( "note" );
	const name_t name_warning = name( "warning" );
	const name_t name_quote = name( "quote" );
	const name_t name_code = name( "code" );
	const name_t name_ellipsis = name( "ellipsis" );
	const name_t name_locked = name( "locked" );
	const name_t name_static = name( "static" );
	const name_t name_protected = name( "protected" );
	const name_t name_restricted = name( "restricted" );
	const name_t name_local = name( "local" );
	const name_t name_read = name( "read" );
	const name_t name_modify = name( "modify" );
	const name_t name_take = name( "take" );
	const name_t name_give = name( "give" );
	const name_t name_in = name( "int" );
	const name_t name_for = name( "for" );
	const name_t name_until = name( "until" );
	const name_t name_uid = name( "uid" );
	const name_t name_is = name( "is" );
	const name_t name_size = name( "size" );
	const name_t name_swap = name( "swap" );
	const name_t name_find = name( "find" );
	const name_t name_as = name( "as" );
	const name_t name_continue = name( "continue" );
	const name_t name_break = name( "break" );
	const name_t name_call = name( "call" );
	const name_t name_bumpcall = name( "bumpcall" );
	const name_t name_raise = name( "raise" );
	const name_t name_literal = name( "literal" );
	const name_t name_scope = name( "scope" );
	const name_t name_defvar = name( "defvar" );
}
