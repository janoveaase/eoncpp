#include "Scope.h"
#include <eontypes/Action.h>


namespace eon
{
	namespace scope
	{
		Global::~Global()
		{
			for( auto& elm : Actions )
			{
				for( auto act : elm.second )
					delete act;
			}
		}





		std::list<type::Action*> Global::getActions( name_t name, const EonType& inst_type, const EonType& args,
			const EonType& return_type ) const noexcept
		{
			std::list<type::Action*> result;
			auto found = Actions.find( name );
			if( found == Actions.end() )
				return result;
			for( auto i = found->second.begin(); i != found->second.end(); ++i )
			{
				auto& action = **i;

				// Step 1: Make sure object type matches
				if( action.instanceType() == inst_type )
				{
					// Step 2: Make sure return type matches
					if( action.returnType().compatibleWith( return_type ) )
					{
						// Step 3: Make sure arguments match
						if( args.compatibleWith( action.arguments().type() ) )
							result.push_back( &action );
					}
				}
			}
			return result;
		}
		std::list<type::Action*> Global::getActions( name_t name, const EonType& inst_type, const EonType& args )
			const noexcept
		{
			std::list<type::Action*> result;
			auto found = Actions.find( name );
			if( found == Actions.end() )
				return result;
			for( auto i = found->second.begin(); i != found->second.end(); ++i )
			{
				auto& action = **i;

				if( action.instanceType() == inst_type )
				{
					if( args.compatibleWith( action.arguments().type() ) )
						result.push_back( &action );
				}
			}
			return result;
		}
	}
}
