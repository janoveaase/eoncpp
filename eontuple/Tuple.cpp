#include "Tuple.h"
#include "TupleValue.h"
#include "NameValue.h"
#include "MetaValue.h"
#include <queue>


namespace eon
{
	tuple::tuple( form tuple_form,
		std::initializer_list<name_t> name_attributes )
	{
		Form = tuple_form;
		for( auto name : name_attributes )
			append( tup::valueptr( new tup::nameval( name ) ) );
	}

	tuple& tuple::operator=( const tuple& other )
	{
		clear();
		for( auto a : other.Attributes )
		{
			Attributes.push_back( a );
			if( a->Name != no_name )
				Named[ a->Name ] = Attributes[ Attributes.size() - 1 ];
		}
		Form = other.Form;
		return *this;
	}




	bool tuple::contains( name_t value ) const noexcept
	{
		for( auto attribute : Attributes )
		{
			if( attribute->Value->isName()
				&& attribute->Value->name_value() == value )
				return true;
		}
		return false;
	}

	tup::path tuple::path( const tuple* subtuple ) const noexcept
	{
		tup::path p;
		if( Parent != nullptr )
			p = Parent->path( this );

		if( subtuple != nullptr )
		{
			for( auto attribute : Attributes )
			{
				if( attribute->Value->isTuple()
					&& &attribute->Value->tuple_value() == subtuple
					&& attribute->Name != no_name )
					p.add( attribute->Name );
			}
		}
		return p;
	}


	string tuple::str( size_t& pos_on_line, size_t indentation_level,
		tup::perm format ) const noexcept
	{
		bool allow_oneliner = static_cast<bool>(
			format & tup::perm::allow_oneliner );
		bool allow_multiliner = static_cast<bool>(
			format & tup::perm::allow_multiliner );

		// Check if we can/must put everything on a single line
		if( ( Parent != nullptr && Parent->Parent != nullptr )
			|| allow_oneliner || !allow_multiliner )
		{
			string s;
			auto pos = pos_on_line;
			for( auto attribute : Attributes )
			{
				if( allow_multiliner && (
					( attribute->Value->basicType()
						>= tup::basic_type::tuple_t )
					|| ( attribute->Name != no_name ) ) )
				{
					s = "";
					break;
				}
				else
				{
					if( !s.empty() )
						s += " ";
					if( attribute->Name != no_name )
						s += *attribute->Name + "=";
					s += attribute->Value->str( pos, indentation_level,
						attribute->Name != no_name ? tup::perm::allow_oneliner : format );
					if( allow_multiliner
						&& Form != form::metadata && s.numChars() >= 79 )
					{
						s = "";
						break;
					}
				}
			}
			if( !s.empty() )
				return s;
		}

		// Use indentation to express the tuple
		string s, indent( indentation_level * 2, SpaceChr );
		for( auto attribute : Attributes )
		{
			if( !s.empty() )
			{
				s += NewlineChr;
				s += indent;
				pos_on_line = indent.numChars();
			}
			if( attribute->Name != no_name )
			{
				s += *attribute->Name;
				pos_on_line += attribute->Name->numChars();
				if( attribute->MetaData )
				{
					s += "<" + attribute->MetaData->str( ++pos_on_line,
						indentation_level, format ) + ">";
					++pos_on_line;
				}
				s += "=";
				++pos_on_line;
			}
			s += attribute->Value->str( pos_on_line, indentation_level,
				attribute->Name != no_name ? tup::perm::allow_oneliner
					: format );
		}
		return s;
	}


	void tuple::validate() const
	{
		for( auto attribute : Attributes )
		{
			if( attribute->MetaData && !attribute->MetaData->empty() )
				_validate( *attribute->MetaData, attribute->MetaData->Parent,
					attribute );
			if( attribute->Value->isTuple() )
				attribute->Value->tuple_value().validate();
		}
	}

