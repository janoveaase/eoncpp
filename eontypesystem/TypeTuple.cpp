#include "TypeTuple.h"
#include "TypeTupleFactory.h"
#include "TypeMap.h"
#include "Tuple.h"


namespace eon
{
	std::hash<name_t> TypeTuple::NameHasher;




	EON_TEST( TypeTuple, TypeTuple, void1,
		EON_EQ( nn, TypeTuple().NameValue ) );
	EON_TEST( TypeTuple, TypeTuple, void2,
		EON_EQ( 0, TypeTuple().TupleValue.size() ) );
	EON_TEST( TypeTuple, TypeTuple, name,
		EON_EQ( name_bool, TypeTuple( name_bool ).NameValue ) );




	EON_TEST_3STEP( TypeTuple, operator_asgn, name_copy,
		TypeTuple other( name_bool ),
		TypeTuple obj = other,
		EON_EQ( name_bool, obj.NameValue ) );
	EON_TEST_3STEP( TypeTuple, operator_asgn, tuple_copy,
		TypeTuple other = typetuple::newStatic( { { nn, name_bool }, { name_action, name_int } } ),
		TypeTuple obj = other,
		EON_EQ( "T(tuple, type=static, value=(bool, action=int))", obj.str() ) );

	TypeTuple& TypeTuple::operator=( TypeTuple&& other ) noexcept
	{
		NameValue = other.NameValue; other.NameValue = no_name;
		TupleValue = std::move( other.TupleValue );
		NamedAttributes = std::move( other.NamedAttributes );
		return *this;
	}

	TypeTuple& TypeTuple::arguments( std::initializer_list<std::pair<name_t, TypeTuple>> args )
	{
		_addAttribute( name_args, typetuple::convert( args ) );
		return *this;
	}

	TypeTuple& TypeTuple::set( name_t name, TypeTuple&& sub_tuple )
	{
		if( name == no_name )
			throw type::NoName();
		_ensureSubTuple();
		auto found = NamedAttributes.find( name );
		if( found == NamedAttributes.end() )
		{
			NamedAttributes[ name ] = TupleValue.size();
			TupleValue.push_back( std::move( sub_tuple ) );
		}
		else
			found->second = std::move( sub_tuple );
		return *this;
	}

	void TypeTuple::expandSubTuples()
	{
		if( numAttributes() < 2 || NameValue == name_tuple )
			return;
		*this = typetuple::newStatic( std::move( *this ) );
	}




	index_t TypeTuple::indexOf( name_t attribute_name ) const
	{
		auto found = NamedAttributes.find( attribute_name );
		if( found != NamedAttributes.end() )
			return found->second;
		throw type::NotFound( "TypeTuple named attribute: " + eon::str( attribute_name ) );
	}


	void TypeTuple::str( Stringifier& strf ) const
	{
		strf.prepend( "T" );
		_toStr( strf, true );
	}

	//index_t TypeTuple::sizeOfObject() const noexcept
	//{
	//	if( isName() )
	//		return type::sizeOf( NameValue );
	//	else
	//		return sizeof( Tuple );
	//}




	int TypeTuple::compare( const TypeTuple& other ) const noexcept
	{
		if( NameValue == no_name )
			return other.NameValue == no_name ? 0 : -1;
		else if( other.NameValue == no_name )
			return 1;
		if( NameValue != other.NameValue )
			return NameValue < other.NameValue ? -1 : 1;
		if( NameValue == name_ttypetuple )
			return _attributesCompare( other );
		return 0;
	}

	bool TypeTuple::compatibleWith( const TypeTuple& other ) const noexcept
	{
		if( !*this || !other )	// Void is NOT compatible!
			return false;

		// If simple types then IS compatible if same name value!
		if( isName() && other.isName() )
			return NameValue == other.NameValue;

		// All named attributes must match on type
		std::unordered_set<index_t> compared_by_name;
		if( !_equalOnNamedArgs( other, compared_by_name ) )
			return false;

		// All unnamed attributes must match on position and type
		return _equalOnUnnamedArgs( other, compared_by_name );
	}

	size_t TypeTuple::hash() const noexcept
	{
		size_t val = NameHasher( NameValue );
		for( auto& sub : TupleValue )
			val = val * 2734561 + sub.hash();
		return val;
	}




	const name_t name_ttypetuple{ compilerName( "#typetuple" ) };
	const name_t name_value{ name( "value" ) };

	void TypeTuple::_addAttribute( name_t name, TypeTuple&& value )
	{
		if( name == no_name )
			TupleValue.push_back( std::move( value ) );
		else
		{
			auto found = NamedAttributes.find( name );
			if( found != NamedAttributes.end() )
			{
				auto& value = TupleValue[ found->second ];
				value = std::move( value );
			}
			else
			{
				NamedAttributes[ name ] = TupleValue.size();
				TupleValue.push_back( std::move( value ) );
			}
		}
	}

