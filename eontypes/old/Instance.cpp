#include "Instance.h"
#include <eonexpression/Expression.h>


namespace eon
{
	instance::instance( tup::varscope& scope, eon::tuple* value, uint32_t refs )
	{
		Type = value->type();
		Value.Tuple = value;
		References = refs;
	}




	instance* instance::copy() const
	{
		auto i = new instance();
		i->addRef();
		if( Type != nullptr )
		{
			i->Type = Type;
			if( Type->isPrimitive() )
				i->Value = Value;

			else if( Type->is( name_bytes ) )
				i->Value.Bytes = new std::string( *Value.Bytes );
			else if( Type->is( name_string ) )
				i->Value.String = new string( *Value.String );
			else if( Type->is( name_regex ) )
				i->Value.Regex = new regex( *Value.Regex );
			else if( Type->is( name_path ) )
				i->Value.Path = new path( *Value.Path );
			else if( Type->is( name_expression ) )
				i->Value.Expression = new expression( *Value.Expression );
			else if( Type->is( name_array ) )
				i->Value.Array = Value.Array->copy();
/*			else if( Type->is( name_list ) )
			{
				for( auto elm : *Value.List )
					i->Value.Array->push_back( elm->copy() );
			}*/

			else if( Type->isTuple() )
				i->Value.Tuple = Value.Tuple->copy();

			else if( Type->isReference() )
			{
				i->Value.Reference = Value.Reference;
				i->Value.Reference->addRef();
			}
		}
		return i;
	}

	string instance::str() const
	{
		if( Type == nullptr )
			return "";
		if( Type->is( name_bool ) )
			return string( Value.Bool );
		else if( Type->is( name_byte ) )
			return string( Value.Byte );
		else if( Type->is( name_char ) )
			return string( Value.Char );
		else if( Type->is( name_int ) )
			return string( Value.Int );
		else if( Type->is( name_int8 ) )
			return string( Value.Int8 );
		else if( Type->is( name_int16 ) )
			return string( Value.Int16 );
		else if( Type->is( name_int32 ) )
			return string( Value.Int32 );
		else if( Type->is( name_int64 ) )
			return string( Value.Int64 );
		else if( Type->is( name_float ) )
			return string( Value.Float64 );
		else if( Type->is( name_float32 ) )
			return string( Value.Float32 );
		else if( Type->is( name_float64 ) )
			return string( Value.Float64 );
		else if( Type->is( name_float128 ) )
			return string( Value.Float128 );
		else if( Type->is( name_position ) )
			return string( Value.Position );
		else if( Type->is( name_name ) )
			return string( Value.Name );

		else if( Type->is( name_bytes ) )
			return "b\"" + *Value.Bytes + "\"";
		else if( Type->is( name_string ) )
			return "\"" + *Value.String + "\"";
		else if( Type->is( name_regex ) )
			return "?" + Value.Regex->str();
		else if( Type->is( name_path ) )
			return "p\"" + Value.Path->str() + "\"";
		else if( Type->is( name_expression ) )
			return "e(" + Value.Expression->infixStr() + ")";

		else if( Type->is( name_array ) )
			return Value.Array->str();
/*		else if( Type->is( name_list ) )
		{
			string s = "[";
			bool first = true;
			for( auto& inst : *Value.List )
			{
				if( first )
					first = false;
				else
					s += ", ";
				s += inst->str();
			}
			s += "]";
			return s;
		}*/

		else if( Type->isTuple() )
			return "(" + Value.Tuple->str() + ")";

		else if( Type->isReference() )
			return Value.Reference->str();

		else
			return "";
	}




	void instance::copyAssign( const instance* other )
	{
		auto copy = other->copy();
		Type = copy->Type; copy->Type = nullptr;
		Value = copy->Value;
		References = 0;
		delete copy;
	}
	void instance::moveAssign( instance* other )
	{
		Type = other->Type; other->Type = nullptr;
		Value = other->Value;
		References = 0;
	}




