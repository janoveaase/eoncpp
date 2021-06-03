#pragma once

#include <eonexcept/Exception.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception throw when trying to append a complete/non-relative path.
	EONEXCEPT( CompletePath );




	/**************************************************************************
	  Eon Path Class - eon::path

	  Paths in Æon consist of 5 elements (which need not all be present, and
	  where some will overlap):

	  1. Protocol - like "file", "tcp", "https", etc.
	  2. Root - like "/" on Unix or "C:" or UNC host on Windows
	     Does not include protocol!
		 NOTE: "/" will only be considered root if the next directory is
		       a standard top level directory or an existing top level
			   directory on the local host!
	  3. Directory - like "/home/john" on Unix or "C:\Users\John" on Windows
	     Does not include protocol, but will include root (if any)!
	  4. Base - base name
	     Includes extension (if any)
	  5. Extension - everything following the last point (if any) in the base
	     name

	  Æon paths consistently use forward slashes as separator - also on
	  Windows. Use the [eon::path::native()] method to get the path as a
	  string, formatted according to the current system you are running on.
	  Or use [eon::path::windows()] to get the path as a string specifically
	  formatted for Windows.
	**************************************************************************/
	class path
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		path() = default;
		inline path( const path& other ) { *this = other; }
		inline path( path&& other ) noexcept { *this = std::move( other ); }

		inline path( const string& raw_path ) { *this = raw_path; }
		inline path( const substring& raw_path ) { *this = raw_path; }

		explicit inline path( const std::string& raw_path ) {
			*this = raw_path; }
		explicit inline path( const char* raw_path ) { *this = raw_path; }




		/**********************************************************************
		  Assignment Methods
		**********************************************************************/
	public:

		path& operator=( const path& other );
		path& operator=( path&& other ) noexcept;

		inline path& operator=( const string& raw_path ) {
			return *this = raw_path.substr(); }
		path& operator=( const substring& raw_path );

		inline path& operator=( const std::string& raw_path ) {
			return *this = substring( raw_path ); }
		inline path& operator=( const char* raw_path ) {
			return *this = substring( raw_path ); }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if path is empty
		inline bool empty() const noexcept { return Raw.empty(); }

		//* Check that the path is not empty
		inline operator bool() const noexcept { return !Raw.empty(); }

		//* Get number of UTF-8 characaters in the path
		inline size_t numChars() const noexcept { return Raw.numChars(); }


		//* Check if a protocol is present
		inline bool hasProtocol() const noexcept { return !Protocol.empty(); }

		//* Check if the path is relative (missing protocol and root elements)
		inline bool isRelative() const noexcept { return Root.empty(); }


		//* Get protocol element - 'false' substring if not present
		inline const substring& protocol() const noexcept { return Protocol; }

		//* Get a copy of the path without the protocol element
		inline substring pathWithoutProtocol() const noexcept { return
			Protocol ? Raw.substr( Protocol.end() + 3 ) : Raw.substr(); }

		//* Get root element - 'false' substring if not present
		inline const substring& root() const noexcept { return Root; }

		//* Get a copy of the path without the root (and the protocol) element
		substring pathWithoutRoot() const noexcept;

		//* Get directory element - 'false' substring if not present
		inline const substring& dir() const noexcept { return Dir; }

		//* Get base element - 'false' substring if not present
		inline const substring& base() const noexcept { return Base; }

		//* Get base element without extension
		inline substring baseWithoutExt() const noexcept { return Ext.empty()
			? Base : Raw.substr( Base.begin(), Ext.begin() - 1 ); }

		//* Get extension element - 'false' substring if not present
		inline const substring& ext() const noexcept { return Ext; }


		//* Get a copy of 'this' path, but with the 'base' element removed
		path parent() const;


		//* Get the path as a string
		inline const string& str() const noexcept { return Raw; }
		inline operator const string&() const noexcept { return Raw; }
		inline const string& operator*() const noexcept { return Raw; }
		inline const string* operator->() const noexcept { return &Raw; }

		//* Get the path as std::string
		inline const std::string& stdstr() const noexcept {
			return Raw.stdstr(); }

		//* Get the path as a C-string
		inline const char* c_str() const noexcept { return Raw.c_str(); }

		//* Get path as string, adjusted to local platform
		string native() const;

		//* Get path as string, specifically for Windows
		inline string windows() const {
			return Raw.replace( ForwSlashChr, BackSlashChr ); }



		/******************************************************************
		  Write Methods
		******************************************************************/
	public:

		//* Clear the path (make it empty)
		inline void clear() noexcept {
			Raw.clear(); Protocol.clear(); Root.clear(); Dir.clear();
			Base.clear(); Ext.clear(); }

		//* Extend 'this' path with that of another
		//* Throws [eon::CompletePath] exception if 'other' is a complete path
		//* (has protocol and/or root)!
		//* NOTE: If a path starts with a slash - other.root() == '/' (root on
		//* Unix) and is longer than 1 character (more than just the slash
		//* alone), and you know it isn't a complete path, use
		//* 'other.pathWithoutRoot()' to avoid the exception!
		path& operator+=( const path& other );
		
		//* Extend 'this' path with what's in 'str'
		//* This is the same as calling += path( str )!
		inline path& operator+=( const string& str ) {
			return *this += path( str ); }
		inline path& operator+=( const std::string& str ) {
			return *this += path( str ); }
		inline path& operator+=( const char* str ) {
			return *this += path( str ); }

		//* Make sure the path ends in a slash
		//* This will effectively clear base and extension and extend the dir
		//* element.
		path& endInSlash();

		//* Make sure the path does not end in a slash
		//* This will (possibly) shrink the dir element and create base and
		//* extension.
		path& noEndSlash();


		//* Remove element from path
		path& removeProtocol();
		path& removeRoot();
		path& removeBase();
		path& removeExt();


		//* Make path contain current working directory
		path& cwd();




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:
		void quickParse( const substring& path );
		void identifyElements();
		void identifyProtocol();
		void identifyRoot();
		void identifyDir();
		void identifyBase();




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		string Raw;
		substring Protocol;
		substring Root;
		substring Dir;
		substring Base;
		substring Ext;
	};
}
