#pragma once
#include <eonstring/String.h>
#include <set>
#include <mutex>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon defines 'a name' as any sequence of letters (all [eon::charcat]
	  letter categories), numbers (all [eon::charcat] number categories) and
	  underscore. The only limitation is that the name cannot consist entirely
	  of numbers - there must be at least one letter or underscore. A name can
	  start with a number, though.

	  The 'eon::name_t' type is at core just a pointer to an [eon::string],
	  making it easy to "convert" a 'name' to a 'string'.

	  Every unique name is guaranteed to have the same pointer value, and so
	  any two names can be compared equal/not equal as a basic pointer compare.

	  WARNING: While comparing equal/not equal is both fast and trivial,
	           comparing less than/greater than requires [eon::string::compare]
			   to be involved! This is significantly more costly!

	  Use 'eon::name_t' in cases where alpha-numecial relations are
	  inconsequential. (I.e., where sorting names alpha-numerically is not
	  required.)

	  Uses: As keys in 'std::map', 'std::multimap', 'std::unordered_map',
	        'std::unordered_multimap', and as quick references etc.

	  NOTE: All use of 'name' functions is thread-safe!

	  WARNING: While the [eon::string] value of a name will be the same across
	           processes, the pointer value will not! Do not use the pointer
			   value when streaming/saving etc.!

	  NOTE: There is a small cost every time you create a new 'name' from a
	  'string' since the name has to be looked up and/or inserted into a
	  data structure.
	**************************************************************************/

	//* The defintion of 'eon::name_t'
	using name_t = const string*;

	//* Name 'null' value (invalid name)
	static const name_t no_name{ nullptr };


	//* Common names that can be accessed at no cost at any time
	//* The string value of each name is the latter part, after the underscore.
	extern const name_t name_undef;
	extern const name_t name_bool;
	extern const name_t name_byte;
	extern const name_t name_char;
	extern const name_t name_int;
	extern const name_t name_int8;
	extern const name_t name_int16;
	extern const name_t name_int32;
	extern const name_t name_int64;
	extern const name_t name_float;
	extern const name_t name_pos;
	extern const name_t name_name;
	extern const name_t name_type;
	extern const name_t name_tuple;
	extern const name_t name_bytes;
	extern const name_t name_string;
	extern const name_t name_binary;
	extern const name_t name_regex;
	extern const name_t name_true;
	extern const name_t name_false;
	extern const name_t name_complete;
	extern const name_t name_key;
	extern const name_t name_value;
	extern const name_t name_raw;
	extern const name_t name_meta;
	extern const name_t name_reference;
	extern const name_t name_variable;
	extern const name_t name_var;
	extern const name_t name_expression;
	extern const name_t name_docs;
	extern const name_t name_b;
	extern const name_t name_test;
	extern const name_t name_title;
	extern const name_t name_pattern;
	extern const name_t name_optional;
	extern const name_t name_no;
	extern const name_t name_off;
	extern const name_t name_negative;
	extern const name_t name_cancel;
	extern const name_t name_abort;
	extern const name_t name_output;
	extern const name_t name_token;
	extern const name_t name_scan;
	extern const name_t name_loop;
	extern const name_t name_while;
	extern const name_t name_match;
	extern const name_t name_condition;
	extern const name_t name_id;
	extern const name_t name_include;
	extern const name_t name_structure;
	extern const name_t name_locate;
	extern const name_t name_section;
	extern const name_t name_if;
	extern const name_t name_skip;
	extern const name_t name_space;



	/**************************************************************************
	  Eon Name Class - eon::name

	  This class keeps track of all the [eon::name_t] 'name's in use, and also
	  provides the functions needed to convert an [eon::string] to a 'name'.
	**************************************************************************/
	class name
	{
		/**********************************************************************
		  Name Functions
		**********************************************************************/
	public:

		//* Convert an [eon::string] into a name
		//* Returns the name, [eon::no_name] if the 'source' string is not a
		//* valid name.
		static inline name_t get( const string& source ) {
			return get( string( source ) ); }
		static inline name_t get( const std::string& name ) {
			return get( substring( name ) ); }
		static inline name_t get( const char* name ) {
			return get( substring( name ) ); }
		static name_t get( string&& source ) noexcept;

		//* Check that the [eon::name_t] value is a registered name
		//* Will return false on [eon::no_name]
		static bool registered( name_t value );

		//* Check if an [eon::string] is a valid name
		//* NOTE: [eon::name::get] will call this!
		static bool valid( const string& source ) noexcept;



		
		/**********************************************************************
		  Destruction of the name class deallocates all the stored names.
		**********************************************************************/
	public:
		~name();
	private:
		std::vector<name_t> Names;
		std::set<name_t, string_lt> NameLookup;
		std::mutex NamesLock;
	};
}
