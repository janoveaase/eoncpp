#include "Eof.h"
#include <eontokenizer/Tokenizer.h>


namespace eon
{
	bool eonof::validate( name_t document, name_t pattern_document )
	{
		auto doc = Docs.attribute( document );
		if( !doc || !doc->isTuple() )
			return false;

		doc->tuple_value().validate( Vars);

		if( pattern_document != no_name )
		{
			if( Patterns.find( pattern_document ) == Patterns.end() )
				throw tup::Invalid( "The specified pattern document \""
					+ *pattern_document
					+ "\" is not a \"pattern document\"!" );
			auto ptrn = Docs.attribute( pattern_document );
			if( !ptrn )
				throw tup::Invalid( "Pattern document not available: "
					+ *pattern_document );
			if( ptrn->isTuple() && ptrn->hardTuple() )
				ptrn->tuple_value().validate( doc->hardTuple(), Vars );
		}

		return true;
	}




	void eonof::load( const path& file )
	{
		textfilereader reader( file );
		reader.open();
		auto name = eon::name::get( file.baseWithoutExt() );
		source src( file.str(), reader.read( SIZE_MAX ) );
		_parse( name, src );
	}
	void eonof::parse( const string& str, name_t document_name )
	{
		source src( *document_name, string( str ) );
		_parse( document_name, src );
	}




	void eonof::_parse( name_t name, source& src )
	{
		tf::Parser parser{ sourceref( src ) };
		while( true )
		{
			// Get document name and meta data - if any
			auto result = parser.parseDocumentStart( Vars );
			if( !result )
				break;
			auto doc_name = result.attribute( 0 )->hardName();
			auto doc_meta = result.attribute( 1 )->hardMeta();

			// Use given name if no name in header
			if( doc_name == no_name )
				doc_name = name;

			// Prepare meta data
			tupleptr meta;
			if( doc_meta )
				meta = tupleptr( new tuple( std::move( doc_meta ) ) );

			// If this is a pattern document, we need to record it as such
			if( meta && meta->containsUnnamedValue( name_pattern, Vars ) )
				Patterns.insert( doc_name );

			// Create (empty) tuple with these details
			Docs.append( tup::valueptr( new tup::tupleval() ), Vars,
				doc_name, meta );
			auto& doc = *(tuple*)&Docs.attribute(
				Docs.numAttributes() - 1 )->hardTuple();

			// Keep on parsing until end of document
			while( parser.parseDocumentAttribute( doc, Vars ) )
				;
		}
	}
}
