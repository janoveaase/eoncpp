#include "Tuple.h"
#include <eonregex/RegEx.h>
#include <eonstring/NamePath.h>
#include <eonfilesys/Path.h>
#include <eonexpression/Expression.h>
#include "TupleFactory.h"
#include "TypeTupleFactory.h"


namespace eon
{
	Tuple& Tuple::operator=( const Tuple& other )
	{
		Type = other.Type;
		Permissions = other.Permissions;
		Parent = nullptr;
		_replaceCopyAttributes( other );
		return *this;
	}
	Tuple& Tuple::operator=( Tuple&& other ) noexcept
	{
		Type = std::move( other.Type );
		Permissions = other.Permissions; other.Permissions = TuplePerm::all_normal;
		Parent = nullptr;
		_replaceMoveAttributes( other );
		return *this;
	}

	Tuple& Tuple::copyAttributes( const Tuple& other )
	{
		std::set<index_t> taken;
		for( auto& named : other.NamedAttributes )
		{
			attribute( named.first ) = other.Attributes[ named.first ];
			taken.insert( named.second );
		}
		for( index_t i = 0; i < other.Attributes.size(); ++i )
		{
			if( taken.find( i ) == taken.end() )
				Attributes[ i ] = other.Attributes[ i ];
		}
		return *this;
	}




	Tuple& Tuple::addType( const TypeTuple& type )
	{
		_assertAddPerm( Type.at( name_type ).nameValue(), type );
		Attributes.push_back( Attribute::newTypeTuple( type, type::Qualifier::_none ) );
		if( type == name_tuple )
			Attributes[ Attributes.size() - 1 ].value<Tuple>().Parent = this;
		return *this;
	}
	Tuple& Tuple::addType( name_t name, const TypeTuple& type )
	{
		_assertAddPerm( Type.at( name_type ).nameValue(), type );
		if( name != no_name )
			_addAttributeName( name, type );
		_addAttributeValue( Attribute::newTypeTuple( type, type::Qualifier::_none ) );
		return *this;
	}

	Tuple& Tuple::add( name_t name, Attribute&& value )
	{
		_assertAddPerm( Type.at( name_type ).nameValue(), value.type() );
		if( name != no_name )
			_addAttributeName( name, value.type() );
		_addAttributeValue( std::move( value ) );
		return *this;
	}

	Tuple& Tuple::remove( index_t attribute_num )
	{
		_assertRemPerm();
		_assertInRange( attribute_num );
		Attributes.erase( Attributes.begin() + attribute_num );
		NamedAttributes.removeTwo( attribute_num );
		return *this;
	}
	Tuple& Tuple::remove( int_t attribute_num )
	{
		index_t pos = attribute_num >= 0 ? static_cast<index_t>( attribute_num ) : static_cast<index_t>( -attribute_num );
		if( attribute_num < 0 )
		{
			_assertInRange( pos );
			pos = Attributes.size() - pos;
		}
		return remove( pos );
	}

	Attribute* Tuple::findAttributeIncludeParent( name_t attribute_name ) noexcept
	{
		auto pos = index( attribute_name );
		if( pos < Attributes.size() )
			return &Attributes[ pos ];
		if( Parent )
			return Parent->findAttributeIncludeParent( attribute_name );
		return nullptr;
	}
	const Attribute* Tuple::findAttributeIncludeParent( name_t attribute_name ) const noexcept
	{
		auto pos = index( attribute_name );
		if( pos < Attributes.size() )
			return &Attributes[ pos ];
		if( Parent )
			return Parent->findAttributeIncludeParent( attribute_name );
		return nullptr;
	}

	bool Tuple::flag( name_t name ) const noexcept
	{
		for( index_t i = 0; i < Attributes.size(); ++i )
		{
			if( Attributes[ i ].type() == name_name
				&& !NamedAttributes.containsTwo( i )
				&& Attributes[ i ].value<name_t>() == name )
				return true;
		}
		return false;
	}




