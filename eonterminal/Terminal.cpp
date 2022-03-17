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
		if( found == Styles.end() )
			*this << "<<invalid style>>";
		else
			_setStyle( found->second );
		return *this;
	}




#ifdef EON_WINDOWS
	Terminal::color Terminal::_fg( WORD raw, WORD& out )
	{
		out = 0;
		bool bright = raw & FOREGROUND_INTENSITY;
		if( bright )
			out |= FOREGROUND_INTENSITY;
		if( raw & FOREGROUND_RED )
		{
			out |= FOREGROUND_RED;
			if( raw & FOREGROUND_GREEN )
			{
				out |= FOREGROUND_GREEN;
				if( raw & FOREGROUND_BLUE )
				{
					out |= FOREGROUND_BLUE;
					return bright ? Terminal::color::white : Terminal::color::bright;
				}
				else
					return bright ? Terminal::color::magenta_bright : Terminal::color::magenta_dark;
			}
			else
				return bright ? Terminal::color::red_bright : Terminal::color::red_dark;
		}
		else if( raw & FOREGROUND_GREEN )
		{
			out |= FOREGROUND_GREEN;
			if( raw & FOREGROUND_BLUE )
			{
				out |= FOREGROUND_BLUE;
				return bright ? Terminal::color::cyan_bright : Terminal::color::cyan_dark;
			}
			else
				return bright ? Terminal::color::green_bright : Terminal::color::green_dark;
		}
		else if( raw & FOREGROUND_BLUE )
		{
			out |= FOREGROUND_BLUE;
			return bright ? Terminal::color::blue_bright : Terminal::color::blue_dark;
		}
		else
			return bright ? Terminal::color::dark : Terminal::color::black;
	}
	Terminal::color Terminal::_bg( WORD raw, WORD& out )
	{
		out = 0;
		bool bright = raw & BACKGROUND_INTENSITY;
		if( bright )
			out |= BACKGROUND_INTENSITY;
		if( raw & BACKGROUND_RED )
		{
			out |= BACKGROUND_RED;
			if( raw & BACKGROUND_GREEN )
			{
				out |= BACKGROUND_GREEN;
				if( raw & BACKGROUND_BLUE )
				{
					out |= BACKGROUND_BLUE;
					return bright ? Terminal::color::white : Terminal::color::bright;
				}
				else
					return bright ? Terminal::color::magenta_bright : Terminal::color::magenta_dark;
			}
			else
				return bright ? Terminal::color::red_bright : Terminal::color::red_dark;
		}
		else if( raw & BACKGROUND_GREEN )
		{
			out |= BACKGROUND_GREEN;
			if( raw & BACKGROUND_BLUE )
			{
				out |= BACKGROUND_BLUE;
				return bright ? Terminal::color::cyan_bright : Terminal::color::cyan_dark;
			}
			else
				return bright ? Terminal::color::green_bright : Terminal::color::green_dark;
		}
		else if( raw & BACKGROUND_BLUE )
		{
			out |= BACKGROUND_BLUE;
			return bright ? Terminal::color::blue_bright : Terminal::color::blue_dark;
		}
		else
			return bright ? Terminal::color::dark : Terminal::color::black;
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

		Foreground = _fg( buffer.wAttributes, FGColors[ color::normal] );
		Background = _bg( buffer.wAttributes, BGColors[ color::normal ] );

		FGColors[ color::black ] = 0;
		FGColors[ color::dark ] = FOREGROUND_INTENSITY;
		FGColors[ color::red_dark ] = FOREGROUND_RED;
		FGColors[ color::red_bright ] = FOREGROUND_INTENSITY | FOREGROUND_RED;
		FGColors[ color::green_dark ] = FOREGROUND_GREEN;
		FGColors[ color::green_bright ] = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
		FGColors[ color::yellow_dark ] = FOREGROUND_RED | FOREGROUND_GREEN;
		FGColors[ color::yellow_bright ] = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
		FGColors[ color::blue_dark ] = FOREGROUND_BLUE;
		FGColors[ color::blue_bright ] = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
		FGColors[ color::magenta_dark ] = FOREGROUND_BLUE | FOREGROUND_RED;
		FGColors[ color::magenta_bright ] = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED;
		FGColors[ color::cyan_dark ] = FOREGROUND_BLUE | FOREGROUND_GREEN;
		FGColors[ color::cyan_bright ] = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;
		FGColors[ color::bright ] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		FGColors[ color::white ] = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

		BGColors[ color::black ] = 0;
		BGColors[ color::dark ] = BACKGROUND_INTENSITY;
		BGColors[ color::red_dark ] = BACKGROUND_RED;
		BGColors[ color::red_bright ] = BACKGROUND_INTENSITY | BACKGROUND_RED;
		BGColors[ color::green_dark ] = BACKGROUND_GREEN;
		BGColors[ color::green_bright ] = BACKGROUND_INTENSITY | BACKGROUND_GREEN;
		BGColors[ color::yellow_dark ] = BACKGROUND_RED | BACKGROUND_GREEN;
		BGColors[ color::yellow_bright ] = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN;
		BGColors[ color::blue_dark ] = BACKGROUND_BLUE;
		BGColors[ color::blue_bright ] = BACKGROUND_INTENSITY | BACKGROUND_BLUE;
		BGColors[ color::magenta_dark ] = BACKGROUND_BLUE | BACKGROUND_RED;
		BGColors[ color::magenta_bright ] = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED;
		BGColors[ color::cyan_dark ] = BACKGROUND_BLUE | BACKGROUND_GREEN;
		BGColors[ color::cyan_bright ] = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN;
		BGColors[ color::bright ] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
		BGColors[ color::white ] = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

		switch( Background )
		{
			case color::black:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::red_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::red_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::green_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::green_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::yellow_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::yellow_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::blue_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::blue_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::magenta_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::magenta_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::cyan_dark:
				FGColors[ color::red ] = FGColors[ color::red_bright ];
				FGColors[ color::green ] = FGColors[ color::green_bright ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_bright ];
				FGColors[ color::blue ] = FGColors[ color::blue_bright ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_bright ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_bright ];
				BGColors[ color::red ] = BGColors[ color::red_dark ];
				BGColors[ color::green ] = BGColors[ color::green_dark ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_dark ];
				BGColors[ color::blue ] = BGColors[ color::blue_dark ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_dark ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_dark ];
				break;
			case color::cyan_bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::bright:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
			case color::white:
				FGColors[ color::red ] = FGColors[ color::red_dark ];
				FGColors[ color::green ] = FGColors[ color::green_dark ];
				FGColors[ color::yellow ] = FGColors[ color::yellow_dark ];
				FGColors[ color::blue ] = FGColors[ color::blue_dark ];
				FGColors[ color::magenta ] = FGColors[ color::magenta_dark ];
				FGColors[ color::cyan ] = FGColors[ color::cyan_dark ];
				BGColors[ color::red ] = BGColors[ color::red_bright ];
				BGColors[ color::green ] = BGColors[ color::green_bright ];
				BGColors[ color::yellow ] = BGColors[ color::yellow_bright ];
				BGColors[ color::blue ] = BGColors[ color::blue_bright ];
				BGColors[ color::magenta ] = BGColors[ color::magenta_bright ];
				BGColors[ color::cyan ] = BGColors[ color::cyan_bright ];
				break;
		}
		Styles[ style::normal ] = { color::normal, color::normal };
		Styles[ style::black ] = { color::black, color::normal };
		Styles[ style::dark ] = { color::dark, color::normal };
		Styles[ style::white ] = { color::white, color::normal };
		Styles[ style::bright ] = { color::bright, color::normal };
		Styles[ style::red ] = { color::red, color::normal };
		Styles[ style::green ] = { color::green, color::normal };
		Styles[ style::yellow ] = { color::yellow, color::normal };
		Styles[ style::blue ] = { color::blue, color::normal };
		Styles[ style::magenta ] = { color::magenta, color::normal };
		Styles[ style::cyan ] = { color::cyan, color::normal };

		switch( Foreground )
		{
			case color::black:
				Styles[ style::strong ] = { color::black, color::normal };
				Styles[ style::weak ] = { color::dark, color::normal };
				break;
			case color::dark:
				Styles[ style::strong ] = { color::black, color::normal };
				Styles[ style::weak ] = { color::bright, color::normal };
				break;
			case color::red_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::red_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::green_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::green_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::yellow_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::yellow_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::blue_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::blue_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::magenta_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::magenta_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::cyan_dark:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::cyan_bright:
				Styles[ style::strong ] = { color::normal, color::normal };
				Styles[ style::weak ] = { color::normal, color::normal };
				break;
			case color::bright:
				Styles[ style::strong ] = { color::white, color::normal };
				Styles[ style::weak ] = { color::dark, color::normal };
				break;
			case color::white:
				Styles[ style::strong ] = { color::white, color::normal };
				Styles[ style::weak ] = { color::bright, color::normal };
				break;
		}
		Styles[ style::note ] = { color::white, color::dark };
		Styles[ style::warning ] = { color::white, color::yellow_dark };
		Styles[ style::error ] = { color::white, color::red_bright };
		Styles[ style::success ] = { color::white, color::green_dark };
		Styles[ style::quote ] = { color::blue_dark, color::yellow_bright };
		Styles[ style::reference ] = { color::yellow_dark, color::normal };
		Styles[ style::code ] = { color::red_dark, color::bright };

		Init = true;
	}
	void Terminal::_setColors() noexcept
	{
		SetConsoleTextAttribute( Console, FGColors[ Foreground ] | BGColors[ Background ] );
	}
