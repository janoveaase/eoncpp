#pragma once
#include "TokenParser.h"
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Re-tokenizer Class - eon::ReTokenParser

	  Given a stream of raw tokens, run through them and combine into larger
	  tokens as defined by a set of rules. The result will be a new token
	  stream with more meaningful tokens.
	**************************************************************************/
	class ReTokenizer
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		ReTokenizer() = default;
		virtual ~ReTokenizer() { for( auto rule : Rules ) delete rule; };




		/**********************************************************************
		  Configuration

		  Set up rules for re-tokenizing.

		  Examples, assuming raw tokens of type "letters", "digits",
		  "underscore", and "point".

			Rule for creating "name" tokens in Æon style:
				name=combo(letters, digits, underscore), exclude=?/^\d+$/
			The regex will identify pure digit sequences and exclude them.

			Rule for creating "float" tokens:
			    float=sequence(digits, point, digits)

		  Each rule must be one of:
		    1. Name + Combo [+ Exclude]
			2. Name + PrefixCombo [+ Exclude]
			3. Name + Sequence [+ Exclude]
			4. Name + Enclose
			5. Name + LineStart
			6. Name + Remove
		  Combo:
			A list of names for which token types must match. All consecutive
			tokens for which the type is in the list will be included in the
			final token. Matching is greedy but a full match can be excluded
			if it matches the Exclude regex.
		  PrefixCombo:
		    Same as Combo, but the first token must match a specific string
		  Sequence:
			A list of names for which tokens must match in corresponding
			sequence. A match can be negated if it also matches the Exclude
			regex.
		  Enclose:
			A start token and an end token encloses the tokens inbetween into
			a single token. Start and end tokens are not part of the new token,
			so make sure to name it properly!
			An escape token can be named to avoid escaped end token!
		  PrefixEnclose:
		    Same as Enclose, but the first token must match a specific string
		  LineStart:
		    A token occurring at the start of a line (first token of source or
			first after a newline).
		  Remove:
			A list of names for which tokens with matching type shall be
			removed.

		  Multiple rules can specify the same name (type name of target token).

		  Any raw tokens that don't get matched by any rules are accepted as-
		  is.
		**********************************************************************/
	public:

		//* Super-class for re-tokenizing rules
		class RuleDef
		{
		public:
			RuleDef( name_t name ) { Name = name; }
			virtual ~RuleDef() = default;
			inline name_t name() const noexcept { return Name; }
			virtual bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept = 0;
		protected:
			name_t Name{ no_name };
		};

		class RemoveRule : public RuleDef
		{
		public:
			inline RemoveRule( std::set<name_t>&& remove ) noexcept : RuleDef( no_name ) {
				Remove = std::move( remove ); }
			virtual ~RemoveRule() = default;
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			std::set<name_t> Remove;
		};
		class EncloseRule : public RuleDef
		{
		public:
			inline EncloseRule( name_t name, name_t enclose_char, name_t escape ) noexcept : RuleDef( name ) {
				EncloseStart = enclose_char; EncloseEnd = enclose_char; Escape = escape; }
			inline EncloseRule( name_t name, name_t enclose_start, name_t enclose_end, bool nested ) noexcept
				: RuleDef( name ) { EncloseStart = enclose_start; EncloseEnd = enclose_end; Nested = nested; }
			virtual ~EncloseRule() = default;
			virtual bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		protected:
			bool _match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept;
			name_t EncloseStart{ no_name }, EncloseEnd{ no_name }, Escape{ no_name };
			bool Nested{ false };
		};
		class PrefixEncloseRule : public EncloseRule
		{
		public:
			inline PrefixEncloseRule( name_t name, string prefix, name_t enclose_char, name_t escape ) noexcept
				: EncloseRule( name, enclose_char, escape ) { Prefix = prefix; }
			inline PrefixEncloseRule( name_t name, string prefix, name_t enclose_start, name_t enclose_end, bool nested )
				noexcept : EncloseRule( name, enclose_start, enclose_end, nested ) { Prefix = prefix; }
			virtual ~PrefixEncloseRule() = default;
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			string Prefix;
		};
		class ComboRule : public RuleDef
		{
		public:
			inline ComboRule( name_t name, std::set<name_t>&& combo, regex&& exclude = regex() ) : RuleDef( name ) {
				Combo = std::move( combo ); Exclude = std::move( exclude ); }
			virtual ~ComboRule() = default;
			virtual bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		protected:
			bool _match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept;
			std::set<name_t> Combo;
			regex Exclude;
		};
		class PrefixComboRule : public ComboRule
		{
		public:
			inline PrefixComboRule( name_t name, string prefix, std::set<name_t>&& combo, regex&& exclude = regex() )
				: ComboRule( name, std::move( combo ), std::move( exclude ) ) { Prefix = prefix; }
			virtual ~PrefixComboRule() = default;
			virtual bool match( TokenParser& parser, std::vector<Token>&output ) const noexcept override;
		private:
			string Prefix;
		};
		class SequenceRule : public RuleDef
		{
		public:
			inline SequenceRule( name_t name, std::vector<name_t>&& sequence, regex&& exclude = regex() ) : RuleDef( name ) {
				Sequence = std::move( sequence ); Exclude = std::move( exclude ); }
			virtual ~SequenceRule() = default;
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			std::vector<name_t> Sequence;
			regex Exclude;
		};
		class LinestartRule : public RuleDef
		{
		public:
			inline LinestartRule( name_t name, name_t linestart ) : RuleDef( name ) { Linestart = linestart; }
			virtual ~LinestartRule() = default;
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		private:
			name_t Linestart{ no_name };
		};



		//* Check if there are any rules present
		inline operator bool() const noexcept { return !Rules.empty(); }

		
		//* Add a combo rule		