	void tuple::validate( const tuple& other )
	{
		for( auto attribute : Attributes )
		{
			if( attribute->Name == no_name )
				continue;
			auto value = other.attribute( attribute->Name );
			if( !value )
			{
				if( !attribute->MetaData
					|| !attribute->MetaData->contains( name_optional ) )
					throw tup::Invalid( "At \"" + other.path().str()
						+ "\": Missing non-optional \"" + *attribute->Name
						+ "\" attribute!" );
				continue;
			}
			if( attribute->Value->isMeta()
				&& !attribute->Value->meta_value().empty() )
				_validate( attribute->Value->meta_value(), &other,
					other.Named.at( attribute->Name ) );
			else if( attribute->Value->isRef() )
			{
				auto refd = find( attribute->Value->ref_value() );
				if( !refd || refd->isRef() )
					throw tup::Invalid( "At \"" + other.path().str()
						+ "\": Reference for \"" + *attribute->Name
						+ "\" doesn't lead to a valid target!" );
			}
			else if( attribute->Value->isTuple() )
			{
				if( !value->isTuple() )
					throw tup::Invalid( "At \"" + other.path().str()
						+ "\": Value of \"" + *attribute->Name
						+ "\" must be a tuple!" );
				attribute->Value->tuple_value().validate(
					value->tuple_value() );
			}
			else
			{
				if( attribute->Value->basicType() != value->basicType() )
					throw tup::Invalid( "At \"" + other.path().str()
						+ "\": Expected basic type of \"" + *attribute->Name
						+ "\" to be " + *tup::mapBasicType(
							attribute->Value->basicType() ) + "!" );
				if( !attribute->Value->equal( value ) )
				{
					size_t pos{ 0 };
					throw tup::Invalid( "At \"" + other.path().str()
						+ "\": Expected value of \"" + *attribute->Name
						+ "\" to be: " + attribute->Value->str( pos, 0,
							tup::perm::allow_oneliner ) );
				}
			}
		}
	}




	void tuple::set( size_t attribute_pos, const tup::valueptr& value )
	{
		if( attribute_pos < Attributes.size() )
		{
			auto attribute = Attributes[ attribute_pos ];
			if( attribute->Value->basicType() != value->basicType() )
				throw tup::WrongType();
			if( Form == form::plain )
			{
				if( attribute->Value->isTuple() )
				{
					if( attribute->Value->tupleType() != value->tupleType() )
						throw tup::WrongType();
				}
			}
			attribute->Value = value;
			if( attribute->Value->isTuple() )
				attribute->Value->tuple_value().Parent = this;
			if( attribute->Value->isRef() || attribute->Value->isTuple() )
				_resolveReferences();
		}
		else if( attribute_pos == Attributes.size() )
			append( value );
		else
			throw tup::WrongType();
	}
	void tuple::append( const tup::valueptr& value, name_t name,
		const tupleptr& metadata )
	{
		if( name != no_name && Named.find( name ) != Named.end() )
			throw tup::DuplicateName();

		auto attribute = AttributePtr( new Attribute() );
		attribute->Name = name;
		attribute->Pos = Attributes.size();
		attribute->Value = value;
		if( Form == form::plain )
		{
			attribute->MetaData = metadata;
			if( attribute->MetaData )
				attribute->MetaData->Parent = this;
		}
		if( attribute->Value->isTuple() )
			attribute->Value->tuple_value().Parent = this;
		Attributes.push_back( attribute );
		if( attribute->Name != no_name )
			Named[ attribute->Name ] = attribute;
		if( attribute->Value->isRef() || attribute->Value->isTuple() )
			_resolveReferences();
	}




	bool tuple::equal( const tuple& other ) const noexcept
	{
		if( Attributes.size() != other.Attributes.size() )
			return false;

		for( auto ia = Attributes.begin(), ib = other.Attributes.begin();
			ia != Attributes.end();		// We need only test one!
			++ia, ++ib )
		{
			auto a = *ia, b = *ib;
			if( a->Name != b->Name				// Must have same name
				|| a->MetaData != b->MetaData )	//    and meta data
				return false;

			if( !a->Value->equal( b->Value ) )
				return false;
		}
		return true;
	}
	bool tuple::compatible( const tuple& other ) const noexcept
	{
		// Unlike 'equal', we allow 'other' to have fewer attributes
		if( Attributes.size() > other.Attributes.size() )
			return false;

		for( auto& b : other.Attributes )
		{
			if( b->Name != no_name )
			{
				auto found = Named.find( b->Name );
				if( found == Named.end() )
					return false;
				if( b->Value->basicType()
					!= found->second->Value->basicType() )
					return false;
				if( b->Value->basicType() == tup::basic_type::tuple_t
					&& b->Value->tupleType()
						!= found->second->Value->tupleType() )
					return false;
			}
		}
		return true;
	}




