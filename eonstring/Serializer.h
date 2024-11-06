#pragma once
#include "String.h"
#include "Hex.h"
#include "Name.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Serializer Class - eon::serializer
	//
	// Interface for serializers.
	//
	class serializer
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	public:

		// Exception thrown when trying to read a serialized element of the wrong type
		EONEXCEPT( WrongType );




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		serializer() = default;
		serializer( const serializer& ) noexcept = default;
		virtual ~serializer() = default;


		serializer& operator=( const serializer& ) noexcept = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Storing
		//
	public:

		inline bool isStoring() const noexcept { return IsStoring; }

		inline serializer& operator<<( bool value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( byte_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( char value ) { _encode( static_cast<byte_t>( value ), _byteStore() ); return *this; }
		inline serializer& operator<<( char_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( int value ) { _encode( static_cast<int_t>( value ), _byteStore() ); return *this; }
		inline serializer& operator<<( short_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( long_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( flt_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( low_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( high_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( index_t value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( name_t value ) { _encode( value, _byteStore() ); return *this; }
		// inline serializer& operator<<( bits value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( const string& value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( const std::string& value ) { _encode( value, _byteStore() ); return *this; }
		inline serializer& operator<<( const char* value ) { _encode( value, _byteStore() ); return *this; }
		// inline serializer& operator<<( const chars& value ) { _encode( value, _byteStore() ); return *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Retrieving
		//
	public:

		inline bool isRetrieving() const noexcept { return !IsStoring; }

		inline serializer& operator>>( bool& value ) {
			value = _decodeBool( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( byte_t& value ) {
			value = _decodeByte( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( char& value ) {
			value = _decodeByte( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( char_t& value ) {
			value = _decodeChar( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( int_t& value ) {
			value = _decodeInt( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( short_t& value ) {
			value = _decodeShort( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( long_t& value ) {
			value = _decodeLong( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( flt_t& value ) {
			value = _decodeFloat( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( low_t& value ) {
			value = _decodeLow( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( high_t& value ) {
			value = _decodeHigh( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( index_t& value ) {
			value = _decodeIndex( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( name_t& value ) {
			value = _decodeName( _bytePos(), _byteStore().end() ); return *this; }
		//inline serializer& operator>>( bits& value ) {
		//	value = _decodeBits( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( string& value ) {
			value = _decodeStr( _bytePos(), _byteStore().end() ); return *this; }
		inline serializer& operator>>( std::string& value ) {
			value = _decodeBytes( _bytePos(), _byteStore().end() ); return *this; }
		//inline serializer& operator>>( chars& value ) {
		//	value = _decodeChars( _bytePos(), _byteStore().end() ); return *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Methods for sub-classes
		//
	protected:

		virtual std::string& _byteStore() noexcept = 0;
		virtual std::string::const_iterator& _bytePos() noexcept = 0;




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	protected:

		void _encode( bool value, std::string& bytes ) const;
		void _encode( byte_t value, std::string& bytes ) const;
		void _encode( char_t value, std::string& bytes ) const;
		void _encode( int_t value, std::string& bytes ) const;
		void _encode( short_t value, std::string& bytes ) const;
		void _encode( long_t value, std::string& bytes ) const;
		void _encode( flt_t value, std::string& bytes ) const;
		void _encode( low_t value, std::string& bytes ) const;
		void _encode( high_t value, std::string& bytes ) const;
		void _encode( index_t value, std::string& bytes ) const;
		void _encode( name_t value, std::string& bytes ) const;
		// void _encode( const bits& value, std::string& bytes ) const;
		void _encode( const std::string& value, std::string& bytes ) const;
		void _encode( const string& value, std::string& bytes ) const;
		void _encode( const char* value, std::string& bytes ) const;
		// void _encode( const chars& value, std::string& bytes ) const;

		bool _decodeBool( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		byte_t _decodeByte( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		char_t _decodeChar( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		int_t _decodeInt( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		short_t _decodeShort( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		long_t _decodeLong( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		flt_t _decodeFloat( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		low_t _decodeLow( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		high_t _decodeHigh( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		index_t _decodeIndex( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		name_t _decodeName( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		// bits _decodeBits( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		std::string _decodeBytes( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		string _decodeStr( std::string::const_iterator& pos, std::string::const_iterator end ) const;
		// chars _decodeChars( std::string::const_iterator& pos, std::string::const_iterator end );

		std::string::const_iterator _expectType(
			name_t exp_type, std::string::const_iterator pos, std::string::const_iterator end ) const;

		inline bool _isStoring() const noexcept { return IsStoring; }
		inline void _setStoring() noexcept { IsStoring = true; }
		inline void _setReading() noexcept { IsStoring = false; }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		static const char record_sep{ 0x1E };
		bool IsStoring{ true };
	};
}
