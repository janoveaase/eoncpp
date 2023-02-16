#include "SourceReporter.h"
#include "String.h"
#include <cmath>


namespace eon
{
	namespace source
	{
		string severityToString( severity value )
		{
			switch( value )
			{
				case severity::fatal:
					return "  FATAL";
				case severity::error:
					return "  ERROR";
				case severity::serious:
					return "SERIOUS";
				case severity::warning:
					return "WARNING";
				case severity::note:
					return "   NOTE";
				case severity::info:
					return "   INFO";
				default:
					return "   ?\?\??";
			}
		}




		EON_TEST( Message, Message, default_level,
			EON_EQ( severity::none, Message().level() ) );
		EON_TEST( Message, Message, default_text,
			EON_EQ( "", Message().text() ) );
		EON_TEST( Message, Message, default_source,
			EON_EQ( Ref(), Message().source() ) );

		string Message::str() const
		{
			string s = severityToString( Level );
			if( Source )
				s << _formatSource();
			else if( !Text.empty() )
				s += " " + Text;
			return s;
		}
		EON_TEST_2STEP( Message, str, single_source_line,
			String source( "test", "line 1 line 2 line 3" ),
			EON_EQ( "   NOTE in \"test\" (1:8-1:14): An example\n1|line 1 line 2 line 3\n |-------^^^^^^-------",
				Message( severity::note, "An example", Ref( source, Pos( 7, 7, 0, 7 ), Pos( 13, 13, 0, 13 ) ) ).str() ) );
		EON_TEST_2STEP( Message, str, multiline,
			String source( "test", "line 1\nline 2\nline 3" ),
			EON_EQ( "   NOTE in \"test\" (2:1-2:7): An example\n2|line 2\n |^^^^^^",
				Message( severity::note, "An example", Ref( source, Pos( 7, 7, 1, 0 ), Pos( 13, 13, 1, 6) ) ).str() ) );


		string Message::_formatSource() const
		{
			string s;
			s << " in \"" << Source.name() << "\"";
			if( Source.explicitEnd() )
				s << _formatPosition();
			s << ":";
			if( s.numChars() + Text.numChars() >= 80 )
				s << "\n";
			else
				s << " ";
			s << Text;
			if( Source.explicitEnd() )
				s << _formatSourceIndicator();
			return s;
		}
		EON_TEST_2STEP( Message, _formatSource, single_source_line,
			String source( "test", "line 1 line 2 line 3" ),
			EON_EQ( " in \"test\" (1:8-1:14): An example\n1|line 1 line 2 line 3\n |-------^^^^^^-------",
				Message( severity::note, "An example",
					Ref( source, Pos( 7, 7, 0, 7 ), Pos( 13, 13, 0, 13 ) ) )._formatSource() ) );
		EON_TEST_2STEP( Message, _formatSource, multiline,
			String source( "test", "line 1\nline 2\nline 3" ),
			EON_EQ( " in \"test\" (2:1-2:7): An example\n2|line 2\n |^^^^^^",
				Message( severity::note, "An example",
					Ref( source, Pos( 7, 7, 1, 0 ), Pos( 13, 13, 1, 6 ) ) )._formatSource() ) );

		string Message::_formatPosition() const
		{
			string s;
			s << " (" << Source.startStr();
			if( Source.numChars() > 1 )
				s << "-" << Source.endStr();
			s << ")";
			return s;
		}
		EON_TEST_2STEP( Message, _formatPosition, single_source_line,
			String source( "test", "line 1 line 2 line 3" ),
			EON_EQ( " (1:8-1:14)", Message( severity::note, "An example", Ref( source, Pos( 7, 7, 0, 7 ),
				Pos( 13, 13, 0, 13 ) ) )._formatPosition() ) );
		EON_TEST_2STEP( Message, _formatPosition, multiline,
			String source( "test", "line 1\nline 2\nline 3" ),
			EON_EQ( " (2:1-2:7)", Message( severity::note, "An example", Ref( source, Pos( 7, 7, 1, 0 ),
				Pos( 13, 13, 1, 6 ) ) )._formatPosition() ) );

		Message::SourceIndicator::SourceIndicator( Ref source )
		{
			Source = source;
			Newline = Source.numChars() == 1 && Source.str() == "\n";
			Start = Source.start();
			LineNumDigits = Source.end().Line < 10 ? 1 : static_cast<size_t>(
				log10( static_cast<long double>( Source.end().Line ) ) );
		}
		EON_NO_TEST( Message_SourceIndicator, SourceIndicator );
#ifdef EON_TEST_MODE
		struct Indicator
		{
			Indicator( Pos start = Pos( 12, 12, 1, 5 ), Pos end = Pos( 15, 15, 1, 8 ) )
			{
				Source = String( "test", "line 1\nline two\nthird line" );
				Obj = Message::SourceIndicator( Ref( Source, start, end ) );
			}
			Indicator& startLine( Pos pos )
			{
				Obj.Line = Ref( *(Raw*)&Obj.Source.source(), pos, pos );
				Obj.Line.moveStartToStartOfLine();
				Obj.Line.moveEndToEndOfLine();
				return *this;
			}
			Indicator& moveStart( Pos target ) { Obj.Start = target; return startLine( target ); }
			String Source;
			Message::SourceIndicator Obj;
			string Str;
		};
#endif

		string Message::_formatSourceIndicator() const
		{
			SourceIndicator si( Source );
			return si.format();
		}
		EON_NO_TEST( Message, _formatSourceIndicator );

