#include "Test.h"


namespace eontest
{
	std::list<EonTest::TestRef>* EonTest::_EonTests_{ nullptr };
	std::list<std::string>* EonTest::_EonClasses_{ nullptr };


	bool EonTest::createSandboxDir( const std::filesystem::path& target, std::error_code& error ) noexcept
	{
		if( !removeSandboxDir( target, error ) )
			return false;
		return std::filesystem::create_directories( target, error );
	}
	bool EonTest::removeSandboxDir( const std::filesystem::path& target, std::error_code& error ) noexcept
	{
		if( std::filesystem::exists( target ) )
			return std::filesystem::remove_all( target, error );
		return true;
	}


	bool EonTest::_registerEonTest_( const std::string& test_class, const std::string& test_name, const std::string& line,
		FactoryMain* test )
	{
		if( _EonTests_ == nullptr )
		{
			_EonTests_ = new std::list<TestRef>();
			_EonClasses_ = new std::list<std::string>();
		}
		if( std::find( _EonClasses_->begin(), _EonClasses_->end(), test_class ) == _EonClasses_->end() )
			_EonClasses_->push_back( test_class );
		_EonTests_->push_back( TestRef( test_class, test_name, test ) );
		return true;
	}

	void EonTest::_resetEon_() noexcept
	{
		if( _EonTests_ )
		{
			for( auto& cls : *_EonTests_ )
				delete cls.Factory;
			delete _EonTests_;
			delete _EonClasses_;
			_EonTests_ = nullptr;
			_EonClasses_ = nullptr;
		}
	}

	std::string EonTest::_encodeEon_( const std::string& str, size_t& diffpos )
	{
		auto start = diffpos;
		std::string encoded;
		size_t pos = 0;
		for( auto c : str )
		{
			switch( c )
			{
				case '\\':
					encoded += "\\\\";
					if( pos < start )
						++diffpos;
					break;
				case '\0':
					encoded += "\\0";
					if( pos < start )
						++diffpos;
					break;
				case '\b':
					encoded += "\\b";
					if( pos < start )
						++diffpos;
					break;
				case '\t':
					encoded += "\\t";
					if( pos < start )
						++diffpos;
					break;
				case '\n':
					encoded += c;
					break;
				case '\v':
					encoded += "\\v";
					if( pos < start )
						++diffpos;
					break;
				case '\r':
					encoded += "\\r";
					if( pos < start )
						++diffpos;
					break;
				default:
				{
					if( c < ' ' || c > '~' )
					{
						auto enc = "&#" + std::to_string( static_cast<int>( c ) ) + ";";
						encoded += enc;
						if( pos < start )
							diffpos += enc.size() - 1;
					}
					else
						encoded += c;
				}
			}
			++pos;
		}
		return encoded;
	}



	bool EonTest::_reportDiff( const std::string& expected, const std::string& actual,
		const char* exp_expr, const char* act_expr )
	{
		Failed = true;
		size_t start_pos{ 0 };
		size_t dummy1{ SIZE_MAX }, dummy2{ SIZE_MAX };
		auto exp_lines = _splitLines( _encodeEon_( expected, dummy1 ) );
		auto act_lines = _splitLines( _encodeEon_( actual, dummy2 ) );
		auto diff_line = _findFirstDiffLine( exp_lines, act_lines );
		eon::term << eon::style::strong << "Expected expression: " << eon::style::green << exp_expr << eon::style::normal
			<< "\n";
		eon::term << eon::style::strong << "  Actual expression: " << eon::style::red << act_expr << eon::style::normal
			<< "\n";
		if( diff_line == exp_lines.size() || diff_line == act_lines.size() )
		{
			if( diff_line < exp_lines.size() )
			{
				eon::term << eon::style::strong << "Expected line #" << eon::string( diff_line + 1 ) << ": "
					<< eon::style::normal << "\"" << eon::style::green
					<< _extractLine( exp_lines[ diff_line ], 0, 79 - 19, start_pos ) << eon::style::normal << "\"\n";
				eon::term << eon::style::red << "No matching actual line!\n";
			}
			else
			{
				eon::term << "No matching expected line!\n";
				eon::term << "  Actual line #" << eon::string( diff_line + 1 ) << ": \""
					<< _extractLine( act_lines[ diff_line ], 0, 79 - 19, start_pos ) << "\"\n";
			}
		}
		else
		{
			auto diff_pos = _findFirstDiffPos( exp_lines[ diff_line ], act_lines[ diff_line ] );
			eon::term << "Expected line #" << eon::string( diff_line + 1 ) << ": \""
				<< _extractLine( exp_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			eon::term << "  Actual line #" << eon::string( diff_line + 1 ) << ": \""
				<< _extractLine( act_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			eon::term << eon::string( diff_pos - start_pos + 19, ' ' ) << "^\n";
			std::string marker{ "Different here!" };
			if( diff_pos > marker.size() + 2 )
				eon::term << eon::string( diff_pos - start_pos + 18 - marker.size() - 1, ' ' ) << marker << " -'\n";
			else
				eon::term << eon::string( diff_pos - start_pos + 19, ' ' ) << "'- " << marker << "\n";
		}
		return false;
	}



	std::vector<std::string> EonTest::_splitLines( const std::string& str ) const
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
	size_t EonTest::_findFirstDiffLine( const std::vector<std::string>& expected, const std::vector<std::string>& actual )
		const
	{
		size_t diff_line = expected.size() < actual.size() ? expected.size() : actual.size();
		for( size_t line = 0; line < expected.size() && line < actual.size(); ++line )
		{
			if( expected[ line ] != actual[ line ] )
			{
				diff_line = line;
				break;
			}
		}
		return diff_line;
	}
	size_t EonTest::_findFirstDiffPos( const std::string& expected, const std::string& actual ) const
	{
		for( size_t i = 0; i < expected.size() && i < actual.size(); ++i )
		{
			if( expected[ i ] != actual[ i ] )
				return i;
		}
		return expected.size() < actual.size() ? expected.size() : actual.size();
	}
	std::string EonTest::_extractLine( const std::string& line, size_t diff_pos, size_t available_size, size_t& start_pos )
		const
	{
		size_t start{ 0 }, end{ 0 };
		if( diff_pos <= 16 )
			start_pos = 0;
		else
		{
			start = diff_pos - 13;
			start_pos = start - 3;
		}
		end = start_pos + available_size;
		if( end > line.size() )
			end = line.size();
		else if( end < line.size() )
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
