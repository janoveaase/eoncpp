#include "SourceReporter.h"
#include <cmath>


namespace eon
{
	namespace source
	{
		string Message::str() const
		{
			string s;
			switch( Level )
			{
				case severity::fatal:
					s = "  FATAL";
					break;
				case severity::error:
					s = "  ERROR";
					break;
				case severity::serious:
					s = "SERIOUS";
					break;
				case severity::warning:
					s = "WARNING";
					break;
				case severity::note:
					s = "   NOTE";
					break;
				case severity::info:
					s = "   INFO";
					break;
				default:
					s = "   ?\?\??";
					break;
			}
			if( Source )
			{
				s += " in \"" + Source.name() + "\"";
				if( Source.explicitEnd() )
				{
					s += " (" + Source.startStr();
					if( Source.numChars() > 1 )
						s += "-" + Source.endStr();
					s += ")";
				}
				s += ":";
				if( s.numChars() + Text.numChars() >= 80 )
					s += "\n";
				else
					s += " ";
				s += Text;
				if( Source.explicitEnd() )
				{
					bool newline = Source.numChars() == 1 && Source.str() == "\n";
					Pos start = Source.start();
					auto lnumdigits = Source.end().line() < 10 ? 1 : static_cast<size_t>( log10( Source.end().line() ) );
					for( int i = 0; ; ++i )
					{
						s += "\n";
						auto line = Ref( *(Raw*)&Source.source(), start, start );
						line.pullStartOfLine();
						line.pushEndOfLine();
						s += string( start.line() + 1 ).padLeft( lnumdigits, ' ' ) + "|" + line.str() + "\n";
						s += string().padRight( lnumdigits, ' ' ) + "|";
						if( start.line() == Source.start().line() )
							s += string( start.charPos() - line.start().charPos(), '-' );
						if( newline )
						{
							if( i == 0 )
								s += "^";
						}
						else
						{
							size_t length{ 0 };
							if( Source.start().line() == Source.end().line() )
								length = Source.end().charPos() - Source.start().charPos();
							else if( start.line() == Source.start().line() )
								length = line.end().charPos() - Source.start().charPos();
							else if( start.line() == Source.end().line() )
								length = Source.end().charPos() - start.charPos();
							else
								length = line.end().charPos() - line.start().charPos();
							s += string( length, '^' );
							if( start.line() == Source.end().line() )
								s += string( line.end().charPos() - Source.end().charPos(), '-' );
						}

						if( start.line() == Source.end().line() )
							break;
						start = ( (Raw*)&Source.source() )->push( line.end(), 1 );
					}
				}
			}
			else if( !Text.empty() )
				s += " " + Text;
			return s;
		}




		void Reporter::report( severity level, string&& text, Ref source )
		{
			Message message( level, std::move( text ), source );
			if( level <= severity::warning )
				Issues.push_back( message );
			for( auto& target : Targets )
			{
				if( level >= target.Trg->filterHigh() && level <= target.Trg->filterLow() )
					target.Trg->report( message );
			}
		}
	}
}
