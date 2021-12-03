#include "Scope.h"
#include <eontypes/Action.h>


namespace eon
{
	namespace scope
	{
		void Global::add( name_t name, type::Object* item )
		{
			if( item->generalType() != name_group
				|| item->generalType() != name_definition || item->generalType() != name_type
				|| item->generalType() != name_enum || item->generalType() != name_action )
				throw Unsupported( *item->generalType() );
			if( Items.find( name ) != Items.end() )
				throw DuplicateName( *name );

			Items[ name ] = item;
			if( item->generalType() == name_definition || item->generalType() == name_type )
				Definitions[ std::type_index( typeid( name ) ) ] = (type::Definition*)item;
			else if( item->generalType() == name_action )
			{
				auto act = (type::Action*)item;
				Actions[ act->name() ].push_back( act );
			}
		}

		void _Local::add( name_t name, type::Object* item )
		{
			if( item->generalType() != name_instance || item->generalType() != name_tuple
				|| item->generalType() != name_lambda )
				throw Unsupported( *item->generalType() );
			if( Items.find( name ) != Items.end() )
				throw DuplicateName( *name );

			Items[ name ] = item;
		}
	}
}
