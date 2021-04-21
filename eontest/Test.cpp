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
		}
	}
}