	void tuple::_resolveReferences()
	{
		for( auto attribute : Attributes )
		{
			if( attribute->Value->isRef() )
			{
				auto ref = dynamic_cast<tup::refval*>( &*attribute->Value );
				if( !ref->target() )
				{
					tup::valueptr found;
					tuple* parent{ nullptr };
					auto i_am_root = Parent == nullptr;
					auto i_am_doc = !i_am_root && Parent->Parent == nullptr;
					auto possible_rel_path = ref->ref_value().at( 0 )
						!= name_docs;

					if( possible_rel_path )
					{
						found = _find( ref->ref_value(), 0, false, &parent );
						if( !found && !i_am_doc && !i_am_root )
							found = document()->_find(
								ref->ref_value(), 0, false, &parent );
					}
					if( !found )
					{
						if( i_am_doc )
							found = Parent->_find(
								ref->ref_value(), 0, false, &parent );
						else if( !i_am_root )
							found = document()->Parent->_find(
								ref->ref_value(), 0, false, &parent );
					}

					std::set<tup::path> seen{ ref->ref_value() };
					while( found && found->isRef() && parent != nullptr )
					{
						if( seen.find( found->ref_value() ) != seen.end() )
						{
							throw tup::CircularReferencing( "The reference \""
								+ found->ref_value().str()
								+ "\" leads to a circle" );
						}
						seen.insert( found->ref_value() );
						found = parent->_find(
							found->ref_value(), 0, false, &parent );
					}
					ref->target( found );
				}
			}
			else if( attribute->Value->isTuple() )
				attribute->Value->tuple_value()._resolveReferences();
		}
	}

	tup::valueptr tuple::_find( const tup::path& path, size_t pos,
		bool resolve_refs, tuple** parent ) const noexcept
	{
		if( pos == 0 && !path.empty() )
		{
			if( path.at( 0 ) == name_docs )
			{
				if( Parent == nullptr )
					++pos;
				else
					return Parent->_find( path, pos, resolve_refs, parent );
			}
		}
		if( pos < path.size() )
		{
			auto name = path.at( pos );
			if( name != no_name )
			{
				auto attrib = attribute( name );
				if( attrib != nullptr )
				{
					if( pos + 1 == path.size() )
					{
						if( resolve_refs && attrib->isRef() )
							return dynamic_cast<tup::refval*>(
								&*attrib )->target();
						else
						{
							if( parent != nullptr )
								*parent = (tuple*)this;
							return attrib;
						}
					}
					else if( attrib->isTuple() )
						return attrib->tuple_value()._find(
							path, pos + 1, resolve_refs, parent );
				}
			}
		}
		return tup::valueptr();
	}