//		inline void addComboRule( name_t name, std::set<name_t>&& combo, regex&& exclude = regex() ) {
//			Rules.push_back( new ComboRule( name, std::move( combo ), std::move( exclude ) ) ); }
		
		//* Add a sequence rule
//		inline void addSequenceRule( name_t name, std::vector<name_t>&& sequence, regex&& exclude = regex() ) {
//			Rules.push_back( new SequenceRule( name, std::move( sequence ), std::move( exclude ) ) ); }
		

		//* Add an enclose rule using same character as start and end, with an
		//* optional escape token
//		inline void addSimpleEncloseRule( name_t name, name_t enclose_char, name_t escape = no_name ) {
//			Rules.push_back( new EncloseRule( name, enclose_char, escape ) ); }

		//* Add an enclose rule with explicit start and end, optionally with
		//* escape token.
//		inline void addStartEndEncloseRule( name_t name, name_t enclose_start, name_t enclose_end, name_t escape = no_name ) {
//			Rules.push_back( new EncloseRule( name, enclose_start, enclose_end, escape ) ); }

		//* Add an enclose rule with explicit start and end, optionally with
		//* escape token.
		//* Any occurrence of an additional start token before the end token
		//* will increase a nesting counter. End tokens will decrease it and
		//* end the enclosure only when the counter reaches zero.
//		inline void addNestedEncloseRule( name_t name, name_t enclose_start, name_t enclose_end, name_t escape = no_name ) {
//			Rules.push_back( new EncloseRule( name, enclose_start, enclose_end, true ) ); }
		

		//* Add a line-start token - token that must occur at the beginning of
		//* the line to be matched
//		inline void addLinestart( name_t name, name_t linestart ) {
//			Rules.push_back( new LinestartRule( name, linestart ) ); }
		
		//* Add a remove rule - remove tokens with type matching one in the set
//		inline void addRemoveRule( std::set<name_t>&& remove ) { Rules.push_back( new RemoveRule( std::move( remove ) ) ); }

		//* Add a custom rule
//		inline void addCustomRule( RuleDef* rule ) { Rules.push_back( rule ); }

		//* Add a rule
		inline void addRule( RuleDef* rule ) { Rules.push_back( rule ); }




		/**********************************************************************
		  Operations
		**********************************************************************/
	public:

		//* Run re-tokenizing
		std::vector<Token> operator()( TokenParser& parser ) const noexcept;




	private:
		std::vector<RuleDef*> Rules;
	};
};
