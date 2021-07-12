#pragma once

#include "FileDefs.h"
#include <filesystem>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon File System Class - eon::fsys

	  Interactions with the file system.
	  All methods are static!
	**************************************************************************/
	class fsys
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		fsys() = delete;




		/**********************************************************************
		  Basic Functions
		**********************************************************************/
	public:

		//* Check if the path refers to an existing file or directory
		static inline bool exists( const path& target ) {
			return std::filesystem::exists( std::filesystem::path(
				target.stdstr() ) ); }

		//* Get the size of a file
		static inline uintmax_t size( const path& target ) {
			return std::filesystem::file_size( std::filesystem::path(
				target.stdstr() ) ); }
	};
}
