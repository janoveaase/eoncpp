#include "EdfParserData.h"
#include "ExpressionParser.h"
#include "TypeTupleParser.h"


namespace eon
{
	namespace parser
	{
		EdfData::EdfData( parser::State& state, Tuple& scope )
		{
			State = &state;
			Scope = &scope;
			if( !State->Tokens.atEnd() )
				Source = State->Tokens.viewed().source();
			if( !tokens().atEnd() && tokens().viewed().is( name_indentation ) )
				Indentations.push( Indentation::normal( tokens().viewed().str().numChars() ) );
			else
				Indentations.push( Indentation::normal( 0 ) );
		}




		bool EdfData::atOffsetIsGreaterIndentation( index_t offset ) const noexcept
		{
			if( !_atOffsetIs( offset, name_indentation ) )
				return false;
			auto indentation = State->Tokens.peekAhead( offset ).source().numChars();
			if( Indentations.empty() )
				return indentation > 0;
			if( Indentations.top().Forced )
				return indentation >= Indentations.top().Value;
			else
				return indentation > Indentations.top().Value;
		}

		bool EdfData::atOffsetIsSameOrGreaterIndentation( index_t offset ) const noexcept
		{
			if( !_atOffsetIs( offset, name_indentation ) )
				return false;
			auto indentation = State->Tokens.peekAhead( offset ).source().numChars();
			return Indentations.empty() ? indentation >= Indentations.top().Value : indentation >= 0;
		}

		bool EdfData::atOffsetIsLowerIndentation( index_t offset ) const noexcept
		{
			return !Indentations.empty() && State->Tokens.exists( static_cast<int>( offset ) )
				&& State->Tokens.peek( State->Tokens.viewedPos() + offset ).is( name_indentation )
				&& State->Tokens.peek( State->Tokens.viewedPos() + offset ).str().numChars() < Indentations.top().Value;
		}




		void EdfData::recordAttributeName()
		{
			AttributeName = eon::name( tokens().viewed().str() );
			NameSource = tokens().viewed().source();
			tokens().forward();
		}

		void EdfData::recordNameValueSeparator()
		{
			NameSepSource = tokens().viewed().source();
			tokens().forward();
		}




		bool EdfData::ensureNewLine()
		{
			if( State->Tokens.viewed().is( name_newline ) )
				return true;
			error( "Expected tuple attributes to follow on indented line below!" );
			return false;
		}

		void EdfData::error( eon::string&& message, source::Ref* source )
		{
			Errors = true;
			source::Ref src = source != nullptr ? *source
				: !State->Tokens.atEnd() ? State->Tokens.viewed().source() : State->Tokens.last().source();
			State->Report->error( std::move( message ), src );
		}




		source::Ref EdfData::source() const noexcept
		{
			if( Source )
			{
				if( State->Tokens.atEnd() )
					return source::Ref(
						*(source::Raw*)&Source.source(), Source.start(), State->Tokens.last().source().end() );
				else
					return source::Ref(
						*(source::Raw*)&Source.source(), Source.start(), State->Tokens.viewed().source().end() );
			}
			else
				return Source;
		}
	}
}