	void TypeTuple::_addTupleAttribute( name_t name, TypeTuple&& value )
	{
		if( exists( name_type ) && at( name_type ) != name_static )
			throw type::AccessDenied( "Cannot add attributes to type-tuple for " + at( name_type ).str() + "!" );
		if( name == name_type && !exists( name_type ) )
			_addAttribute( name, std::move( value ) );
		else
		{
			if( !exists( name_value ) )
				_addAttribute( name_value, TypeTuple() );
			auto& tvalue = at( name_value );
			tvalue._ensureSubTuple();
			tvalue._addAttribute( name, std::move( value ) );
		}
	}

	void TypeTuple::_toStr( Stringifier& strf, bool top_level ) const
	{
		_writePrefix( strf, top_level );
		if( isName() )
			strf.word( eon::str( NameValue ) );
		else
			_writeComplex( strf );
		_writePostfix( strf, top_level );
	}
	void TypeTuple::_writeComplex( Stringifier& strf ) const
	{
		auto names = _reverseNameIndexMap();
		for( index_t i = 0; i < TupleValue.size(); ++i )
			_writeComplex( strf, names, i );
	}
	void TypeTuple::_writeComplex(
		Stringifier& strf, const std::unordered_map<index_t, name_t>& names, index_t attrib_no ) const
	{
		if( attrib_no > 0 )
			strf.punct( "," );
		_writeNamedAttribName( strf, names, attrib_no );
		TupleValue[ attrib_no ]._toStr( strf, false );
	}
	void TypeTuple::_writeNamedAttribName(
		Stringifier& strf, const std::unordered_map<index_t, name_t>& names, index_t attrib_no ) const
	{
		auto name = names.find( attrib_no );
		if( name != names.end() )
		{
			strf.word( eon::str( name->second ) );
			strf.op2( "=" );
		}
	}

	std::unordered_map<index_t, name_t> TypeTuple::_reverseNameIndexMap() const
	{
		std::unordered_map<index_t, name_t> names;
		for( auto& elm : NamedAttributes )
			names[ elm.second ] = elm.first;
		return names;
	}


	std::unordered_set<name_t> TypeTuple::PrimTypes;
	void TypeTuple::_populatePrims()
	{
		if( PrimTypes.empty() )
		{
			PrimTypes = {
				name_bool,
				name_byte, name_char,
				name_int, name_short, name_long,
				name_float, name_low,
				name_index,
				name_handle };
#ifndef _DEBUG
			PrimTypes.insert( name_name );
#endif
		}
	}

	std::unordered_set<name_t> TypeTuple::BasicTypes;
	void TypeTuple::_populateBasics()
	{
		if( BasicTypes.empty() )
		{
			BasicTypes = {
				name_high,
				name_bytes,
				name_string, name_stringchar, name_substring,
				name_chars,
				name_real, name_complex,
				name_regex,
				name_namepath,
				name_path,
				name_time, name_date, name_timezone, name_dst, name_timepoint, name_timespan, name_timer,
				name_expression,
				name_array, name_vector, name_list, name_set, name_heap };
#ifdef _DEBUG
			BasicTypes.insert( name_name );
#endif
		}
	}


	int TypeTuple::_attributesCompare( const TypeTuple& other ) const noexcept
	{
		index_t i{ 0 };
		for( ; i < TupleValue.size() && i < other.TupleValue.size(); ++i )
		{
			auto cmp = TupleValue[ i ].compare( other.TupleValue[ i ] );
			if( cmp != 0 )
				return cmp;
		}
		return TupleValue.size() > i ? 1 : other.TupleValue.size() > i ? -1 : 0;
	}

	bool TypeTuple::_equalOnNamedArgs(
		const TypeTuple& other, std::unordered_set<index_t>& compared_by_name ) const noexcept
	{
		for( auto& named : NamedAttributes )
		{
			auto found = other.NamedAttributes.find( named.first );
			if( found == other.NamedAttributes.end() )
				return false;
			if( !TupleValue[ named.second ].compatibleWith( other.TupleValue[ found->second ] ) )
				return false;
			compared_by_name.insert( named.second );
		}
		return true;
	}
	bool TypeTuple::_equalOnUnnamedArgs(
		const TypeTuple& other, const std::unordered_set<index_t>& compared_by_name ) const noexcept
	{
		for( size_t i = 0; i < TupleValue.size(); ++i )
		{
			// Skip if already compared by name!
			if( compared_by_name.find( i ) != compared_by_name.end() )
				continue;

			if( i >= other.TupleValue.size() )
				return false;
			if( !TupleValue[ i ].compatibleWith( other.TupleValue[ i ] ) )
				return false;
		}
		return true;
	}
}