	Tuple& Tuple::registerType( name_t type )
	{
		_assertTypPerm();
		if( !exists( name_dottypes ) )
			addTuple( name_dottypes, Tuple( name_data ) );
		auto& types = get<Tuple>( name_dottypes );
		if( types.exists( type ) )
			throw type::DuplicateName( "Cannot register type: " + eon::str( type ) );
		types.add( Attribute::newName( type, type::Qualifier::_none ) );
		return *this;
	}

	bool Tuple::isType( name_t name ) const noexcept
	{
		if( ( Permissions & TuplePerm::types ) )
		{
			if( exists( name_dottypes ) )
			{
				if( get<Tuple>( name_dottypes ).flag( name ) )
					return true;
			}
		}
		return Parent ? Parent->isType( name ) : false;
	}




	Tuple& Tuple::registerAction( type::Action* action )
	{
		_assertActPerm();
		auto f = Actions.find( action->signature() );
		if( Actions.find( action->signature() ) != Actions.end() )
			throw type::DuplicateName( "Cannot register action: " + action->signature().str() );
		Actions[ action->signature() ] = action;
		auto found = ActionsByName.find( action->signature().at( name_name ).nameValue() );
		if( found == ActionsByName.end() )
			ActionsByName[ action->signature().at( name_name ).nameValue() ] = { action->signature() };
		else
			found->second.insert( action->signature() );
		return *this;
	}

	const std::set<TypeTuple>& Tuple::signatures( name_t action_name ) const
	{
		if( Parent )
			return Parent->signatures( action_name );
		static std::set<TypeTuple> none;
		auto found = ActionsByName.find( action_name );
		return found != ActionsByName.end() ? found->second : none;
	}
	std::set<TypeTuple> Tuple::signatures( name_t action_name, name_t type_name ) const
	{
		if( Parent )
			return Parent->signatures( action_name, type_name );
		return _signatures( action_name, { { name_type, type_name } } );
	}
	std::set<TypeTuple> Tuple::signatures( name_t action_name, name_t type_name, const TypeTuple& args ) const
	{
		if( Parent )
			return Parent->signatures( action_name, type_name, args );
		return _signatures( action_name, { { name_type, type_name } }, &args );
	}
	std::set<TypeTuple> Tuple::signatures(
		name_t action_name, name_t category, name_t type_name, const TypeTuple& args ) const
	{
		if( Parent )
			return Parent->signatures( action_name, type_name, args );
		return _signatures( action_name, { { name_type, type_name }, { name_category, category } }, &args );
	}




	Tuple::Tuple( name_t tuple_type_name, TuplePerm permissions, Tuple* parent )
	{
		_populateData();
		Type = typetuple::_new( tuple_type_name );
		_perm( tuple_type_name, permissions );
		Parent = parent;
	}

	Tuple::Tuple(
		name_t tuple_type_name, std::initializer_list<AttributePair> attributes, TuplePerm permissions, Tuple* parent )
	{
		_populateData();
		if( tuple_type_name == name_static )
			_constructStatic( attributes );
		else
			_constructNonstatic( tuple_type_name, attributes );
		_perm( tuple_type_name, permissions );
		Parent = parent;
	}
	Tuple::Tuple( name_t tuple_type_name, std::vector<AttributePair> attributes, TuplePerm permissions, Tuple* parent )
	{
		_populateData();
		if( tuple_type_name == name_static )
			_constructStatic( attributes );
		else
			_constructNonstatic( tuple_type_name, attributes );
		_perm( tuple_type_name, permissions );
		Parent = parent;
	}
	Tuple::Tuple( TypeTuple optional_tuple_attributes )
	{
		_populateData();
		Type = typetuple::_new( name_optional );
		Type.set( name_tuple, std::move( optional_tuple_attributes ) );
		_perm( name_optional, TuplePerm::none );
	}




