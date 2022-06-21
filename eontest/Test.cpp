#include "Test.h"


namespace eontest
{
	using namespace eon;
	std::list<_EonTest::TestRef>* _EonTest::_EonTests_{ nullptr };
	std::list<std::string>* _EonTest::_EonClasses_{ nullptr };


	bool _EonTest::_registerEonTest_( const std::string& test_class, const std::string& test_name, const std::string& line,
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

	void _EonTest::_resetEon_() noexcept
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

	std::string _EonTest::_encodeEon_( const std::string& str, size_t& diffpos )
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


	bool _EonTest::_testMatch( const string& expected, const string& actual, const char* exp_expr, const char* act_expr )
	{
		bool matches{ true };
		auto exp_lines = expected.splitSequential<std::vector<string>>( '\n' );
		auto act_lines = actual.splitSequential<std::vector<string>>( '\n' );
		size_t lno = 0;
		for( ; lno < exp_lines.size() && lno < act_lines.size(); ++lno )
		{
			if( exp_lines[ lno ].empty() )
			{
				if( act_lines[ lno ].empty() )
					continue;
				else
				{
					matches = false;
					term << style::strong << "Expected expression: " << style::green << exp_expr << style::normal << "\n";
					term << style::strong << "  Actual expression: " << style::red << act_expr << style::normal << "\n";
					term << style::strong << "Expected line #" << ( lno + 1 ) << ": " << style::green << "<empty>"
						<< style::normal << "\n";
					term << style::strong << "  Actual line #" << ( lno + 1 ) << " value: " << style::red
						<< act_lines[ lno ] << style::normal << "\n";
					continue;
				}
			}
			regex exp( exp_lines[ lno ] );
			if( !exp.match( act_lines[ lno ] ) )
			{
				matches = false;
				term << style::strong << "Expected expression: " << style::green << exp_expr << style::normal << "\n";
				term << style::strong << "  Actual expression: " << style::red << act_expr << style::normal << "\n";
				term << style::strong << "Expected line #" << ( lno + 1 ) << " regex: " << style::green << exp_lines[ lno ]
					<< style::normal << "\n";
				term << style::strong << "  Actual line #" << ( lno + 1 ) << " value: " << style::red << act_lines[ lno ]
					<< style::normal << "\n";
			}
		}
		auto missing_act = exp_lines.size() - lno;
		auto superfluous_act = act_lines.size() - lno;
		if( missing_act > 0 || superfluous_act > 0 )
		{
			matches = false;
			term << style::strong << "Expected expression: " << style::green << exp_expr << style::normal << "\n";
			term << style::strong << "  Actual expression: " << style::red << act_expr << style::normal << "\n";
		}
		if( missing_act > 0 )
		{
			if( missing_act == 1 )
				term << style::strong << "Expected line #" << ( lno + 1 ) << ": " << style::red << "No actual line!"
				<< style::normal << "\n";
			else
				term << style::strong << "Expected lines #" << ( lno + 1 ) << "-" << exp_lines.size() << ": " << style::red
				<< "No actual lines!" << style::normal << "\n";
		}
		else if( superfluous_act > 0 )
		{
			if( superfluous_act == 1 )
				term << style::strong << "  Actual line #" << ( lno + 1 ) << ": " << style::red
				<< "Extra line not in expected!" << style::normal << "\n";
			else
				term << style::strong << "  Actual lines #" << ( lno + 1 ) << "-" << act_lines.size() << ": " << style::red
				<< "Extra lines not in expected!" << style::normal << "\n";
		}
		return matches;
	}


	bool _EonTest::_reportDiff( const std::string& expected, const std::string& actual,
		const char* exp_expr, const char* act_expr )
	{
		Failed = true;
		index_t start_pos{ 0 };
		index_t dummy1{ INDEX_MAX }, dummy2{ INDEX_MAX };
		auto exp_lines = _splitLines( _encodeEon_( expected, dummy1 ) );
		auto act_lines = _splitLines( _encodeEon_( actual, dummy2 ) );
		index_t diff_line = _findFirstDiffLine( exp_lines, act_lines );
		term << style::strong << "Expected expression: " << style::green << exp_expr << style::normal << "\n";
		term << style::strong << "  Actual expression: " << style::red << act_expr << style::normal << "\n";
		if( diff_line == exp_lines.size() || diff_line == act_lines.size() )
		{
			if( diff_line < exp_lines.size() )
			{
				term << style::strong << "Expected line #" << string( diff_line + 1 ) << ": " << style::normal << "\""
					<< style::green << _extractLine( exp_lines[ diff_line ], 0, 79 - 19, start_pos ) << style::normal
					<< "\"\n";
				term << style::red << "No matching actual line!\n";
			}
			else
			{
				term << "No matching expected line!\n";
				term << "  Actual line #" << string( diff_line + 1 ) << ": \""
					<< _extractLine( act_lines[ diff_line ], 0, 79 - 19, start_pos ) << "\"\n";
			}
		}
		else
		{
			auto diff_pos = _findFirstDiffPos( exp_lines[ diff_line ], act_lines[ diff_line ] );
			term << "Expected line #" << string( diff_line + 1 ) << ": \""
				<< _extractLine( exp_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			term << "  Actual line #" << string( diff_line + 1 ) << ": \""
				<< _extractLine( act_lines[ diff_line ], diff_pos, 79 - 19, start_pos ) << "\"\n";
			term << string( diff_pos - start_pos + 19, ' ' ) << "^\n";
			std::string marker{ "Different here!" };
			if( diff_pos > marker.size() + 2 )
				term << string( diff_pos - start_pos + 18 - marker.size() - 1, ' ' ) << marker << " -'\n";
			else
				term << string( diff_pos - start_pos + 19, ' ' ) << "'- " << marker << "\n";
		}
		return false;
	}



	std::vector<std::string> _EonTest::_splitLines( const std::string& str ) const
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
	size_t _EonTest::_findFirstDiffLine( const std::vector<std::string>& expected, const std::vector<std::string>& actual )
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
	size_t _EonTest::_findFirstDiffPos( const std::string& expected, const std::string& actual ) const
	{
		for( size_t i = 0; i < expected.size() && i < actual.size(); ++i )
		{
			if( expected[ i ] != actual[ i ] )
				return i;
		}
		return expected.size() < actual.size() ? expected.size() : actual.size();
	}
	std::string _EonTest::_extractLine( const std::string& line, size_t diff_pos, size_t available_size, size_t& start_pos )
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


	EonTestSandbox::EonTestSandbox( string test_class, string test_name, string sandbox_root )
	{
		_prepSandbox( test_class, test_name, sandbox_root );
		Failed = !_createSandbox();
	}
	EonTestSandbox::~EonTestSandbox()
	{
		_removeSandbox();
	}

	void EonTestSandbox::_prepSandbox( string test_class, string test_name, string sandbox_root )
	{
#ifdef EON_WINDOWS
		test_class = test_class.replace( "/", "\\" );
		test_name = test_name.replace( "/", "\\" );
		sandbox_root = sandbox_root.replace( "/", "\\" );
#else
		test_class = test_class.replace( "\\", "/" );
		test_name = test_name.replace( "\\", "/" );
		sandbox_root = sandbox_root.replace( "\\", "/" );
#endif
		std::filesystem::path root{ sandbox_root.stdstr() };
		if( root.empty() )
		{
#ifdef EON_WINDOWS
			char* buffer{ nullptr };
			size_t bufsize{ 0 };
			auto error = _dupenv_s( &buffer, &bufsize, "TMP" );
			if( error == 0 )
			{
				root = buffer;
				free( buffer );
			}
			else
				root = "C:\temp";
#else
			root = "/tmp";
#endif
			root /= "eon_sandbox";
		}
		Sandbox = root / sandbox_root.stdstr();
		if( !test_class.empty() )
			Sandbox /= std::filesystem::path( test_class.stdstr() );
		if( !test_name.empty() )
			Sandbox /= std::filesystem::path( test_name.stdstr() );
	}
	bool EonTestSandbox::_createSandbox() noexcept
	{
		if( Sandbox.empty() )
			return false;
		if( !_removeSandbox() )
			return false;
		std::error_code error;
		if( std::filesystem::create_directories( Sandbox, error ) )
			return true;
		else
		{
			ADD_FAILURE() << "Failed to create sandbox directory \"" + Sandbox.string() + "\": " + error.message();
			return false;
		}
	}
	bool EonTestSandbox::_removeSandbox() noexcept
	{
		if( Sandbox.empty() )
		{
			ADD_FAILURE() << "Have no sandbox directory specified!";
			return false;
		}
		if( !std::filesystem::exists( Sandbox ) )
			return true;
		std::error_code error;
		if( std::filesystem::remove_all( Sandbox, error ) && !std::filesystem::exists( Sandbox ) )
			return true;
		else
		{
			ADD_FAILURE() << "Failed to remove sandbox directory \"" + Sandbox.string() + "\": " + error.message();
			return false;
		}
	}
}
