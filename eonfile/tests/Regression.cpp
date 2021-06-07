#include "Regression.h"



namespace eon
{
	TEST( FileTest, binary_read_write )
	{
		binaryfilewriter writer( testfile );
		binaryfilereader reader( testfile );

		std::string read, written{ "01aA?\b\t" };
		written += '\0';

		REQUIRE_NO_EXCEPT( writer.open() ) << "Failed to open output file";
		REQUIRE_NO_EXCEPT( writer.write( written ) ) << "Failed to write";
		writer.close();

		REQUIRE_NO_EXCEPT( reader.open() ) << "Failed to open input file";
		REQUIRE_NO_EXCEPT( read = reader.read( 99 ) ) << "Failed to read";
		WANT_EQ( written, read ) << "Wrong data read";
	}

	TEST( FileTest, text_read_write )
	{
		textfilewriter writer( testfile );
		textfilereader reader( testfile );

		string read, written{ "01aA?\b\t" };
		written += '\0';
		written += char_t( 0x20AC );
		written += char_t( 0xD8 );

		REQUIRE_NO_EXCEPT( writer.open() ) << "Failed to open output file";
		REQUIRE_NO_EXCEPT( writer.write( written ) ) << "Failed to write";
		writer.close();

		REQUIRE_NO_EXCEPT( reader.open() ) << "Failed to open input file";
		REQUIRE_NO_EXCEPT( read = reader.read( 99 ) ) << "Failed to read";
		WANT_EQ( written, read ) << "Wrong data read";
	}
}