	int instance::compare( const instance& other ) const
	{
		if( Type == nullptr )
			return other.Type == nullptr ? 0 : -1;
		if( other.Type == nullptr )
			return 1;
		if( Type->isReference() )
			return Value.Reference->compare( other );
		else if( other.Type->isReference() )
			return compare( *other.Value.Reference );
		auto cmp = Type->compare( *other.Type );
		if( cmp != 0 )
			return cmp;

		if( Type->is( name_bool ) )
			return Value.Bool < other.Value.Bool ? -1 : Value.Bool == other.Value.Bool ? 0 : 1;
		else if( Type->is( name_byte ) )
			return Value.Byte < other.Value.Byte ? -1 : Value.Byte == other.Value.Byte ? 0 : 1;
		else if( Type->is( name_int ) )
			return Value.Int < other.Value.Int ? -1 : Value.Int == other.Value.Int ? 0 : 1;
		else if( Type->is( name_int8 ) )
			return Value.Int8 < other.Value.Int8 ? -1 : Value.Int8 == other.Value.Int8 ? 0 : 1;
		else if( Type->is( name_int16 ) )
			return Value.Int16 < other.Value.Int16 ? -1 : Value.Int16 == other.Value.Int16 ? 0 : 1;
		else if( Type->is( name_int32 ) )
			return Value.Int32 < other.Value.Int32 ? -1 : Value.Int32 == other.Value.Int32 ? 0 : 1;
		else if( Type->is( name_int64 ) )
			return Value.Int64 < other.Value.Int64 ? -1 : Value.Int64 == other.Value.Int64 ? 0 : 1;
		else if( Type->is( name_float ) )
			return Value.Float64 < other.Value.Float64 ? -1 : Value.Float64 == other.Value.Float64 ? 0 : 1;
		else if( Type->is( name_float32 ) )
			return Value.Float32 < other.Value.Float32 ? -1 : Value.Float32 == other.Value.Float32 ? 0 : 1;
		else if( Type->is( name_float64 ) )
			return Value.Float64 < other.Value.Float64 ? -1 : Value.Float64 == other.Value.Float64 ? 0 : 1;
		else if( Type->is( name_float128 ) )
			return Value.Float128 < other.Value.Float128 ? -1 : Value.Float128 == other.Value.Float128 ? 0 : 1;
		else if( Type->is( name_position ) )
			return Value.Position < other.Value.Position ? -1 : Value.Position == other.Value.Position ? 0 : 1;
		else if( Type->is( name_name ) )
			return Value.Name < other.Value.Name ? -1 : Value.Name == other.Value.Name ? 0 : 1;

		else if( Type->is( name_bytes ) )
			return Value.Bytes->compare( *other.Value.Bytes );
		else if( Type->is( name_string ) )
			return Value.String->compare( *other.Value.String );
		else if( Type->is( name_regex ) )
			return Value.Regex->str().compare( other.Value.Regex->str() );
		else if( Type->is( name_path ) )
			return Value.Path->str().compare( other.Value.Path->str() );
		else if( Type->is( name_expression ) )
			return Value.Expression->infixStr().compare( other.Value.Expression->infixStr() );

		else if( Type->is( name_array ) )
			return Value.Array->compare( *other.Value.Array );
/*		else if( Type->is( name_list ) )
		{
			auto i = Value.List->begin(), j = other.Value.List->begin();
			for( ; i != Value.List->end() && j != other.Value.List->end(); ++i, ++j )
			{
				auto cmp = (*i)->compare( **j );
				if( cmp != 0 )
					return cmp;
			}
			if( i != Value.List->end() )
				return -1;
			else
				return j != other.Value.List->end() ? 1 : 0;
		}*/

		else if( Type->isTuple() )
			return Value.Tuple->compare( *other.Value.Tuple );

		else
			throw WrongType();
	}




	void instance::_delete()
	{
		if( Type == nullptr )
			return;
		else if( Type->isStandard() )
		{
			if( Type->is( name_bytes ) )
				delete Value.Bytes;
			else if( Type->is( name_string ) )
				delete Value.String;
			else if( Type->is( name_regex ) )
				delete Value.Regex;
			else if( Type->is( name_path ) )
				delete Value.Path;
			else if( Type->is( name_expression ) )
				delete Value.Expression;

			else if( Type->is( name_array ) )
				delete Value.Array;
			else if( Type->is( name_list ) )
				delete Value.List;
		}
		else if( Type->isTuple() )
			delete Value.Tuple;
		else if( Type->isReference() )
			Value.Reference->delRef();
		Type = nullptr;
	}
}