	void Tuple::_perm( name_t tuple_type, TuplePerm permissions )
	{
		if( tuple_type == name_static )
			Permissions = TuplePerm::modify;
		else if( tuple_type == name_optional )
			Permissions = TuplePerm::modify;
		else if( tuple_type == name_dynamic || tuple_type == name_data )
			Permissions = TuplePerm::all_normal;
		else
			Permissions = permissions;
	}

	void Tuple::_assertAddPerm( name_t tuple_type, const TypeTuple& type ) const
	{
		if( !( Permissions & TuplePerm::add ) )
			throw type::AccessDenied( "Tuple does not permit adding attributes: " + eon::str( tuple_type ) );
		if( tuple_type == name_data && ( !type.isTuple() || type.at( name_type ) != name_data ) )
		{
			if( !type.isName() || LegalForDataTuple.find( type.nameValue() ) == LegalForDataTuple.end() )
				throw type::AccessDenied(
					"Tuple does not permit adding attribute: " + eon::str( tuple_type ) + " cannot add " + type.str()
					+ "!" );
		}
	}
	void Tuple::_assertRemPerm() const
	{
		if( !( Permissions & TuplePerm::remove ) )
			throw type::AccessDenied( "Tuple does not permit removing attributes: " + Type.at( name_type ).str() );
	}
	void Tuple::_assertModPerm() const
	{
		if( !( Permissions & TuplePerm::modify ) )
			throw type::AccessDenied( "Tuple does not permit modifying attributes: " + Type.at( name_type ).str() );
	}
	void Tuple::_assertTypPerm() const
	{
		if( !( Permissions & TuplePerm::types ) )
			throw type::AccessDenied( "Tuple does not permit types: " + Type.at( name_type ).str() );
	}
	void Tuple::_assertActPerm() const
	{
		if( !( Permissions & TuplePerm::actions ) )
			throw type::AccessDenied( "Tuple does not permit actions: " + Type.at( name_type ).str() );
	}

	void Tuple::_assertInRange( index_t attribute_num ) const
	{
		if( attribute_num >= Attributes.size() )
			throw type::NotFound();
	}


	void Tuple::_add( name_t tuple_type, Attribute&& value )
	{
		_assertAddPerm( tuple_type, value.type() );
		if( value.type() == name_tuple )
			value.value<Tuple>().Parent = this;
		Attributes.push_back( std::move( value ) );
	}
	void Tuple::_add( name_t tuple_type, name_t attribute_name, Attribute&& value )
	{
		if( attribute_name != no_name )
		{
			if( exists( attribute_name ) )
				throw type::DuplicateName( "Cannot add tuple attribute \"" + eon::str( attribute_name ) + "\" to tuple!" );
			_assertAddPerm( tuple_type, value.type() );
			NamedAttributes.insert( attribute_name, Attributes.size() );
		}
		if( value.type() == name_tuple )
			value.value<Tuple>().Parent = this;
		Attributes.push_back( std::move( value ) );
	}


