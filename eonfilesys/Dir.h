#pragma once

#include "Path.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class file;

	/**************************************************************************
	  Eon Directory Class - eon::directory
	**************************************************************************/
	class directory
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		directory() = default;
		inline directory( const directory& other ) { *this = other; }
		inline directory( directory&& other ) noexcept {
			*this = std::move( other ); }

		inline directory( const path& file_path ) { *this = file_path; }
		inline directory( path&& file_path ) {
			*this = std::move( file_path ); }
		inline directory( const string& raw_path ) { *this = raw_path; }
		inline directory( const substring& raw_path ) { *this = raw_path; }
		inline directory( const std::filesystem::path& std_path ) {
			*this = std_path; }
		explicit inline directory( const std::string& raw_path ) {
			*this = raw_path; }
		explicit inline directory( const char* raw_path ) {
			*this = raw_path; }

		inline virtual ~directory() = default;




		/**********************************************************************
		  Assignment Methods
		**********************************************************************/
	public:

		inline directory& operator=( const directory& other ) {
			Path = other.Path; return *this; }
		inline directory& operator=( directory&& other ) noexcept {
			Path = std::move( other.Path ); return *this; }
		
		inline directory& operator=( const path& dir_path ) {
			return *this = eon::path( dir_path ); }
		directory& operator=( path&& dir_path );
		inline directory& operator=( const string& raw_path ) {
			return *this = eon::path( raw_path ).makeDirPath(); }
		inline directory& operator=( const substring& raw_path ) {
			return *this = eon::path( raw_path ).makeDirPath(); }
		inline directory& operator=( const std::filesystem::path& std_path ) {
			return *this = eon::path( std_path ).makeDirPath(); }
		inline directory& operator=( const std::string& raw_path ) {
			return *this = eon::path( raw_path ).makeDirPath(); }
		inline directory& operator=( const char* raw_path ) {
			return *this = std::move( eon::path( raw_path ).makeDirPath() ); }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		// Get the path
		inline const eon::path& path() const noexcept { return Path; }

		// Check if the directory exists
		inline bool exists() const { return Path.exists(); }
		inline operator bool() const noexcept { return Path.exists(); }




		/******************************************************************
		  File system operations

		  WARNING: These will change the file system (if successful)
		  
		  These methods throw filesys::Failure if something goes wrong
		******************************************************************/
	public:

		//* Make sure the directory exists, create it and parents if necessary
		directory& ensureExists();


		//* Rename the directory (inside the same parent directory)
		//* Will fail if a directory with the new name already exists!
		directory& rename( const string& new_name );

		//* Rename the directory (inside the same parent directory)
		//* If a directory exists with the new name, it will be replaced!
		directory& forceRename( const string& new_name );


		//* Move the directory to another parent directory
		//* Will fail if a directory with the same name exists in the target
		//* directory!
		directory& move( const directory& new_parent_dir );

		//* Move the directory to another parent directory
		//* If a directory with the same name exists in the target directory,
		//* it will be replaced
		directory& forceMove( const directory& new_parent_dir );


		//* Copy the directory to a target represented by another directory
		//* object
		//* Will fail if the target already exists
		void copy( const directory& target ) const;

		//* Copy the directory to a target represented by another directory
		//* object
		//* Will replace if the target already exists
		void forceCopy( const directory& target ) const;

		//* Copy the directory to a target represented by another directory
		//* object
		//* Will merge if the target already exists, files will be replaced if
		//* and only if they are different (are older in target).
		void mergeCopy( const directory& target ) const;


		//* Access directory contents using iteration
		class iterator;
		iterator begin() const noexcept;
		iterator end() const noexcept;


		//* Remove directory from the file system - if it exists
		//* NOTE: Will remove any and all contents as well!
		void remove() const;




		/******************************************************************
		  Directory iterator
		******************************************************************/
	public:

		class iterator
		{
		public:
			iterator() = default;
			iterator( const directory& d );

			inline operator bool() const noexcept {
				return Base != std::filesystem::directory_iterator(); }

			inline const eon::path& operator*() const noexcept { return Path; }
			inline const eon::path* operator->() const noexcept {
				return &Path; }

			iterator& operator++() noexcept;

			inline bool operator==( const iterator& other )
				const noexcept { return Base == other.Base; }
			inline bool operator!=( const iterator& other )
				const noexcept { return Base != other.Base; }
		private:
			std::filesystem::directory_iterator Base;
			eon::path Path;
		};




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		eon::path Path;
	};
}
