#include "Tuple.h"
#include <eonvariables/NameValue.h>
#include <eonvariables/ReferenceValue.h>
#include <eonvariables/MetaValue.h>
#include <eonvariables/TupleValue.h>
#include <eonvariables/FunctionValue.h>
#include <queue>


namespace eon
{
	tuple::tuple( form tuple_form, vars::variables& varcache, std::initializer_list<name_t> name_attributes )
	{
		Form = tuple_form;
		for( auto name : name_attributes )
			append( vars::nameval::create( name ), varcache );
	}
	tuple::tuple( std::initializer_list<std::pair<name_t, vars::valueptr>> attributes )
	{
		Form = form::plain;
		for( auto& elm : attributes )
			append( elm.second, elm.first );
	}




	bool tuple::containsUnnamedValue( name_t value ) const noexcept
	{
		for( auto attr : Attributes )
		{
			if( !attr->named() && attr->value()->isName()&& attr->value()->actualName() == value )
				return true;
		}
		return false;
	}
	bool tuple::containsUnnamedValue( const vars::valueptr& value ) const noexcept
	{
		for( auto attr : Attributes )
		{
			if( !attr->named() && attr->value()->type() == value->type() )
			{
				if( attr->value()->actualCompare( value ) == 0 )
					return true;
			}
		}
		return false;
	}

	nameref tuple::path( const vars::variables& varcache, const tuple* subtuple ) const noexcept
	{
		nameref p;
		if( Parent != nullptr )
			p = Parent->path( varcache, this );

		if( subtuple != nullptr )
		{
			for( auto attr : Attributes )
			{
				if( attr->value()->isTuple() && &attr->value()->actualTuple() == subtuple && attr->named() )
					p.add( attr->name() );
			}
		}
		return p;
	}


	string tuple::str( size_t& pos_on_line, size_t indentation_level ) const
	{
		// If we have braced or meta data, we will keep things compact (one
		// line if possible). Otherwise, we will have each attribute on its
		// own line.
		if( Form == form::braced || Form == form::metadata )
			return _strCompact( pos_on_line, indentation_level );
		else
			return _strMultiline( pos_on_line, indentation_level );
	}
	string tuple::_strCompact( size_t& pos_on_line, size_t indentation_level ) const
	{
		string str, indent( indentation_level * 2, SpaceChr );
		for( auto& attr : Attributes )
		{
			string sub, separator;
			if( !str.empty() )
				separator = " ";
			if( attr->named() )
			{
				sub += *attr->name() + "=";
				if( pos_on_line + sub.numChars() + separator.numChars() >= 79 )
				{
					str += "\n  " + indent + sub;
					pos_on_line = 2 + indent.numChars() + sub.numChars();
					separator = "";
					sub = "";
				}
				else
				{
					str += separator + sub;
					pos_on_line += separator.numChars() + sub.numChars();
					separator = "";
					sub = "";
				}
			}

			auto pos = pos_on_line + separator.numChars();
			sub += attr->value()->str( pos, indentation_level );
			if( pos >= 79 )
			{
				str += "\n" + indent;
				pos_on_line = indent.numChars();
				separator = "";
				pos = pos_on_line;
				sub = attr->value()->str( pos, indentation_level );
			}
			str += separator + sub;
			pos_on_line = pos;
			separator = "";
			sub = "";
		}
		return str;
	}
	string tuple::_strMultiline( size_t& pos_on_line, size_t indentation_level ) const
	{
		string str, indent( indentation_level * 2, SpaceChr );
		auto pos = pos_on_line;
		bool new_line = false;
		for( auto attr : Attributes )
		{
			if( !str.empty() )
			{
				str += NewlineChr;
				str += indent;
				pos = indent.numChars();
				new_line = true;
			}
			if( attr->named() )
			{
				str += *attr->name();
				pos += attr->name()->numChars();
				if( attr->meta() )
				{
					str += "<" + attr->meta()->str( ++pos,
						indentation_level + 1 ) + ">";
					++pos;
				}
				str += "=";
				new_line = false;
				++pos;
			}
			auto sub = attr->value()->str( pos, indentation_level );
			if( !new_line && !str.empty()
				&& ( ( attr->value()->isTuple() && !attr->value()->actualTuple().braced() )
					|| attr->value()->isFunction() )
				&& ( sub.contains( NewlineChr ) || ( str.endsWith( '=' ) ) ) )
			{
				str.erase( substring( str.last(), str.end() ) );
				str += ":\n  " + indent;
				pos = indent.numChars();
			}
			if( attr->value()->isTuple() && !attr->value()->actualTuple().empty()
				&& !attr->value()->actualTuple().braced() && !sub.startsWith( '-' ) && !attr->named() )
			{
				str += "- ";
				pos += 2;
			}
			str += sub;
		}
		pos_on_line = pos;
		return str;
	}


