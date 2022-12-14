#pragma once
#include "Serializer.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Byte Serializer Class - eon::byteserializer
	//
	// A name path is a list of names that can be used to identify a
	// resource in a hierarchial structure by defining a path through the
	// structure to a target.
	//
	// In string format, the names (elements) are separated using forward slash.
	//
	class byteserializer : public serializer
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct an object that is ready to serialize
		byteserializer() = default;

		// Construct an object that is ready to de-serialize
		inline byteserializer( std::string bytes ) noexcept { *this = bytes; }

		inline byteserializer( const byteserializer& other ) { *this = other; }
		inline byteserializer( byteserializer&& other ) noexcept { *this = std::move( other ); }

		virtual ~byteserializer() = default;


		inline byteserializer& operator=( std::string bytes ) noexcept {
			Bytes = std::move( bytes ); Pos = Bytes.begin(); IsStoring = false; return *this; }

		inline byteserializer& operator=( const byteserializer& other ) {
			Bytes = other.Bytes; Pos = Bytes.begin() + ( other.Pos - other.Bytes.begin() );
			*static_cast<serializer*>( this ) = other; return *this; }
		inline byteserializer& operator=( byteserializer&& other ) noexcept {
			Bytes = std::move( other.Bytes ); Pos = Bytes.begin() + ( other.Pos - other.Bytes.begin() );
			other.Pos = other.Bytes.begin(); *static_cast<serializer*>( this ) = other; return *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if serializer is empty
		inline bool empty() const noexcept { return Bytes.empty(); }

		// Get number of bytes in stream
		inline index_t size() const noexcept { return Bytes.size(); }

		// Access the bytes
		inline const std::string& bytes() const noexcept { return Bytes; }




		///////////////////////////////////////////////////////////////////////
		//
		// Methods for sub-classes
		//
	protected:

		inline std::string& _byteStore() noexcept { return Bytes; }
		inline std::string::const_iterator& _bytePos() noexcept { return Pos; }




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::string Bytes;
		std::string::const_iterator Pos{ Bytes.begin() };
	};
}
