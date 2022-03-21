#include "OperatorAction.h"


namespace eon
{
	namespace actions
	{
		const string OperatorAction::NamePrefix{ "$op_" };

		OperatorAction::OperatorAction( const EonType& instance_type, type::operators::code op_code,
			const EonType& return_type, DynamicTuple arguments, std::initializer_list<name_t> raises, source::Ref source )
			: Action( instance_type, Type::_operator, compilerName( NamePrefix + type::operators::mapCode( op_code ) ),
				return_type, arguments, raises, source )
		{
			OpCode = op_code;
			NumArgs = Arguments.numAttributes();
		}

		OperatorAction::OperatorAction( type::operators::code op_code, index_t num_args, source::Ref source )
			: Action( name_operator, Type::_operator, compilerName( NamePrefix + type::operators::mapCode( op_code ) ),
				EonType(), DynamicTuple(), {}, source )
		{
			OpCode = op_code;
			NumArgs = num_args;
		}

		type::Object* OperatorAction::execute( scope::Scope& scope, type::Node& action_node )
		{
			throw type::AccessDenied( "Cannot execute 'operator action' - action not implemented!" );
		}
	}
}
