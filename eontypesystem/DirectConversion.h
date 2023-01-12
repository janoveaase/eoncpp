#pragma once

#include "ExpressionNode.h"
#include "TypeTuple.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'expression' namespace encloses all private Expression functionality
	//
	namespace expression
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Expression Direct Conversion Class - eon::expression::DirectConversion
		//
		// Use this class to check if a literal value of type B can be assigned
		// directly to a variable of type A.
		//
		class DirectConversion
		{
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//
		public:

			DirectConversion() = default;


			// Check if conversion from type and value of the source node to
			// the target type is legal.
			// Rules:
			//   int->short : SHORT_MIN <= value <= SHORT_MAX
			//   int->long  : Always legal!
			//   int->float : Always legal!
			//   int->low	: LOW_MIN <= value <= LOW_HIGH
			//   int->high  : Always legal!
			//   int->index : 0 < value
			//   long->index: 0 < value
			//   float->low : LOW_MIN <= value <= LOW_HIGH
			//   float->high: Always legal!
			bool legal( name_t target_type, const Node& source ) noexcept;

			// Convert a verified 'legal' value to the target type.
			Attribute convert( const Node& source, name_t target_type ) const;



			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			void _ensureConverters();

			static bool _testIntToShort( const Node& source ) noexcept;
			static inline bool _testIntToLong( const Node& source ) noexcept { return true; }
			static inline bool _testIntToFloat( const Node& source ) noexcept { return true; }
			static bool _testIntToLow( const Node& source ) noexcept;
			static inline bool _testIntToHigh( const Node& source ) noexcept { return true; }
			static inline bool _testIntToIndex( const Node& source ) noexcept { return source.value().value<int_t>() >= 0; }
			static inline bool _testLongToIndex( const Node& source ) noexcept { return source.value().value<long_t>() >= 0; }
			static bool _testFloatToLow( const Node& source ) noexcept;
			static inline bool _testFloatToHigh( const Node& source ) noexcept { return true; }

			static Attribute _convertIntToShort( const Node& source );
			static Attribute _convertIntToLong( const Node& source );
			static Attribute _convertIntToFloat( const Node& source );
			static Attribute _convertIntToLow( const Node& source );
			static Attribute _convertIntToHigh( const Node& source );
			static Attribute _convertIntToIndex( const Node& source );
			static Attribute _convertLongToIndex( const Node& source );
			static Attribute _convertFloatToLow( const Node& source );
			static Attribute _convertFloatToHigh( const Node& source );



			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			using Tester = std::function<bool( const Node& )>;
			using Converter = std::function<Attribute( const Node& )>;
			static std::unordered_map<name_t, std::unordered_map<name_t, std::pair<Tester, Converter>>> Converters;

		};
	}
}