	void Tuple::_str( ToStrData data ) const
	{
		// Tuple formatting rules
		//   1. Start out with formal type (e.g., "static(", "dynamic(", ...)
		//   2. Non-tuple attributes to be listed on the same line
		//   3. Unnamed tuple attributes to be listed on new lines
		//   4. Named tuple attributes to be listed with name + colon on new line, then tuple value on new line after that.
		//   5. Tuple values of same type as root will only start with "(" - no parenthesis if named value,
		//      otherwise include type
		//   6. Non-tuple attributes following a tuple attribute to be listed on same new line after tuple value
		//   7. End with closing ")"

		_writeTupleStart( data );
		_writeTupleAttributes( data );
		_writeTupleEnd( data );
	}
	void Tuple::_writeTupleStart( ToStrData& data ) const
	{
		if( _needFormalPrefix( data ) )
			_writeFormalTuplePrefix( data );
		else
//		else if( !data.IndentedBlock )
			_writeInformalTuplePrefix( data );
	}
	bool Tuple::_needFormalPrefix( ToStrData& data ) const
	{
		return data.isRoot()
			|| data.root().tuple().type().at( name_type ).nameValue() != type().at( name_type ).nameValue();
	}
	void Tuple::_writeFormalTuplePrefix( ToStrData& data ) const
	{
		name_t tuple_type = type().at( name_type ).nameValue();
		if( tuple_type == name_static )
			data.strf().prepend( "static" );
		else if( tuple_type == name_dynamic )
			data.strf().prepend( "dynamic" );
		else if( tuple_type == name_data )
			data.strf().prepend( "data" );
		data.strf().start_grp2( "(" );
		data.EndWithCloseParen = true;
	}
	void Tuple::_writeInformalTuplePrefix( ToStrData& data ) const
	{
		data.strf().start_grp2( "(" );
		data.EndWithCloseParen = true;
	}
	void Tuple::_writeTupleAttributes( ToStrData& data ) const
	{
		for( index_t i = 0; i < Attributes.size(); ++i )
		{
			data.AttribNo = i;
			_writeAttribute( data );
		}
	}
	void Tuple::_writeTupleEnd( ToStrData& data ) const
	{
		if( data.EndWithCloseParen )
			data.strf().end_grp2( ")" );
		//if( data.IndentedBlock && data.IndentedByAttribute >= 0 )
		//	data.strf().end_block();
		//if( data.EndWithCloseParen )
		//	data.strf().end_grp2( ")", stringify::Type::end_block );
	}
	void Tuple::_writeAttribute( ToStrData& data ) const
	{
		_separateAttributes( data );

		data.Attrib = &Attributes[ data.AttribNo ];
		data.NamedAttribute = NamedAttributes.containsTwo( data.AttribNo );
		if( data.Attrib->type().isTuple() )
			_writeAttributeTuple( data );
		else
			_writeNormalAttribute( data );
	}
	void Tuple::_separateAttributes( ToStrData& data ) const
	{
		if( data.First )
			data.First = false;
		else
		{
			data.strf().punct( "," );
//			data.strf().punct( ",", stringify::Type::end_block );
			data.strf().tert_split();
		}
	}
	void Tuple::_writeAttributeTuple( ToStrData& data ) const
	{
//		_formatTupleAttributeLine( data );
		if( data.NamedAttribute )
			_writeNamedNormalAttributeName( data );
//			_writeNamedTupleAttributeName( data );
		auto sub = ToStrData::newSub( data, data.Attrib->value<Tuple>(), data.NamedAttribute );
		data.Attrib->value<Tuple>()._str( sub );
//		if( data.NamedAttribute )
//			data.strf().end_block();
//		if( data.IndentedBlock && data.IndentedByAttribute > 0 )
//		{
//			data.strf().end_block();
//			data.IndentedBlock = false;
//		}
		data.PrevWasTuple = true;
	}
/*	void Tuple::_formatTupleAttributeLine( ToStrData& data ) const
	{
		if( !data.IndentedBlock )
		{
			data.strf().start_block();
			data.IndentedBlock = true;
			data.IndentedByAttribute = static_cast<int>( data.AttribNo );
		}
		else if( data.NamedAttribute )
			data.strf().hard_lf();
	}*/
/*	void Tuple::_writeNamedTupleAttributeName( ToStrData& data ) const
	{
		data.strf().word( eon::str( NamedAttributes.two( data.AttribNo ) ) );
		data.strf().punct( "=" );
		data.strf().start_block();
	}*/
	void Tuple::_writeNormalAttribute( ToStrData& data ) const
	{
//		_formatNormalAttributeLine( data );
		if( data.NamedAttribute )
			_writeNamedNormalAttributeName( data );
		_writeValue( data.strf(), *data.Attrib );
	}
/*	void Tuple::_formatNormalAttributeLine( ToStrData& data ) const
	{
		if( data.PrevWasTuple )
		{
			data.PrevWasTuple = false;
			if( !data.IndentedBlock )
			{
				data.strf().start_block();
				data.IndentedBlock = true;
				data.IndentedByAttribute = 0;
			}
		}
	}*/
	void Tuple::_writeNamedNormalAttributeName( ToStrData& data ) const
	{
		data.strf().word( eon::str( NamedAttributes.two( data.AttribNo ) ) );
		data.strf().op2( "=" );
	}
	void Tuple::_writeValue( Stringifier& strf, const Attribute& attribute ) const
	{
		name_t type = attribute.type().nameValue();
		auto prefix = StrPrefixPostfixForTypes.find( type );
		if( prefix != StrPrefixPostfixForTypes.end() )
			strf.prepend( prefix->second.first );

		if( type == name_bool )
			strf.word( string( attribute.value<bool>() ) );
		else if( type == name_byte )
			strf.word( string( attribute.value<byte_t, type::hint::byte>() ) );
		else if( type == name_char )
			strf.word( string( attribute.value<char_t>() ) );
		else if( type == name_int )
			strf.word( string( attribute.value<int_t>() ) );
		else if( type == name_short )
			strf.word( string( attribute.value<short_t>() ) );
		else if( type == name_long )
			strf.word( string( attribute.value<long_t>() ) );
		else if( type == name_float )
			strf.word( string( attribute.value<flt_t>() ) );
		else if( type == name_low )
			strf.word( string( attribute.value<low_t>() ) );
		else if( type == name_high )
			strf.word( string( attribute.value<high_t>() ) );
		else if( type == name_index )
			strf.word( string( attribute.value<index_t, type::hint::index>() ) );
		else if( type == name_name )
			strf.word( eon::str( attribute.value<name_t>() ) );
		else if( type == name_b8 )
			strf.word( string( attribute.value<b8_t, type::hint::bits>() ) );
		else if( type == name_b16 )
			strf.word( string( attribute.value<b16_t, type::hint::bits>() ) );
		else if( type == name_b32 )
			strf.word( string( attribute.value<b32_t, type::hint::bits>() ) );
		else if( type == name_b64 )
			strf.word( string( attribute.value<b64_t, type::hint::bits>() ) );
		else if( type == name_bytes )
			strf.word( string( attribute.value<std::string>() ) );
		else if( type == name_string )
			strf.word( attribute.value<string>().replace( { { "\"", "\\\"" }, { "\n", "\\n" } } ) );
		//		else if( type == name_real )
		//			strf.word( string( attribute.value<real_t>() ) );
		//		else if( type == name_complex )
		//			strf.word( string( attribute.value<complex_t>() ) );
		else if( type == name_regex )
			strf.word( attribute.value<regex>().str() );
		else if( type == name_regex )
			strf.word( attribute.value<regex>().str() );
		else if( type == name_namepath )
			strf.word( string( attribute.value<namepath>().str() ) );
		else if( type == name_path )
			strf.word( string( attribute.value<path>().str() ) );
		else if( type == name_typetuple )
			attribute.value<TypeTuple>().str( strf );
		//		else if( type == name_time )
		//			strf.word( string( attribute.value<time_t>() ) );
		//		else if( type == name_date )
		//			strf.word( string( attribute.value<date_t>() ) );
		//		else if( type == name_timezone )
		//			strf.word( string( attribute.value<timezone_t>() ) );
		//		else if( type == name_dst )
		//			strf.word( string( attribute.value<dst_t>() ) );
		//		else if( type == name_timespan )
		//			strf.word( string( attribute.value<timespan_t>() ) );
		else if( type == name_expression )
			strf.word( attribute.value<Expression>().str() );

		if( prefix != StrPrefixPostfixForTypes.end() )
			strf.append( prefix->second.second );
	}

