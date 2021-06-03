#include "Tokenizer.h"


namespace eon
{
	std::vector<token> tokenizer::operator()( const source& source )
	{
		SrcRef = source;
		auto& chars = Characters::get();
		while( SrcRef.chr() != nochar )
		{
			auto category = chars.category( SrcRef.chr() );
			if( category >= charcat::letter_lowercase
				&& category <= charcat::letter_uppercase )
				scanLetter( tokencat::letter );
			else if( category == charcat::number_ascii_digit )
				scanNumber( tokencat::number );
			else if( category == charcat::number_decimal_digit )
				scanSameChar( tokencat::extended_number );
			else if( category >= charcat::number_letter
				&& category <= charcat::number_other )
				scanSameChar( tokencat::other_number );
			else if( category == charcat::separator_space )
			{
				if( SrcRef.chr() == NewlineChr )
				{
					Tokens.push_back( token( SrcRef, tokencat::newline ) );
					if( !SrcRef.advance() )
						break;
				}
				else
					scanSameChar( tokencat::space );
			}
			else if( category >= charcat::separator_line
				&& category <= charcat::separator_paragraph )
				scanSameChar( tokencat::separator );
			else if( category >= charcat::punctuation_connector
				&& category <= charcat::punctuation_other )
				scanSingleChar( tokencat::punctuation );
			else if( category >= charcat::symbol_currency
				&& category <= charcat::symbol_other )
				scanSingleChar( tokencat::symbol );
			else if( category >= charcat::other_control
				&& category <= charcat::other_surrogate )
				scanSingleChar( tokencat::other );
			else
				scanSameChar( tokencat::undef );
		}
		return std::move( Tokens );
	}

	void tokenizer::scanSameChar( tokencat category )
	{
		auto pos = SrcRef.pos();
		while( pos.advance( SrcRef.source() ) && pos.chr() == SrcRef.chr() )
			;
		Tokens.push_back( token(
			SrcRef.source(), SrcRef.pos().line(),	// Source and line number
			SrcRef.pos().area().begin(),			// First character
			pos.area().begin(),						// End character
			category ) );
		SrcRef = pos;	// Move past the read token
	}
	void tokenizer::scanSingleChar( tokencat category )
	{
		auto pos = SrcRef.pos();
		pos.advance( SrcRef.source() );
		Tokens.push_back( token(
			SrcRef.source(), SrcRef.pos().line(),	// Source and line number
			SrcRef.pos().area().begin(),			// First character
			pos.area().begin(),						// End character
			category ) );
		SrcRef = pos;	// Move past the read token
	}
	void tokenizer::scanLetter( tokencat category )
	{
		auto& chars = Characters::get();
		auto pos = SrcRef.pos();
		while( pos.advance( SrcRef.source() ) && chars.isLetter( pos.chr() ) )
			;
		Tokens.push_back( token(
			SrcRef.source(), SrcRef.pos().line(),	// Source and line number
			SrcRef.pos().area().begin(),			// First character
			pos.area().begin(),						// End character
			category ) );
		SrcRef = pos;	// Move past the read token
	}
	void tokenizer::scanNumber( tokencat category )
	{
		auto& chars = Characters::get();
		auto pos = SrcRef.pos();
		while( pos.advance( SrcRef.source() )
			&& chars.isNumberAsciiDigit( pos.chr() ) )
			;
		Tokens.push_back( token(
			SrcRef.source(), SrcRef.pos().line(),	// Source and line number
			SrcRef.pos().area().begin(),			// First character
			pos.area().begin(),						// End character
			category ) );
		SrcRef = pos;	// Move past the read token
	}
}
