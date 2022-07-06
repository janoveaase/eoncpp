#pragma once

#include "RxDefs.h"
#include <map>
#include <eonstring/Name.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::rx' namespace enclosed special elements for Eon regular
	// expressions
	//
	namespace rx
	{
		using captures_t = std::unordered_map<name_t, substring>;

		class RxData
		{
		public:
			RxData() = default;
			inline RxData( const RxData& other ) {
				if( other.Captures ) { Captures = new captures_t( *other.Captures ); }
				Src = other.Src; CmpFlags = other.CmpFlags; Pos = other.Pos; Marker = other.Marker; }
			inline RxData( RxData&& other ) noexcept { *this = std::move( other ); }
			inline RxData( const substring& source, Flag flags, uint16_t marker ) noexcept {
				Src = source; Pos = Src.begin(); CmpFlags = flags; Marker = marker; }
			virtual ~RxData() { reset(); }

			inline void reset() noexcept { if( Captures ) { delete Captures; Captures = nullptr; } }

			inline RxData& operator=( const RxData& other ) {
				reset(); if( other.Captures ) { Captures = new captures_t( *other.Captures ); }
				Src = other.Src; CmpFlags = other.CmpFlags; Pos = other.Pos; Marker = other.Marker; return *this; }
			inline RxData& operator=( RxData&& other ) noexcept { reset(); if( other.Captures ) {
				Captures = other.Captures; other.Captures = nullptr; } Src = other.Src; CmpFlags = other.CmpFlags;
				other.CmpFlags = Flag::none; Pos = other.Pos; Marker = other.Marker; return *this; }

			inline const substring& source() const noexcept { return Src; }
			inline const string::iterator& pos() const noexcept { return Pos; }
			inline bool valid() const noexcept { return Pos && Pos < Src.end(); }
			inline char_t operator()() const noexcept { return valid() ? *Pos : NullChr; }
			inline bool prev( char_t c ) const noexcept { return Pos && Pos > Src.begin() && *( Pos - 1 ) == c; }
			inline char_t prev() const noexcept { return Pos && Pos > Src.begin() ? *( Pos - 1 ) : NullChr; }
			inline bool next( char_t c ) const noexcept { auto n = Pos + 1; return n ? *n == c : false; }
			inline char_t next() const noexcept { auto n = Pos + 1; return n ? *n : NullChr;}

			inline index_t remaining() const noexcept { return Src.end() - Pos; }

			// Advance to next character in string
			// To make it possible to advance upon success in matching, the
			// return value is always true.
			inline bool advance() noexcept { if( Pos ) ++Pos; return true; }

			inline operator bool() const noexcept { return Pos != Src.end(); }
			inline Flag flags() const noexcept { return CmpFlags; }
			inline bool icase() const noexcept { return CmpFlags & Flag::icase; }
			inline bool lines() const noexcept { return CmpFlags & Flag::lines; }
			inline bool bounds() const noexcept { return CmpFlags & Flag::bounds; }
			inline bool speedOnly() const noexcept { return ( CmpFlags & Flag::speed ) && !( CmpFlags & Flag::accuracy ); }
			inline bool accuracyOnly() const noexcept {
				return ( CmpFlags & Flag::accuracy ) && !( CmpFlags & Flag::speed ); }

			inline uint16_t marker() const noexcept { return Marker; }


			// Captures
			inline void registerCapture( name_t name, const substring& match ) {
				if( !Captures ) Captures = new captures_t(); (*Captures)[ name ] = match; }
			substring findCapture( name_t name ) const noexcept;
			inline captures_t* captures() noexcept { return Captures; }
			inline captures_t* claimCaptures() noexcept { auto capt = Captures; Captures = nullptr; return capt; }
			inline void addCaptures( captures_t* other ) { if( other ) for( auto& capture : *other ) registerCapture( capture.first, capture.second ); }




		private:
			substring Src;
			string_iterator Pos;
			Flag CmpFlags{ Flag::none };
			captures_t* Captures{ nullptr };
			uint16_t Marker{ 0 };
		};
	}
}