	void tuple::validate( vars::variables& varcache ) const
	{
		for( auto attr : Attributes )
		{
			if( attr->meta() && !attr->meta()->empty() )
				_validate( *attr->meta(), attr->meta()->Parent, attr, varcache );
			if( attr->value()->isTuple() )
				attr->value()->actualTuple().validate( varcache );
		}
	}

	void tuple::validate( const tuple& other, vars::variables& varcache ) const
	{
		for( auto attr : Attributes )
		{
			// If we haven't got a name, then make sure the other tuple has an
			//* unnamed value that matches.
			if( !attr->named() )
			{
				if( !other.containsUnnamedValue( attr->value() ) )
				{
					size_t pos{ 0 };
					throw Invalid( "Attribute \"" + other.path( varcache ).str()
						+ "\": Expected tuple value to contain: " + attr->value()->str( pos, 0 ) );
				}
				continue;
			}
			auto value = other.at( attr->name() );
			if( !value )
			{
				if( !attr->meta() || !attr->meta()->containsUnnamedValue( name_optional ) )
					throw Invalid( "Attribute \"" + other.path( varcache ).str()
						+ "\": Missing non-optional \"" + *attr->name() + "\" attribute!" );
				continue;
			}
			if( attr->value()->isMeta() && !attr->value()->actualMeta().empty() )
				_validate( attr->value()->actualMeta(), &other, other.Named.at( attr->name() ), varcache );
			else if( attr->value()->isReference() )
			{
				auto refd = find( attr->value()->actualReference() );
				if( !refd || refd->isReference() )
					throw Invalid( "Attribute \"" + other.path( varcache ).str() + "\": Reference for \""
						+ *attr->name() + "\" doesn't lead to a valid target!" );
			}
			else if( attr->value()->isTuple() )
			{
				if( !value->isTuple() )
					throw Invalid( "Attribute \""
						+ other.path( varcache ).str() + "\": Value of \""
						+ *attr->name() + "\" must be a tuple!" );
				attr->value()->actualTuple().validate( value->actualTuple(), varcache );
			}
			else
			{
				if( attr->value()->type() != value->type() )
					throw Invalid( "Attribute \""
						+ other.path( varcache ).str()
						+ "\": Expected basic type of \"" + *attr->name()
						+ "\" to be " + *vars::mapTypeCode( attr->value()->type() ) + "!" );
				if( attr->value()->actualCompare( value ) != 0 )
				{
					size_t pos{ 0 };
					throw Invalid( "Attribute \""
						+ other.path( varcache ).str() + "\": Expected value of \""
						+ *attr->name() + "\" to be: "
						+ attr->value()->str( pos, 0 ) );
				}
			}
		}
	}




