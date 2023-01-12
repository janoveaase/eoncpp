#pragma once

#include "String.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'stringify' namespace encloses internal declarations for Stringifier
	//
	namespace stringify
	{
		// Element types
		enum class Type
		{
			none,
			word,				// Basic "word", 1 space before and after - with exceptions
			doubleq,			// Double quoted string
			singleq,			// Single quoted string
			append,				// Append to previous element, no space between
			prepend,			// Prepend to next element, no space between
			op1,				// Operator type 1, 1 space before and after, can be overriden
			op2,				// Operator type 2, no space before or after, can be overridden
			punct,				// Punctuation, no space towards previous element, 1 space towards next
			start_grp1,			// Start a grouped section type 1       No space outside and 1
			end_grp1,			// End a grouped section type 1           inside, can be overridden.
			start_grp2,			// Start a grouped section type 2       No space outside and no
			end_grp2,			// End a gropued section type 2           inside, can be overridden.
			start_block,		// Start a block (indented set of lines)
			end_block,			// End a block (indentation back to normal)
			start_chapt,		// Start a "chapter", empty line before (except at start) and after (except at end)
			end_chapt,			// End a "chapter"
			hard_lf,			// Hard line-feed
			prim_split,			// Primary split point for long lines
			prim_split_block,	// Primary split point for long lines, make remainder an indented block
			sec_split,			// Secondary split point for long lines
			sec_split_block,	// Secondary split point for long lines, make remainder an indented block
			tert_split,			// Tertiary split point for long lines
			tert_split_block,	// Tertiary split point for long lines, make remainder an indented block
		};

		// Configuration
		struct Conf
		{
			bool spaceBefore( Type type ) const noexcept {
				return type == Type::word || type == Type::doubleq || type == Type::singleq || type == Type::prepend
					|| ( type == Type::op1 && SpacesBeforeAfterOp1 ) || ( type == Type::op2 && SpacesBeforeAfterOp2 )
					|| ( type == Type::start_grp1 && SpacesOutsideGrp1 ) || ( type == Type::end_grp1 && SpacesInsideGrp1 )
					|| ( type == Type::start_grp2 && SpacesOutsideGrp2 )
					|| ( type == Type::end_grp2 && SpacesInsideGrp2 ); }
			bool spaceAfter( Type type ) const noexcept {
				return type == Type::word || type == Type::doubleq || type == Type::singleq || type == Type::append
					|| type == Type::punct
					|| ( type == Type::op1 && SpacesBeforeAfterOp1 ) || ( type == Type::op2 && SpacesBeforeAfterOp2 )
					|| ( type == Type::start_grp1 && SpacesInsideGrp1 ) || ( type == Type::end_grp1 && SpacesOutsideGrp1 )
					|| ( type == Type::start_grp2 && SpacesInsideGrp2 )
					|| ( type == Type::end_grp2 && SpacesOutsideGrp2 ); }

			inline bool spaceBetween( Type t1, Type t2 ) const noexcept {
				return t1 == Type::punct && t2 != Type::end_block && t2 != Type::append && t2 != Type::prim_split
					&& t2 != Type::prim_split_block && t2 != Type::sec_split && t2 != Type::sec_split_block
					&& t2 != Type::tert_split && t2 != Type::tert_split_block ? true : spaceAfter( t1 )
					&& spaceBefore( t2 ); }

			bool SpacesBeforeAfterOp1{ true };
			bool SpacesBeforeAfterOp2{ false };

			bool SpacesOutsideGrp1{ true };
			bool SpacesInsideGrp1{ false };
			bool SpacesOutsideGrp2{ false };
			bool SpacesInsideGrp2{ false };

			// When a line exceeds the soft line width, primary split points are examined first, then secondary,
			// then tertiary. If a split point is found before the max line width, the line is split there.
			index_t SoftLineWidth{ 125 };

			// When a line exceeds the hard line width (even after soft splits), then cut the line at hard - 1 and add a
			// backslash to the end of the cut line.
			index_t HardLineWidth{ SIZE_MAX };
		};

		// Element class
		struct Element
		{
			Element() = default;
			inline Element( Type type ) noexcept { ElmType = type; }
			inline Element( Type type, string&& value ) noexcept { ElmType = type; Value = std::move( value ); }

			inline bool word() const noexcept { return ElmType == Type::word; }
			inline bool doubleq() const noexcept { return ElmType == Type::doubleq; }
			inline bool singleq() const noexcept { return ElmType == Type::singleq; }
			inline bool append() const noexcept { return ElmType == Type::append; }
			inline bool prepend() const noexcept { return ElmType == Type::prepend; }
			inline bool op1() const noexcept { return ElmType == Type::op1; }
			inline bool op2() const noexcept { return ElmType == Type::op2; }
			inline bool punct() const noexcept { return ElmType == Type::punct; }
			inline bool start_grp1() const noexcept { return ElmType == Type::start_grp1; }
			inline bool end_grp1() const noexcept { return ElmType == Type::end_grp1; }
			inline bool start_grp2() const noexcept { return ElmType == Type::start_grp2; }
			inline bool end_grp2() const noexcept { return ElmType == Type::end_grp2; }
			inline bool start_block() const noexcept { return ElmType == Type::start_block; }
			inline bool end_block() const noexcept { return ElmType == Type::end_block; }
			inline bool start_chapt() const noexcept { return ElmType == Type::start_chapt; }
			inline bool end_chapt() const noexcept { return ElmType == Type::end_chapt; }
			inline bool hard_lf() const noexcept { return ElmType == Type::hard_lf; }
			inline bool prim_split() const noexcept { return ElmType == Type::prim_split; }
			inline bool prim_split_block() const noexcept { return ElmType == Type::prim_split_block; }
			inline bool sec_split() const noexcept { return ElmType == Type::sec_split; }
			inline bool sec_split_block() const noexcept { return ElmType == Type::sec_split_block; }
			inline bool tert_split() const noexcept { return ElmType == Type::tert_split; }
			inline bool tert_split_block() const noexcept { return ElmType == Type::tert_split_block; }
			inline bool isSplitPlain() const noexcept {
				return ElmType == Type::prim_split || ElmType == Type::sec_split || ElmType == Type::tert_split; }
			inline bool isSplitBlock() const noexcept { return ElmType == Type::prim_split_block
				|| ElmType == Type::sec_split_block || ElmType == Type::tert_split_block; }

			Type ElmType{ Type::none };
			string Value;
		};
		using ElementPtr = std::shared_ptr<Element>;

		// Line of elements
		struct Line
		{
			Line() {};
			inline Line( index_t indentation ) { Indentation = indentation; }

			inline bool empty() const noexcept { return Elements.empty(); }
			inline bool size() const noexcept { return Elements.size(); }
			inline void push_back( Element element ) {
				Elements.push_back( ElementPtr( new Element( element ) ) ); }
			inline ElementPtr last() const { return Elements.empty() ? ElementPtr() : Elements[ Elements.size() - 1 ]; }

			std::vector<ElementPtr> Elements;
			index_t Indentation{ 0 };
		};
	}
}
