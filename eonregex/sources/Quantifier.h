#pragma once

#include "RxDefs.h"


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
		/*
		* Quantifier objects are attributes to Node objects
		* They specify how many times the node must or can match, minimum/
		* maximum
		*/
		class Quantifier
		{
		public:
			Quantifier() = default;
			Quantifier( Quantifier& other ) noexcept { *this = other; };
			Quantifier( Quantifier&& other ) noexcept { *this = other; };
			virtual ~Quantifier() = default;

			inline Quantifier& operator=( const Quantifier& other ) {
				Min = other.Min; Max = other.Max; Greedy = other.Greedy;
				return *this; }
			inline Quantifier& operator=( Quantifier&& other ) noexcept {
				*this = other; return *this; }

			inline operator bool() const noexcept { return Set; }

			inline size_t min() const noexcept { return Min; }
			inline size_t max() const noexcept { return Max; }
			inline bool greedy() const noexcept { return Greedy; }

		private:
			void _set( size_t min, size_t max, bool greedy ) {
				Min = min; Max = max; Greedy = greedy; Set = true; }

		private:
			bool Set{ false };
			size_t Min{ 1 };
			size_t Max{ 1 };
			bool Greedy{ true };

			friend class Graph;
		};
	}
}