	tuple& tuple::operator=( const tuple& other )
	{
		clear();
		for( auto a : other.Attributes )
		{
			if( a->value()->isTuple() )
				a->value()->tuple_value().Parent = this;
			else if( a->value()->isMeta() )
				a->value()->meta_value().Parent = this;
			Attributes.push_back( a );
			if( a->name() != no_name )
				Named[ a->name() ] = Attributes[ Attributes.size() - 1 ];
		}
		Form = other.Form;
		return *this;
	}
	tuple& tuple::operator=( tuple&& other ) noexcept
	{
		Attributes = std::move( other.Attributes );
		for( auto& attr : Attributes )
		{
			if( attr->value()->isTuple() )
				attr->value()->tuple_value().Parent = this;
			else if( attr->value()->isMeta() )
				attr->value()->meta_value().Parent = this;
			else if( attr->value()->isFunction() )
				attr->value()->function_value().Parent = this;
		}
		Named = std::move( other.Named );
		Form = other.Form; other.Form = form::plain;
		return *this;
	}

	void tuple::set( size_t attribute_pos, const vars::valueptr& value, vars::variables& varcache )
	{
		if( attribute_pos < Attributes.size() )
		{
			auto attr = Attributes[ attribute_pos ];
			if( attr->value()->type() != value->type() )
				throw vars::WrongType();
			if( Form == form::plain )
			{
				if( attr->value()->isTuple() )
				{
					if( attr->value()->tupleType() != value->tupleType() )
						throw vars::WrongType();
				}
			}
			attr->value( value );
			if( attr->value()->isTuple() )
				attr->value()->tuple_value().Parent = this;
			else if( attr->value()->isMeta() )
				attr->value()->meta_value().Parent = this;
			else if( attr->value()->isFunction() )
				attr->value()->function_value().Parent = this;
		}
		else if( attribute_pos == Attributes.size() )
			append( value, varcache );
		else
			throw vars::WrongType();
	}
	attributeptr tuple::append( const vars::valueptr& value, name_t name, const tupleptr& metadata )
	{
		if( name != no_name && Named.find( name ) != Named.end() )
			throw DuplicateName();

		auto attr = attributeptr( new attribute( name, Attributes.size(), value ) );
		if( Form != form::metadata )
		{
			attr->meta( metadata );
			if( attr->meta() )
				attr->meta()->Parent = this;
		}
		if( attr->value()->isTuple() )
			attr->value()->tuple_value().Parent = this;
		Attributes.push_back( attr );
		if( attr->named() )
			Named[ attr->name() ] = attr;
		if( attr->value()->isTuple() )
			attr->value()->tuple_value().Parent = this;
		if( attr->value()->isMeta() )
			attr->value()->meta_value().Parent = this;
		else if( attr->value()->isFunction() )
			attr->value()->function_value().Parent = this;

		return attr;
	}
	void tuple::append( const vars::valueptr& value, vars::variables& varcache, name_t name, const tupleptr& metadata )
	{
		auto attr = append( value, name, metadata );
		if( metadata )
		{
			auto var = metadata->find( { name_var } );
			if( var )
			{
				if( var->isName() )
					varcache.set( var->actualName(), attr->value() );
				else if( var->isVariable() )
					varcache.set( var->actualVariable(), attr->value() );
			}

			if( value->isFunction() )
				dynamic_cast<vars::functionval*>( &*value )->functionMeta( metadata );
		}
	}

