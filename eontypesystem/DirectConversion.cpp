#include "DirectConversion.h"
#include <functional>


namespace eon
{
	namespace expression
	{
		std::unordered_map<name_t, std::unordered_map<name_t, std::pair<DirectConversion::Tester, DirectConversion::Converter>>> DirectConversion::Converters;
		void DirectConversion::_ensureConverters()
		{
			if( Converters.empty() )
			{
				Converters[ name_int ][ name_short ] = { &_testIntToShort, &_convertIntToShort };
				Converters[ name_int ][ name_long ] = { &_testIntToLong, &_convertIntToLong };
				Converters[ name_int ][ name_float ] = { &_testIntToFloat, &_convertIntToFloat };
				Converters[ name_int ][ name_low ] = { &_testIntToLow, &_convertIntToLow };
				Converters[ name_int ][ name_high ] = { &_testIntToHigh, &_convertIntToHigh };
				Converters[ name_int ][ name_index ] = { &_testIntToIndex, &_convertIntToIndex };
				Converters[ name_long ][ name_index ] = { &_testLongToIndex, &_convertLongToIndex };
				Converters[ name_float ][ name_low ] = { &_testFloatToLow, &_convertFloatToLow };
				Converters[ name_float ][ name_high ] = { &_testFloatToHigh, &_convertFloatToHigh };
			}
		}


		bool DirectConversion::legal( name_t target_type, const Node& source ) noexcept
		{
			_ensureConverters();
			
			auto found_source = Converters.find( source.type().name() );
			if( found_source == Converters.end() )
				return false;

			auto found_target = found_source->second.find( target_type );
			if( found_target == found_source->second.end() )
				return false;

			return found_target->second.first( source );
		}

		Attribute DirectConversion::convert( const Node& source, name_t target_type ) const {
			return Converters[ source.type().name() ][ target_type ].second( source ); }




		bool DirectConversion::_testIntToShort( const Node& source ) noexcept
		{
			int_t value = source.value().value<int_t>();
			return value >= EON_SHORT_MIN && value <= EON_SHORT_MAX;
		}

		bool DirectConversion::_testIntToLow( const Node& source ) noexcept
		{
			int_t value = source.value().value<int_t>();
			return value >= EON_LOW_MIN && value <= EON_LOW_MAX;
		}

		bool DirectConversion::_testFloatToLow( const Node& source ) noexcept
		{
			flt_t value = source.value().value<flt_t>();
			return value >= EON_LOW_MIN && value <= EON_LOW_MAX;
		}


		Attribute DirectConversion::_convertIntToShort( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<short_t>( source.value().value<int_t>() ),
				name_short,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertIntToLong( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<long_t>( source.value().value<int_t>() ),
				name_long,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertIntToFloat( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<flt_t>( source.value().value<int_t>() ),
				name_float,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertIntToLow( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<low_t>( source.value().value<int_t>() ),
				name_low,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertIntToHigh( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<high_t>( source.value().value<int_t>() ),
				name_high,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertIntToIndex( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<index_t>( source.value().value<int_t>() ),
				name_index,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertLongToIndex( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<index_t>( source.value().value<long_t>() ),
				name_index,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertFloatToLow( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<low_t>( source.value().value<flt_t>() ),
				name_low,
				source.type().qualifier(),
				source.source() );
		}

		Attribute DirectConversion::_convertFloatToHigh( const Node& source )
		{
			return Attribute::newExplicit(
				static_cast<high_t>( source.value().value<flt_t>() ),
				name_high,
				source.type().qualifier(),
				source.source() );
		}
	}
}
