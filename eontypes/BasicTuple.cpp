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
			Perm = perm.find( Type.name() )->second;

			if( Type.name() == name_plain )
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




		bool BasicTuple::compatibleWith( const Object& other ) const noexcept
		{
			if( other.type().name() == name_data || other.type().name() == name_dynamic )
			{
				auto& tuple = *(BasicTuple*)&other;
				if( Type.name() == name_plain || Type.name() == name_data || Type.name() == name_dynamic )
				{
					if( Attributes.size() > tuple.Attributes.size() )
						return false;
					for( size_t i = 0; i < Attributes.size(); ++i )
					{
						if( Attributes[ i ].type().attributeName() != no_name )
						{
							auto& tup_attr = tuple.at( Attributes[ i ].type().attributeName() );
							if( Attributes[ i ].value() && tup_attr.value() )
							{
								if( !Attributes[ i ].value()->compatibleWith( *tup_attr.value() ) )
									return false;
							}
							else
							{
								if( !Attributes[ i ].type().compatibleWith( tup_attr.type() ) )
									return false;
							}
						}
					}
					return true;
				}
				else
				{
					// Must match on first attribute
					return tuple.empty() ? false : Type.compatibleWith( tuple.at( static_cast<index_t>( 0 ) ).type() );
				}
			}
			else
				return Type.compatibleWith( other.type() );
		}




		void BasicTuple::str( Stringifier& str ) const
		{
			stack<const BasicTuple*> ancestors;
			_str( str, ancestors, false );
		}




		index_t BasicTuple::deepCount() const noexcept
		{
			index_t count = numAttributes();
			for( auto& element : Attributes )
			{
				auto type = element.type().name();
				if( ( type == name_data && element.name() == no_name ) || type == name_plain || type == name_dynamic )
					count += dynamic_cast<BasicTuple*>( element.value() )->deepCount();
			}
			return count;
		}

		index_t BasicTuple::depth() const noexcept
		{
			index_t d = Attributes.empty() ? 0 : 1;
			for( auto& element : Attributes )
			{
				if( element.type().name() == name_plain || element.type().name() == name_data
					|| element.type().name() == name_dynamic )
				{
					auto dd = dynamic_cast<BasicTuple*>( element.value() )->depth();
					if( dd + 1 > d )
						d = dd + 1;
				}
			}
			return d;
		}

		bool BasicTuple::hasTupleAttributes() const noexcept
		{
			for( auto& element : Attributes )
			{
				if( element.type().name() == name_plain || element.type().name() == name_data
					|| element.type().name() == name_dynamic )
					return true;
			}
			return false;
		}

		bool BasicTuple::hasNamedSubtupleAttributes() const noexcept
		{
			for( auto& element : Attributes )
			{
				if( element.type().name() == name_plain || element.type().name() == name_data
					|| element.type().name() == name_dynamic )
				{
					if( element.name() != no_name )
						return true;
					else if( static_cast<BasicTuple*>( element.value() )->hasNamedSubtupleAttributes() )
						return true;
				}
			}
			return false;
		}

		bool BasicTuple::hasFlag( name_t flag_name ) const noexcept
		{
			for( auto& elm : Attributes )
			{
				if( elm.name() == no_name && elm.type() == name_name
					&& ( (NameInstance*)elm.value() )->value() == flag_name )
					return true;
			}
			return false;
		}


		BasicTuple& BasicTuple::addName( name_t name, name_t value )
		{
			return *this += Attribute( name, new NameInstance( value, source::Ref() ) );
		}
		BasicTuple& BasicTuple::addName( name_t value )
		{
			return *this += Attribute( new NameInstance( value, source::Ref() ) );
		}

		BasicTuple* BasicTuple::addPlainTuple( name_t name, source::Ref source )
		{
			if( Finalized )
				throw AccessDenied();
			auto type = EonType( name_plain, source );
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
		BasicTuple* BasicTuple::addDynamicTuple( name_t name, source::Ref source )
		{
			if( Finalized )
				throw AccessDenied();
			static auto type = EonType( name_dynamic, source );
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
		BasicTuple* BasicTuple::addDataTuple( name_t name, source::Ref source )
		{
			if( Finalized )
				throw AccessDenied();
			static auto type = EonType( name_data, source );
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




		void BasicTuple::_generateTupleID()
		{
			for( auto& attribute : Attributes )
				Type << EonType( attribute.type() );
		}

		void BasicTuple::_assertCanAddVar( const EonType& type )
		{
			if( !_canAddVar() )
				throw AccessDenied();
			
			if( Type.name() == name_data )
			{
				if( type.name() == name_dynamic )
					throw AccessDenied( eon::str( type.name() ).ucFirst()
						+ " tuple is not a legal attribute type for tuple type " + eon::str( Type.name() ) + "!" );
				if( !Handler::legalForDataTuple( type ) )
					throw AccessDenied( type.str() + " is not a legal attribute type for tuple type \""
						+ eon::str( Type.name() ) + "\"!" );
			}
		}

		void BasicTuple::_assertValidForMe( const BasicTuple& other )
		{
			if( Type.name() == name_data )
			{
				for( auto& attribute : other )
				{
					if( !Handler::legalForDataTuple( attribute.type() ) )
						throw AccessDenied();
				}
			}
		}

		const name_t& BasicTuple::_nameValue( index_t pos ) const noexcept {
			return ( (const NameInstance*)Attributes[ pos ].value() )->value(); }

		std::type_index BasicTuple::tupleType() const noexcept {
			return typeid( Tuple ); }

		void BasicTuple::_str( Stringifier& str, stack<const BasicTuple*>& ancestors, int indented, bool is_block ) const
		{
			if( ancestors.empty() || ancestors.bottom()->type().name() != type().name() )
				formalPrefix( str );
			else if( !is_block || ancestors.bottom()->type().name() != name_data )
				informalPrefix( str );
			ancestors.push( this );

			bool newline_between_attributes = false;
			bool started_block = false;
			bool need_block_on_newline = false;

			auto dcount = deepCount();
			bool split_attributes = dcount > 10;
			if( !split_attributes )
			{
				auto depth = this->depth();
				if( isData() )
				{
					if( depth > 2 && dcount > 5 )
						split_attributes = true;
				}
				else if( ( isPlain() || isDynamic() ) && depth > 4 )
					split_attributes = true;
			}
			
			if( split_attributes )
			{
				newline_between_attributes = true;
				if( ancestors.size() == 1 )
				{
					started_block = true;
					str.start_block();
				}
				else if( !is_block )
					need_block_on_newline = true;
			}
			else if( isData() && is_block && depth() > 1 )
				newline_between_attributes = true;

			index_t attrib_no{ 0 };
			for( auto& element : Attributes )
			{
				if( ++attrib_no > 1 )
				{
					str.punct( ",", eon::stringify::Type::end_block ); 
					if( newline_between_attributes )
					{
						str.hard_lf();
						if( need_block_on_newline )
						{
							need_block_on_newline = false;
							str.start_block();
							started_block = true;
						}
					}
					else
					{
						switch( ancestors.size() )
						{
							case 1:
								str.prim_split_block();
								break;
							case 2:
								str.sec_split_block();
								break;
							default:
								str.tert_split_block();
								break;
						}
					}
				}

				if( element.type().name() == name_plain
					|| element.type().name() == name_data
					|| element.type().name() == name_dynamic )
				{
					auto ttype = element.type().name();
					if( element.name() )
					{
						if( ttype == name_data )
						{
							bool another_started_block = false;
							if( !started_block && str.lastElement() != stringify::Type::start_block )
							{
								str.start_block();
								another_started_block = true;
							}
							str.word( eon::str( element.name() ) );
							str.append( ":" );
							str.start_block();
							auto& dt = *(const BasicTuple*)element.value();
							dt._str( str, ancestors, indented > 0 ? indented - 1 : 0, true );
							str.end_block();
							if( another_started_block )
								str.end_block();
						}
						else
						{
							str.word( eon::str( element.name() ) );
							str.op2( "=" ).tert_split_block();
							auto& dt = *(const BasicTuple*)element.value();
							dt._str( str, ancestors, false );
						}
					}
					else
					{
						auto& dt = *(const BasicTuple*)element.value();
						dt._str( str, ancestors );
					}
				}
				else
				{
					if( element.name() )
						str.word( eon::str( element.name() ) ).op2( "=" );
					element.value()->str( str );
				}
			}

			ancestors.pop();
			if( ancestors.empty() || ancestors.bottom()->type().name() != type().name() )
				formalPostfix( str );
			else if( !is_block || ancestors.bottom()->type().name() != name_data )
				informalPostfix( str );

			if( started_block )
				str.end_block();
		}

		bool BasicTuple::isPlainTuple( const std::type_info& type ) const { return type == typeid( Tuple ); }
		bool BasicTuple::isDynamicTuple( const std::type_info& type ) const { return type == typeid( DynamicTuple ); }
		bool BasicTuple::isDataTuple( const std::type_info& type ) const { return type == typeid( DataTuple ); }
//		bool BasicTuple::isLambda( const std::type_info& type ) const { return false; }// return type == typeid( Lambda ); }
	}
}
