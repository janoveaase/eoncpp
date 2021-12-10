#include "Regression.h"



namespace eon
{
	TEST( TerminalTest, styles )
	{
		term << "This test requires manual inspection of the output!\n\n";

		term << "normal:    " << style::normal << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "strong:    " << style::strong << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "weak:      " << style::weak << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "black:     " << style::black << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "dark:      " << style::dark << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "white:     " << style::white << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "bright:    " << style::bright << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "red:       " << style::red << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "green:     " << style::green << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "yellow:    " << style::yellow << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "blue:      " << style::blue << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "magenta:   " << style::magenta << " The quick brown fox jumped over the lazy dog "
			<< style::normal << "\n";
		term << "cyan:      " << style::cyan << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "note:      " << style::note << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "warning:   " << style::warning << " The quick brown fox jumped over the lazy dog "
			<< style::normal << "\n";
		term << "error:     " << style::error << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "success:   " << style::success << " The quick brown fox jumped over the lazy dog "
			<< style::normal << "\n";
		term << "quote:     " << style::quote << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
		term << "reference: " << style::reference << " The quick brown fox jumped over the lazy dog "
			<< style::normal << "\n";
		term << "code:      " << style::code << " The quick brown fox jumped over the lazy dog " << style::normal << "\n";
	}

	TEST( TerminalTest, viewport )
	{
		term << "This test requires manual inspection of the output!\n\n";

		term << " " << eon::string().padRight( term.width() - 2, '-' ) << " \n";
		term << "|" << eon::string().padRight( term.width() - 2 ) << "|\n";
		term << "|  " << style::note << eon::string().padRight( term.width() - 6 ) << style::normal << "  |\n";
		term << "|  " << style::note << " This frame should fill the terminal window's"
			<< eon::string().padRight( term.width() - 51 )
			<< style::normal << "  |\n";
		term << "|  " << style::note << " visible area, with a newline character at"
			<< eon::string().padRight( term.width() - 48 )
			<< style::normal << "  |\n";
		term << "|  " << style::note << " the end of each line." << eon::string().padRight( term.width() - 28 )
			<< style::normal << "  |\n";
		term << "|  " << style::note << eon::string().padRight( term.width() - 6 ) << style::normal << "  |\n";
		for( size_t line = 7; line < term.height(); ++line )
			term << "|" + eon::string().padRight( term.width() - 2 ) << "|\n";
		term << " " << eon::string().padRight( term.width() - 2, '-' ) << " \n";
	}
}
