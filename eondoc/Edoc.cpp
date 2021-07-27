#include "Edoc.h"
#include <eontokenizer/Tokenizer.h>


namespace eon
{
	void edoc::load( const file& input_file, const string& override_name )
	{
		auto name = input_file.path().name();
		source src{ input_file.path(), input_file.loadText() };
		auto document = _parse( name, src );
		add( std::move( document ), override_name.empty() ? name
			: override_name );
	}
	void edoc::parse( const string& str, const string& document_name )
	{
		source src( document_name, string( str ) );
		auto document = _parse( document_name, src );
		add( std::move( document ), document_name );
	}




	doc::document edoc::_parse( const string& name, source& src )
	{
		doc::Parser parser{ sourceref( src ) };
		return parser.parseDocument();
	}
}