	std::unordered_set<name_t> Tuple::LegalForDataTuple;
	void Tuple::_populateLegalForDataTuple()
	{
		if( LegalForDataTuple.empty() )
		{
			LegalForDataTuple = {
				name_bool,
				name_byte, name_char,
				name_int, name_short, name_long,
				name_float, name_low, name_high,
				name_index,
				name_name,
				name_bytes,
				name_string,
				name_real, name_complex,
				name_regex,
				name_namepath,
				name_path,
				name_typetuple,
				name_time, name_date, name_timezone, name_dst, name_timespan,
				name_expression};
		}
	}

	std::unordered_map<name_t, std::pair<string, string>> Tuple::StrPrefixPostfixForTypes;
	void Tuple::_populateStrPrefixPostfixForTypes()
	{
		if( StrPrefixPostfixForTypes.empty() )
		{
			StrPrefixPostfixForTypes = {
				{ name_bool, { "", "" } },
				{ name_byte, { "B'", "'" } },
				{ name_char, { "'", "'" } },
				{ name_int, { "", "" } },
				{ name_short, { "", "" } },
				{ name_long, { "", "" } },
				{ name_float, { "", "" } },
				{ name_low, { "", "" } },
				{ name_high, { "", "" } },
				{ name_index, { "", "" } },
				{ name_name, { "", "" } },
				{ name_bytes, { "B\"", "\"" } },
				{ name_string, { "\"", "\"" } },
				{ name_real, { "", "r" } },
				{ name_complex, { "", "i" } },
				{ name_regex, { "r\"", "\"" } },
//				{ name_namepath, { "@", "" } },
				{ name_path, { "p\"", "\"" } },
				{ name_time, { "", "" } },
				{ name_date, { "", "" } },
				{ name_timezone, { "", "" } },
				{ name_dst, { "", "" } },
				{ name_timespan, { "", "" } },
				{ name_expression, { "ex(", ")" } } };
		}
	}


