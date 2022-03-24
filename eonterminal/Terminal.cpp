#include "Terminal.h"
#ifdef EON_WINDOWS
#	include <Windows.h>
#else
#	include <sys/ioctl.h>
#	include <unistd.h>
#endif


namespace eon
{
	Terminal term;




	Terminal& Terminal::operator<<( string data )
	{
		if( !Init )
			_checkPlatform();
		std::cout << data;
		return *this;
	}

	Terminal& Terminal::operator<<( style term_style )
	{
		if( !Init )
			_checkPlatform();
		auto found = Styles.find( term_style );
		if( found != Styles.end() )
			_setStyle( found->second );
		return *this;
	}




#ifdef EON_WINDOWS
	Terminal::Color Terminal::_fg( WORD raw )
	{
		Color fg;
		fg.Intensity = ( raw & FOREGROUND_INTENSITY ) ? intensity::bright : intensity::normal;
		if( raw & FOREGROUND_RED )
		{
			if( raw & FOREGROUND_GREEN )
			{
				if( raw & FOREGROUND_BLUE )
					fg.Name = color::white;
				else
					fg.Name = color::magenta;
			}
			else
				fg.Name = color::red;
		}
		else if( raw & FOREGROUND_GREEN )
		{
			if( raw & FOREGROUND_BLUE )
				fg.Name = color::cyan;
			else
				fg.Name = color::green;
		}
		else if( raw & FOREGROUND_BLUE )
			fg.Name = color::blue;
		else
			fg.Name = color::black;
		return fg;
	}
	Terminal::Color Terminal::_bg( WORD raw )
	{
		Color bg;
		bg.Intensity = (raw & BACKGROUND_INTENSITY) ? intensity::bright : intensity::normal;
		if( raw & BACKGROUND_RED )
		{
			if( raw & BACKGROUND_GREEN )
			{
				if( raw & BACKGROUND_BLUE )
					bg.Name = color::white;
				else
					bg.Name = color::magenta;
			}
			else
				bg.Name = color::red;
		}
		else if( raw & BACKGROUND_GREEN )
		{
			if( raw & BACKGROUND_BLUE )
				bg.Name = color::cyan;
			else
				bg.Name = color::green;
		}
		else if( raw & BACKGROUND_BLUE )
			bg.Name = color::blue;
		else
			bg.Name = color::black;
		return bg;
	}
	void Terminal::_checkPlatform() noexcept
	{
		Console = GetStdHandle( STD_OUTPUT_HANDLE );
		if( Console == INVALID_HANDLE_VALUE || Console == nullptr )
		{
			Level = level::non_functioning;
			return;
		}
		CONSOLE_SCREEN_BUFFER_INFO buffer;
		if( !GetConsoleScreenBufferInfo( Console, &buffer ) )
		{
			Level = level::non_functioning;
			return;
		}
		MaxWidth = buffer.dwSize.X;
		MaxHeight = buffer.dwSize.Y;
		Width = buffer.srWindow.Right;
		Height = buffer.srWindow.Bottom;
		CX = buffer.dwCursorPosition.X;
		CY = buffer.dwCursorPosition.Y;

		auto fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		auto bg = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		FGColors[ color::normal ] = buffer.wAttributes & fg;
		FGColors[ color::black ] = 0;
		FGColors[ color::white ] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		FGColors[ color::red ] = FOREGROUND_RED;
		FGColors[ color::green ] = FOREGROUND_GREEN;
		FGColors[ color::yellow ] = FOREGROUND_RED | FOREGROUND_GREEN;
		FGColors[ color::blue ] = FOREGROUND_BLUE;
		FGColors[ color::magenta ] = FOREGROUND_RED | FOREGROUND_BLUE;
		FGColors[ color::cyan ] = FOREGROUND_GREEN | FOREGROUND_BLUE;

		BGColors[ color::normal ] = buffer.wAttributes & bg;
		BGColors[ color::black ] = 0;
		BGColors[ color::white ] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
		BGColors[ color::red ] = BACKGROUND_RED;
		BGColors[ color::green ] = BACKGROUND_GREEN;
		BGColors[ color::yellow ] = BACKGROUND_RED | BACKGROUND_GREEN;
		BGColors[ color::blue ] = BACKGROUND_BLUE;
		BGColors[ color::magenta ] = BACKGROUND_RED | BACKGROUND_BLUE;
		BGColors[ color::cyan ] = BACKGROUND_GREEN | BACKGROUND_BLUE;

		Styles[ style::normal ] = StyleDetails();
		Styles[ style::black ] = StyleDetails( Color( color::black ) );
		Styles[ style::dark ] = StyleDetails( Color( color::black ) );
		Styles[ style::white ] = StyleDetails( Color( color::white ) );
		Styles[ style::bright ] = StyleDetails( Color( color::white ) );
		Styles[ style::red ] = StyleDetails( Color( color::red ) );
		Styles[ style::green ] = StyleDetails( Color( color::green ) );
		Styles[ style::yellow ] = StyleDetails( Color( color::yellow ) );
		Styles[ style::blue ] = StyleDetails( Color( color::blue ) );
		Styles[ style::magenta ] = StyleDetails( Color( color::magenta ) );
		Styles[ style::cyan ] = StyleDetails( Color( color::cyan ) );

		auto background = _bg( buffer.wAttributes );
		bool bg_bright = background.Intensity == intensity::bright || background.Name == color::yellow || background.Name == color::white;
		_prepareStyles( bg_bright ? intensity::bright : intensity::dark );

		Init = true;
	}
#else
	void Terminal::_checkPlatform() noexcept
	{
		winsize size;
		ioctl( STDOUT_FILENO, TIOCGWINSZ, &size );
		Width = size.ws_col;
		Height = size.ws_row - 1;

		FGColors[ color::normal ] = -1;
		FGColors[ color::black ] = 30;
		FGColors[ color::white ] = 37;
		FGColors[ color::red ] = 31;
		FGColors[ color::green ] = 32;
		FGColors[ color::yellow ] = 33;
		FGColors[ color::blue ] = 34;
		FGColors[ color::magenta ] = 35;
		FGColors[ color::cyan ] = 36;

		BGColors[ color::normal ] = -1;
		BGColors[ color::black ] = 40;
		BGColors[ color::white ] = 47;
		BGColors[ color::red ] = 41;
		BGColors[ color::green ] = 42;
		BGColors[ color::yellow ] = 43;
		BGColors[ color::blue ] = 44;
		BGColors[ color::magenta ] = 45;
		BGColors[ color::cyan ] = 46;

		_prepareStyles( Background );

		Init = true;
	}
#endif

