#include "BasicTuple.h"
#include "Name.h"
#include "MetaData.h"
#include "Tuple.h"
#include "TypeHandler.h"
#include "DynamicTuple.h"
#include "DataTuple.h"
#include "MetaData.h"


namespace eon
{
	namespace type
	{
		void BasicTuple::finalize() noexcept
		{
			if( tupleType() == name_plain )
				TState = State::_static;
			_generateTupleID();
			Finalized = true;
		}

		BasicTuple& BasicTuple::operator=( const BasicTuple& other )
		{
			Finalized = false;
			TState = other.TState;
			*static_cast<Object*>( this ) = other;
			for( auto& attribute : other )
			{
				if( attribute.name() != no_name )
					NamedAttribs[ attribute.name() ] = Attributes.size();
				Attributes.push_back( attribute );
			}
			Finalized = other.Finalized;
			return *this;
		}



		void BasicTuple::str( Stringifier& str ) const
		{
			auto ttype = tupleType();
			if( ttype == name_meta )
			{
				str.addWord( "M(" );
				str.noSpacing();
			}
			str.pushTuple( ttype );
			_str( str );
			str.popTuple();
			if( ttype == name_meta )
			{
				str.markRaw();
				str.addWord( ")" );
			}
		}




		BasicTuple* BasicTuple::addPlainTuple( name_t name )
		{
			if( Finalized )
				throw AccessDenied();
			static TypeTuple type( name_plain );
			_assertCanAdd( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( *name );
				NamedAttribs[ name ] = Attributes.size();
			}
			auto tuple = new Tuple();
			Attributes.push_back( Attribute( name, tuple->type(), tuple ) );
			return tuple;
		}
		BasicTuple* BasicTuple::addDynamicTuple( name_t name )
		{
			if( Finalized )
				throw AccessDenied();
			static TypeTuple type( name_dynamic );
			_assertCanAdd( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( *name );
				NamedAttribs[ name ] = Attributes.size();
			}
			auto tuple = new DynamicTuple();
			Attributes.push_back( Attribute( name, tuple->type(), tuple ) );
			return tuple;
		}
		BasicTuple* BasicTuple::addDataTuple( name_t name )
		{
			if( Finalized )
				throw AccessDenied();
			static TypeTuple type( name_data );
			_assertCanAdd( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( *name );
				NamedAttribs[ name ] = Attributes.size();
			}
			auto tuple = new DataTuple();
			Attributes.push_back( Attribute( name, tuple->type(), tuple ) );
			return tuple;
		}
		BasicTuple* BasicTuple::addMetaData( name_t name )
		{
			if( Finalized )
				throw AccessDenied();
			static TypeTuple type( name_meta  );
			_assertCanAdd( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( *name );
				NamedAttribs[ name ] = Attributes.size();
			}
			auto tuple = new MetaData();
			Attributes.push_back( Attribute( name, tuple->type(), (Object*)tuple ) );
			return tuple;
		}

		BasicTuple& BasicTuple::operator+=( Attribute attribute )
		{
			_assertCanAdd( attribute.type() );
			if( attribute.name() != no_name )
			{
				if( exists( attribute.name() ) )
					throw DuplicateName( *attribute.name() );
				NamedAttribs[ attribute.name() ] = Attributes.size();
			}
			Attributes.push_back( attribute );
			if( Finalized )
				_generateTupleID();
			return *this;
		}
		BasicTuple& BasicTuple::operator+=( const BasicTuple& other )
		{
			for( auto& attribute : other.Attributes )
			{
				_assertCanAdd( attribute.type() );
				if( attribute.name() != no_name )
				{
					if( exists( attribute.name() ) )
						throw DuplicateName( *attribute.name() );
					NamedAttribs[ attribute.name() ] = Attributes.size();
				}
				Attributes.push_back( attribute );
			}
			if( Finalized )
				_generateTupleID();
			return *this;
		}

		BasicTuple& BasicTuple::operator-=( const BasicTuple& other )
		{
			_assertCanRemove();
			if( !other.type().compatibleWith( Type ) )
				throw AccessDenied();

			// Keep only unnamed attributes that have no positional match in
			// 'other', and named attributes that have a named counterpart in
			// 'other'.
			auto old = std::move( Attributes );
			auto old_names = std::move( NamedAttribs );
			for( size_t i = 0; i < other.Attributes.size(); ++i )
			{
				auto& other_elm = other.Attributes[ i ];
				if( other_elm.name() != no_name && old_names.find( other_elm.name() ) == old_names.end() )
				{
					if( old[ i ].name() != no_name )
						NamedAttribs[ old[ i ].name() ] = Attributes.size();
					Attributes.push_back( std::move( old[ i ] ) );
				}
			}
			for( size_t i = other.Attributes.size(); i < old.size(); ++i )
			{
				if( old[ i ].name() != no_name )
					NamedAttribs[ old[ i ].name() ] = Attributes.size();
				Attributes.push_back( std::move( old[ i ] ) );
			}
			if( Finalized )
				_generateTupleID();
			return *this;
		}

