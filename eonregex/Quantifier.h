#pragma once

#include "RxDefs.h"


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
		// Quantifier objects are attributes to Node objects
		// They specify how many times the node must or can match, minimum/
		// maximum
		class Quantifier
		{
		public:
			Quantifier() = default;
			inline Quantifier( Quantifier& other ) noexcept { *this = other; };
			inline Quantifier( Quantifier&& other ) noexcept { *this = other; };
			inline Quantifier( index_t min, index_t max, bool greedy ) { Min = min; Max = max; Greedy = greedy; Set = true; }
			virtual ~Quantifier() = default;

			inline Quantifier& operator=( const Quantifier& other ) {
				Min = other.Min; Max = other.Max; Greedy = other.Greedy; return *this; }
			inline Quantifier& operator=( Quantifier&& other ) noexcept { *this = other; return *this; }

			inline operator bool() const noexcept { return Set; }

			inline index_t minQ() const noexcept { return Min; }
			inline index_t maxQ() const noexcept { return Max; }
			inline bool greedy() const noexcept { return Greedy; }


			inline bool operator==( const Quantifier& other ) const noexcept {
				return Min == other.Min && Max == other.Max && Greedy == other.Greedy; }
			inline bool operator!=( const Quantifier& other ) const noexcept { return !( *this == other ); }


			string str() const {
				string s;
				if( Max == INDEX_MAX )
					s = Min == 0 ? "*" : Min == 1 ? "+" : "{" + string( Min ) + ",}";
				else if( Min == Max )
					s = Min == 1 ? "" : "{" + string( Min ) + "}";
				else if( Min == 0 && Max == 1 )
					s = "?";
				else
					s = "{" + ( Min == 0 ? string( "," ) : string( Min ) + "," ) + string( Max ) + "}";
				return Greedy ? s : s + "?"; }

		private:
			void _set( index_t min, index_t max, bool greedy ) { Min = min; Max = max; Greedy = greedy; Set = true; }

		private:
			bool Set{ false };
			index_t Min{ 1 };
			index_t Max{ 1 };
			bool Greedy{ true };

			friend class Graph;
			friend class Node;
			friend class NodeGroup;
		};
	}
}