	void Terminal::_prepareStyles( intensity background )
	{

		Styles[ style::normal ] = StyleDetails();
		Styles[ style::black ] = StyleDetails( Color( color::black, intensity::dark ) );
		Styles[ style::dark ] = StyleDetails( Color( color::black, intensity::bright ) );
		Styles[ style::white ] = StyleDetails( Color( color::white, intensity::bright ) );
		Styles[ style::bright ] = StyleDetails( Color( color::white, intensity::dark ) );
		Styles[ style::cyan ] = StyleDetails( Color( color::cyan ) );

		if( background == intensity::bright )
		{
			Styles[ style::red ] = StyleDetails( Color( color::red ) );
			Styles[ style::green ] = StyleDetails( Color( color::green ) );
			Styles[ style::yellow ] = StyleDetails( Color( color::yellow ) );
			Styles[ style::blue ] = StyleDetails( Color( color::blue ) );
			Styles[ style::magenta ] = StyleDetails( Color( color::magenta ) );

			Styles[ style::strong ] = StyleDetails( Color( color::black, intensity::dark ), fontstyle::bold );
			Styles[ style::weak ] = StyleDetails( Color( color::white, intensity::dark ) );
			Styles[ style::note ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::black, intensity::bright ) );
			Styles[ style::warning ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::yellow, intensity::dark ) );
			Styles[ style::error ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::red ) );
			Styles[ style::success ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::green ) );
			Styles[ style::quote ] = StyleDetails( Color( color::blue, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
			Styles[ style::reference ] = StyleDetails( Color( color::black, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
			Styles[ style::code ] = StyleDetails( Color( color::magenta, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
		}
		else if( background == intensity::dark )
		{
			Styles[ style::red ] = StyleDetails( Color( color::red, intensity::bright ) );
			Styles[ style::green ] = StyleDetails( Color( color::green, intensity::bright ) );
			Styles[ style::yellow ] = StyleDetails( Color( color::yellow, intensity::bright ) );
			Styles[ style::blue ] = StyleDetails( Color( color::blue, intensity::bright ) );
			Styles[ style::magenta ] = StyleDetails( Color( color::magenta, intensity::bright ) );

			Styles[ style::strong ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::bold );
			Styles[ style::weak ] = StyleDetails( Color( color::black, intensity::bright ) );
			Styles[ style::note ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::black, intensity::bright ) );
			Styles[ style::warning ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::yellow, intensity::dark ) );
			Styles[ style::error ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::red ) );
			Styles[ style::success ] = StyleDetails( Color( color::white, intensity::bright ), fontstyle::normal, Color( color::green ) );
			Styles[ style::quote ] = StyleDetails( Color( color::blue, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
			Styles[ style::reference ] = StyleDetails( Color( color::black, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
			Styles[ style::code ] = StyleDetails( Color( color::magenta, intensity::dark ), fontstyle::normal, Color( color::white, intensity::dark ) );
		}
	}

#ifdef EON_WINDOWS
	void Terminal::_setStyle( const StyleDetails& details )
	{
		auto color = FGColors[ details.Foreground.Name ] | BGColors[ details.Background.Name ];
		if( details.Foreground.Intensity == intensity::bright )
			color |= FOREGROUND_INTENSITY;
		if( details.Background.Intensity == intensity::bright )
			color |= BACKGROUND_INTENSITY;
		SetConsoleTextAttribute( Console, color );
	}
#else
	void Terminal::_setStyle( const StyleDetails& details )
	{
		if( details.Foreground.Name == color::normal || details.Background.Name == color::normal )
			std::cout << "\033[0m";
		static std::string format;
		format = "\033[";
		if( details.Foreground.Name != color::normal )
		{
			if( details.FStyle == fontstyle::bold )
				format += "1;";
			switch( details.Foreground.Intensity )
			{
				case intensity::bright:
					format += std::to_string( FGColors[ details.Foreground.Name ] + 60 );
					break;
				default:
					format += std::to_string( FGColors[ details.Foreground.Name ] );
					break;
			}
		}
		if( details.Background.Name != color::normal )
		{
			if( format[ format.size() - 1 ] != '[' )
				format += ";";
			switch( details.Background.Intensity )
			{
				case intensity::bright:
					format += std::to_string( BGColors[ details.Background.Name ] + 60 );
					break;
				default:
					format += std::to_string( BGColors[ details.Background.Name ] );
					break;
			}
		}
		format += "m";
		std::cout << format;
	}
#endif
}
