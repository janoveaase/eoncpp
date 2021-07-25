#pragma once

#include "Path.h"
#include "FileBuffer.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon File Class - eon::file
	**************************************************************************/
	class file
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		file() = default;
		inline file( const file& other ) { *this = other; }
		inline file( file&& other ) noexcept { *this = std::move( other ); }

		inline file( const path& file_path ) { *this = file_path; }
		inline file( path&& file_path ) { *this = std::move( file_path ); }
		inline file( const string& raw_path ) { *this = raw_path; }
		inline file( const substring& raw_path ) { *this = raw_path; }
		inline file( const std::filesystem::path& std_path ) {
			*this = std_path; }
		explicit inline file( const std::string& raw_path ) {
			*this = raw_path; }
		explicit inline file( const char* raw_path ) { *this = raw_path; }

		inline virtual ~file() = default;




		/**********************************************************************
		  Assignment Methods
		**********************************************************************/
	public:

		inline file& operator=( const file& other ) {
			Path = other.Path; return *this; }
		inline file& operator=( file&& other ) noexcept {
			Path = std::move( other.Path ); return *this; }

		inline file& operator=( const path& file_path ) {
			return *this = eon::path( file_path ); }
		file& operator=( path&& file_path );
		inline file& operator=( const string& raw_path ) {
			return *this = eon::path( raw_path ).makeFilePath(); }
		inline file& operator=( const substring& raw_path ) {
			return *this = eon::path( raw_path ).makeFilePath(); }
		inline file& operator=( const std::filesystem::path& std_path ) {
			return *this = eon::path( std_path ).makeFilePath(); }
		inline file& operator=( const std::string& raw_path ) {
			return *this = eon::path( raw_path ).makeFilePath(); }
		inline file& operator=( const char* raw_path ) {
			return *this = eon::path( raw_path ).makeFilePath(); }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		// Get the path
		inline const eon::path& path() const noexcept { return Path; }

		// Check if the file exists
		inline bool exists() const { return Path.exists(); }
		inline operator bool() const noexcept { return Path.exists(); }

		// Get file size in bytes
		std::uintmax_t size() const {
			return std::filesystem::file_size( Path.stdpath() ); }




		/******************************************************************
		  File system operations

		  WARNING: These will change the file system (if successful)
		  
		  These methods throw filesys::Failure if something goes wrong
		******************************************************************/
	public:

		//* Create empty file if it does not exist, update last modified time
		//* if it does.
		void touch() const;


		//* Rename the file (inside the same directory)
		//* Will fail if a file with the new name already exists!
		file& rename( const string& new_name );

		//* Rename the file (inside the same directory)
		//* If a file exists with the new name, it will be replaced!
		file& forceRename( const string& new_name );


		//* Move the file to another directory
		//* Will fail if a file with the same name exists in the target
		//* directory!
		file& move( const eon::path& target_dir );

		//* Move the file to another directory
		//* If a file with the same name exists in the target directory, it
		//* will be replaced
		file& forceMove( const eon::path& target_dir );


		//* Copy the file to a target represented by another file object
		//* Will fail if the target already exists
		void copy( const file& target ) const;

		//* Copy the file to a target represented by another file object
		//* Will replace if the target already exists
		void forceCopy( const file& target ) const;


		//* Load the contents of the file
		string loadText( std::uintmax_t max_chars = UINTMAX_MAX );
		std::string loadBinary( std::uintmax_t max_bytes = UINTMAX_MAX );

		//* Load the file, line by line
		class iterator;
		iterator begin() const noexcept;
		iterator end() const noexcept;


		//* Save string to file
		//* If the file already exists, its contents will be replaced!
		void save( string& text ) const;
		void save( std::string& bytes ) const;


		//* Remove file from the file system - if it exists
		void remove() const;




		/******************************************************************
		  File iterator
		******************************************************************/
	public:

		class iterator
		{
		public:
			iterator() = default;
			iterator( const file& f );

			inline operator bool() const noexcept {
				return !Buffer.filePath().empty(); }

			inline const string& operator*() const noexcept { return Line; }
			inline const string* operator->() const noexcept { return &Line; }

			inline iterator& operator++() noexcept {
				_readLine(); return *this; }

			inline bool operator==( const iterator& other )
				const noexcept {
				return Buffer.filePath() == other.Buffer.filePath(); }
			inline bool operator!=( const iterator& other )
				const noexcept {
				return Buffer.filePath() != other.Buffer.filePath(); }
		private:
			void _readLine();
			filebuffer Buffer;
			string Line;
		};




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		eon::path Path;
	};
}
