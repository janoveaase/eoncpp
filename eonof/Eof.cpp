#include "Eof.h"
#include <eontokenizer/Tokenizer.h>


namespace eon
{
	bool eonof::validate( name_t document, name_t pattern_document )
	{
		auto doc = Docs.at( document );
		if( !doc || !doc->isTuple() )
			return false;

		doc->tuple_value().resolveAllReferences( Vars );

		doc->tuple_value().validate( Vars);

		if( pattern_document != no_name )
		{
			if( Patterns.find( pattern_document ) == Patterns.end() )
				throw EofInvalid( "The specified pattern document \"" + *pattern_document
					+ "\" is not a \"pattern document\"!" );
			auto ptrn = Docs.at( pattern_document );
			if( !ptrn )
				throw EofInvalid( "Pattern document not available: " + *pattern_document );
			if( ptrn->isTuple() && ptrn->actualTuple() )
				ptrn->tuple_value().validate( doc->actualTuple(), Vars );
		}

		return true;
	}




	void eonof::loadReplace( const file& input_file )
	{
		auto name = eon::name::get( input_file.path().str() );
		source src( input_file.path(), input_file.loadText() );
		_parse( name, src, false );
	}
	void eonof::loadMerge( const file& input_file )
	{
		auto name = eon::name::get( input_file.path().str() );
		source src( input_file.path(), input_file.loadText() );
		_parse( name, src, true );
	}

	void eonof::parseReplace( const string& str, name_t document_name )
	{
		source src( *document_name, string( str ) );
		_parse( document_name, src, false );
	}
	void eonof::parseMerge( const string& str, name_t document_name )
	{
		source src( *document_name, string( str ) );
		_parse( document_name, src, true );
	}



	vars::valueptr eonof::_get( const nameref& ref, vars::valueptr context ) const noexcept
	{
		if( ref.empty() )
			return vars::valueptr();
		if( ref.at( 0 ) != name_docs )
		{
			if( context )
			{
				const tuple* tup{ nullptr };
				if( context->isTuple() )
					tup = &context->actualTuple();
				else if( context->isMeta() )
					tup = &context->actualMeta();
				else if( context->isFunction() )
					tup = &context->actualFunction();
				if( tup )
				{
					auto found = tup->find( ref );
					if( found )
						return found;
					found = tup->document()->find( ref );
					if( found )
						return found;
				}
			}
		}
		return Docs.find( ref );
	}
	void eonof::_set( const nameref& ref, vars::valueptr context, const vars::valueptr& value )
	{
		if( ref.empty() )
			throw vars::InvalidReference( "Cannot set empty reference!" );
		if( !value )
			throw vars::NoValue( "Cannot set void value for reference " + ref.str() + "!" );
		auto found = _get( ref, context );
		if( !found )
			throw vars::NotFound( "Reference " + ref.str() + " has no target!" );
		if( found->type() != value->type() )
			throw vars::WrongType( "Reference " + ref.str() + " (" + *vars::mapTypeCode( found->type() )
				+ ") cannot have a value of type " + *vars::mapTypeCode( value->type() ) + " set!" );
		found->setActual( value );
	}




	void eonof::_parse( name_t name, source& src, bool merge )
	{
		tf::Parser parser{ sourceref( src ) };
		while( true )
		{
			// Get document name and meta data - if any
			auto result = parser.parseDocumentStart( Vars, merge );
			if( !result )
				break;
			auto doc_name = result.at( 0 )->actualName();
			auto doc_meta = result.at( 1 )->actualMeta();

			// Use given name if no name in header
			if( doc_name == no_name )
				doc_name = name;

			// Prepare meta data
			tupleptr meta;
			if( doc_meta )
				meta = tupleptr( new tuple( std::move( doc_meta ) ) );

			// If this is a pattern document, we need to record it as such
			if( meta && meta->containsUnnamedValue( name_pattern ) )
				Patterns.insert( doc_name );

			vars::valueptr doc;

			if( Docs.exists( name ) )
			{
				// Choose strategy for existing document
				doc = Docs.at( name );
				if( !merge )
					doc->clear();
			}
			else
			{
				// Create (empty) tuple with these details
				Docs.append( vars::tupleval::create(), Vars, doc_name, meta );
				doc = Docs.at( Docs.numAttributes() - 1 );
			}

			// Keep on parsing until end of document
			auto& docp = dynamic_cast<vars::tupleval*>( &*doc )->tuple_value();
			while( parser.parseDocumentAttribute( docp, Vars, merge ) )
				;

			docp.setAttributeContext( doc );
		}
	}
}
