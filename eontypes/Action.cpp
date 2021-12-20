#include "Action.h"
#include "Name.h"


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
		Action::Action( const TypeTuple& type, type::operators::code op_code, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( actions::Type::_operator, name::compilerGet( type::operators::mapCode( op_code ) ),
				return_type.asName(), arguments ) )
		{
			_create( type, op_code, return_type, arguments, raises );
		}
		Action::Action( const TypeTuple& type, type::operators::code op_code, name_t return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( actions::Type::_operator, name::compilerGet( type::operators::mapCode( op_code ) ),
				return_type, arguments ) )
		{
			_create( type, op_code, TypeTuple( { return_type } ), arguments, raises );
		}
		Action::Action( name_t type, type::operators::code op_code, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( actions::Type::_operator, name::compilerGet( type::operators::mapCode( op_code ) ),
				return_type.asName(), arguments ) )
		{
			_create( TypeTuple( { type } ), op_code, return_type, arguments, raises );
		}
		Action::Action( name_t type, type::operators::code op_code, name_t return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( actions::Type::_operator, name::compilerGet( type::operators::mapCode( op_code ) ),
				return_type, arguments ) )
		{
			_create( TypeTuple( { type } ), op_code, TypeTuple( { return_type } ), arguments, raises );
		}

		Action::Action( const TypeTuple& type, actions::Type action_type, name_t name, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( action_type, name, return_type.asName(), arguments ) )
		{
			_create( type, action_type, name, return_type, arguments, raises );
		}
		Action::Action( const TypeTuple& type, actions::Type action_type, name_t name, name_t return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( action_type, name, return_type, arguments ) )
		{
			_create( type, action_type, name, TypeTuple( { return_type } ), arguments, raises );
		}
		Action::Action( name_t type, actions::Type action_type, name_t name, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( action_type, name, return_type.asName(), arguments ) )
		{
			_create( TypeTuple( { type } ), action_type, name, return_type, arguments, raises );
		}
		Action::Action( name_t type, actions::Type action_type, name_t name, name_t return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
			: Object( _makeSignature( action_type, name, return_type, arguments ) )
		{
			_create( TypeTuple( { type } ), action_type, name, TypeTuple( { return_type } ), arguments, raises );
		}




		name_t Action::_makeSignature( actions::Type type, name_t name, name_t returntype, const DynamicTuple& arguments )
			const
		{
			string signature{ mapType( type ) + "@" + *name };
			if( returntype != no_name )
				signature += "@" + *returntype;
			for( auto& argument : arguments )
				signature += "$" + *argument.value()->type().asName();
			return name::compilerGet( std::move( signature ) );
		}

		void Action::_create( const TypeTuple& type, type::operators::code op_code, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
		{
			InstanceType = type;
			ActionType = actions::Type::_operator;
			Name = name::compilerGet( type::operators::mapCode( op_code ) );
			ReturnType = return_type;
			_initPrefixFirstAction();
			Arguments += arguments;
			_initPrefixLastAction();
			Raises = raises;
			OpCode = op_code;
			InputPrecedence = type::operators::inputPrecedence( op_code );
			StackPrecedence = type::operators::stackPrecedence( op_code );
		}
		void Action::_create( const TypeTuple& type, actions::Type action_type, name_t name, const TypeTuple& return_type,
			DynamicTuple arguments, std::initializer_list<name_t> raises )
		{
			InstanceType = type;
			ActionType = action_type;
			Name = name;
			ReturnType = return_type;
			_initPrefixFirstAction();
			Arguments += arguments;
			_initPrefixLastAction();
			Raises = raises;
			OpCode = type::operators::code::call;
			InputPrecedence = type::operators::inputPrecedence( type::operators::code::call );
			StackPrecedence = type::operators::stackPrecedence( type::operators::code::call );
		}

		void Action::_initPrefixFirstAction()
		{
			if( ActionType == actions::Type::prefix_first )
				Arguments += Attribute( name_me, InstanceType, type::Qualifier::modify );
		}
		void Action::_initPrefixLastAction()
		{
			if( ActionType == actions::Type::prefix_last )
				Arguments += Attribute( name_me, InstanceType, type::Qualifier::modify );
		}
	}
}
