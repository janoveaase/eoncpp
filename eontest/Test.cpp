#include "Test.h"


namespace eontest
{
	std::list<EonTest::TestRef>* EonTest::Tests{ nullptr };
	std::list<std::string>* EonTest::Classes{ nullptr };


	bool EonTest::registerTest(
		const std::string& test_class, const std::string& test_name,
		const std::string& line, FactoryMain* test )
	{
		if( Tests == nullptr )
		{
			Tests = new std::list<TestRef>();
			Classes = new std::list<std::string>();
		}
		if( std::find( Classes->begin(), Classes->end(), test_class )
			== Classes->end() )
			Classes->push_back( test_class );
		Tests->push_back( TestRef( test_class, test_name, test ) );
		return true;
	}

	void EonTest::reset() noexcept
	{
		if( Tests )
		{
			for( auto& cls : *Tests )
				delete cls.Factory;
			delete Tests;
			delete Classes;
			Tests = nullptr;
			Classes = nullptr;
		}
	}



	bool EonTest::_testEq(
		const std::string& expected, const std::string& actual,
		const char* exp_expr, const char* act_expr )
	{
		if( expected == actual )
			return true;
		Failed = true;
		size_t start_pos{ 0 };
		auto exp_lines = _splitLines( expected );
		auto act_lines = _splitLines( actual );
		auto diff_line = _findFirstDiffLine( exp_lines, act_lines );
		if( diff_line == exp_lines.size() || diff_line == act_lines.size() )
		{
			_Messages << "\nFailed to compare equal (at line "
				<< std::to_string( diff_line + 1 ) << ")\n";
			_Messages << "Expected expression: --|" << exp_expr << "|--\n";
			_Messages << "  Actual expression: --|" << act_expr << "|--\n";
			if( diff_line < exp_lines.size() )
				_Messages << "Expected value: \"" << _extractLine(
					exp_lines[ diff_line ], 0, 79 - 19, start_pos ) << "\"\n";
			else
				_Messages << "  Actual value: \"" << _extractLine(
					act_lines[ diff_line ], 0, 79 - 19, start_pos ) << "\"\n";
		}
		else
		{
			auto diff_pos = _findFirstDiffPos(
				exp_lines[ diff_line ], act_lines[ diff_line ] );
			_Messages << "\nFailed to compare equal (at line "
				<< std::to_string( diff_line + 1 ) << ", position "
				<< std::to_string( diff_pos + 1 ) << ")\n";
			_Messages << "Expected expression: --|" << exp_expr << "|--\n";
			_Messages << "  Actual expression: --|" << act_expr << "|--\n";
			_Messages << "Expected value: \"" << _extractLine(
				exp_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			_Messages << "  Actual value: \"" << _extractLine(
				act_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			_Messages << std::string( diff_pos - start_pos + 17, ' ' ) << "^\n";
			std::string marker{ "Different here" };
			if( diff_pos > marker.size() + 2 )
				_Messages << std::string( diff_pos - start_pos + 17 - marker.size() - 1, ' ' )
					<< marker << "-'\n";
			else
				_Messages << std::string( diff_pos - start_pos + 17, ' ' ) << "'-"
				<< marker << "\n";
		}
		return false;
	}



	std::vector<std::string> EonTest::_splitLines( const std::string& str )
		const
	{
		std::vector<std::string> lines{ std::string() };
		for( auto c : str )
		{
			if( c == '\n' )
				lines.push_back( std::string() );
			else
				lines[ lines.size() - 1 ] += c;
		}
		return lines;
	}
	size_t EonTest::_findFirstDiffLine(
		const std::vector<std::string>& expected,
		const std::vector<std::string>& actual ) const
	{
		size_t diff_line = expected.size() < actual.size() ? expected.size()
			: actual.size();
		for( size_t line = 0;
			line < expected.size() && line < actual.size();
			++line )
		{
			if( expected[ line ] != actual[ line ] )
			{
				diff_line = line;
				break;
			}
		}
		return diff_line;
	}
	size_t EonTest::_findFirstDiffPos( const std::string& expected,
		const std::string& actual ) const
	{
		for( size_t i = 0; i < expected.size() && i < actual.size(); ++i )
		{
			if( expected[ i ] != actual[ i ] )
				return i;
		}
		return expected.size() < actual.size() ? expected.size()
			: actual.size();
	}
	std::string EonTest::_extractLine( const std::string& line,
		size_t diff_pos, size_t available_size, size_t& start_pos ) const
	{
		if( line.size() < available_size )
		{
			start_pos = 0;
			return line;
		}
		size_t start{ 0 }, end{ 0 };
		if( diff_pos <= 13 )
			start_pos = 0;
		else
		{
			start = diff_pos - 13;
			start_pos = start - 3;
		}
		end = available_size - diff_pos;
		if( end < line.size() )
			end -= 3;

		std::string result;
		if( start > 0 )
			result += "...";
		result += line.substr( start, end - start );
		if( end < line.size() )
			result += "...";
		return result;
	}
}
