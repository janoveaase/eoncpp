#include "TypeTuple.h"
#include "TypeMap.h"
#include "Tuple.h"


namespace eon
{
	std::hash<name_t> TypeTuple::NameHasher;




	TypeTuple& TypeTuple::operator=( TypeTuple&& other ) noexcept
	{
		NameValue = other.NameValue; other.NameValue = no_name;
		Qual = other.Qual; other.Qual = type::Qualifier::none;
		TupleValue = std::move( other.TupleValue );
		NamedAttributes = std::move( other.NamedAttributes );
		return *this;
	}




	TypeTuple TypeTuple::tuple( name_t tuple_type )
	{
		TypeTuple tup;
		tup.add( name_tuple );
		tup.add( name_type, tuple_type );
		return tup;
	}

	TypeTuple TypeTuple::tupleOfNames( std::initializer_list<name_t> names )
	{
		TypeTuple tup;
		for( auto name : names )
			tup.add( name );
		return tup;
	}

	TypeTuple TypeTuple::tupleOfNamedAttributes( std::initializer_list<std::pair<name_t, TypeTuple>> attributes )
	{
		TypeTuple tup;
		for( auto& attribute : attributes )
			tup.add( attribute.first, attribute.second );
		return tup;
	}


	TypeTuple TypeTuple::expression( TypeTuple&& return_type )
	{
		TypeTuple tup;
		tup.add( name_expression );
		tup.add( name_return, std::move( return_type ) );
		return tup;
	}

	TypeTuple TypeTuple::action(
		name_t type_name,
		name_t action_name,
		name_t category,
		TypeTuple&& return_type,
		TypeTuple&& arguments )
	{
		TypeTuple tup;
		tup.add( name_action );
		tup.add( name_name, action_name );
		tup.add( name_type, type_name );
		tup.add( name_category, category );
		tup.add( name_return, std::move( return_type ) );
		tup.add( name_args, std::move( arguments ) );
		return tup;
	}

	TypeTuple TypeTuple::action(
		name_t type_name,
		name_t action_name,
		name_t category,
		TypeTuple&& return_type,
		const TypeTuple& arguments )
	{
		TypeTuple tup;
		tup.add( name_action );
		tup.add( name_name, action_name );
		tup.add( name_type, type_name );
		tup.add( name_category, category );
		tup.add( name_return, return_type );
		tup.add( name_args, std::move( arguments ) );
		return tup;
	}

	TypeTuple TypeTuple::action(
		name_t type_name,
		name_t action_name,
		name_t category,
		const TypeTuple& return_type,
		const TypeTuple& arguments )
	{
		TypeTuple tup;
		tup.add( name_action );
		tup.add( name_name, action_name );
		tup.add( name_type, type_name );
		tup.add( name_category, category );
		tup.add( name_return, return_type );
		tup.add( name_args, arguments );
		return tup;
	}

	TypeTuple TypeTuple::action(
		name_t type_name,
		name_t action_name,
		name_t category,
		TypeTuple&& return_type,
		std::initializer_list<std::pair<name_t, TypeTuple>> arguments )
	{
		TypeTuple tup;
		tup.add( name_action );
		tup.add( name_name, action_name );
		tup.add( name_type, type_name );
		tup.add( name_category, category );
		tup.add( name_return, std::move( return_type ) );
		tup.add( name_args, args( arguments ) );
		return tup;
	}

	TypeTuple TypeTuple::args( std::initializer_list<TypeTuple> attributes )
	{
		TypeTuple tup;
		for( auto& elm : attributes )
			tup.add( elm );
		return tup;
	}

	TypeTuple TypeTuple::args( std::initializer_list<std::pair<name_t, TypeTuple>> attributes )
	{
		TypeTuple tup;
		for( auto& elm : attributes )
			tup.add( elm.first, elm.second );
		return tup;
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




	const name_t name_ttypetuple{ compilerName( "#typetuple" ) };

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
		return TupleValue.size() < i ? -1 : other.TupleValue.size() < i ? 1 : 0;
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
