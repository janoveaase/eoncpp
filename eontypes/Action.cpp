#include "Action.h"
#include "Name.h"
#include "Node.h"
#include "Reference.h"


namespace eon
{
	namespace actions
	{
		string mapType( Type type )
		{
			switch( type )
			{
				case Type::instance:
					return "instance";
				case Type::prefix_first:
					return "prefix_first";
				case Type::prefix_last:
					return "prefix_last";
				case Type::get:
					return "get";
				case Type::set:
					return "set";
				case Type::_operator:
					return "operator";
				case Type::helper:
					return "helper";
				case Type::type:
					return "type";
				default:
					return "<undef>";
			}
		}
	}
	namespace type
	{
		Action::Action( const EonType& instance_type, actions::Type action_type, name_t name, const EonType& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises, source::Ref source )
			: Object( _generateType( name, return_type, arguments ), source )
		{
			_create( instance_type, action_type, name, return_type, arguments, raises );
		}




		index_t Action::numArgsWithoutDefaultValue() const noexcept
		{
			index_t num = 0;
			for( auto& arg : Arguments )
			{
				if( arg.value() == nullptr )
					++num;
			}
			return num;
		}




		EonType Action::_generateType( name_t name, const EonType& returntype, const DynamicTuple& arguments )
		{
			// Action types are defined as: T(name=<name>, type=<returntype>, args=(<arguments>)
			EonType type( name, name_name );
			type << EonType( returntype, name_type );			
			EonType args;
			for( auto& argument : arguments )
				args << EonType( argument.type() );
			type << EonType( args, name_args );
			return type;
		}

		void Action::_create( const EonType& instance_type, actions::Type action_type, name_t name,
			const EonType& return_type, DynamicTuple arguments, std::initializer_list<name_t> raises )
		{
			InstanceType = instance_type;
			ActionType = action_type;
			Name = name;
			ReturnType = return_type;
			_initPrefixFirstAction();
			Arguments += arguments;
			_initPrefixLastAction();
			Arguments.finalize();
			Raises = raises;
		}

		void Action::_initPrefixFirstAction()
		{
			if( ActionType == actions::Type::prefix_first )
				Arguments += Attribute( name_me, new Modifiable( InstanceType, source::Ref() ) );
		}
		void Action::_initPrefixLastAction()
		{
			if( ActionType == actions::Type::prefix_last )
				Arguments += Attribute( name_me, new Modifiable( InstanceType, source::Ref() ) );
		}

		Object* Action::_childValue( scope::Scope& scope, Node& action_node, size_t arg_no )
		{
			auto& child = action_node.child( arg_no );
			if( child.isAction() )
				return child.action().execute( scope, child );
			else if( child.isValue() )
				return child.value();
			else
				return ( (NameType*)scope.find( name_name ) )->instantiate( child.name() );
		}
	}
}
