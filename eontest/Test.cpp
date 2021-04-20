#include "Test.h"


namespace eontest
{
	std::list<EonTest::TestRef>* EonTest::Tests{ nullptr };
//	std::map<std::string, std::map<std::string, FactoryMain*>>* EonTest::Tests{ nullptr };
	std::list<std::string>* EonTest::Classes{ nullptr };
//	std::list<std::string> EonTest::Failed;


	bool EonTest::registerTest(
		const std::string& test_class, const std::string& test_name,
		const std::string& line, FactoryMain* test )
	{
		if( Tests == nullptr )
		{
			Tests = new std::list<TestRef>();
			Classes = new std::list<std::string>();
		}
		if( std::find( Classes->begin(), Classes->end(), test_class ) == Classes->end() )
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
//			Failed.clear();
		}
	}

/*
	bool Tester::run( const std::string& filter )
	{
		std::regex filt( filter.stdstr() );
		for( auto& class_name : *Classes )
		{
			auto& cls = (*Tests)[ class_name ];
			for( auto& nam : cls )
			{
				if( !filter.empty() && !std::regex_match( class_name.stdstr()
					+ "." + nam.first.stdstr(), filt ) )
					continue;

				std::cout << "Running " << class_name.stdstr() << "."
					<< nam.first.stdstr() << ": ";
				try
				{
					auto test = nam.second->createTest(
						class_name, nam.first );
					test->test_body();
					delete test;
				}
				catch( eon::Exception& e )
				{
					fail( class_name + "." + nam.first, "", {
						"Exception while running test", e.what() } );
				}
				catch( std::exception& e )
				{
					fail( class_name + "." + nam.first, "", {
						"Std::exception while running test", e.what() } );
				}
				catch( ... )
				{
					fail( class_name + "." + nam.first, "", {
						"Unknown exception while running test" } );
				}

				if( FailMessages.empty() )
					std::cout << "OK\n";
				else
				{
					Failed.push_back( class_name + "." + nam.first );
					std::cout << "FAIL\n";
					for( auto& message : FailMessages )
					{
						auto lines = message.splitSequential<std::vector<
							eon::substring>>( eon::NewlineChr );
						for( auto& line : lines )
							std::cout << "  " << line.stdstr() << "\n";
					}
					FailMessages.clear();
				}
				if( !InfoMessages.empty() )
				{
					for( auto& message : InfoMessages )
						std::cout << "  " << message.stdstr() << "\n";
					InfoMessages.clear();
				}
			}
		}

		if( Failed.empty() )
			std::cout
			<< "\n-- ALL OK ----------------------------------------\n";
		else
		{
			std::cout
				<< "\n-- FAIL ------------------------------------------\n";
			for( auto& name : Failed )
				std::cout << "\n  - " << name.stdstr();
		}
		std::cout << "\n";
		return Failed.empty();
	}



	std::string Tester::encode( const std::string& in )
	{
		std::string out;
		for( auto c : in )
		{
			switch( c )
			{
				case '\a':
					out += "\\a";
					break;
				case '\b':
					out += "\\b";
					break;
				case '\f':
					out += "\\f";
					break;
				case '\t':
					out += "\\t";
					break;
				case '\n':
					out += "\\n";
					break;
				case '\r':
					out += "\\r";
					break;
				case '\v':
					out += "\\v";
					break;
				case '\\':
					out += "\\\\";
					break;
				default:
					out += c;
			}
		}
		return out;
	}
	bool Tester::_testEq(
		const std::string& expected, const std::string& actual,
		const std::string& test_id, const std::string& line )
	{
		if( expected == actual )
			return true;
		if( expected.findFirst( '\n' ) )
		{
			std::vector<std::string> errors;
			errors.push_back( "Expected equal values, but they're not!" );
			auto exp = expected.splitSequential<std::vector<eon::substring>>(
				eon::NewlineChr );
			auto act = expected.splitSequential<std::vector<eon::substring>>(
				eon::NewlineChr );
			size_t l = 0;
			for( ; l < exp.size() && l < act.size(); ++l )
			{
				if( exp[ l ] == act[ l ] )
					continue;
				errors.push_back(
					"line " + std::string::toString( l + 1 ) + ":\n" );
				errors.push_back(
					"Expected: \"" + exp[ l ].stdstr() + "\"" );
				errors.push_back(
					"Actual  : \"" + act[ l ].stdstr() + "\"" );
			}
			fail( test_id, line, errors );
		}
		else
		{
			fail( test_id, line, {
				"Expected equal values, but they're not!",
				"  Expected : \"" + encode( expected ) + "\"",
				"    Actual : \"" + encode( actual ) + "\"" } );
		}
		return false;
	}*/
}
