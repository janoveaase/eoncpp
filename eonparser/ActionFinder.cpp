#include "ActionFinder.h"


namespace eon
{
	namespace parser
	{
		TypeTuple ActionFinder::find( Details& details ) const
		{
			std::set<TypeTuple> action_signatures = scope::global().signatures(
				details.actionName(), details.category(), details.typeName(), details.args() );
			if( action_signatures.size() == 1 )
				return *action_signatures.begin();
			else
				return _notFound( details, std::move( action_signatures ) );
		}




		TypeTuple ActionFinder::_notFound( Details& details, std::set<TypeTuple>&& possible ) const
		{

			if( possible.empty() )
				details.reporter().error(
					"Action for T(" + eon::str( details.typeName() ) + ") is not supported!", details.source() );
			else if( possible.size() > 1 )	// This should not be possible ...
			{
				details.reporter().error(
					"Multiple " + eon::str( details.actionName() ) + " options for T(" + eon::str( details.typeName() )
					+ ") from " + details.args().str() + "!", details.source() );
				bool first = true;
				for( auto& type : possible )
				{
					string lead = first ? "Could be" : "or";
					first = false;
					auto action = scope::global().action( type );
					details.reporter().note(
						lead + " from " + type.str() + " ...", action ? action->source() : source::Ref() );
				}
			}
			return TypeTuple();
		}
	}
}