	void tuple::addReplace( const vars::valueptr& value, vars::variables& varcache, name_t name, const tupleptr& metadata )
	{
		auto found = Named.find( name );
		if( found == Named.end() )
		{
			append( value, varcache, name, metadata );
			return;
		}
		auto& attr = *found->second;
		attr.meta( metadata );
		attr.value( value );
		if( attr.value()->isTuple() )
			attr.value()->tuple_value().Parent = this;
		else if( attr.value()->isMeta() )
			attr.value()->meta_value().Parent = this;

		if( metadata )
		{
			auto var = metadata->find( { name_var } );
			if( var )
			{
				if( var->isName() )
					varcache.set( var->actualName(), attr.value() );
				else if( var->isVariable() )
					varcache.set( var->actualVariable(), attr.value() );
			}

			if( value->isFunction() )
				dynamic_cast<vars::functionval*>( &*value )->functionMeta( metadata );
		}
	}
	void tuple::addMerge( const vars::valueptr& value, vars::variables& varcache, name_t name, const tupleptr& metadata )
	{
		if( name == no_name )
		{
			append( value, varcache, name, metadata );
			return;
		}
		auto found = Named.find( name );
		if( found == Named.end() )
		{
			append( value, varcache, name, metadata );
			return;
		}
		auto& attr = *found->second;
		if( attr.meta() )
		{
			if( metadata )
				attr.meta()->merge( *metadata, varcache );
		}
		else if( metadata )
		{
			attr.meta( metadata );
			attr.meta()->Parent = this;
		}
		if( attr.value()->isTuple() )
		{
			if( value->isTuple() )
				attr.value()->tuple_value().merge(
					value->tuple_value(), varcache );
			else
			{
				attr.value( value );
				attr.value()->tuple_value().Parent = this;
			}
		}
		else if( attr.value()->isMeta() )
		{
			if( value->isMeta() )
				attr.value()->meta_value().merge(
					value->meta_value(), varcache );
			else
			{
				attr.value( value );
				attr.value()->meta_value().Parent = this;
			}
		}
		else
			attr.value( value );

		if( metadata )
		{
			auto var = metadata->find( { name_var } );
			if( var )
			{
				if( var->isName() )
					varcache.set( var->actualName(), attr.value() );
				else if( var->isVariable() )
					varcache.set( var->actualVariable(), attr.value() );
			}

			if( value->isFunction() )
				dynamic_cast<vars::functionval*>( &*value )->functionMeta( metadata );
		}
	}

	void tuple::append( const tuple& other, vars::variables& varcache )
	{
		if( Form != other.Form )
			throw Incompatible();
		for( auto attr : Attributes )
		{
			if( Named.find( attr->name() ) == Named.end() )
				append( attr->value(), varcache, attr->name() );
		}
	}
	void tuple::merge( const tuple& other, vars::variables& varcache )
	{
		for( auto& attr : other.Attributes )
			addMerge( attr->value(), varcache, attr->name(), attr->meta() );
	}

	void tuple::setAttributeContext( vars::valueptr context )
	{
		for( auto attr : Attributes )
		{
			if( attr->value()->isExpression() )
				attr->value()->expression_value().setContext( context );
			else if( attr->value()->isReference() )
				dynamic_cast<vars::refval*>( &*attr->value() )->context( context );
		}
	}

	void tuple::_resolveReferences( const vars::variables& varcache, bool all )
	{
		for( auto attr : Attributes )
		{
			if( attr->value()->isReference() )
			{
				auto ref = dynamic_cast<vars::refval*>( &*attr->value() );
				if( !ref->target() )
				{
					vars::valueptr found;
					tuple* parent{ nullptr };
					auto i_am_root = Parent == nullptr;
					auto i_am_doc = !i_am_root && Parent->Parent == nullptr;
					auto possible_rel_path = ref->actualReference().at( 0 ) != name_docs;

					if( possible_rel_path )
					{
						found = _find( ref->actualReference(), 0, false, &parent );
						if( !found && !i_am_doc && !i_am_root )
							found = document()->_find( ref->actualReference(), 0, false, &parent );
					}
					if( !found )
					{
						if( i_am_doc )
							found = Parent->_find( ref->actualReference(), 0, false, &parent );
						else if( !i_am_root )
							found = document()->Parent->_find( ref->actualReference(), 0, false, &parent );
					}

					std::set<nameref> seen{ ref->actualReference() };
					while( found && found->isReference() && parent != nullptr )
					{
						if( seen.find( found->actualReference() ) != seen.end() )
						{
							throw CircularReferencing( "The reference \"" + found->actualReference().str()
								+ "\" leads to a circle" );
						}
						seen.insert( found->actualReference() );
						found = parent->_find( found->actualReference(), 0, false, &parent );
					}
					if( all & !found )
						throw vars::NotFound( "Reference to \"" + ref->actualReference().str() + "\" not found!" );
					ref->target( found );
				}
			}
			else if( attr->value()->isTuple() )
				attr->value()->tuple_value()._resolveReferences( varcache, all );
		}
	}