		string Message::SourceIndicator::format()
		{
			string str;
			for( I = 0; _write( str ); ++I )
				;
			return str;
		}
		EON_TEST_2STEP( Message, format, basic,
			Indicator ind( Pos( 0, 0, 0, 0 ), Pos( 26, 26, 2, 10 ) ),
			EON_EQ( "\n1|line 1\n |^^^^^^\n2|line two\n |^^^^^^^^\n3|third line\n |^^^^^^^^^^", ind.Obj.format() ) );

		bool Message::SourceIndicator::_write( string& str )
		{
			str << _startLine();
			if( Start.Line == Source.start().Line )
				str << string( Start.CharPos - Line.start().CharPos, '-' );
			str << _writeIndicator();
			if( Start.Line == Source.end().Line )
				return false;
			Start = ( (Raw*)&Source.source() )->getPosAtOffset( Line.end(), 1 );
			return true;
		}
		EON_TEST_2STEP( Message_SourceIndicator, _write, true,
			Indicator ind( Pos( 0, 0, 0, 0 ), Pos( 26, 26, 2, 10 ) ),
			EON_TRUE( ind.Obj._write( ind.Str ) ) );
		EON_TEST_3STEP( Message_SourceIndicator, _write, value,
			Indicator ind( Pos( 0, 0, 0, 0 ), Pos( 26, 26, 2, 10 ) ),
			ind.Obj._write( ind.Str ),
			EON_EQ( "\n1|line 1\n |^^^^^^", ind.Str ) );
		EON_TEST_2STEP( Message_SourceIndicator, _write, false,
			Indicator ind( Pos( 0, 0, 0, 0 ), Pos( 26, 26, 2, 10 ) ),
			EON_FALSE( ind.moveStart( Pos( 26, 26, 2, 10 ) ).Obj._write( ind.Str ) ) );

		string Message::SourceIndicator::_startLine()
		{
			string str = "\n";
			Line = Ref( *(Raw*)&Source.source(), Start, Start );
			Line.moveStartToStartOfLine();
			Line.moveEndToEndOfLine();
			str << string( Start.Line + 1 ).padLeft( LineNumDigits, ' ' ) << "|" << Line.str() << "\n";
			str << string().padRight( LineNumDigits, ' ' ) << "|";
			return str;
		}
		EON_TEST_2STEP( Message_SourceIndicator, _startLine, basic,
			Indicator ind,
			EON_EQ( "\n2|line two\n |", ind.Obj._startLine() ) );

		string Message::SourceIndicator::_writeIndicator()
		{
			if( Newline )
				return I == 0 ? "^" : "";
			else
			{
				size_t length = _selectLineLength();
				string str;
				str << string( length, '^' );
				if( Start.Line == Source.end().Line )
					str << string( Line.end().CharPos - Source.end().CharPos, '-' );
				return str;
			}
		}
		EON_TEST_2STEP( Message_SourceIndicator, _writeIndicator, newline_zero,
			Indicator ind( Pos( 6, 6, 0, 6 ), Pos( 7, 7, 1, 0 ) ),
			EON_EQ( "^", ind.Obj._writeIndicator() ) );
		EON_TEST_3STEP( Message_SourceIndicator, _writeIndicator, newline_other,
			Indicator ind( Pos( 6, 6, 0, 6 ), Pos( 7, 7, 1, 0 ) ),
			ind.Obj.I = 1,
			EON_EQ( "", ind.Obj._writeIndicator() ) );
		EON_TEST_2STEP( Message_SourceIndicator, _writeIndicator, line_len,
			Indicator ind( Pos( 3, 3, 0, 3 ), Pos( 6, 6, 0, 6 ) ),
			EON_EQ( "^^^", ind.startLine( Pos( 0, 0, 0, 0 ) ).Obj._writeIndicator() ) );

		size_t Message::SourceIndicator::_selectLineLength()
		{
			if( Source.start().Line == Source.end().Line )
				return Source.end().CharPos - Source.start().CharPos;
			else if( Start.Line == Source.start().Line )
				return Line.end().CharPos - Source.start().CharPos;
			else if( Start.Line == Source.end().Line )
				return Source.end().CharPos - Start.CharPos;
			else
				return Line.end().CharPos - Line.start().CharPos;
		}
		EON_TEST_2STEP( Message_SourceIndicator, _selectLineLength, same_line,
			Indicator ind,
			EON_EQ( 3, ind.Obj._selectLineLength() ) );
		EON_TEST_2STEP( Message_SourceIndicator, _selectLineLength, first_line,
			Indicator ind( Pos( 0, 0, 0, 0 ) ),
			EON_EQ( 6, ind.startLine( Pos( 0, 0, 0, 0 ) ).Obj._selectLineLength() ) );
		EON_TEST_2STEP( Message_SourceIndicator, _selectLineLength, last_line,
			Indicator ind( Pos( 14, 14, 1, 7 ), Pos( 26, 26, 2, 10 ) ),
			EON_EQ( 10, ind.moveStart( Pos( 16, 16, 2, 0 ) ).Obj._selectLineLength() ) );
		EON_TEST_2STEP( Message_SourceIndicator, _selectLineLength, other_line,
			Indicator ind( Pos( 0, 0, 0, 0 ), Pos( 20, 20, 2, 4 ) ),
			EON_EQ( 8, ind.moveStart( Pos( 8, 8, 1, 1 ) ).Obj._selectLineLength() ) );




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