	void tuple::_validate( tuple& metadata, const tuple* parent,
		const AttributePtr& value ) const
	{
		static name_t max_depth = name::get( "max_depth" ),
			min_length = name::get( "min_length" ),
			max_length = name::get( "max_length" ),
			min = name::get( "min" ), max = name::get( "max" ),
			format = name::get( "format" ),
			flags = name::get( "flags" ),
			options = name::get( "options" );

		for( size_t i = 0; i < metadata.numAttributes(); ++i )
		{
			auto meta = metadata.attribute( i );
			if( meta )
			{
				if( metadata.isNamed( i ) )
				{
					auto name = metadata.name( i );
					if( name == name_type )
						_validateType( meta, parent, value->Value );
					else if( name == max_depth )
					{
						if( value->Value->isTuple() )
							_validateMaxDepth( meta, parent, value->Value->tuple_value() );
						else
							_mismatchingMeta( value->Name, "'max_depth' meta data can only be applied on 'tuple' values!" );
					}
					else if( name == min_length )
					{
						if( value->Value->isString() )
							_validateMinLength( meta, parent, value->Value->string_value() );
						else if( value->Value->isBinary() )
							_validateMinLength( meta, parent, value->Value->binary_value() );
						else if( value->Value->isTuple() )
							_validateMinLength( meta, parent, value->Value->tuple_value() );
						else
							_mismatchingMeta( value->Name, "'min_length' meta data can only be applied on 'bytes', 'string', and 'tuple' values!" );
					}
					else if( name == max_length )
					{
						if( value->Value->isString() )
							_validateMaxLength( meta, parent, value->Value->string_value() );
						else if( value->Value->isBinary() )
							_validateMaxLength( meta, parent, value->Value->binary_value() );
						else if( value->Value->isTuple() )
							_validateMaxLength( meta, parent, value->Value->tuple_value() );
						else
							_mismatchingMeta( value->Name, "'max_length' meta data can only be applied on 'bytes', 'string', and 'tuple' values!" );
					}
					else if( name == min )
					{
						if( value->Value->isInt() )
							_validateMin( meta, parent, value->Value->int_value() );
						else if( value->Value->isFloat() )
							_validateMin( meta, parent, value->Value->float_value() );
						else
							_mismatchingMeta( value->Name, "'min' meta data can only be applied on 'int' and 'float' values!" );
					}
					else if( name == max )
					{
						if( value->Value->isInt() )
							_validateMax( meta, parent, value->Value->int_value() );
						else if( value->Value->isFloat() )
							_validateMax( meta, parent, value->Value->float_value() );
						else
							_mismatchingMeta( value->Name, "'max' meta data can only be applied on 'int' and 'float' values!" );
					}
					else if( name == format )
					{
						if( value->Value->isString() )
							_validateFormat( meta, parent, value->Value->string_value() );
						else if( value->Value->isRaw() )
							_validateFormat( meta, parent, value->Value->raw_value() );
						else
							_mismatchingMeta( value->Name, "'format' meta data can only be applied on 'string' and 'raw' values!" );
					}
					else if( name == flags )
					{
						if( value->Value->isTuple() )
							_validateFlags( meta, parent, value->Value->tuple_value() );
						else
							_mismatchingMeta( value->Name, "'flags' meta data can only be applied on 'tuple' values!" );
					}
					else if( name == options )
					{
						if( value->Value->isName() )
							_validateOptions( meta, parent, value->Value->name_value() );
						else
							_mismatchingMeta( value->Name, "'options' meta data can only be applied on 'name' values!" );
					}
				}
			}
		}
	}

