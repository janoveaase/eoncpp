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
	// "Stringifying" is turning objects into a human-readable string/text
	// where formatting is handled more or less automatically.
	//
	// Once a Stringifier object has been created, we can "push" elements of
	// varying types, keeping a max line length and automatically maintaining
	// indentation.
	//
	// The possible types are:
	//
	//   word:
	//     Sequence of characters (not necessarily 'true' words).
	//     If pushed after 'word' or 'stop', then add a space inbetween.
	//   append:
	//     Append 'word' to 'word.
	//     If pushed after 'word', do not add space. Otherwise, identical to
	//     'word' (and treated as word by following pushes).
	//   prefix:
	//     This is a 'word' that is a prefix to whatever follows, so no space
	//     after!
	//   operator:
	//     Sequence of characters (not necessarily 'true' operators).
	//     If operators are specified to have spaces around them (default),
	//     then they will be added, otherwise no space before or after.
	//     Allow auto-splitting of long lines BEFORE!
	//   opname:
	//     Operator name, same as 'operator' except:
	//       Always with space before and after.
	//   specialop:
	//     Special operator, same as 'operator' except:
	//       Separate space setting where default is to not include.
	//   stop:
	//     Sequence of characters that ends something.
	//     There will be no space before it, but one will typically be added
	//     after.
	//     Allow auto-splitting long lines AFTER!
	//     Do not put on new line if after endblock!
	//   open:
	//     Single character with space before it, optional (default off) after.
	//     Allow auto-splitting of long lines AFTER!
	//   close:
	//     Single character with optional (default off) space before it, space
	//     after except before 'stop'.
	//     Allow auto-splitting of long lines AFTER!
	//   startblock:
	//     Zero or more characters marking the start of a new (indented) block.
	//     Hard newline after and indent following lines until 'endblock'.
	//   endblock:
	//     Zero or more characters marking the end of a (indented) block.
	//
	class Stringifier
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct a stringifier
		inline Stringifier( index_t max_line_width = 125 ) { MaxLineWidth = max_line_width; }




		///////////////////////////////////////////////////////////////////
		//
		// Settings
		//
	public:

		inline void spacesAroundOperators( bool on ) noexcept { OpSpaces1 = on; }
		inline void spacesAroundSpecialOperators( bool on ) noexcept { OpSpaces2 = on; }

		inline void spacesInsideOpenClose( bool on ) noexcept { OpenCloseSpaces = on; }




		///////////////////////////////////////////////////////////////////
		//
		// Pushing elements
		//
	public:

		inline Stringifier& pushWord( string&& word ) { return push( Type::word, std::move( word ) ); }
		inline Stringifier& pushAppend( string&& word ) { return push( Type::append, std::move( word ) ); }
		inline Stringifier& pushPrefix( string&& word ) { return push( Type::prefix, std::move( word ) ); }
		inline Stringifier& pushOperator( string&& opr ) { return push( Type::opr, std::move( opr ) ); }
		inline Stringifier& pushOpName( string&& name ) { return push( Type::opname, std::move( name ) ); }
		inline Stringifier& pushSpecialOp( string&& opr ) { return push( Type::specop, std::move( opr ) ); }
		inline Stringifier& pushStop( string&& chars ) { return push( Type::stop, std::move( chars ) ); }
		inline Stringifier& pushOpen( string&& chars = "" ) { return push( Type::open, std::move( chars ) ); }
		inline Stringifier& pushClose( string&& chars = "" ) { return push( Type::close, std::move( chars ) ); }
		inline Stringifier& pushStartBlock( string&& chars = "" ) { return push( Type::startblock, std::move( chars ) ); }
		inline Stringifier& pushEndBlock( string&& chars = "" ) { return afterLF() ? _replace(
			Type::endblock, std::move( chars ) ) : push( Type::endblock, std::move( chars ) ); }

		inline Stringifier& pushWord( const string& word ) { return push( Type::word, string( word ) ); }
		inline Stringifier& pushAppend( const string& word ) { return push( Type::append, string( word ) ); }
		inline Stringifier& pushPrefix( const string& word ) { return push( Type::prefix, string( word ) ); }
		inline Stringifier& pushOperator( const string& opr ) { return push( Type::opr, string( opr ) ); }
		inline Stringifier& pushOpName( const string& name ) { return push( Type::opname, string( name ) ); }
		inline Stringifier& pushSpecialOp( const string& opr ) { return push( Type::specop, string( opr ) ); }
		inline Stringifier& pushStop( const string& chars ) { return push( Type::stop, string( chars ) ); }
		inline Stringifier& pushOpen( const string& chars ) { return push( Type::open, string( chars ) ); }
		inline Stringifier& pushClose( const string& chars ) { return push( Type::close, string( chars ) ); }
		inline Stringifier& pushStartBlock( const string& chars ) { return push( Type::startblock, string( chars ) ); }
		inline Stringifier& pushEndBlock( const string& chars ) { return push( Type::endblock, string( chars ) ); }

		// Hard newline
		inline Stringifier& endLine() { return afterLF() || afterBlock() ? *this : _push( Type::lf, "\n" ); }




		///////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Get formatted string
		string str() const;




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		enum class Type
		{
			none,
			word,
			append,
			prefix,
			opr,
			opname,
			specop,
			stop,
			open,
			close,
			startblock,
			endblock,

			space,
			lf
		};
		struct Element
		{
			Element() = default;
			Element( Type what, string&& value, index_t raw_line_pos ) noexcept {
				What = what; Value = std::move( value ); RawLinePos = raw_line_pos; }

			Type What{ Type::none };
			string Value;
			index_t RawLinePos{ 0 };
		};

		inline bool afterWord() const noexcept { return LastType == Type::word; }
		inline bool afterOp1() const noexcept { return LastType == Type::opr; }
		inline bool afterOp2() const noexcept { return LastType == Type::specop; }
		inline bool afterStop() const noexcept { return LastType == Type::stop; }
		inline bool afterOpen() const noexcept { return LastType == Type::open; }

		inline bool afterSpace() const noexcept { return LastType == Type::space; }
		inline bool afterLF() const noexcept { return LastType == Type::lf; }
		inline bool afterBlock() const noexcept { return LastType == Type::endblock; }

		Stringifier& push( Type what, string&& value );

		inline Stringifier& _push( Type what, string&& value ) {
			Elements.push_back( Element( what, std::move( value ), _curLinePos() ) ); LastType = what; return *this; }
		inline Stringifier& _replace( Type what, string&& value ) {
			*Elements.rbegin() = Element( what, std::move( value ), 0 ); return *this; }

		inline index_t _curLinePos() const noexcept {
			return Elements.empty() ? 0 : Elements.rbegin()->RawLinePos + Elements.rbegin()->Value.numChars(); }

		void _processLine( string& line, std::vector<string>& lines, std::vector<index_t>& splitpoints,
			index_t indentation, bool endline ) const;
		void _processSplitPoints( string& line, std::vector<string>& lines, std::vector<index_t>& splitpoints,
			index_t indentation, bool endline ) const;






		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		index_t MaxLineWidth{ 125 };
		std::vector<Element> Elements;
		Type LastType{ Type::none };

		bool OpSpaces1{ true }, OpSpaces2{ false };
		bool OpenCloseSpaces{ false };
	};
}
