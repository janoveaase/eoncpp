#pragma once

#include <eonstring/String.h>
#include <eonstring/Hex.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	class VTS
	{
	public:
		static eon::string toString( const eon::string& value ) { return "\"" + value.escapeAll() + "\""; }
		static eon::string toString( const eon::substring& value ) {
			return "\"" + eon::string( value ).escapeAll() + "\""; }
		static eon::string toString( const eon::string_iterator& value ) { return  value.encode(); }
		static eon::string toString( const std::string& value ) { return "\"" + eon::string( value ).escapeAll() + "\""; }
		static eon::string toString( const std::wstring& value ) { return "\"" + eon::string( value ).escapeAll() + "\""; }
		static eon::string toString( const char* value ) { return "\"" + eon::string( value ).escapeAll() + "\""; }
		static eon::string toString( eon::char_t value )
		{
			eon::string v{ "@char=" };
			if( ( value >= 32 && value <= 126 ) || ( value >= 161 && value <= 255 ) )
				v += eon::string( value );
			v += "::" + eon::string( static_cast<eon::int_t>( value ) );
			v += "::";
			const char* bytes = (const char*)&value;
			for( int i = 4; i > 0; --i )
				v += eon::byteToHex( bytes[ i - 1 ] );
			return v;
		}
		static eon::string toString( char value )
		{
			eon::string v{ "@char=" };
			if( ( value >= 32 && value <= 126 ) || ( value >= char( 161 ) && value <= char( 255 ) ) )
				v += eon::string( value );
			v += "::" + eon::string( static_cast<eon::int_t>( value ) );
			v += "::" + eon::byteToHex( value );
			return v;
		}
		static eon::string toString( eon::byte_t value )
		{
			eon::string v{ "@byte=" };
			if( ( value >= 32 && value <= 126 ) || ( value >= 161 && value <= 255 ) )
				v += eon::string( value );
			v += "::" + eon::string( static_cast<eon::int_t>( value ) );
			v += "::" + eon::byteToHex( value );
			return v;
		}
		static eon::string toString( eon::int_t value ) { return eon::string( value ); }
		static eon::string toString( eon::long_t value ) { return eon::string( value ); }
		static eon::string toString( eon::short_t value ) { return eon::string( value ); }
		static eon::string toString( eon::flt_t value ) { return eon::string( value ); }
		static eon::string toString( eon::high_t value ) { return eon::string( value ); }
		static eon::string toString( eon::low_t value ) { return eon::string( value ); }
		static eon::string toString( eon::index_t value ) { return eon::string( value ); }
		static eon::string toString( eon::name_t value ) { return eon::str( value ); }

		// Unsupported generic type.
		template<typename T>
		static eon::string toString( const T& value )
		{
			eon::string type = typeid( value ).name();
			return "@type=" + type;
		}
	};
}
