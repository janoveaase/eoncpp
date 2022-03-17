#include "BasicTuple.h"
#include "Name.h"
#include "Tuple.h"
#include "TypeHandler.h"
#include "DynamicTuple.h"
#include "DataTuple.h"


namespace eon
{
	namespace type
	{
		void BasicTuple::finalize() noexcept
		{
			static std::unordered_map<name_t, Permission> perm{
				{ name_open, Permission::mod_var },
				{ name_plain, Permission::mod_var },
				{ name_dynamic, Permission::open },
				{ name_data, Permission::open },
				{ name_lambda, Permission::add_var | Permission::mod_var },
				{ name_args, Permission::add_var | Permission::mod_var },
				{ name_return, Permission::add_var | Permission::mod_var },

				{ name_locked, Permission::none },
				{ name_static, Permission::mod_var },
				{ name_protected, Permission::add_var },
				{ name_restricted, Permission::add_var | Permission::mod_var },
				{ name_open, Permission::open },

				{ name_global, Permission::open | Permission::add_cache | Permission::add_grp },
				{ name_action, Permission::add_var | Permission::mod_var },
				{ name_local, Permission::add_var | Permission::mod_var }
			};
			Perm = perm.find( tupleType() )->second;

			_generateTupleID();
			Finalized = true;
		}

		BasicTuple& BasicTuple::operator=( const BasicTuple& other )
		{
			Finalized = false;
			Perm = other.Perm;
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
			if( ttype == name_data || ttype == name_dynamic )
			{
				str.spacingOnce();
				str.addWord( eon::str( ttype ) + "(" );
				str.noSpacing();
			}
			else if( ttype == name_plain )
			{
				str.spacingOnce();
				str.addWord( "p(" );
				str.noSpacing();
			}
			str.pushTuple( ttype );
			_str( str );
			str.popTuple();
			if( isTupleType( ttype ) )
			{
				str.markRaw();
				str.addWord( ")" );
				str.spacingOnce();
			}
		}




		BasicTuple* BasicTuple::addPlainTuple( name_t name )
		{
			if( Finalized )
				throw AccessDenied();
			static auto type = TypeTuple::tuple( name_plain );
			_assertCanAddVar( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( eon::str( name ) );
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
			static auto type = TypeTuple::tuple( name_dynamic );
			_assertCanAddVar( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( eon::str( name ) );
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
			static auto type = TypeTuple::tuple( name_data );
			_assertCanAddVar( type );
			if( name )
			{
				if( exists( name ) )
					throw DuplicateName( eon::str( name ) );
				NamedAttribs[ name ] = Attributes.size();
			}
			auto tuple = new DataTuple();
			Attributes.push_back( Attribute( name, tuple->type(), tuple ) );
			return tuple;
		}

		BasicTuple& BasicTuple::operator+=( Attribute&& attribute )
		{
			_assertCanAddVar( attribute.type() );
			if( attribute.name() != no_name )
			{
				if( exists( attribute.name() ) )
					throw DuplicateName( eon::str( attribute.name() ) );
				NamedAttribs[ attribute.name() ] = Attributes.size();
			}
			Attributes.push_back( std::move( attribute ) );
			if( Finalized )
				_generateTupleID();
			return *this;
		}
		BasicTuple& BasicTuple::operator+=( const BasicTuple& other )
		{
			for( auto& attribute : other.Attributes )
			{
				_assertCanAddVar( attribute.type() );
				if( attribute.name() != no_name )
				{
					if( exists( attribute.name() ) )
						throw DuplicateName( eon::str( attribute.name() ) );
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
			_assertCanDelVar();
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
				else if( general_type == name_instance )
					cmp = ( (Instance*)lhs.value() )->compare( *(Instance*)rhs.value() );

				// TODO: Should we include enums and definitions here?

				if( cmp != 0 )
					return cmp;
			}
			return pos < other.Attributes.size() ? -1 : pos < Attributes.size() ? 1 : 0;
		}




		TypeTuple BasicTuple::_makeTupleID( name_t tuple_type, const std::initializer_list<Object*>& values )
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
			return TypeTuple::tuple( tuple_type, elements );
		}
		TypeTuple BasicTuple::_makeTupleID( name_t tuple_type, const std::initializer_list<Attribute>& attributes )
		{
			std::vector<TypeElement*> elements;
			for( auto& attribute : attributes )
			{
				if( attribute.type().isName() )
					elements.push_back( new NameElement( attribute.type().asName() ) );
				else
					elements.push_back( new TypeTuple( attribute.type() ) );
			}
			return TypeTuple::tuple( tuple_type, elements );
		}
		void BasicTuple::_generateTupleID()
		{
			std::vector<TypeElement*> elements;
			for( auto& attribute : Attributes )
			{
				if( attribute.type().isName() )
					elements.push_back( new NameElement( attribute.name(), attribute.type().asName() ) );
				else
					elements.push_back( new TypeTuple( TypeTuple::rename( attribute.type(), attribute.name() ) ) );
			}
			Type = TypeTuple::tuple( Type.tupleType(), elements );
		}

		void BasicTuple::_assertCanAddVar( const TypeTuple& type )
		{
			if( !_canAddVar() )
				throw AccessDenied();
			
			if( tupleType() == name_data )
			{
				if( type.isTuple() && type.tupleType() != name_data )
					throw AccessDenied( eon::str( type.tupleType() ).ucFirst()
						+ " tuple is not a legal attribute type for tuple type " + eon::str( tupleType() ) + "!" );
				if( !Handler::legalForDataTuple( type ) )
					throw AccessDenied( type.str() + " is not a legal attribute type for tuple type "
						+ eon::str( tupleType() ) + "!" );
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

		void BasicTuple::_assertValidForMe( const BasicTuple& other )
		{
			if( tupleType() == name_data )
			{
				for( auto& attribute : other )
				{
					if( !Handler::legalForDataTuple( attribute.type() ) )
						throw AccessDenied();
				}
			}
		}

		const name_t& BasicTuple::_nameValue( index_t pos ) const noexcept
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
					str.addRaw( "," );
					str.resetRaw();
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
					else
					{
						str.pushTuple( ttype );
						explicitPrefix( str );
						( (const DataTuple*)element.value() )->_str( str );
						str.noSpacing();
						explicitPostfix( str );
						str.popTuple();
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

		bool BasicTuple::isDynamicTuple( const std::type_info& type ) const { return type == typeid( DynamicTuple ); }
		bool BasicTuple::isDataTuple( const std::type_info& type ) const { return type == typeid( DataTuple ); }
		bool BasicTuple::isLambda( const std::type_info& type ) const { return false; }// return type == typeid( Lambda ); }
	}
}