		BasicTuple& BasicTuple::remove( name_t name )
		{
			if( generalType() != name_dynamic )
				throw AccessDenied();
			auto found = NamedAttribs.find( name );
			if( found == NamedAttribs.end() )
				throw NotFound();
			Attributes.erase( Attributes.begin() + found->second );
			NamedAttribs.erase( found );
			if( Finalized )
				_generateTupleID();
			return* this;
		}
		BasicTuple& BasicTuple::remove( size_t pos )
		{
			if( generalType() != name_dynamic )
				throw AccessDenied();
			if( pos >= Attributes.size() )
				throw NotFound();
			for( auto named = NamedAttribs.begin(); named != NamedAttribs.end(); ++named )
			{
				if( named->second == pos )
				{
					NamedAttribs.erase( named );
					break;
				}
			}
			Attributes.erase( Attributes.begin() + pos );
			if( Finalized )
				_generateTupleID();
			return *this;
		}


		int BasicTuple::compare( const BasicTuple& other ) const noexcept
		{
			index_t pos = 0;
			index_t size = Attributes.size() < other.Attributes.size() ? Attributes.size() : other.Attributes.size();
			int cmp{ 0 };
			for( ; pos < size; ++pos )
			{
				auto& lhs = Attributes[ pos ];
				auto& rhs = other.Attributes[ pos ];

				// Make sure the names are the same
				if( lhs.name() != rhs.name() )
					return lhs.name() < rhs.name() ? -1 : 1;

				// Make sure the types are the same
				cmp = _type( pos ).compare( other._type( pos ) );
				if( cmp != 0 )
					return cmp;

				// Make sure we have values
				if( !lhs.value() )
				{
					if( rhs.value() )
						continue;
					else
						return -1;
				}
				else if( !rhs.value() )
					return 1;

				// Make sure general types matches
				auto general_type = lhs.value()->generalType();
				if( general_type != rhs.value()->generalType() )
					return general_type < rhs.value()->generalType() ? -1 : 1;

				// If we have tuples, then compare as such
				if( general_type == name_dynamic )
					cmp = ( (BasicTuple*)lhs.value() )->compare( *(BasicTuple*)rhs.value() );
				else if( general_type == name_tuple )
					cmp = ( (Tuple*)lhs.value() )->compare( *(Tuple*)rhs.value() );
				else if( general_type == name_meta )
					cmp = ( (MetaData*)lhs.value() )->compare( *(MetaData*)rhs.value() );
				else if( general_type == name_instance )
					cmp = ( (Instance*)lhs.value() )->compare( *(Instance*)rhs.value() );

				// TODO: Should we include enums and definitions here?

				if( cmp != 0 )
					return cmp;
			}
			return pos < other.Attributes.size() ? -1 : pos < Attributes.size() ? 1 : 0;
		}




		TypeTuple BasicTuple::_makeTupleID( const std::initializer_list<Object*>& values )
		{
			std::vector<TypeElement*> elements;
			for( auto value : values )
			{
				if( !value )
					throw NullValue();
				auto& type = value->type();
				if( type.isName() )
					elements.push_back( new NameElement( type.asName() ) );
				else
					elements.push_back( new TypeTuple( type ) );
			}
			return TypeTuple( tupleType(), elements );
		}
		TypeTuple BasicTuple::_makeTupleID( const std::initializer_list<Attribute>& attributes )
		{
			std::vector<TypeElement*> elements;
			for( auto attribute : attributes )
			{
				if( attribute.type().isName() )
					elements.push_back( new NameElement( attribute.type().asName() ) );
				else
					elements.push_back( new TypeTuple( attribute.type() ) );
			}
			return TypeTuple( tupleType(), elements );
		}
		void BasicTuple::_generateTupleID()
		{
			std::vector<TypeElement*> elements;
			for( auto& attribute : Attributes )
			{
				if( attribute.type().isName() )
					elements.push_back( new NameElement( attribute.name(), attribute.type().asName() ) );
				else
					elements.push_back( new TypeTuple( attribute.name(), attribute.type() ) );
			}
			Type = TypeTuple( tupleType(), elements );
		}

