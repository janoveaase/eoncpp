#pragma once

#include <filesystem>
#include <eonexcept/Exception.h>
#include <eonstring/String.h>
#include <eonterminal/Terminal.h>
#include "TestFactory.h"
#include "TestName.h"
#include "Details.h"
#include "Location.h"
#include "ValueAsString.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// Super-class for all tests.
	class TestBase
	{
	public:
		TestBase() = delete;
		inline TestBase( TestName name ) noexcept { Details.Name = std::move( name ); }
		inline bool runTest() { test_body(); return Details.Success; }

	public:

		// A test reference, to be stored and used when a specific test is to be run.
		struct TestRef
		{
			TestRef() = default;
			inline TestRef( TestName name, TestLocation location, FactoryMain* factory ) noexcept {
				Name = std::move( name ); Location = std::move( location ); Factory = factory; }
			TestName Name;
			TestLocation Location;
			FactoryMain* Factory{ nullptr };
		};

	protected:
		virtual void test_body() = 0;

		virtual eon::string canRun() const = 0;

	public:
		static bool registerTest( TestName name, TestLocation location, FactoryMain* test );

	public:
		TestDetails Details;

	public:
		static std::unique_ptr<std::list<TestRef>> TestsList;
		static std::unique_ptr<std::set<TestName>> TestNames;
	};

	// Super-class for tests without sandbox.
	class EonTest : public TestBase
	{
	public:
		EonTest() = delete;
		EonTest( TestName name ) : TestBase( name ) {}
		eon::string canRun() const override { return eon::string::Empty; }
	};

	// Super-class for tests with sandbox.
	class EonTestSandbox : public TestBase
	{
	public:
		EonTestSandbox() = delete;
		EonTestSandbox( TestName name ) : TestBase( name ) { _prepareSandboxDir(); }
		virtual ~EonTestSandbox() {}
		eon::string canRun() const override { return sandboxDir().empty() ? "Could not create sandbox!" : ""; }
		const std::filesystem::path& sandboxDir() const noexcept { return SandboxDir; }
		const eon::string& sandboxDirStr() const noexcept { return SandboxDirStr; }

	private:
		void _prepareSandboxDir() noexcept;
		bool _ensureNewDir( std::filesystem::path& path ) noexcept;
		bool _enum( std::filesystem::path& path );
		bool _removeDir( const std::filesystem::path& path ) noexcept;
		bool _makeDir( const std::filesystem::path& path ) noexcept;

	private:
		std::filesystem::path SandboxDir;
		eon::string SandboxDirStr;
	};




	// Binary comparison methods
	class BinaryOp
	{
	public:
		static inline bool eq( const char* a, const char* b ) { return eon::string( a ) == eon::string( b ); }
		template<typename T1>
		static inline bool eq( const T1& a, const char* b ) { return eon::string( a ) == eon::string( b ); }
		template<typename T2>
		static inline bool eq( const char* a, const T2& b ) { return eon::string( a ) == eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool eq( const T1& a, const T2& b ) { return a == b; }

		static inline bool ne( const char* a, const char* b ) { return eon::string( a ) != eon::string( b ); }
		template<typename T1>
		static inline bool ne( const T1& a, const char* b ) { return eon::string( a ) != eon::string( b ); }
		template<typename T2>
		static inline bool ne( const char* a, const T2& b ) { return eon::string( a ) != eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool ne( const T1& a, const T2& b ) { return a != b; }

		static inline bool lt( const char* a, const char* b ) { return eon::string( a ) < eon::string( b ); }
		template<typename T1>
		static inline bool lt( const T1& a, const char* b ) { return eon::string( a ) < eon::string( b ); }
		template<typename T2>
		static inline bool lt( const char* a, const T2& b ) { return eon::string( a ) < eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool lt( const T1& a, const T2& b ) { return a < b; }

		static inline bool le( const char* a, const char* b ) { return eon::string( a ) <= eon::string( b ); }
		template<typename T1>
		static inline bool le( const T1& a, const char* b ) { return eon::string( a ) <= eon::string( b ); }
		template<typename T2>
		static inline bool le( const char* a, const T2& b ) { return eon::string( a ) <= eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool le( const T1& a, const T2& b ) { return a <= b; }

		static inline bool gt( const char* a, const char* b ) { return eon::string( a ) > eon::string( b ); }
		template<typename T1>
		static inline bool gt( const T1& a, const char* b ) { return eon::string( a ) > eon::string( b ); }
		template<typename T2>
		static inline bool gt( const char* a, const T2& b ) { return eon::string( a ) > eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool gt( const T1& a, const T2& b ) { return a > b; }

		static inline bool ge( const char* a, const char* b ) { return eon::string( a ) >= eon::string( b ); }
		template<typename T1>
		static inline bool ge( const T1& a, const char* b ) { return eon::string( a ) >= eon::string( b ); }
		template<typename T2>
		static inline bool ge( const char* a, const T2& b ) { return eon::string( a ) >= eon::string( b ); }
		template<typename T1, typename T2>
		static inline bool ge( const T1& a, const T2& b ) { return a >= b; }
	};




	// Class for constructing self-deleting non-copyable arrays of primitive type values.
	// (Constructing e.g., a "const unsigned char[4] = { 'a', 'b', 'c', 'd' }"
	// does not work well in macros.)
	template<typename primitive_T>
	class PrimitiveArray
	{
	public:
		inline PrimitiveArray() { Array = new primitive_T[ 1 ]; Array[ 0 ] = 0; }
		inline PrimitiveArray( std::initializer_list<primitive_T> elements )
		{
			Array = new primitive_T[ elements.size() + 1 ];
			size_t i = 0;
			for( auto element : elements )
				Array[ i++ ] = element;
		}
		PrimitiveArray( const PrimitiveArray& other ) = delete;
		inline PrimitiveArray( PrimitiveArray&& other ) noexcept { Array = other.Array; other.Array = nullptr; }
		~PrimitiveArray() { if( Array != nullptr ) delete[] Array; }

		PrimitiveArray& operator=( const PrimitiveArray& ) = delete;
		PrimitiveArray& operator=( PrimitiveArray&& other ) = delete;

		inline const primitive_T* value() const noexcept { return Array; }

	private:
		primitive_T* Array{ nullptr };
	};


	// Work-around for when "unsigned char" is not working in test macros.
	using uchar_t = unsigned char;
}
