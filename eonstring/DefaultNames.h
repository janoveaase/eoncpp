#pragma once

#include "NameDefs.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// This is a no-name value that is different from no_name in that it has an
	// actual value ("$NN"). Use this when a null value should not be used, but
	// you still want a no-name/undefined/to-be-decided-later type of name.
	extern const name_t name_nn;

	
	// Common names that can be accessed at no cost at any time
	// The string value of each name is the latter part, after the underscore.

	// Primitives
	extern const name_t name_bool;
	extern const name_t name_true;
	extern const name_t name_false;
	extern const name_t name_byte;
	extern const name_t name_char;
	extern const name_t name_int;
	extern const name_t name_short;
	extern const name_t name_long;
	extern const name_t name_float;
	extern const name_t name_low;
	extern const name_t name_high;
	extern const name_t name_index;
	extern const name_t name_name;
	extern const name_t name_handle;
	extern const name_t name_bits;
	extern const name_t name_b8;
	extern const name_t name_b16;
	extern const name_t name_b32;
	extern const name_t name_b64;

	// Basic types
	extern const name_t name_bytes;
	extern const name_t name_chars;
	extern const name_t name_string;
	extern const name_t name_stringchar;
	extern const name_t name_substring;
	extern const name_t name_real;
	extern const name_t name_complex;
	extern const name_t name_regex;
	extern const name_t name_namepath;
	extern const name_t name_path;
	extern const name_t name_time;
	extern const name_t name_date;
	extern const name_t name_timezone;
	extern const name_t name_dst;
	extern const name_t name_timepoint;
	extern const name_t name_timespan;
	extern const name_t name_timer;
	extern const name_t name_array;
	extern const name_t name_vector;
	extern const name_t name_list;
	extern const name_t name_set;
	extern const name_t name_heap;
	extern const name_t name_expression;

	// Tuples
	extern const name_t name_tuple;
	extern const name_t name_basic;
	extern const name_t name_static;
	extern const name_t name_optional;
	extern const name_t name_dynamic;
	extern const name_t name_data;
	extern const name_t name_category;
	extern const name_t name_args;
	extern const name_t name_scope;
	extern const name_t name_unit;
	extern const name_t name_cache;
	extern const name_t name_dottypes;

	// Actions
	extern const name_t name_action;
	extern const name_t name_constructor;
	extern const name_t name_destructor;
	extern const name_t name_get;
	extern const name_t name_firstarg;
	extern const name_t name_lastarg;
	extern const name_t name_operator;
	extern const name_t name_helper;
	extern const name_t name_type;
	extern const name_t name_typetuple;

	// Constructs
	extern const name_t name_loop;
	extern const name_t name_while;
	extern const name_t name_until;
	extern const name_t name_for;
	extern const name_t name_each;
	extern const name_t name_in;
	extern const name_t name_if;
	extern const name_t name_else;
	extern const name_t name_where;
	extern const name_t name_as;

	// Operator names
	extern const name_t name_not;
	extern const name_t name_and;
	extern const name_t name_or;
	extern const name_t name_xor;
	extern const name_t name_plus;
	extern const name_t name_minus;
	extern const name_t name_multiply;
	extern const name_t name_divide;
	extern const name_t name_element;
	extern const name_t name_continue;
	extern const name_t name_break;
	extern const name_t name_call;
	extern const name_t name_bumpcall;
	extern const name_t name_raise;
	extern const name_t name_uid;
	extern const name_t name_is;
	extern const name_t name_size;
	extern const name_t name_of;
	extern const name_t name_swap;
	extern const name_t name_with;
	extern const name_t name_find;
	extern const name_t name_assign;
	extern const name_t name_member;
	extern const name_t name_length;
	extern const name_t name_match;
	extern const name_t name_return;

	// Exceptions
	extern const name_t name_memory_denied;
	extern const name_t name_not_found;

	// Styles
	extern const name_t name_normal;
	extern const name_t name_strong;
	extern const name_t name_weak;
	extern const name_t name_red;
	extern const name_t name_green;
	extern const name_t name_yellow;
	extern const name_t name_blue;
	extern const name_t name_magenta;
	extern const name_t name_cyan;
	extern const name_t name_dark;
	extern const name_t name_black;
	extern const name_t name_bright;
	extern const name_t name_white;
	extern const name_t name_note;
	extern const name_t name_warning;
	extern const name_t name_quote;
	extern const name_t name_code;

	// Permissions
	extern const name_t name_read;
	extern const name_t name_modify;
	extern const name_t name_take;
	extern const name_t name_give;

	// Parsing
	extern const name_t name_syntax;
	extern const name_t name_token;
	extern const name_t name_scan;
	extern const name_t name_skip;
	extern const name_t name_space;
	extern const name_t name_tab;
	extern const name_t name_letters;
	extern const name_t name_digits;
	extern const name_t name_underscore;
	extern const name_t name_brace;
	extern const name_t name_open;
	extern const name_t name_close;
	extern const name_t name_open_square;
	extern const name_t name_close_square;
	extern const name_t name_open_curly;
	extern const name_t name_close_curly;
	extern const name_t name_open_angle;
	extern const name_t name_close_angle;
	extern const name_t name_slash;
	extern const name_t name_backslash;
	extern const name_t name_doublequote;
	extern const name_t name_singlequote;
	extern const name_t name_point;
	extern const name_t name_colon;
	extern const name_t name_comma;
	extern const name_t name_symbol;
	extern const name_t name_questionmark;
	extern const name_t name_hash;
	extern const name_t name_at;
	extern const name_t name_indentation;
	extern const name_t name_undef;
	extern const name_t name_newline;
	extern const name_t name_literal;
	extern const name_t name_comment;
	extern const name_t name_define;

	/*
	extern const name_t name_condition;
	extern const name_t name_id;
	extern const name_t name_include;
	extern const name_t name_structure;
	extern const name_t name_locate;
	extern const name_t name_section;

	extern const name_t name_binary;
	extern const name_t name_me;
	extern const name_t name_complete;
	extern const name_t name_key;
	extern const name_t name_value;
	extern const name_t name_raw;
	extern const name_t name_function;
	extern const name_t name_functions;
	extern const name_t name_reference;
	extern const name_t name_variable;
	extern const name_t name_var;
	extern const name_t name_operand;
	extern const name_t name_docs;
	extern const name_t name_test;
	extern const name_t name_title;
	extern const name_t name_pattern;
	extern const name_t name_optional;
	extern const name_t name_yes;
	extern const name_t name_no;
	extern const name_t name_on;
	extern const name_t name_off;
	extern const name_t name_positive;
	extern const name_t name_negative;
	extern const name_t name_ok;
	extern const name_t name_cancel;
	extern const name_t name_active;
	extern const name_t name_inactive;
	extern const name_t name_success;
	extern const name_t name_failure;
	extern const name_t name_error;
	extern const name_t name_abort;
	extern const name_t name_output;
	extern const name_t name_source;
	extern const name_t name_target;
	extern const name_t name_global;
	extern const name_t name_instance;
	extern const name_t name_definition;
	extern const name_t name_enum;
	extern const name_t name_other;
	extern const name_t name_pos;
	extern const name_t name_group;
	extern const name_t name_lambda;
	extern const name_t name_state;
	extern const name_t name_shared;
	extern const name_t name_ellipsis;
	extern const name_t name_locked;
	extern const name_t name_static;
	extern const name_t name_protected;
	extern const name_t name_restricted;
	extern const name_t name_local;
	extern const name_t name_as;
	extern const name_t name_defvar;
	extern const name_t name_posix;
	extern const name_t name_special;
	extern const name_t name_details;
	extern const name_t name_from;
	extern const name_t name_to;
	*/


	// Macro for definining common names (prefixed by 'name_') in other places
#define EON_NAME( new_name ) const name_t name_##new_name{ eon::name( #new_name ) }

	// Macro for defining common compiler names (prefixed by 'name_') in other places
#define EON_COMPILERNAME( new_name, text ) const name_t name_##new_name{ eon::compilerName( text ) }

	// Macro for defining common compiler names (prefixed by 'symbol_') in other places
#define EON_SYMBOL( symb_name, symbol ) const name_t symbol_##symb_name{ eon::compilerName( symbol ) }
}
