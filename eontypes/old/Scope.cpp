#include "Scope.h"


namespace eon
{
	namespace tup
	{
		VarScope::~VarScope()
		{
			for( auto& ref : RefTypes )
				delete ref.second;
			for( auto& type : TypeSpecs )
				delete type.second;
			if( TupleSpecs.size() > 0 )
			{
				std::vector<tuplespec*> to_delete;
				to_delete.reserve( TupleSpecs.size() );
				for( auto spec : TupleSpecs )
					to_delete.push_back( spec );
				for( auto spec : to_delete )
					delete spec;
			}
		}




		const typespec* VarScope::primitive( name_t name )
		{
			auto found = TypeSpecs.find( name );
			if( found != TypeSpecs.end() )
				return found->second->Type;
			else
			{
				_registerType( name, new primitivespec( name ) );
				return primitive( name );
			}
		}

		const typespec* VarScope::standard( name_t name )
		{
			auto found = TypeSpecs.find( name );
			if( found != TypeSpecs.end() )
				return found->second->Type;
			else
			{
				_registerType( name, new standardspec( name ) );
				return standard( name );
			}
		}

		const tuplespec* VarScope::tuple( tuplespec&& spec )
		{
			auto found = TupleSpecs.find( &spec );
			if( found != TupleSpecs.end() )
				return *found;
			else
				return *TupleSpecs.insert( new tuplespec( std::move( spec ) ) ).first;
		}

		void VarScope::registerCustomType( name_t name, typespec* type )
		{
			_registerType( name, type );
		}

		const referencespec* VarScope::reference( const typespec* target_type )
		{
			auto found = RefTypes.find( target_type );
			if( found != RefTypes.end() )
				return found->second;
			else
			{
				auto ref = new referencespec( target_type );
				RefTypes[ target_type ] = ref;
				return ref;
			}
		}




		void VarScope::addInstance( name_t name, eon::instance* object )
		{
			auto found = Instances.find( name );
			if( found == Instances.end() )
				throw DuplicateNameError( *name );
			Instances[ name ] = object;
			object->addRef();
		}

		instance* VarScope::instance( name_t name ) noexcept
		{
			auto found = Instances.find( name );
			if( found != Instances.end() )
				return found->second;
			if( Parent != nullptr )
				return Parent->instance( name );
			return nullptr;
		}




		void VarScope::registerAction( name_t type, actionspec&& signature, action* act )
		{
			auto tp = TypeSpecs.find( type );
			if( tp == TypeSpecs.end() )
				throw TypeNotFound( *type );
			if( tp->second->Actions.find( &signature ) != tp->second->Actions.end() )
				throw DuplicateNameError( signature.str() );
			tp->second->Actions[ new actionspec( std::move( signature ) ) ] = act;
		}




		VarScope::Spec::~Spec()
		{
			delete Type;
			for( auto& act : Actions )
				delete act.second;
		}

		VarScope::Spec& VarScope::_registerType( name_t name, typespec* type )
		{
			auto found = TypeSpecs.find( name );
			if( found != TypeSpecs.end() )
				throw DuplicateNameError( *name );
			auto spec = new Spec( type );
			TypeSpecs[ name ] = spec;
			return *spec;
		}
	}
}
