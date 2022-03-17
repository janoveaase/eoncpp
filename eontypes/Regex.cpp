#include "Regex.h"
#include "RegexActions.h"


namespace eon
{
	void registerRegex( scope::Global& scope )
	{
		scope.add( name_regex, new RegexType() );
		registerRegexActions( scope );
	}




	type::Instance* RegexType::instantiate( const regex& value ) const { return new RegexInstance( value, source::Ref() ); }
	type::Instance* RegexType::instantiate( regex&& value ) const {
		return new RegexInstance( std::move( value ), source::Ref() ); }
}