	vars::valueptr tuple::find( const nameref& path ) const noexcept
	{
		auto found = _find( path, 0, false, nullptr );
		return found && found->isReference() ? dynamic_cast<vars::refval*>( &*found )->target() : found;
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
			if( a->name() != b->name()			// Must have same name
				|| a->meta() != b->meta() )		//    and meta data
				return false;

			if( a->value()->actualCompare( b->value() ) != 0 )
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
			if( b->name() != no_name )
			{
				auto found = Named.find( b->name() );
				if( found == Named.end() )
					return false;
				if( b->value()->type() != found->second->value()->type() )
					return false;
				if( b->value()->type() == vars::type_code::tuple_t
					&& b->value()->tupleType() != found->second->value()->tupleType() )
					return false;
			}
		}
		return true;
	}




	vars::valueptr tuple::_find( const nameref& path, size_t pos, bool resolve_refs, tuple** parent ) const noexcept
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
		if( pos < path.numElms() )
		{
			auto name = path.at( pos );
			if( name != no_name )
			{
				auto attrib = at( name );
				if( attrib )
				{
					if( pos + 1 == path.numElms() )
					{
						if( resolve_refs && attrib->isReference() )
							return dynamic_cast<vars::refval*>( &*attrib )->target();
						else
						{
							if( parent != nullptr )
								*parent = (tuple*)this;
							return attrib;
						}
					}
					else if( attrib->isTuple() )
						return attrib->tuple_value()._find( path, pos + 1, resolve_refs, parent );
				}
			}
		}
		return vars::valueptr();
	}


	void tuple::_validate( const tuple& metadata, const tuple* parent,
		const attributeptr& value, vars::variables& varcache ) const
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
			auto meta = metadata.at( i );
			if( meta )
			{
				if( metadata.isNamed( i ) )
				{
					auto name = metadata.name( i );
					if( name == name_type )
						_validateType( meta, parent, value->value(), varcache );
					else if( name == max_depth )
					{
						if( value->value()->isTuple() )
							_validateMaxDepth( meta, parent, value->value()->actualTuple(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'max_depth' meta data can only be applied on 'tuple' values!", varcache );
					}
					else if( name == min_length )
					{
						if( value->value()->isString() )
							_validateMinLength( meta, parent, value->value()->actualString(), varcache );
						else if( value->value()->isBinary() )
							_validateMinLength( meta, parent, value->value()->actualBinary(), varcache );
						else if( value->value()->isTuple() )
							_validateMinLength( meta, parent, value->value()->actualTuple(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'min_length' meta data can only be applied on 'bytes', 'string', and 'tuple' values!",
								varcache );
					}
					else if( name == max_length )
					{
						if( value->value()->isString() )
							_validateMaxLength( meta, parent, value->value()->actualString(), varcache );
						else if( value->value()->isBinary() )
							_validateMaxLength( meta, parent, value->value()->actualBinary(), varcache );
						else if( value->value()->isTuple() )
							_validateMaxLength( meta, parent, value->value()->actualTuple(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'max_length' meta data can only be applied on 'bytes', 'string', and 'tuple' values!",
								varcache );
					}
					else if( name == min )
					{
						if( value->value()->isInt() )
							_validateMin( meta, parent, value->value()->actualInt(), varcache );
						else if( value->value()->isFloat() )
							_validateMin( meta, parent, value->value()->actualFloat(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'min' meta data can only be applied on 'int' and 'float' values!", varcache );
					}
					else if( name == max )
					{
						if( value->value()->isInt() )
							_validateMax( meta, parent, value->value()->actualInt(), varcache );
						else if( value->value()->isFloat() )
							_validateMax( meta, parent, value->value()->actualFloat(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'max' meta data can only be applied on 'int' and 'float' values!", varcache );
					}
					else if( name == format )
					{
						if( value->value()->isString() )
							_validateFormat( meta, parent, value->value()->actualString(), varcache );
						else if( value->value()->isRaw() )
							_validateFormat( meta, parent, value->value()->actualRaw(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'format' meta data can only be applied on 'string' and 'raw' values!", varcache );
					}
					else if( name == flags )
					{
						if( value->value()->isTuple() )
							_validateFlags( meta, parent, value->value()->actualTuple(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'flags' meta data can only be applied on 'tuple' values!", varcache );
					}
					else if( name == options )
					{
						if( value->value()->isName() )
							_validateOptions( meta, parent, value->value()->actualName(), varcache );
						else
							_mismatchingMeta( value->name(),
								"'options' meta data can only be applied on 'name' values!", varcache );
					}
				}
			}
		}
	}

	void tuple::_validateType( vars::valueptr meta, const tuple* location, const vars::valueptr& value,
		const vars::variables& varcache ) const
	{
		if( !meta->isName() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'type' meta data must have a 'name' value!" );
		if( vars::mapTypeCode( value->type() ) != meta->actualName() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": Type of \"" + path( varcache ).str() + "\" must be \"" + *meta->actualName() + "\"!" );
	}

	void tuple::_validateMaxDepth( vars::valueptr meta, const tuple* location, const tuple& tupl,
		const vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_depth' meta data must have an 'int' value!" );
		auto max_depth = meta->actualInt();

		std::queue<std::pair<attributeptr, size_t>> queue;
		for( auto attr : tupl.Attributes )
		{
			if( attr->value()->isTuple() )
				queue.push( std::make_pair( attr, 0 ) );
		}

		int64_t depth = -1;
		while( !queue.empty() )
		{
			auto elm = queue.front();
			queue.pop();

			depth = elm.second;
			if( depth > max_depth )
				break;

			for( auto attr : elm.first->value()->tuple_value().Attributes )
			{
				if( attr->value()->isTuple() )
					queue.push( std::make_pair( attr, elm.second + 1 ) );
			}
		}
		if( depth >= max_depth )
			throw Invalid( "Depth of \"" + path( varcache ).str()
				+ "\" is exceeding " + string( max_depth ) + "!" );
	}

	void tuple::_validateMinLength( vars::valueptr meta, const tuple* location, const hex& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->targetInt( varcache );
		if( value.size() < static_cast<size_t>( min_length * 2 ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' has been exceeded!" );
	}
	void tuple::_validateMinLength( vars::valueptr meta, const tuple* location, const string& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->targetInt( varcache );
		if( value.numChars() < static_cast<size_t>( min_length ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' has been exceeded!" );
	}
	void tuple::_validateMinLength( vars::valueptr meta, const tuple* location, const tuple& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' meta data must have an 'int' value!" );
		auto min_length = meta->targetInt( varcache );
		if( value.numAttributes() < static_cast<size_t>( min_length ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min_length' has been exceeded!" );
	}

	void tuple::_validateMaxLength( vars::valueptr meta, const tuple* location, const hex& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->targetInt( varcache );
		if( value.size() > static_cast<size_t>( max_length * 2 ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' has been exceeded!" );
	}
	void tuple::_validateMaxLength( vars::valueptr meta, const tuple* location, const string& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->targetInt( varcache );
		if( value.numChars() > static_cast<size_t>( max_length ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' has been exceeded!" );
	}
	void tuple::_validateMaxLength( vars::valueptr meta, const tuple* location, const tuple& value,
		vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' meta data must have an 'int' value!" );
		auto max_length = meta->targetInt( varcache );
		if( value.numAttributes() > static_cast<size_t>( max_length ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max_length' has been exceeded!" );
	}

	void tuple::_validateMin( vars::valueptr meta, const tuple* location, int64_t value, vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min' meta data must have an 'int' value!" );
		auto min = meta->targetInt( varcache );
		if( value < min )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min' has been exceeded!" );
	}
	void tuple::_validateMin( vars::valueptr meta, const tuple* location, double value, vars::variables& varcache ) const
	{
		if( !meta->isFloat() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min' meta data must have a 'float' value!" );
		auto min = meta->targetFloat( varcache );
		if( value < min )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'min' has been exceeded!" );
	}

	void tuple::_validateMax( vars::valueptr meta, const tuple* location, int64_t value, vars::variables& varcache ) const
	{
		if( !meta->isInt() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max' meta data must have an 'int' value!" );
		auto max = meta->targetInt( varcache );
		if( value > max )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max' has been exceeded!" );
	}
	void tuple::_validateMax( vars::valueptr meta, const tuple* location, double value, vars::variables& varcache ) const
	{
		if( !meta->isFloat() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max' meta data must have a 'float' value!" );
		auto max = meta->targetFloat( varcache );
		if( value > max )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'max' has been exceeded!" );
	}

	void tuple::_validateFormat( vars::valueptr meta, const tuple* location, const string& value,
		vars::variables& varcache ) const
	{
		if( !meta->isRegex() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'format' meta data must have a 'regex' value!" );
		auto format = meta->targetRegex( varcache );
		if( !format.match( value ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'format' validation failed!" );
	}
	void tuple::_validateFormat( vars::valueptr meta, const tuple* location, const std::vector<string>& value,
		vars::variables& varcache ) const
	{
		if( !meta->isRegex() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'format' meta data must have a 'regex' value!" );
		auto format = meta->targetRegex( varcache );
		bool match = !value.empty();
		for( auto& line : value )
		{
			if( !format.match( line ) )
			{
				match = false;
				break;
			}
		}
		if( !match )
			throw Invalid( "Attribute \"" + location->path( varcache ).str() + "\": 'format' validation failed!" );
	}

	void tuple::_validateFlags( vars::valueptr meta, const tuple* location, const tuple& value,
		const vars::variables& varcache ) const
	{
		if( !meta->isTuple() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'flags' meta data must have a 'tuple' value (with unnamed name attributes)!" );
		auto flags = meta->tuple_value();
		for( auto attr : value.Attributes )
		{
			if( !attr->value()->isName() )
				throw Invalid( "Attribute \"" + location->path( varcache ).str()
					+ "\": 'flags' validation failed (value contains non-name attribute(s))!" );
			if( !flags.containsUnnamedValue( attr->value()->actualName() ) )
				throw Invalid( "Attribute \"" + location->path( varcache ).str()
					+ "\": 'flags' validation failed (\"" + *attr->value()->actualName() + "\" is not a valid flag)!" );
		}
	}

	void tuple::_validateOptions( vars::valueptr meta, const tuple* location, name_t value, vars::variables& varcache ) const
	{
		if( !meta->isTuple() )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'options' meta data must have a 'tuple' value (with unnamed name attributes)!" );
		auto options = meta->targetTuple( varcache );
		if( !options.containsUnnamedValue( value ) )
			throw Invalid( "Attribute \"" + location->path( varcache ).str()
				+ "\": 'options' validation failed (\"" + *value + "\" is not a valid option)!" );
	}

	void tuple::_mismatchingMeta( name_t attribute_name, const string& info, const vars::variables& varcache ) const
	{
		auto location = path( varcache );
		location.add( attribute_name );
		throw Invalid( "Attribute \"" + location.str() + "\": " + info );
	}
}