#else
	void Terminal::_checkPlatform() noexcept
	{
		winsize size;
		ioctl( STDOUT_FILENO, TIOCGWINSZ, &size );
		Width = size.ws_col;
		Height = size.ws_row;

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

		Styles[ style::normal ] = StyleDetails();
		Styles[ style::black ] = StyleDetails( color::black, intensity::dark );
		Styles[ style::dark ] = StyleDetails( color::black, intensity::bright );
		Styles[ style::white ] = StyleDetails( color::white, intensity::bright );
		Styles[ style::bright ] = StyleDetails( color::white, intensity::dark );
		Styles[ style::cyan ] = StyleDetails( color::cyan );

		if( Background == intensity::bright )
		{
			Styles[ style::red ] = StyleDetails( color::red );
			Styles[ style::green ] = StyleDetails( color::green );
			Styles[ style::yellow ] = StyleDetails( color::yellow );
			Styles[ style::blue ] = StyleDetails( color::blue );
			Styles[ style::magenta ] = StyleDetails( color::magenta );

			Styles[ style::strong ] = StyleDetails( color::black, intensity::dark, fontstyle::bold );
			Styles[ style::weak ] = StyleDetails( color::white, intensity::dark );
			Styles[ style::note ] = StyleDetails( color::black, intensity::normal, fontstyle::normal, color::white, intensity::bright );
			Styles[ style::warning ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::yellow, intensity::dark );
			Styles[ style::error ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::red );
			Styles[ style::success ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::green );
			Styles[ style::quote ] = StyleDetails( color::magenta, intensity::dark, fontstyle::normal, color::white, intensity::bright );
			Styles[ style::reference ] = StyleDetails( color::yellow, intensity::dark, fontstyle::normal, color::white, intensity::bright );
			Styles[ style::code ] = StyleDetails( color::cyan, intensity::dark, fontstyle::normal, color::white, intensity::bright );
		}
		else if( Background == intensity::dark )
		{
			Styles[ style::red ] = StyleDetails( color::red, intensity::bright );
			Styles[ style::green ] = StyleDetails( color::green, intensity::bright );
			Styles[ style::yellow ] = StyleDetails( color::yellow, intensity::bright );
			Styles[ style::blue ] = StyleDetails( color::blue, intensity::bright );
			Styles[ style::magenta ] = StyleDetails( color::magenta, intensity::bright );

			Styles[ style::strong ] = StyleDetails( color::white, intensity::bright, fontstyle::bold );
			Styles[ style::weak ] = StyleDetails( color::black, intensity::bright );
			Styles[ style::note ] = StyleDetails( color::white, intensity::normal, fontstyle::normal, color::black, intensity::normal );
			Styles[ style::warning ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::yellow, intensity::dark );
			Styles[ style::error ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::red );
			Styles[ style::success ] = StyleDetails( color::white, intensity::bright, fontstyle::normal, color::green );
			Styles[ style::quote ] = StyleDetails( color::magenta, intensity::bright, fontstyle::normal, color::black, intensity::normal );
			Styles[ style::reference ] = StyleDetails( color::yellow, intensity::bright, fontstyle::normal, color::black, intensity::normal );
			Styles[ style::code ] = StyleDetails( color::cyan, intensity::bright, fontstyle::normal, color::black, intensity::normal );
		}

		Init = true;
	}
#endif

#ifdef EON_WINDOWS
	void Terminal::_setStyle( const StyleDetails& details )
	{
	}
#else
	void Terminal::_setStyle( const StyleDetails& details )
	{
		if( details.Foreground == color::normal || details.Background == color::normal )
			std::cout << "\033[0m";
		static std::string format;
		format = "\033[";
		if( details.Foreground != color::normal )
		{
			if( details.FStyle == fontstyle::bold )
				format += "1;";
			switch( details.FGIntensity )
			{
				case intensity::bright:
					format += std::to_string( FGColors[ details.Foreground ] + 60 );
					break;
				default:
					format += std::to_string( FGColors[ details.Foreground ] );
					break;
			}
		}
		if( details.Background != color::normal )
		{
			if( format[ format.size() - 1 ] != '[' )
				format += ";";
			switch( details.BGIntensity )
			{
				case intensity::bright:
					format += std::to_string( BGColors[ details.Background ] + 60 );
					break;
				default:
					format += std::to_string( BGColors[ details.Background ] );
					break;
			}
		}
		format += "m";
		std::cout << format;
	}
#endif
}
