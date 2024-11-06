#pragma once
#include <eonstring/Name.h>
#include <eonstring/String.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// All [eon::exception] objects must have an [eon::name]. If an
	// [eon::exception] is attempted constructed with [eon::no_name] as name or
	// an arbitrary name value that isn't registered.
	//
	class BadName : public std::exception
	{
	public:
		using std::exception::exception;
		const char* what() const noexcept override { return "Bad Name!"; }
	};




	// Macro for generating sub-classes of [eon::exception]
	// NOTE: Unlike [eon::exception], sub-classes constructed with this macro
	// are constructed without an explicit name (the 'name' macro argument is
	// used as name).
#define EONEXCEPT( exception_name )\
	class exception_name : public ::eon::exception {\
	public:\
		using ::eon::exception::exception;\
		exception_name() : exception( ::eon::name( #exception_name ) ) {}\
		explicit exception_name( const ::eon::string& info ) : exception( ::eon::name( #exception_name ), info ) {}\
		explicit exception_name(\
			::eon::string&& info ) : exception( ::eon::name( #exception_name ), std::move( info ) ) {}\
		explicit exception_name(\
			const ::std::string& info ) : exception( ::eon::name( #exception_name ), ::eon::string( info ) ) {}\
		explicit exception_name(\
			::std::string&& info ) : exception( ::eon::name( #exception_name ), ::eon::string( std::move( info ) ) ) {}\
		explicit exception_name(\
			const char* info ) : exception( ::eon::name( #exception_name ), ::eon::string( info ) ) {}\
	}




	///////////////////////////////////////////////////////////////////////////
	// Eon Exception Class - eon::exception
	//
	// Apart from [eon::string], all Eon classes that may throw explicit
	// exceptions, will throw exceptions of this class or derived from this
	// class.
	//
	class exception : public std::exception
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction is not possible!
		exception() = delete;

		// All exceptions must have at least an [eon::name]
		// Will throw [eon::BadName] if 'name' is [eon::no_name].
		inline explicit exception( name_t name ) { if( name != no_name ) Name = name; else throw BadName(); }

		// Construct exception with name and an 'info' message
		// Will throw [eon::BadName] if 'name' is [eon::no_name].
		inline exception( name_t name, const string& info ) { _set( name, string( info ) ); }
		inline exception( name_t name, string&& info ) { _set( name, std::move( info ) ); }
		inline exception( name_t name, const std::string& info ) { _set( name, string( info ) ); }
		inline exception( name_t name, std::string&& info ) { _set( name, string( std::move( info ) ) ); }
		inline exception( name_t name, const char* info ) { _set( name, string( info ) ); }

		// Copy the 'other' exception
		inline exception( const exception& ) = default;

		// Take ownership of the 'other' exception's details
		inline exception( exception&& ) noexcept = default;

		~exception() override = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
	public:

		// Copy the 'other' exception
		inline exception& operator=( const exception& ) = default;

		// Take ownership of the 'other' exception's details
		inline exception& operator=( exception&& ) noexcept = default;


		// Set or add to 'info' message
		inline exception& info( const string& info ) { Info += info; return*this; }
		inline exception& info( string&& info )
		{
			if( Info.empty() )
				Info = std::move( info );
			else Info += info;
			return *this;
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Get the message info
		const char* what() const noexcept override;

		// Get combined name and info
		virtual string details() const;

		// Get the exception name
		name_t name() const noexcept { return Name; }

		// Get the info message
		const string& info() const noexcept { return Info; }




	private:
		inline void _set( name_t name, string&& info )
		{
			if( name == no_name )
				throw BadName();
			Name = name;
			Info = std::move( info );
		}

		name_t Name{ no_name };
		string Info;
	};
}
