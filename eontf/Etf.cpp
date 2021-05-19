#include "Etf.h"
#include <eontokenizer/Tokenizer.h>


namespace eon
{
	const tup::valueptr etf::get( const tup::path& path ) const noexcept
	{
		if( !path )
			return tup::valueptr();
		if( path.at( 0 ) == name_docs )
			return Docs.get( path.chopFirst() );
		else
			return Docs.get( path );
	}
	const tup::valueptr etf::getFinal( const tup::path& path ) const noexcept
	{
		if( !path )
			return tup::valueptr();

		size_t first_real_element = 0;
		if( path.at( 0 ) == name_docs )
			++first_real_element;

		// If we only have one real element, then we are looking for a document
		if( first_real_element == path.size() - 1 )
			return Docs._get( path, first_real_element );

		auto found = _get( Docs, path, first_real_element );
		if( found.first == nullptr )
			return found.second;

		// Keep resolving until we have a 'real' value
		std::set<tup::path> seen;
		while( found.second->isRef() )
		{
			// Empty reference finds nothing
			if( found.second->ref_value().empty() )
				return tup::valueptr();

			// Update seen paths
			if( seen.find( found.second->ref_value() ) != seen.end() )
				break;
			seen.insert( found.second->ref_value() );

			// If our reference doesn't start with 'docs', it is a relative
			// reference, either to the attribute's parent or to the document
			// it's in.
			if( found.second->ref_value().at( 0 ) != name_docs )
			{
				auto new_found = _get( *found.first,
					found.second->ref_value(), 0 );
				if( new_found.first != nullptr || new_found.second )
				{
					if( new_found.first != nullptr )
						found = new_found;
					else
						found.second = new_found.second;
				}
				else
				{
					auto doc = Docs.get(
						{ found.second->ref_value().at( 0 ) } );
					auto doc_tupl = found.first->document();
					if( doc && doc->isTuple()
						&& &doc->tuple_value() == doc_tupl )
						found = _get( *doc_tupl, found.second->ref_value(),
							1 );
				}
			}

			// We have a complete reference
			else
				found = _get( Docs, found.second->ref_value(), 1 );
		}

		return found.second;
	}

	bool etf::validate( name_t document, name_t pattern_document ) const
	{
		auto doc = Docs.attribute( document );
		if( !doc || !doc->isTuple() )
			return false;

		doc->tuple_value().validate();

		if( pattern_document != no_name )
		{
			auto ptrn = Docs.attribute( pattern_document );
			if( !ptrn )
				throw tup::Invalid( "Pattern document not available: "
					+ *pattern_document );
			if( !ptrn->isTuple() || !ptrn->tuple_value() )
				ptrn->tuple_value().validate( doc->tuple_value() );
		}

		return true;
	}




	void etf::load( const path& file )
	{
		textfilereader reader( file );
		auto name = eon::name::get( file.baseWithoutExt() );
		source src( file.str(), reader.read( SIZE_MAX ) );
		_parse( name, src );
	}
	void etf::parse( const string& str, name_t document_name )
	{
		source src( *document_name, string( str ) );
		_parse( document_name, src );
	}




	tup::valueptr etf::get( const tup::path& path ) noexcept
	{
		if( !path )
			return tup::valueptr();
		if( path.at( 0 ) == name_docs )
			return Docs.get( path.chopFirst() );
		else
			return Docs.get( path );
	}




	void etf::_parse( name_t name, source& src )
	{
		tf::Parser parser{ sourceref( src ) };
		while( true )
		{
			// Get document name and meta data - if any
			auto result = parser.parseDocumentStart();
			if( !result )
				break;
			auto doc_name = result.attribute( 0 )->name_value();
			auto doc_meta = result.attribute( 1 )->meta_value();

			// Use given name if no name in header
			if( doc_name == no_name )
				doc_name = name;

			// Prepare meta data
			tupleptr meta;
			if( doc_meta )
				meta = tupleptr( new tuple( std::move( doc_meta ) ) );

			// Create (empty) tuple with these details
			Docs.append( tup::valueptr( new tup::tupleval() ),
				doc_name, meta );
			auto& doc = Docs.attribute(
				Docs.numAttributes() - 1 )->tuple_value();

			// Keep on parsing until end of document
			while( parser.parseDocumentAttribute( doc ) )
				;
		}
	}

	std::pair<tuple*, tup::valueptr> etf::_get( const tuple& root,
		const tup::path& path_to_attribute, size_t first_path_elm ) const
	{
		if( path_to_attribute.size() - first_path_elm == 1 )
			return std::make_pair( nullptr, root._get(
				path_to_attribute, first_path_elm ) );
		auto parent_path = path_to_attribute.chopLast();
		auto parent = root._get( parent_path, first_path_elm );
		if( !parent || !parent->isTuple() )
			return std::make_pair( nullptr, tup::valueptr() );
		auto attrib = parent->tuple_value().attribute(
			path_to_attribute.last() );
		if( !attrib )
			return std::make_pair( nullptr, tup::valueptr() );
		return std::make_pair( &parent->tuple_value(), attrib );
	}
}
