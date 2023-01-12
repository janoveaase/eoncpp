#include "Action.h"
#include "Tuple.h"
#include <eonregex/RegEx.h>
#include <eonstring/NamePath.h>
#include <eonfilesys/Path.h>
#include "Expression.h"


namespace eon
{
	std::unordered_map<name_t, Attribute::TypeHandlerPtr> Attribute::TypeHandlers;


	Attribute Attribute::newTuple( Tuple&& value, type::Qualifier qualifier, source::Ref source )
	{
		Attribute a;
		a.Type = value.type(); a.Type.qualifier( qualifier );
		a.Value = new Tuple( std::move( value ) );
		a.Source = source;
		return a;
	}

	Attribute Attribute::newName( name_t value, type::Qualifier qualifier, source::Ref source )
	{
		Attribute a;
		a.Type = name_name;
		a.Type.qualifier( qualifier );
		a.Value = new name_t( value );
		a.Source = source;
		return a;
	}

	Attribute Attribute::newTypeTuple( TypeTuple&& value, type::Qualifier qualifier, source::Ref source )
	{
		Attribute a;
		a.Type = name_typetuple;
		a.Type.qualifier( qualifier );
		a.Value = new TypeTuple( std::move( value ) );
		a.Source = source;
		return a;
	}

	Attribute Attribute::newTypeTuple( const TypeTuple& value, type::Qualifier qualifier, source::Ref source )
	{
		Attribute a;
		a.Type = name_typetuple;
		a.Type.qualifier( qualifier );
		a.Value = new TypeTuple( value );
		a.Source = source;
		return a;
	}




	Attribute& Attribute::operator=( const Attribute& other )
	{
		_clear();
		Type = other.Type;
		auto handler = TypeHandlers.find( Type.name() );
		if( handler != TypeHandlers.end() )
		{
			Value = handler->second->copy( other.Value );
			Owned = true;
		}
		Source = other.Source;
		return *this;
	}
	Attribute& Attribute::operator=( Attribute& other )
	{
		_clear();
		Type = other.Type;
		Value = other.Value;
		Source = other.Source;
		Owned = false;
		return *this;
	}
	Attribute& Attribute::operator=( Attribute&& other ) noexcept
	{
		_clear();
		Type = std::move( other.Type );
		Value = other.Value;
		other.Value = nullptr;
		Owned = other.Owned;
		other.Owned = true;
		Source = other.Source;
		return *this;
	}




	void Attribute::str( Stringifier& strf ) const
	{
		auto handler = TypeHandlers.find( Type.name() );
		if( handler != TypeHandlers.end() )
			handler->second->str( strf, Value );
	}




	void Attribute::_prep()
	{
		if( TypeHandlers.empty() )
		{
			TypeHandlers = {
				{ name_bool, TypeHandlerPtr( new TypeHandlerStrSupported<bool>() ) },
				{ name_byte, TypeHandlerPtr( new TypeHandlerStrSupported<byte_t>() ) },
				{ name_char, TypeHandlerPtr( new TypeHandlerStrSupported<char_t>() ) },
				{ name_int, TypeHandlerPtr( new TypeHandlerStrSupported<int_t>() ) },
				{ name_short, TypeHandlerPtr( new TypeHandlerStrSupported<short_t>() ) },
				{ name_long, TypeHandlerPtr( new TypeHandlerStrSupported<long_t>() ) },
				{ name_float, TypeHandlerPtr( new TypeHandlerStrSupported<flt_t>() ) },
				{ name_low, TypeHandlerPtr( new TypeHandlerStrSupported<low_t>() ) },
				{ name_high, TypeHandlerPtr( new TypeHandlerStrSupported<high_t>() ) },
				{ name_index, TypeHandlerPtr( new TypeHandlerStrSupported<index_t>() ) },
				{ name_name, TypeHandlerPtr( new TypeHandlerName() ) },
				{ name_handle, TypeHandlerPtr( new TypeHandlerStrSupported<handle_t>() ) },
				{ name_bytes, TypeHandlerPtr( new TypeHandlerStrSupported<std::string>() ) },
				{ name_string, TypeHandlerPtr( new TypeHandlerStr() ) },
				{ name_regex, TypeHandlerPtr( new TypeHandlerStrMethod<regex>() ) },
				{ name_namepath, TypeHandlerPtr( new TypeHandlerStrMethod<namepath>() ) },
				{ name_path, TypeHandlerPtr( new TypeHandlerStrMethod<path>() ) },
				{ name_tuple, TypeHandlerPtr( new TypeHandlerStrMethod<Tuple>() ) },
				{ name_expression, TypeHandlerPtr( new TypeHandlerStrMethod<Expression>() ) }
				// TODO: Add more!
			};
		}
	}

	void Attribute::_clear() noexcept
	{
		if( Value )
		{
			if( Owned )
			{
				auto handler = TypeHandlers.find( Type.name() );
				if( handler != TypeHandlers.end() )
					handler->second->destroy( Value );
			}
			Value = nullptr;
		}
	}

	TypeTuple Attribute::_tupleType( void* value ) const noexcept
	{
		return ((Tuple*)value )->type();
	}
}
