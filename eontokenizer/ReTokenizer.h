#pragma once
#include "TokenParser.h"
#include <eonregex/RegEx.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	// Eon Re-tokenizer Class - eon::ReTokenParser
	//
	// Given a stream of raw tokens, run through them and combine into larger
	// tokens as defined by a set of rules. The result will be a new token
	// stream with more meaningful tokens.
	//
	class ReTokenizer
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		ReTokenizer() = default;
		virtual ~ReTokenizer() = default;




		///////////////////////////////////////////////////////////////////////
		// Configuration
		//
		// Set up rules for re-tokenizing.
		//
		// Examples, assuming raw tokens of type "letters", "digits",
		// "underscore", and "point".
		//
		//   Rule for creating "name" tokens in Æon style:
		//     name=combo(letters, digits, underscore), exclude=?/^\d+$/
		// The regex will identify pure digit sequences and exclude them.
		//
		//   Rule for creating "float" tokens:
		//     float=sequence(digits, point, digits)
		//
		// Each rule must be one of:
		//   1. Name + Combo [+ Exclude]
		//   2. Name + PrefixCombo [+ Exclude]
		//   3. Name + Sequence [+ Exclude]
		//   4. Name + LiteralName
		//   5. Name + Regex
		//   6. Name + Enclose
		//   7. Name + LineStart
		//   8. Name + Remove
		// Combo:
		//   A list of names for which token types must match. All consecutive
		//   tokens for which the type is in the list will be included in the
		//   final token. Matching is greedy but a full match can be excluded
		//   if it matches the Exclude regex.
		// PrefixCombo:
		//   Same as Combo, but the first token must match a specific string
		// Alternating:
		//   Two names (A, B) for which the following tokens must match.
		//   Matching can stop on first failed match of A or of B. The longest
		//   possible sequence will be included in the match.
		// PrefixAlternating:
		//   Same as Alternating, but the first token must match a specific string.
		// Sequence:
		//   A list of names for which tokens must match in corresponding
		//   sequence. A match can be negated if it also matches the Exclude
		//   regex.
		// LiteralName:
		//   A list of names for which the next token must match one.
		// Regex:
		//   A regex for which the next token must match.
		// Enclose:
		//   A start token and an end token encloses the tokens inbetween into
		//   a single token. Start and end tokens are not part of the new token,
		//   so make sure to name it properly!
		//   An escape token can be named to avoid escaped end token!
		// PrefixEnclose:
		//   Same as Enclose, but the first token must match a specific string
		// LineStart:
		//   A token occurring at the start of a line (first token of source or
		//   first after a newline).
		// Remove:
		//   A list of names for which tokens with matching type shall be
		//   removed.
		//
		// Multiple rules can specify the same name (type name of target token).
		//
		// Any raw tokens that don't get matched by any rules are accepted as-
		// is.
		//
	public:

		// Super-class for re-tokenizing rules
		class RuleDef
		{
		public:
			inline explicit RuleDef( name_t name ) : Name( name ) {}
			virtual ~RuleDef() = default;
			inline name_t name() const noexcept { return Name; }
			virtual bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept = 0;

		private:
			name_t Name{ no_name };
		};

		class RemoveRule : public RuleDef
		{
		public:
			inline explicit RemoveRule( std::set<name_t>&& remove ) noexcept : RuleDef( no_name ) {
				Remove = std::move( remove ); }
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			std::set<name_t> Remove;
		};

		class EncloseRule : public RuleDef
		{
		public:
			inline EncloseRule( name_t name, name_t enclose_char, name_t escape ) noexcept
				: RuleDef( name ), EncloseStart( enclose_char ), EncloseEnd( enclose_char ), Escape( escape ) {}
			inline EncloseRule( name_t name, name_t enclose_start, name_t enclose_end, bool nested ) noexcept
				: RuleDef( name ), EncloseStart( enclose_start ), EncloseEnd( enclose_end ), Nested( nested ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		protected:
			bool _match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept;
			inline name_t _encloseStart() const noexcept { return EncloseStart; }
			inline name_t _encloseEnd() const noexcept { return EncloseEnd; }
		private:
			name_t EncloseStart{ no_name };
			name_t EncloseEnd{ no_name };
			name_t Escape{ no_name };
			bool Nested{ false };
		};

		class PrefixEncloseRule : public EncloseRule
		{
		public:
			inline PrefixEncloseRule( name_t name, string prefix, name_t enclose_char, name_t escape ) noexcept
				: EncloseRule( name, enclose_char, escape ), Prefix( std::move( prefix ) ) {}
			inline PrefixEncloseRule( name_t name, string prefix, name_t enclose_start, name_t enclose_end, bool nested )
				noexcept : EncloseRule( name, enclose_start, enclose_end, nested ), Prefix( std::move( prefix ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			string Prefix;
		};

		class ComboRule : public RuleDef
		{
		public:
			inline ComboRule( name_t name, std::set<name_t>&& combo, regex&& exclude = regex() )
				: RuleDef( name ), Combo( std::move( combo ) ), Exclude( std::move( exclude ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		protected:
			bool _match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept;
		private:
			std::set<name_t> Combo;
			regex Exclude;
		};

		class PrefixComboRule : public ComboRule
		{
		public:
			inline PrefixComboRule( name_t name, string prefix, std::set<name_t>&& combo, regex&& exclude = regex() )
				: ComboRule( name, std::move( combo ), std::move( exclude ) ), Prefix( std::move( prefix ) ) {}
			bool match( TokenParser& parser, std::vector<Token>&output ) const noexcept override;
		private:
			string Prefix;
		};

		class AlternatingRule : public RuleDef
		{
		public:
			inline AlternatingRule( name_t name, name_t a, name_t b, bool end_on_a = true )
				: RuleDef( name ), A( a ), B( b ), EndOnA( end_on_a ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		protected:
			bool _match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept;
		private:
			name_t A{ no_name };
			name_t B{ no_name };
			bool EndOnA{ true };
		};

		class PrefixAlternatingRule : public AlternatingRule
		{
		public:
			inline PrefixAlternatingRule( name_t name, string prefix, name_t a, name_t b, bool end_on_a = true )
				: AlternatingRule( name, a, b, end_on_a ), Prefix( std::move( prefix ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			string Prefix;
		};

		class SequenceRule : public RuleDef
		{
		public:
			inline SequenceRule( name_t name, std::vector<name_t>&& sequence, regex&& exclude = regex() )
				: RuleDef( name ), Sequence( std::move( sequence ) ), Exclude( std::move( exclude ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			std::vector<name_t> Sequence;
			regex Exclude;
		};

		class LiteralNameRule : public RuleDef
		{
		public:
			inline LiteralNameRule( name_t name, std::set<string>&& names )
				: RuleDef( name ), Names( std::move( names ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			std::set<string> Names;
		};

		class RegexRule : public RuleDef
		{
		public:
			inline RegexRule( name_t name, regex&& pattern ) : RuleDef( name ), Pattern( std::move( pattern ) ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			regex Pattern;
		};

		class LinestartRule : public RuleDef
		{
		public:
			inline LinestartRule( name_t name, name_t linestart ) : RuleDef( name ), Linestart( linestart ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			name_t Linestart{ no_name };
		};



		// Check if there are any rules present
		inline explicit operator bool() const noexcept { return !Rules.empty(); }

		// Add a rule
		inline void addRule( std::shared_ptr<RuleDef> rule ) { Rules.push_back( rule ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		// Run re-tokenizing
		std::vector<Token> operator()( TokenParser& parser ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		bool _matchARule( TokenParser& parser, std::vector<Token>& output ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		std::vector<std::shared_ptr<RuleDef>> Rules;
	};
};