	void Tuple::_replaceCopyAttributes( const Tuple& other )
	{
		Attributes.clear();
		for( auto& attribute : other.Attributes )
		{
			Attributes.push_back( attribute );
			if( attribute.type() == name_tuple )
				Attributes[ Attributes.size() - 1 ].value<Tuple>().Parent = this;
		}
		NamedAttributes = other.NamedAttributes;
	}
	void Tuple::_replaceMoveAttributes( Tuple& other )
	{
		Attributes = std::move( other.Attributes );
		for( auto& attribute : Attributes )
			if( attribute.type() == name_tuple )
				attribute.value<Tuple>().Parent = this;
		NamedAttributes = std::move( other.NamedAttributes );
	}

	void Tuple::_addAttributeName( name_t name, const TypeTuple& type )
	{
		if( exists( name ) )
			throw type::DuplicateName( "Cannot add tuple attribute \"" + eon::str( name ) + "\" to tuple!" );
		_assertAddPerm( Type.at( name_type ).nameValue(), type );
		NamedAttributes.insert( name, Attributes.size() );
	}
	void Tuple::_addAttributeValue( Attribute&& value )
	{
		Attributes.push_back( std::move( value ) );
		auto& attribute = Attributes[ Attributes.size() - 1 ];
		if( attribute.type().isTuple() )
			attribute.value<Tuple>().Parent = this;
	}

	std::set<TypeTuple> Tuple::_signatures(
		name_t action_name,
		const std::vector<std::pair<name_t, name_t>>& name_fields,
		const TypeTuple* args ) const
	{
		auto found = ActionsByName.find( action_name );
		std::set<TypeTuple> actions;
		if( found != ActionsByName.end() )
		{
			for( auto& signature : found->second )
			{
				if( _matchingSignature( signature, name_fields, args ) )
					actions.insert( signature );
			}
		}
		return actions;
	}

	bool Tuple::_matchingSignature(
		const TypeTuple& signature,
		const std::vector<std::pair<name_t, name_t>>& name_fields,
		const TypeTuple* args ) const noexcept
	{
		for( auto& field : name_fields )
		{
			if( signature.at( field.first ) != field.second )
				return false;
		}
		if( args && args->numAttributes() > 0 )
		{
			if( !signature.exists( name_args ) )
				return false;
			if( signature.at( name_args ) != *args )
				return false;
		}
		else if( signature.exists( name_args ) )
			return false;
		//if( args && signature.exists( name_args ) && signature.at( name_args ) != *args )
		//	return false;
		return true;
	}
}
