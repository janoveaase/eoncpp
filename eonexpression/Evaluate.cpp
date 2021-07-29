#include "Evaluate.h"
#include "OperandNode.h"
#include <eonfilesys/FileSys.h>
#include <eontuple/BoolValue.h>
#include <eontuple/FloatValue.h>
#include <eontuple/IntValue.h>
#include <eontuple/StringValue.h>
#include <eontuple/NameValue.h>
#include <eontuple/BinaryValue.h>
#include <eontuple/RawValue.h>
#include <eontuple/TupleValue.h>
#include <eontuple/Tuple.h>
#include <eontuple/MetaValue.h>
#include <eontuple/VariableValue.h>


namespace eon
{
	namespace expr
	{
		tup::valueptr evaluate::unary( tup::operators::code op_code,
			tup::variables& vars, const nodeptr& arg1 )
		{
			try
			{
				return _unary( op_code, vars, arg1 );
			}
			catch( tup::WrongType )
			{
				throw tup::UnsupportedOperand( "Unary operator '"
					+ tup::operators::mapCode( op_code )
					+ "' not supported for "
					+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
						vars ) ) + " operand" );
			}
		}
		tup::valueptr evaluate::binary( tup::operators::code op_code,
			tup::variables& vars, const nodeptr& arg1, const nodeptr& arg2 )
		{
			try
			{
				return _binary( op_code, vars, arg1, arg2 );
			}
			catch( tup::WrongType )
			{
				throw tup::UnsupportedOperand( "Binary operator '"
					+ tup::operators::mapCode( op_code )
					+ "' not supported for "
					+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
						vars ) ) + " vs. "
					+ *tup::mapBasicType( arg2->evaluate( vars )->softType(
						vars ) ) + " operand" );
			}
		}
		tup::valueptr evaluate::ternary( tup::operators::code op_code,
			tup::variables& vars,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			try
			{
				return _ternary( op_code, vars, arg1, arg2, arg3 );
			}
			catch( tup::WrongType )
			{
				throw tup::UnsupportedOperand( "Ternary operator '"
					+ tup::operators::mapCode( op_code ) + "' not supported for "
					+ *tup::mapBasicType( arg2->evaluate( vars )->softType(
						vars ) ) + " vs. "
					+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
						vars ) ) + " vs. "
					+ *tup::mapBasicType( arg3->evaluate( vars )->softType(
						vars ) ) + " operand" );
			}
		}




		tup::valueptr evaluate::_unary( tup::operators::code op_code,
			tup::variables& vars, const nodeptr& arg1 )
		{
			switch( op_code )
			{
				case tup::operators::code::_not:
					return tup::valueptr( new tup::boolval(
						!arg1->evaluate( vars )->softBool( vars ) ) );
				case tup::operators::code::sqrt:
					return tup::valueptr( new tup::floatval( std::sqrt(
						arg1->evaluate( vars )->softFloat( vars ) ) ) );
				case tup::operators::code::unary_minus:
				{
					auto val = arg1->evaluate( vars );
					switch( val->softType( vars ) )
					{
						case tup::basic_type::int_t:
							return tup::valueptr( new tup::intval(
								-val->softInt( vars ) ) );
						case tup::basic_type::float_t:
							return tup::valueptr( new tup::floatval(
								-val->softFloat( vars ) ) );
					}
					break;
				}
				case tup::operators::code::exists:
					return tup::valueptr( new tup::boolval( path(
						arg1->evaluate( vars )->softString(
							vars ) ).exists() ) );
				case tup::operators::code::trim:
					return tup::valueptr( new tup::stringval(
						arg1->evaluate( vars )->softString( vars ).trim() ) );
				case tup::operators::code::rtrim:
					return tup::valueptr( new tup::stringval( arg1->evaluate(
						vars )->softString( vars ).trimTrailing() ) );
				case tup::operators::code::ltrim:
					return tup::valueptr( new tup::stringval( arg1->evaluate(
						vars )->softString( vars ).trimLeading() ) );
				case tup::operators::code::len:
				{
					auto val = arg1->evaluate( vars );
					switch( val->softType( vars ) )
					{
						case tup::basic_type::string_t:
							return tup::valueptr( new tup::intval(
								static_cast<int64_t>( val->softString(
									vars ).numChars() ) ) );
						case tup::basic_type::raw_t:
							return tup::valueptr( new tup::intval(
								static_cast<int64_t>( val->softRaw(
									vars ).size() ) ) );
						case tup::basic_type::binary_t:
							return tup::valueptr( new tup::intval(
								static_cast<int64_t>( val->softBinary(
									vars ).size() / 2 ) ) );
						case tup::basic_type::tuple_t:
							return tup::valueptr( new tup::intval(
								static_cast<int64_t>( val->softTuple(
									vars ).numAttributes() ) ) );
						case tup::basic_type::meta_t:
							return tup::valueptr( new tup::intval(
								static_cast<int64_t>( val->softMeta(
									vars ).numAttributes() ) ) );
					}
					break;
				}
				case tup::operators::code::typeof:
					return tup::valueptr( new tup::nameval( tup::mapBasicType(
						arg1->evaluate( vars )->softType( vars ) ) ) );
				case tup::operators::code::isname:
					return tup::valueptr( new tup::boolval( name::valid(
						arg1->evaluate( vars )->softString( vars ) ) ) );
				case tup::operators::code::toname:
					return tup::valueptr( new tup::nameval( name::get(
						arg1->evaluate( vars )->softString( vars ) ) ) );
				case tup::operators::code::topath:
					return tup::valueptr( new tup::stringval( eon::path(
						arg1->evaluate( vars )->softString( vars ) ).str() ) );
				case tup::operators::code::basename:
					return tup::valueptr( new tup::stringval(
						eon::path( arg1->evaluate( vars )->softString(
							vars ) ).base() ) );
			}
			throw tup::UnsupportedOperand( "Unary operator '"
				+ tup::operators::mapCode( op_code ) + "' not supported for "
				+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
					vars ) ) + " operand" );
		}
		tup::valueptr evaluate::_binary( tup::operators::code op_code,
			tup::variables& vars, const nodeptr& arg1, const nodeptr& arg2 )
		{
			switch( op_code )
			{
				case tup::operators::code::_and:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softBool( vars )
						&& arg2->evaluate( vars )->softBool( vars ) ) );
				case tup::operators::code::_or:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softBool( vars )
						|| arg2->evaluate( vars )->softBool( vars ) ) );
				case tup::operators::code::eq:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) == 0 ) );
				case tup::operators::code::ne:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) != 0 ) );
				case tup::operators::code::lt:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) < 0 ) );
				case tup::operators::code::le:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) <= 0 ) );
				case tup::operators::code::gt:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) > 0 ) );
				case tup::operators::code::ge:
					return tup::valueptr( new tup::boolval(
						arg1->evaluate( vars )->softCompare(
							arg2->evaluate( vars ), vars ) >= 0 ) );
				case tup::operators::code::plus:
				{
					auto val1 = arg1->evaluate( vars );
					auto val2 = arg2->evaluate( vars );
					auto type1 = val1->softType( vars );
					auto type2 = val2->softType( vars );
					tup::basic_type type{ tup::basic_type::undef };
					// If both operands are numbers and one of them is float,
					// the result will also be float.
					if( type1 == tup::basic_type::float_t
						|| ( type1 == tup::basic_type::int_t
							&& type2 == tup::basic_type::float_t ) )
						type = tup::basic_type::float_t;
					// Otherwise, the result will match the type of the first
					// operand.
					else
						type = type1;
					switch( type )
					{
						case tup::basic_type::int_t:
							return tup::valueptr( new tup::intval(
								val1->softInt( vars )
								+ val2->softInt( vars ) ) );
						case tup::basic_type::float_t:
							return tup::valueptr( new tup::floatval(
								val1->softFloat( vars )
								+ val2->softFloat( vars ) ) );
						case tup::basic_type::string_t:
							return tup::valueptr( new tup::stringval(
								val1->softString( vars )
								+ val2->softString( vars ) ) );
						case tup::basic_type::binary_t:
							return tup::valueptr( new tup::binaryval(
								val1->softBinary( vars )
								+ val2->softBinary( vars ) ) );
						case tup::basic_type::raw_t:
						{
							auto a = val1->softRaw( vars );
							auto& b = val2->softRaw( vars );
							a.insert( a.end(), b.begin(), b.end() );
							return tup::valueptr( new tup::rawval( a ) );
						}
						case tup::basic_type::tuple_t:
						{
							auto a = val1->softTuple( vars );
							a.append( val2->softTuple( vars ), vars );
							return tup::valueptr( new tup::tupleval(
								std::move( a ) ) );
						}
						case tup::basic_type::meta_t:
						{
							auto a = val1->softMeta( vars );
							a.append( val2->softMeta( vars ), vars );
							return tup::valueptr( new tup::metaval(
								std::move( a ) ) );
						}
					}
					break;
				}
				case tup::operators::code::minus:
				{
					auto val1 = arg1->evaluate( vars );
					auto val2 = arg2->evaluate( vars );
					auto type1 = val1->softType( vars );
					auto type2 = val2->softType( vars );
					tup::basic_type type{ tup::basic_type::undef };
					// If both operands are numbers and one of them is float,
					// the result will also be float.
					if( type1 == tup::basic_type::float_t
						|| ( type1 == tup::basic_type::int_t
							&& type2 == tup::basic_type::float_t ) )
						type = tup::basic_type::float_t;
					// Otherwise, the result will match the type of the first
					// operand.
					else
						type = type1;
					switch( type )
					{
						case tup::basic_type::int_t:
							return tup::valueptr( new tup::intval(
								val1->softInt( vars )
								- val2->softInt( vars ) ) );
						case tup::basic_type::float_t:
							return tup::valueptr( new tup::floatval(
								val1->softFloat( vars )
								- val2->softFloat( vars ) ) );
						case tup::basic_type::string_t:
							return tup::valueptr( new tup::stringval(
								val1->softString( vars ).remove(
									val2->softString( vars ) ) ) );
					}
					break;
				}
				case tup::operators::code::multiply:
				{
					auto val1 = arg1->evaluate( vars );
					auto val2 = arg2->evaluate( vars );
					auto type1 = val1->softType( vars );
					auto type2 = val2->softType( vars );
					tup::basic_type type{ tup::basic_type::undef };
					// If both operands are numbers and one of them is float,
					// the result will also be float.
					if( type1 == tup::basic_type::float_t
						|| ( type1 == tup::basic_type::int_t
							&& type2 == tup::basic_type::float_t ) )
						type = tup::basic_type::float_t;
					// Otherwise, the result will match the type of the first
					// operand.
					else
						type = type1;
					switch( type )
					{
						case tup::basic_type::int_t:
							return tup::valueptr( new tup::intval(
								val1->softInt( vars )
								* val2->softInt( vars ) ) );
						case tup::basic_type::float_t:
							return tup::valueptr( new tup::floatval(
								val1->softFloat( vars )
								* val2->softFloat( vars ) ) );
					}
					break;
				}
				case tup::operators::code::divide:
				{
					auto val1 = arg1->evaluate( vars );
					auto val2 = arg2->evaluate( vars );
					auto type1 = val1->softType( vars );
					auto type2 = val2->softType( vars );
					tup::basic_type type{ tup::basic_type::undef };
					// If both operands are numbers and one of them is float,
					// the result will also be float.
					if( type1 == tup::basic_type::float_t
						|| ( type1 == tup::basic_type::int_t
							&& type2 == tup::basic_type::float_t ) )
						type = tup::basic_type::float_t;
					// Otherwise, the result will match the type of the first
					// operand.
					else
						type = type1;
					switch( type )
					{
						case tup::basic_type::int_t:
						{
							auto divisor = val2->softInt( vars );
							if( divisor == 0 )
								throw DivisionByZero();
							return tup::valueptr( new tup::intval(
								val1->softInt( vars ) / divisor ) );
						}
						case tup::basic_type::float_t:
						{
							auto divisor = val2->softFloat( vars );
							if( divisor == 0.0 )
								throw DivisionByZero();
							return tup::valueptr( new tup::floatval(
								val1->softFloat( vars ) / divisor ) );
						}
					}
					break;
				}
				case tup::operators::code::pow:
					return tup::valueptr( new tup::floatval(
						std::pow( arg1->evaluate( vars )->softFloat( vars ),
							arg2->evaluate( vars )->softFloat( vars ) ) ) );
				case tup::operators::code::mod:
					return tup::valueptr( new tup::intval(
						arg1->evaluate( vars )->softInt( vars )
						% arg2->evaluate( vars )->softInt( vars ) ) );
				case tup::operators::code::match:
				{
					auto match = arg2->evaluate( vars )->softRegex(
						vars ).match( arg1->evaluate( vars )->softString(
							vars ) );
					if( match )
					{
						for( auto& capt : match )
							vars.set( capt.first, tup::valueptr(
								new tup::stringval( capt.second ) ) );
						return tup::valueptr( new tup::boolval( true ) );
					}
					return tup::valueptr( new tup::boolval( false ) );
				}
				case tup::operators::code::assign:
				{
					auto val1 = arg1->isOperator()
						? arg1->evaluate( vars )
						: dynamic_cast<operandnode*>( &*arg1 )->value();
					auto val2 = arg2->evaluate( vars );
					if( !val2 )
						throw tup::NotFound();
					auto name = no_name;
					if( val1->isVar() )
						name = val1->hardVar();
					else
						name = val1->softName( vars );
					if( val2->isVar() )
					{
						auto val = vars.get( name );
						if( !val )
							throw tup::NotFound( "No such variable: "
								+ *name );
						vars.set( name, val );
					}
					else
						vars.set( name, val2 );
					return tup::valueptr( new tup::variableval( name ) );
				}
				case tup::operators::code::resize:
				{
					auto str = arg1->evaluate( vars )->softString( vars );
					auto sz = arg2->evaluate( vars )->softInt( vars );
					if( sz > 0 && str.numChars() > static_cast<size_t>( sz ) )
						str = str.substr( 0, sz );
					else
						str = str.padRight( sz, ' ' );
					return tup::valueptr( new tup::stringval( str ) );
				}
			}
			throw tup::UnsupportedOperand( "Binary operator '"
				+ tup::operators::mapCode( op_code ) + "' not supported for "
				+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
					vars ) ) + " vs. "
				+ *tup::mapBasicType( arg2->evaluate( vars )->softType(
					vars ) ) + " operand" );
		}
		tup::valueptr evaluate::_ternary( tup::operators::code op_code,
			tup::variables& vars,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			switch( op_code )
			{
				case tup::operators::code::if_else:
				{
					auto condition = arg2->evaluate( vars );
					if( condition && condition->softBool( vars ) )
						return arg1->evaluate( vars );
					else
						return arg3->evaluate( vars );
				}
			}
			throw tup::UnsupportedOperand( "Ternary operator '"
				+ tup::operators::mapCode( op_code ) + "' not supported for "
				+ *tup::mapBasicType( arg2->evaluate( vars )->softType(
					vars ) ) + " vs. "
				+ *tup::mapBasicType( arg1->evaluate( vars )->softType(
					vars ) ) + " vs. "
				+ *tup::mapBasicType( arg3->evaluate( vars )->softType(
					vars ) ) + " operand" );
		}
	}
}