	void tuple::_validateType( tup::valueptr meta, const tuple* meta_owner, const tup::valueptr& value ) const
	{
		if( !meta->isName() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'type' meta data must have a 'name' value!" );
		if( tup::mapBasicType( value->basicType() ) != meta->name_value() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": Type of \"" + path().str()
				+ "\" must be \"" + *meta->name_value() + "\"!" );
	}

	void tuple::_validateMaxDepth( tup::valueptr meta, const tuple* meta_owner, const tuple& tupl ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_depth' meta data must have an 'int' value!" );
		auto max_depth = meta->int_value();

		std::queue<std::pair<AttributePtr, size_t>> queue;
		for( auto attribute : tupl.Attributes )
		{
			if( attribute->Value->isTuple() )
				queue.push( std::make_pair( attribute, 0 ) );
		}

		int64_t depth = -1;
		while( !queue.empty() )
		{
			auto elm = queue.front();
			queue.pop();

			depth = elm.second;
			if( depth > max_depth )
				break;

			for( auto attribute : elm.first->Value->tuple_value().Attributes )
			{
				if( attribute->Value->isTuple() )
					queue.push( std::make_pair( attribute, elm.second + 1 ) );
			}
		}
		if( depth >= max_depth )
			throw tup::Invalid( "Depth of \"" + path().str()
				+ "\" is exceeding " + string( max_depth ) + "!" );
	}

	void tuple::_validateMinLength( tup::valueptr meta, const tuple* meta_owner, const hex& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->int_value();
		if( value.size() < static_cast<size_t>( min_length * 2 ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' has been exceeded!" );
	}
	void tuple::_validateMinLength( tup::valueptr meta, const tuple* meta_owner, const string& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->int_value();
		if( value.numChars() < static_cast<size_t>( min_length ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' has been exceeded!" );
	}
	void tuple::_validateMinLength( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->int_value();
		if( value.numAttributes() < static_cast<size_t>( min_length ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min_length' has been exceeded!" );
	}

	void tuple::_validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const hex& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->int_value();
		if( value.size() > static_cast<size_t>( max_length * 2 ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' has been exceeded!" );
	}
	void tuple::_validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const string& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->int_value();
		if( value.numChars() > static_cast<size_t>( max_length ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' has been exceeded!" );
	}
	void tuple::_validateMaxLength( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->int_value();
		if( value.numAttributes() > static_cast<size_t>( max_length ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max_length' has been exceeded!" );
	}

	void tuple::_validateMin( tup::valueptr meta, const tuple* meta_owner, int64_t value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min' meta data must have an 'int' value!" );
		auto min = meta->int_value();
		if( value < min )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min' has been exceeded!" );
	}
	void tuple::_validateMin( tup::valueptr meta, const tuple* meta_owner, double value ) const
	{
		if( !meta->isFloat() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min' meta data must have a 'float' value!" );
		auto min = meta->float_value();
		if( value < min )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'min' has been exceeded!" );
	}

	void tuple::_validateMax( tup::valueptr meta, const tuple* meta_owner, int64_t value ) const
	{
		if( !meta->isInt() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max' meta data must have an 'int' value!" );
		auto max = meta->int_value();
		if( value > max )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max' has been exceeded!" );
	}
	void tuple::_validateMax( tup::valueptr meta, const tuple* meta_owner, double value ) const
	{
		if( !meta->isFloat() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max' meta data must have a 'float' value!" );
		auto max = meta->float_value();
		if( value > max )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'max' has been exceeded!" );
	}

	void tuple::_validateFormat( tup::valueptr meta, const tuple* meta_owner, const string& value ) const
	{
		if( !meta->isRegex() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'format' meta data must have a 'regex' value!" );
		auto format = meta->regex_value();
		if( !format.match( value ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'format' validation failed!" );
	}
	void tuple::_validateFormat( tup::valueptr meta, const tuple* meta_owner, const std::vector<string>& value ) const
	{
		if( !meta->isRegex() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'format' meta data must have a 'regex' value!" );
		auto format = meta->regex_value();
		if( value.empty() || !format.match( value[ 0 ] ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'format' validation failed!" );
	}

	void tuple::_validateFlags( tup::valueptr meta, const tuple* meta_owner, const tuple& value ) const
	{
		if( !meta->isTuple() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'flags' meta data must have a 'tuple' value (with unnamed name attributes)!" );
		auto flags = meta->tuple_value();
		for( auto attribute : value.Attributes )
		{
			if( !attribute->Value->isName() )
				throw tup::Invalid( "At \"" + meta_owner->path().str()
					+ "\": 'flags' validation failed (value contains non-name attribute(s))!" );
			if( !flags.contains( attribute->Value->name_value() ) )
				throw tup::Invalid( "At \"" + meta_owner->path().str()
					+ "\": 'flags' validation failed (\"" + *attribute->Value->name_value() + "\" is not a valid flag)!" );
		}
	}

	void tuple::_validateOptions( tup::valueptr meta, const tuple* meta_owner, name_t value ) const
	{
		if( !meta->isTuple() )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'options' meta data must have a 'tuple' value (with unnamed name attributes)!" );
		auto options = meta->tuple_value();
		if( !options.contains( value ) )
			throw tup::Invalid( "At \"" + meta_owner->path().str()
				+ "\": 'options' validation failed (\"" + *value + "\" is not a valid option)!" );
	}

	void tuple::_mismatchingMeta( name_t attribute_name, const string& info ) const
	{
		auto location = path();
		location.add( attribute_name );
		throw tup::Invalid( "At \"" + location.str() + "\": " + info );
	}




	tuple::Attribute& tuple::Attribute::operator=(
		const Attribute& other )
	{
		Name = other.Name;
		MetaData = other.MetaData;
		Pos = other.Pos;
		Value = other.Value->copy();
		return *this;
	}
	tuple::Attribute& tuple::Attribute::operator=(
		Attribute&& other ) noexcept
	{
		Name = other.Name; other.Name = no_name;
		MetaData = std::move( other.MetaData );
		Pos = other.Pos; other.Pos = 0;
		Value = other.Value; other.Value = nullptr;
		return *this;
	}
}
