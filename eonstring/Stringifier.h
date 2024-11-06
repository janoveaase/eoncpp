#pragma once

#include "String.h"
#include "StringifierDefs.h"
#include <eonexcept/Exception.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// Exception thrown when adding an end-block without having previously added
	// a start-block.
	EONEXCEPT( MismatchedEndBlock );

	// Exception thrown when trying to generate string data where start-block
	// are unmatched by end-blocks.
	EONEXCEPT( MismatchedStartBlock );


	///////////////////////////////////////////////////////////////////////////
	//
	// "Stringifying" is generating human-readable (mostly) string/text data
	// from objects and data.
	//
	// Once a Stringifier object has been created, we can add various types of
	// elements. When done, we can extract the whole thing as a string where
	// each line is at most a specified number of characters (default is 125),
	// automatically split and indented.
	//
	class Stringifier
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		Stringifier() = default;
		virtual ~Stringifier() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Adding Elements
		//
		// NOTE about "before" argument:
		//   Use this when you add an element B after element A, but you want B
		//   to appear before A in output. (Name 'A' as "before" argument.)
		//   If there are multiple elements of type A in a row, then B will be
		//   moved before all of them, not just the last one added.
		//   This will ignore hard line-feeds.
		//
	public:

		// Add a basic "word", 1 space before and after - with exceptions
		inline Stringifier& word( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::word, std::move( value ), before ); return *this; }

		// Add a double quoted string, escape special characters using string::escape, same rules as "word"
		inline Stringifier& doubleq( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::doubleq, std::move( value ), before ); return *this; }

		// Add a single quoted string, escape special characters using string::escapeAll, same rules as "word"
		inline Stringifier& singleq( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::singleq, std::move( value ), before ); return *this; }

		// Append to previous element, no space between
		inline Stringifier& append( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::append, std::move( value ), before ); return *this; }

		// Prepend to next element, no space between
		inline Stringifier& prepend( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::prepend, std::move( value ), before ); return *this; }

		// Add an operator type 1, 1 space before and after, can be overriden
		inline Stringifier& op1( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::op1, std::move( value ), before ); return *this; }

		// Add an operator type 2, no space before or after, can be overridden
		inline Stringifier& op2( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::op2, std::move( value ), before ); return *this; }

		// Add punctuation, no space towards previous element, 1 space towards next
		inline Stringifier& punct( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::punct, std::move( value ), before ); return *this; }

		// Start a grouped section type 1, 1 space before and 1 after, can be overridden
		inline Stringifier& start_grp1( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::start_grp1, std::move( value ), before ); return *this; }
		// End a grouped section type 1
		inline Stringifier& end_grp1( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::end_grp1, std::move( value ), before ); return *this; }

		// Start a grouped section type 2, no space before or after, can be overridden
		inline Stringifier& start_grp2( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::start_grp2, std::move( value ), before ); return *this; }
		// End a gropued section type 2
		inline Stringifier& end_grp2( string value, stringify::Type before = stringify::Type::none ) {
			_add( stringify::Type::end_grp2, std::move( value ), before ); return *this; }

		// Start a block (hard lf + indented set of lines)
		inline Stringifier& start_block() {
			hard_lf(); ++Indentation; ++Blocks; _add( stringify::Type::start_block ); return *this; }
		// End a block (indentation back to normal + hard lf)
		Stringifier& end_block() {
			if( --Blocks < 0 )
				throw MismatchedEndBlock();
			bool lf = CurLine < Lines.size();
			--Indentation;
			_add( stringify::Type::end_block );
			if( lf )
				hard_lf();
			return *this;
		}

		// Start a "chapter", empty line before (except when at start) and after (except when at end)
		inline Stringifier& start_chapt() { _add( stringify::Type::start_chapt ); return *this; }
		// End a "chapter"
		inline Stringifier& end_chapt() { _add( stringify::Type::end_chapt ); return *this; }

		// Add a hard line-feed
		inline Stringifier& hard_lf()
		{
			if( CurLine < Lines.size() )
				++CurLine;
			return *this;
		}

		// Primary split points. Prefer long lines to be split here.
		// Use the 'block' version to have subsequent lines indented.
		inline Stringifier& prim_split() { _add( stringify::Type::prim_split ); return *this; }
		inline Stringifier& prim_split_block() { _add( stringify::Type::prim_split_block ); return *this; }

		// Secondary split points. Prefer long lines to be split here, if no primary splits are found.
		// Use the 'block' version to have subsequent lines indented.
		inline Stringifier& sec_split() { _add( stringify::Type::sec_split ); return *this; }
		inline Stringifier& sec_split_block() { _add( stringify::Type::sec_split_block ); return *this; }

		// Tertiary split points. Split long lines here if no primary or secondary splits are found.
		// Use the 'block' version to have subsequent lines indented.
		inline Stringifier& tert_split() { _add( stringify::Type::tert_split ); return *this; }
		inline Stringifier& tert_split_block() { _add( stringify::Type::tert_split_block ); return *this; }


		// Check the last element added
		inline stringify::Type lastElement() const noexcept {
			return CurLine < Lines.size() ? Lines[ CurLine ].Elements.rbegin()->get()->ElmType : stringify::Type::none; }




		///////////////////////////////////////////////////////////////////////
		//
		// Generating String Output
		//
	public:

		// Override operator type 1 space settings
		// Default is on=true
		inline void spacesBeforeAfterOp1( bool on ) noexcept { Configuration.SpacesBeforeAfterOp1 = on; }
		inline bool spacesBeforeAfterOp1() const noexcept { return Configuration.SpacesBeforeAfterOp1; }

		// Override operator type 2 space settings
		// Default is on=false
		inline void spacesBeforeAfterOp2( bool on ) noexcept { Configuration.SpacesBeforeAfterOp2 = on; }
		inline bool spacesBeforeAfterOp2() const noexcept { return Configuration.SpacesBeforeAfterOp2; }

		// Override group type 1 space settings
		// Default is outside=true, inside=false
		inline void spacesOutsideGrp1( bool on ) noexcept { Configuration.SpacesOutsideGrp1 = on; }
		inline bool spacesOutsideGrp1() const noexcept { return Configuration.SpacesOutsideGrp1; }
		inline void spacesInsideGrp1( bool on ) noexcept { Configuration.SpacesInsideGrp1 = on; }
		inline bool spacesInsideGrp1() const noexcept { return Configuration.SpacesInsideGrp1; }

		// Override group type 2 space settings
		// Default is outside=false, inside=false
		inline void spacesOutsideGrp2( bool on ) noexcept { Configuration.SpacesOutsideGrp2 = on; }
		inline bool spacesOutsideGrp2() const noexcept { return Configuration.SpacesOutsideGrp2; }
		inline void spacesInsideGrp2( bool on ) noexcept { Configuration.SpacesInsideGrp2 = on; }
		inline bool spacesInsideGrp2() const noexcept { return Configuration.SpacesInsideGrp2; }

		// Set maximum line width, but allow single elements that exceed this to remain unbroken
		// Default is 125
		inline void softLineWidth( index_t limit ) noexcept { Configuration.SoftLineWidth = limit; }
		inline bool softLineWidth() const noexcept { return Configuration.SoftLineWidth; }

		// Set maximum line width, do not allow any lines to exceed this under any circumstances
		// Default is no limit
		inline void hardLineWidth( index_t limit ) noexcept { Configuration.HardLineWidth = limit; if(
			limit < Configuration.SoftLineWidth ) Configuration.SoftLineWidth = limit; }
		inline bool hardLineWidth() const noexcept { return Configuration.HardLineWidth; }


		// Get output as a vector of lines
		// Throws eon::MismatchedStartBlock if there are start-blocks not matched by end-blocks!
		std::vector<string> generateLines() const;

		// Get output as a string
		// Throws eon::MismatchedStartBlock if there are start-blocks not matched by end-blocks!
		inline string generateString() const { return string( "\n" ).join( generateLines() ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		using element_iterator = std::vector<stringify::ElementPtr>::const_iterator;

		class GeneratorData
		{
		public:
			GeneratorData() = delete;
			inline GeneratorData( const std::vector<stringify::Line>& lines ) : Lines( &lines ) {}

			void startLineIteration() noexcept;
			inline bool atEndLine() const noexcept { return CurLine == Lines->end(); }
			void nextLine();
			inline bool atEndElement() const noexcept { return atEndLine() || CurElm == CurLine->Elements.end(); }

			inline stringify::Element& element() const { return **CurElm; }

			inline element_iterator firstElement() const noexcept { return CurLine->Elements.begin(); }
			inline element_iterator& curElement() noexcept { return CurElm; }
			inline void curElement( element_iterator pos ) noexcept { CurElm = pos; }
			inline element_iterator endElement() const noexcept { return CurLine->Elements.end(); }
			inline bool isEndElement( element_iterator pos ) const noexcept { return pos == CurLine->Elements.end(); }

		private:
			const std::vector<stringify::Line>* Lines{ nullptr };
			std::vector<stringify::Line>::const_iterator CurLine;
			element_iterator CurElm;
		public:
			std::vector<string> Output;
			index_t Indentation{ 0 };
			stringify::Line Line;
			bool LongLineSplit{ false };
		};

		inline void _add( stringify::Type type, stringify::Type before = stringify::Type::none ) {
			_add( type, string(), before ); }
		void _add( stringify::Type type, string&& value, stringify::Type before = stringify::Type::none );
		bool _addBefore( stringify::Type type, string& value, stringify::Type before );
		bool _addBefore( stringify::Type type, string& value, stringify::Type before, index_t pos );


		void _processLine( GeneratorData& data ) const;
		bool _haveSomething( GeneratorData& data, element_iterator end ) const;
		void _doHardLineSplit( GeneratorData& data, element_iterator end ) const;
		void _processLineElements( GeneratorData& data, element_iterator end ) const;
		bool _processLineSplit( GeneratorData& data, element_iterator end ) const;

		element_iterator _fillLine( GeneratorData& data ) const noexcept;
		element_iterator _findSplit( element_iterator begin, element_iterator end ) const noexcept;
		element_iterator _findSplit(
			stringify::Type type, stringify::Type type_block, element_iterator begin, element_iterator end ) const noexcept;

		void _splitHard( const stringify::Element& value, GeneratorData& data ) const;
		eon::string::iterator _findStringSplitPoint( eon::string::iterator beg, eon::string::iterator end ) const;
		void _output( GeneratorData& data, element_iterator end ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		stringify::Conf Configuration;
		std::vector<stringify::Line> Lines;
		index_t CurLine{ 0 };
		index_t Indentation{ 0 };
		int Blocks{ 0 };
	};
}
