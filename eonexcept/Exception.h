#pragma once
#include <eonname/Name.h>
#include <eonstring/String.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  All [eon::exception] objects must have an [eon::name]. If an
	  [eon::exception] is attempted constructed with [eon::no_name] as name or
	  an arbitrary name value that isn't registered.
	**************************************************************************/
	class BadName : public std::exception
	{
	public:
		BadName() : std::exception() {}
		const char* what() const noexcept override { return "Bad Name!"; }
	};




	//* Macro for generating sub-classes of [eon::exception]
	//* NOTE: Unlike [eon::exception], sub-classes constructed with this macro
	//* are constructed without an explicit name (the 'name' macro argument is
	//* used as name).
#define EONEXCEPT( exception_name )\
	class exception_name : public ::eon::exception {\
	public:\
		exception_name() : exception( ::eon::name::get( #exception_name ) ) {}\
		exception_name( const ::eon::string& info ) : \
			exception( ::eon::name::get( #exception_name ), info ) {}\
		exception_name( ::eon::string&& info ) : exception( ::eon::name::get( \
			#exception_name ), std::move( info ) ) {}\
	}




	/**************************************************************************
	  Eon Exception Class - eon::exception

	  Apart from [eon::string], all Eon classes that may throw explicit
	  exceptions, will throw exceptions of this class or derived from this
	  class.
	**************************************************************************/
	class exception : public std::exception
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction is not possible!
		exception() = delete;

		//* All exceptions must have at least an [eon::name]
		//* Will throw [eon::BadName] if 'name' is not a registered name
		//* (or [eon::no_name]).
		inline exception( name_t name ) {
			if( name::registered( name ) ) Name = name; else throw BadName(); }

		//* Construct exception with name and an 'info' message
		//* Will throw [eon::BadName] if 'name' is not a registered name
		//* (or [eon::no_name]).
		inline exception( name_t name, const string& info ) {
			if( name::registered( name ) ) { Name = name; Info = info; }
				else throw BadName(); }
		inline exception( name_t name, string&& info ) {
			if( name::registered( name ) ) { Name = name;
				Info = std::move( info ); } else throw BadName(); }

		//* Copy the 'other' exception
		inline exception( const exception& other ) { *this = other; }

		//* Take ownership of the 'other' exception's details
		inline exception( exception&& other ) noexcept {
			*this = std::move( other ); }




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/
	public:

		//* Copy the 'other' exception
		inline exception& operator=( const exception& other ) {
			Name = other.Name; Info = other.Info; return *this; }

		//* Take ownership of the 'other' exception's details
		inline exception& operator=( exception&& other ) noexcept {
			Name = other.Name; Info = std::move( other.Info ); return *this; }


		//* Set or add to 'info' message
		inline exception& info( const string& info ) {
			Info += info; return*this; }
		inline exception& info( string&& info ) { if( Info.empty() ) Info
			= std::move( info ); else Info += info; return *this; }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Get the message info
		virtual const char* what() const noexcept override {
			return Info.empty() ? Name->c_str() : Info.c_str(); }

		//* Get combined name and info
		virtual string details() const {
			return !Info.empty() ? *Name + ": " + Info : *Name; }

		//* Get the exception name
		const name_t name() const noexcept { return Name; }

		//* Get the info message
		const string& info() const noexcept { return Info; }




	private:
		name_t Name{ no_name };
		string Info;
	};
}
