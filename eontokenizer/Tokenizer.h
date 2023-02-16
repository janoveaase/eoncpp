#pragma once
#include "Token.h"
#include <unordered_map>
#include <map>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Tokenizer Class - eon::Tokenizer
	//
	// A tokenizer object will take an [eon::source], scan through it and
	// return a vector of [eon::Token]s.
	//
	class Tokenizer
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	public:

		enum class CharacterGrouping : uint8_t
		{
			single,
			sequence
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction, invalid tokenizer
		Tokenizer() = default;

		// Disallow copy and move
		Tokenizer( const Tokenizer& ) = delete;
		Tokenizer( Tokenizer&& ) = delete;

		// Default destruction
		virtual ~Tokenizer() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Configuration
		//
		// The tokenizer will only identify registered token types, while any
		// sequence of characters that aren't recognized will be grouped in a
		// token marked as [eon::no_name].
		//
		// NOTE: The tokenizer will always be greedy!
		// This means that if it already got a matching token, it will try to
		// match a longer one. Only if that fails, will the former matched
		// token be accepted!  This makes it possible to specify different
		// tokens beginning with the same sequence of characters.
		//
	public:

		// Check if any token types have been registered.
		inline bool hasConfiguration() const noexcept { return static_cast<bool>( Conf ); }

		// Register a single character as a token type.
		inline void registerSingleCharAsToken( char_t chr, name_t as_token ) {
			Conf.CharMap[ chr ] = std::make_pair( as_token, CharacterGrouping::single ); }

		// Register a sequence of a single charcter as a token type.
		inline void registerSingleCharSequenceAsToken( char_t chr, name_t as_token ) {
			Conf.CharMap[ chr ] = std::make_pair( as_token, CharacterGrouping::sequence ); }

		// Register any single character from a list of characters as a token type.
		inline void registerAnySingleCharAsToken( const string& characters, name_t as_token ) {
			for( auto c : characters ) Conf.CharMap[ c ] = std::make_pair( as_token, CharacterGrouping::single ); }

		// Register a sequence of characters in any order from a list as a token type.
		inline void registryAnySingleCharSequenceAsToken( const string& characters, name_t as_token ) {
			for( auto c : characters ) Conf.CharMap[ c ] = std::make_pair( as_token, CharacterGrouping::sequence ); }

		// Register any single character from a character catetory as a token type.
		inline void registerAnySingleCharAsToken( charcat in_category, name_t as_token ) {
			Conf.CatMap[ in_category ] = std::make_pair( as_token, CharacterGrouping::single ); }

		// Register any sequence of characters from a character category as a token type.
		inline void registerCharSequenceAsToken( charcat in_category, name_t as_token ) {
			Conf.CatMap[ in_category ] = std::make_pair( as_token, CharacterGrouping::sequence ); }

		// Register a specific sequence of characters as a token type.
		inline void registerCharSequenceAsToken( string&& sequence, name_t as_type ) {
			if( sequence.numChars() > Conf.LongestSeq ) Conf.LongestSeq = sequence.numChars();
			Conf.SeqMap[ std::move( sequence ) ] = as_type; }

		// Register any valid Eon name as token type 'name' [eon::name_name].
		inline void registerEonNamesAsTokens() noexcept { Conf.MatchEonNames = true; }




		///////////////////////////////////////////////////////////////////////
		//
		// Tokenizing
		//
	public:

		// Get sequence of [eon::Token]s from an [eon::source::Ref]
		std::vector<Token> operator()( source::Ref src );




	PRIVATE:
		class CharMatch
		{
		public:
			inline CharMatch( name_t name, CharacterGrouping type ) { Name = name; Type = type; }

		PRIVATE:
			name_t Name;
			CharacterGrouping Type;
		};

		struct Configuration
		{
			inline operator bool() const noexcept { return !CharMap.empty() || !CatMap.empty() || !SeqMap.empty(); }
			std::unordered_map<char_t, std::pair<name_t, CharacterGrouping>> CharMap;
			std::map<charcat, std::pair<name_t, CharacterGrouping>> CatMap;
			std::unordered_map<string, name_t> SeqMap;
			bool MatchEonNames{ false };
			index_t LongestSeq{ 0 };
		};
		Configuration Conf;

		struct Scanner
		{
			Scanner( const Configuration& conf, source::Ref source );
			std::vector<Token> scan();
			bool _scan();
			name_t _identifyType();
			name_t _extendToMaximum();
			source::Ref _maximizeSource();
			name_t _shrinkToMaxPossible( source::Ref& source );
			name_t _matchCharMap( char_t chr );
			name_t _matchCategoryMap( char_t chr );
			inline bool _isNameCandidate( name_t name ) const noexcept {
				return name == name_letters || name == name_digits || name == name_underscore || name == name_name; }
			void _processNewType( name_t type_name );
			void _processOldType();
			bool _extendWithNewType();
			bool _recordNameToken();
			void _processUnmatched();
			void _processEndOfSource( name_t type_name );

			const Configuration* Conf{ nullptr };
			source::Ref Source, CurMatch;
			name_t CurMatchName{ no_name };
			const Characters* Chars{ nullptr };
			source::Ref Unmatched;
			std::vector<Token> Tokens;
		};
	};
}
