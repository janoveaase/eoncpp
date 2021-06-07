#pragma once

#include "RxDefs.h"
#include <map>
#include <eonname/Name.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::rx' namespace enclosed special elements for Eon regular
	  expressions
	**************************************************************************/
	namespace rx
	{
		class RxData
		{
		public:
			RxData() = default;
			inline RxData( const RxData& other ) { *this = other; }
			inline RxData( RxData&& other ) noexcept {
				*this = std::move( other ); }
			inline RxData( const substring& source ) noexcept {
				Src = source; Pos = Src.begin(); }
			inline RxData( const substring& source, Flag flags ) {
				Src = source; CmpFlags = flags;
				Pos = Src.begin(); }
			virtual ~RxData() { Captures.clear(); };

			inline RxData& operator=( const RxData& other ) {
				Captures = other.Captures; Src = other.Src;
				CmpFlags = other.CmpFlags; Pos = other.Pos; return *this; }
			inline RxData& operator=( RxData&& other ) noexcept {
				Src = other.Src;
				CmpFlags = other.CmpFlags; other.CmpFlags = Flag::none;
				Captures = std::move( other.Captures );
				Pos = other.Pos; return *this; }

			inline const substring& source() const noexcept { return Src; }
			inline const string::iterator& pos() const noexcept { return Pos; }
			inline char_t operator()() const noexcept {
				return Pos ? *Pos : NullChr; }
			inline bool prev( char_t c ) const noexcept {
				return Pos && Pos.numByte() > 0 && *( Pos - 1 ) == c; }
			inline char_t prev() const noexcept {
				return Pos && Pos.numByte() > 0 ? *( Pos - 1 ) : NullChr; }
			inline bool next( char_t c ) const noexcept {
				auto n = Pos + 1; return n ? *n == c : false; }
			inline char_t next() const noexcept {
				auto n = Pos + 1; return n ? *n : NullChr;}

			// Advance to next character in string
			// To make it possible to advance upon success in matching, the
			// return value is always true.
			inline bool advance() noexcept { if( Pos ) ++Pos; return true; }

			inline operator bool() const noexcept {
				return Pos != Src.end(); }
			inline Flag flags() const noexcept { return CmpFlags; }
			inline bool icase() const noexcept {
				return CmpFlags & Flag::icase; }
			inline bool lines() const noexcept {
				return CmpFlags & Flag::lines; }
			inline bool speedOnly() const noexcept { return ( CmpFlags
				& Flag::speed ) && !( CmpFlags & Flag::accuracy ); }
			inline bool accuracyOnly() const noexcept { return ( CmpFlags
				& Flag::accuracy ) && !( CmpFlags & Flag::speed ); }


			// Captures
			inline void registerCapture( name_t name,
				const substring& match ) { Captures[ name ] = match; }
			substring findCapture( name_t name ) const noexcept;
			inline std::map<name_t, substring>& captures() noexcept {
				return Captures; }
			inline std::map<name_t, substring>&& claimCaptures() noexcept {
				return std::move( Captures ); }




		private:
			substring Src;
			string_iterator Pos;
			Flag CmpFlags{ Flag::none };
			std::map<name_t, substring> Captures;
		};
	}
}
