#pragma once

#include <eonexcept/Exception.h>
#include <eonstring/String.h>
#include <eoninlinetest/InlineTest.h>
#include <filesystem>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	namespace filesys
	{
		// Exception thrown when trying to construct an illegal path
		EONEXCEPT( BadPath );

		// Exception thrown when a file or directory operation fails
		EONEXCEPT( Failure );
	};



	///////////////////////////////////////////////////////////////////////////
	// Eon Path Class - eon::path
	//
	// Eon uses forward slash as default directory separator on all platforms.
	// Use the 'systemStr' method to get the path as a string with the
	// specified system's directory separator used.
	//
	// Eon will consider any path/name ending with a slash as a directory, and
	// any path/name not ending with slash as a file.
	// 
	// NOTE: Path objects can refer to non-existing directories and files!
	//
	// Paths consists of the following elements:
	//
	//   1. Root (Examples: "C:/", "//myserver/", "/")
	//      A path with root is 'absolute', a path without is 'relative'!
	//   2. Directories
	//      Zero or more names followed by slash, not including the root and
	//      not including the last directory name if it is a directory path.
	//   3. Parent
	//      Root and directories.
	//   4. Base
	//      Everything except parent. (The last directory name/file name.)
	//   5. Name
	//      The base minus the last dot and whatever follows it.
	//   6. Extension
	//      Whatever follows the last dot in the name
	//
	// WARNING: Concatenating two absolute paths is an error!
	//
	// NOTE: Paths are always normalized upon creation!
	//
	class path
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
		// WARNING: Will throw eon::filesys::BadPath if illegal path is
		//          attempted constructed.
		//
	public:
		path() = default;
		inline path( const path& other ) { *this = other; }
		inline path( path&& other ) noexcept { *this = std::move( other ); }

		inline path( const string& raw_path ) { *this = raw_path; }
		inline path( const substring& raw_path ) { *this = raw_path; }
		inline path( const std::filesystem::path& std_path ) { *this = std_path; }
		inline path( const std::string& raw_path ) { *this = raw_path; }
		inline path( const char* raw_path ) { *this = raw_path ; }
		inline path( char_t chr ) { *this = chr; }

		inline virtual ~path() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Assignment Methods
		//
	public:

		inline path& operator=( const path& other ) { Full = other.Full; return _parse( other ); }
		inline path& operator=( path&& other ) noexcept { Full = std::move( other.Full ); return _parse( other ); }
		inline path& operator=( const string& raw_path ) { return *this = raw_path.substr(); }
		inline path& operator=( const substring& raw_path ) { return _parse( raw_path ); }
		inline path& operator=( const std::filesystem::path& std_path ) { return *this = substring( std_path.string() ); }
		inline path& operator=( const std::string& raw_path ) { return *this = substring( raw_path ); }
		inline path& operator=( const char* raw_path ) { return *this = substring( raw_path ); }
		inline path& operator=( char_t chr ) { return _parse( string( chr ) ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if the path is empty
		inline bool empty() const noexcept { return Name.empty(); }

		// Check if the path is non-empty
		inline operator bool() const noexcept { return !Name.empty(); }

		// Check if the path is a directory or file path
		inline bool dir() const noexcept { return Name.empty() || Name.endsWith( ForwSlashChr ); }
		inline bool file() const noexcept { return !Name.empty() && !Name.endsWith( ForwSlashChr ); }

		// Check if the path is absolute or relative
		inline bool absolute() const noexcept { return !Root.empty(); }
		inline bool relative() const noexcept { return Root.empty(); }

		// Check if the path represents an existing file or directory
		inline bool exists() const noexcept { return std::filesystem::exists( std::filesystem::path( Full.stdstr() ) ); }

		// Check if the path is to an existing directory or file
		// Note that a file path can return true for isDir and vice versa!
		inline bool isDir() const { return std::filesystem::is_directory( std::filesystem::path( Full.stdstr() ) ); }
		inline bool isFile() const { return !isDir(); }

		// Check if parts/elements are present
		inline bool isRoot() const noexcept { return Root && !( Dirs || Base ); }
		inline bool hasRoot() const noexcept { return Root; }
		inline bool hasDirs() const noexcept { return Dirs; }
		inline bool hasParent() const noexcept { return Parent; }
		inline bool isBase() const noexcept { return Base && !Root && !Dirs; }
		inline bool hasBase() const noexcept { return Base; }
		inline bool hasName() const noexcept { return Name; }
		inline bool isExt() const noexcept { return Ext && !Name && !Root && !Dirs; }
		inline bool hasExt() const noexcept { return Ext; }

		// Get path parts/elements
		inline const substring& root() const noexcept { return Root; }
		inline const substring& dirs() const noexcept { return Dirs; }
		inline const substring& parent() const noexcept { return Parent; }
		inline const substring& base() const noexcept { return Base; }
		inline const substring& name() const noexcept { return Name; }
		inline const substring& ext() const noexcept { return Ext; }

		// String conversion
		inline const string& str() const noexcept { return Full; }
		inline const std::string& stdstr() const noexcept { return Full.stdstr(); }
		inline const char* c_str() const noexcept { return Full.c_str(); }

		// Std path
		inline std::filesystem::path stdpath() const { return std::filesystem::path( Full.stdstr() ); }
		inline operator std::filesystem::path() const { return std::filesystem::path( Full.stdstr() ); }

		// System path (using specified system's directory separator)
		// The 'system_name' argument can be one of: 'windows', 'posix'
		// Unknown system will result in default separator being used!
		string systemStr( name_t system_name );




		///////////////////////////////////////////////////////////////////////
		//
		// Concatenation
		//
		// WARNING: Will throw eon::filesys::BadPath if illegal path is
		//          attempted constructed.
		//
		// Use the '/' and '/=' operators to concatenate whole elements to the
		// path!
		//
		// NOTE: Concatenating to a file path will make the original file name
		//       a directory name!
		//
	public:

		path& operator/=( const path& other );
		inline path& operator/=( const string& raw_path ) { return *this /= path( raw_path ); }
		inline path& operator/=( const substring& raw_path ) { return *this /= path( raw_path ); }
		inline path& operator/=( const std::filesystem::path& std_path ) { return *this /= path( std_path ); }
		inline path& operator/=( const std::string& raw_path ) { return *this /= path( raw_path ); }
		inline path& operator/=( const char* raw_path ) { return *this /= path( raw_path ); }
		inline path& operator/=( char_t chr ) { return *this /= Full + chr; }

		friend inline path operator/( const path& a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, const string& b ) { return path( a ) /= b; }
		friend inline path operator/( const string& a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, const substring& b ) { return path( a ) /= b; }
		friend inline path operator/( const substring& a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, const std::filesystem::path& b ) { return path( a ) /= b; }
		friend inline path operator/( const std::filesystem::path& a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, const std::string& b ) { return path( a ) /= b; }
		friend inline path operator/( const std::string& a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, const char* b ) { return path( a ) /= b; }
		friend inline path operator/( const char* a, const path& b ) { return path( a ) /= b; }
		friend inline path operator/( const path& a, char_t b ) { return path( a ) /= b; }
		friend inline path operator/( char_t a, const path& b ) { return path( a ) /= b; }

		// Make all + and += operators illegal
		path& operator+=( const path& other ) = delete;
		path& operator+=( const string& raw_path ) = delete;
		path& operator+=( const substring& raw_path ) = delete;
		path& operator+=( const std::filesystem::path& std_path ) = delete;
		path& operator+=( const std::string& raw_path ) = delete;
		path& operator+=( const char* raw_path ) = delete;
		path& operator+=( char_t chr ) = delete;
		friend path operator+( const path& a, const path& b ) = delete;
		friend path operator+( const path& a, const string& b ) = delete;
		friend path operator+( const string& a, const path& b ) = delete;
		friend path operator+( const path& a, const substring& b ) = delete;
		friend path operator+( const substring& a, const path& b ) = delete;
		friend path operator+( const path& a, const std::filesystem::path& b ) = delete;
		friend path operator+( const std::filesystem::path& a, const path& b ) = delete;
		friend path operator+( const path& a, const std::string& b ) = delete;
		friend path operator+( const std::string& a, const path& b ) = delete;
		friend path operator+( const path& a, const char* b ) = delete;
		friend path operator+( const char* a, const path& b ) = delete;
		friend path operator+( const path& a, char_t b ) = delete;
		friend path operator+( char_t a, const path& b ) = delete;




		///////////////////////////////////////////////////////////////////////
		//
		// Write Methods
		//
	public:

		// Clear the path (make it empty)
		inline void clear() noexcept {
			Root.clear(); Dirs.clear(); Parent.clear(); Base.clear(); Name.clear(); Ext.clear(); Full.clear(); }

		// Set/change root element
		// NOTE: A value will make the path absolute, while empty will make it
		//       relative!
		inline path& root( const substring& elm ) { return _parse( Full = elm + Dirs + Base ); }
		inline path& root( const string& elm ) { return root( elm.substr() ); }
		inline path& root( const std::string& elm ) { return root( substring( elm ) ); }
		inline path& root( const char* elm ) { return root( substring( elm ) ); }

		// Set/change directories element
		inline path& dirs( const substring& part ) { return _parse( Full = Root + part + Base ); }
		inline path& dirs( const string& part ) { return dirs( part.substr() ); }
		inline path& dirs( const std::string& part ) { return dirs( substring( part ) ); }
		inline path& dirs( const char* part ) { return dirs( substring( part ) ); }

		// Set/change the parent part
		// NOTE: Including the root will make the path absolute, omitting will
		//       make it relative!
		inline path& parent( const substring& part ) { return _parse( Full = part + Base ); }
		inline path& parent( const string& part ) { return parent( part.substr() ); }
		inline path& parent( const std::string& part ) { return parent( substring( part ) ); }
		inline path& parent( const char* part ) { return parent( substring( part ) ); }

		// Set/change the base element
		inline path& base( const substring& elm ) { return _parse( Full = Parent + elm ); }
		inline path& base( const string& elm ) { return base( elm.substr() ); }
		inline path& base( const std::string& elm ) { return base( substring( elm ) ); }
		inline path& base( const char* elm ) { return base( substring( elm ) ); }

		// Set/change the name part
		// NOTE: Name ending in slash will make it a directory path, otherwise
		//       a file path
		// Changing a file path to a directory path by this method will cause
		// the extension (if any) to be lost!
		inline path& name( const substring& part ) { return _parse( Parent + part ); }
		inline path& name( const string& part ) { return name( part.substr() ); }
		inline path& name( const std::string& part ) { return name( substring( part ) ); }
		inline path& name( const char* part ) { return name( substring( part ) ); }

		// Set/change the extension part
		// NOTE: Setting an extension to a directory is an error!
		path& ext( const substring& part );
		inline path& ext( const string& part ) { return ext( part.substr() ); }
		inline path& ext( const std::string& part ) { return ext( substring( part ) ); }
		inline path& ext( const char* part ) { return ext( substring( part ) ); }

		// Flip path from directory to file (remove trailing slash)
		inline path& makeFilePath() { return dir() ? *this = Full.slice( 0, -2 ) : *this; }

		// Flip path from file to directory (add trailing slash)
		inline path& makeDirPath() { return file() ? *this = Full + ForwSlashChr : *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		inline int compare( const path& other ) const noexcept { return Full.compare( other.Full ); }

		inline bool operator<( const path& other ) const noexcept { return Full < other.Full; }
		inline bool operator<=( const path& other ) const noexcept { return Full <= other.Full; }
		inline bool operator>( const path& other ) const noexcept { return Full > other.Full; }
		inline bool operator>=( const path& other ) const noexcept { return Full >= other.Full; }
		inline bool operator==( const path& other ) const noexcept { return Full == other.Full; }
		inline bool operator!=( const path& other ) const noexcept { return Full != other.Full; }


		

		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		inline path& _parse( const string& full ) { return _parse( full.substr() ); }
		path& _parse( const substring& full );
		std::list<string> _rawSplit( const substring& full ) const;
		bool _normalize( std::list<string>& elements ) const;
		bool _reconstruct( std::list<string>& elements );
		path& _parse( const path& other ) noexcept;

		bool _isRoot( const string& str ) noexcept;

		path& _ext( const substring& part );



		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		string Full;
		substring Root;
		substring Dirs;
		substring Parent;
		substring Base;
		substring Name;
		substring Ext;
	};
}
