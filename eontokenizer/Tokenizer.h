#pragma once
#include "Token.h"
#include <unordered_map>
#include <map>
#include <vector>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Tokenizer Class - eon::Tokenizer

	  A tokenizer object will take an [eon::source], scan through it and return
	  a vector of [eon::Token]s.
	**************************************************************************/
	class Tokenizer
	{
		/**********************************************************************
		  Definitions
		**********************************************************************/
	public:

		enum class Match : uint8_t
		{
			single,
			sequence
		};




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, invalid tokenizer
		Tokenizer() = default;

		//* Disallow copy and move
		Tokenizer( const Tokenizer& ) = delete;
		Tokenizer( Tokenizer&& ) = delete;

		//* Default destruction
		virtual ~Tokenizer() = default;




		/**********************************************************************
		  Configuration

		  The tokenizer will only identify tokens for recognized sequences of
		  characaters, while any sequence that isn't recognized is marked as
		  such!

		  NOTE: The tokenizer will always be greedy, meaning that if it already
		        got a matching token, it will try to match a longer one. Only
		        if that fails, will the former matched token be accepted!
		  This makes it possible to specify different tokens beginning with the
		  same sequence.
		**********************************************************************/
	public:

		//* Check if the tokenizer has any configuration at all
		inline operator bool() const noexcept { return !CharMap.empty() || !CatMap.empty() || !SeqMap.empty(); }

		//* Make any sequence matching any of these characters a token
		inline void registerTokenChar( name_t type_name, char_t chr, Match type ) {
			CharMap[ chr ] = std::make_pair( type_name, type ); }
		inline void registerTokenChars( name_t type_name, const string& characters, Match type ) {
			for( auto c : characters ) CharMap[ c ] = std::make_pair( type_name, type ); }

		//* Make any sequence of characters in the specified category a token
		inline void registerTokenCharcat( name_t type_name, charcat catetory, Match type ) {
			CatMap[ catetory ] = std::make_pair( type_name, type ); }

		//* Make the specified sequence of characters a token
		inline void registerSequenceToken( name_t type_name, string&& sequence ) {
			SeqMap[ std::move( sequence ) ] = type_name; }




		/**********************************************************************
		  Tokenizing
		**********************************************************************/
	public:

		//* Get sequence of [eon::Token]s from an [eon::source::Ref]
		std::vector<Token> operator()( source::Ref src );




		/**********************************************************************
		  Helpers
		**********************************************************************/
	private:

		name_t _match();




	private:
		class CharMatch
		{
		public:
			inline CharMatch( name_t name, Match type ) { Name = name; Type = type; }

		private:
			name_t Name;
			Match Type;
		};

		source::Ref Source, CurMatch;
		name_t CurMatchName{ no_name };
		std::unordered_map<char_t, std::pair<name_t, Match>> CharMap;
		std::map<charcat, std::pair<name_t, Match>> CatMap;
		std::unordered_map<string, name_t> SeqMap;
		std::vector<Token> Tokens;
	};
};
