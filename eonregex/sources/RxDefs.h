#pragma once
#include <vector>
#include <eonexcept/Exception.h>



namespace eon
{
	namespace rx
	{
		enum class NodeType
		{
			undef,
			op_or,
			op_not,
			node_group,
			capt_group,
			loc_start,
			loc_end,
			loc_wordstart,
			loc_wordend,
			val_fixed,
			val_chargroup,
			val_backslash,
			val_dquote,
			val_any,
			val_wordchar,
			val_lower,
			val_upper,
			val_digit,
			val_space,
			val_punctuation,
			val_escaped,
			val_backref
		};

		static const size_t nsize{ SIZE_MAX };

		enum class Flag : char
		{
			none = 0x00,
			icase = 0x01,
			lines = 0x02,
			speed = 0x04,
			accuracy = 0x08,
			optimize = 0x10
		};

		inline bool operator&( Flag a, Flag b ) noexcept {
			return static_cast<int>( a ) & static_cast<int>( b ); }
		inline Flag& operator|=( Flag& a, Flag b ) noexcept {
			*(int*)&a |= static_cast<int>( b ); return a; }
		inline Flag operator|( Flag a, Flag b ) noexcept {
			return static_cast<Flag>( static_cast<int>( a )
				| static_cast<int>( b ) ); }


		/**********************************************************************
		  Exception used when a regular expression is badly formatted
		**********************************************************************/
		EONEXCEPT( InvalidExpression );
	}
}