		void BasicTuple::_initState( name_t tuple_type ) noexcept
		{
			Type = TypeTuple( tuple_type );
			if( tuple_type == name_plain )
				TState = State::restricted;
			else if( tuple_type == name_dynamic )
				TState = State::open;
			else if( tuple_type == name_meta )
				TState = State::restricted;
			else if( tuple_type == name_lambda )
				TState = State::restricted;
			else if( tuple_type == name_args )
				TState = State::restricted;
		}

		void BasicTuple::_assertCanAdd( const TypeTuple& type )
		{
			if( !_canAdd() )
				throw AccessDenied();
			
			if( tupleType() == name_data || tupleType() == name_meta )
			{
				if( tupleType() == name_meta && type.isTuple() )
				{
					if( type.tupleType() != name_meta )
						throw AccessDenied( type.tupleType()->ucFirst()
							+ " tuple is not a legal attribute type for tuple type " + *tupleType() + "!" );
				}
				if( !Handler::legalForDataTuple( type ) )
					throw AccessDenied( type.str() + " is not a legal attribute type for tuple type " + *tupleType() + "!" );
			}
			
			else if( tupleType() == name_lambda )
			{
				if( type.isName() )
					throw AccessDenied( type.str() + " is not a legal attribute for lambda! "
						"Only named attributes \"state\", \"args\", and \"return\" are possible!" );
				if( type.Name != name_state && type.Name != name_args && type.Name != name_return )
					throw AccessDenied( type.str() + " is not a legal attribute for lambda! "
						"Only named attributes \"state\", \"args\", and \"return\" are possible!" );
				if( type.Name == name_state )
				{
					if( !type.isTupleElement() )
						throw AccessDenied( type.str() + " is not a legal attribute for lambda! "
							"The value of \"state\" must be a tuple!" );
				}
				else if( type.Name == name_args )
				{
					if( !type.isTupleElement() )
						throw AccessDenied( type.str() + " is not a legal attribute for lambda! "
							"The value of \"args\" must be a tuple!" );
				}
				else if( type.Name == name_return )
				{
					if( type.asName() != name_expression )
						throw AccessDenied( type.str() + " is not a legal attribute for lambda! "
							"The value of \"return\" must be an expression!" );
				}
			}
		}

		void BasicTuple::_assertValidFor( name_t tuple_type, const BasicTuple& other )
		{
			if( tuple_type == name_data || tuple_type == name_meta )
			{
				for( auto& attribute : other )
				{
					if( !Handler::legalForDataTuple( attribute.type() ) )
						throw AccessDenied();
				}
			}
		}

		name_t BasicTuple::_nameValue( index_t pos ) const noexcept
		{
			return ( (const NameInstance*)Attributes[ pos ].value() )->value();
		}

		void BasicTuple::_str( type::Stringifier& str ) const
		{
			bool first = true;
			for( auto& element : Attributes )
			{
				if( first )
					first = false;
				else
				{
					str.addWord( "," );
					str.spacingOnce();
				}

				if( element.type().isTuple() )
				{
					auto ttype = element.type().tupleType();
					if( element.name() )
					{
						str.addWord( element.name() );
						if( ttype == name_data )
						{
							str.addWord( ":" );
							str.addIndent();
							str.newLine();
							str.pushTuple( ttype );
							standardPrefix( str );
							( (const DataTuple*)element.value() )->_str( str );
							standardPostfix( str );
							str.popTuple();
							str.noSpacing();
							str.reduceIndent();
						}
						else
						{
							str.addWord( "=" );
							str.pushTuple( ttype );
							explicitPrefix( str );
							( (const DataTuple*)element.value() )->_str( str );
							explicitPostfix( str );
							str.popTuple();
						}
					}
				}
				else
				{
					if( element.name() )
					{
						str.addWord( element.name() );
						str.addWord( "=" );
					}
					element.value()->str( str );
				}
			}
		}

		bool BasicTuple::isDynamicTuple( const std::type_info& type ) { return type == typeid( DynamicTuple ); }
		bool BasicTuple::isDataTuple( const std::type_info& type ) { return type == typeid( DataTuple ); }
		bool BasicTuple::isMetaData( const std::type_info& type ) { return type == typeid( MetaData ); }
		bool BasicTuple::isLambda( const std::type_info& type ) { return false; }// return type == typeid( Lambda ); }
	}
}
