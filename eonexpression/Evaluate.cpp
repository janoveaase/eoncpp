#include "Evaluate.h"
#include "OperandNode.h"
#include <eonfilesys/FileSys.h>
#include <eonvariables/BoolValue.h>
#include <eonvariables/CharValue.h>
#include <eonvariables/IntValue.h>
#include <eonvariables/FloatValue.h>
#include <eonvariables/NameValue.h>
#include <eonvariables/StringValue.h>
#include <eonvariables/BinaryValue.h>
#include <eonvariables/RawValue.h>
#include <eonvariables/ReferenceValue.h>
#include <eonvariables/VariableValue.h>
#include <eonvariables/TupleValue.h>
#include <eontuple/Tuple.h>
#include <eonvariables/MetaValue.h>
#include <eonvariables/FunctionValue.h>
#include <eonvariables/ControlValue.h>
#include <cmath>


namespace eon
{
	namespace expr
	{
		vars::valueptr evaluate::unary( vars::operators::code op_code, vars::variables& varcache, const nodeptr& arg1 )
		{
			try
			{
				return _unary( op_code, varcache, arg1 );
			}
			catch( vars::WrongType )
			{
				throw vars::UnsupportedOperand( "Unary operator '"
					+ vars::operators::mapCode( op_code ) + "' not supported for "
					+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " operand" );
			}
		}
		vars::valueptr evaluate::binary( vars::operators::code op_code,
			vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			try
			{
				return _binary( op_code, varcache, arg1, arg2 );
			}
			catch( vars::WrongType )
			{
				throw vars::UnsupportedOperand( "Binary operator '"
					+ vars::operators::mapCode( op_code )
					+ "' not supported for "
					+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
					+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " operand" );
			}
		}
		vars::valueptr evaluate::ternary( vars::operators::code op_code, vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			try
			{
				return _ternary( op_code, varcache, arg1, arg2, arg3 );
			}
			catch( vars::WrongType )
			{
				throw vars::UnsupportedOperand( "Ternary operator '"
					+ vars::operators::mapCode( op_code ) + "' not supported for "
					+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " vs. "
					+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
					+ *vars::mapTypeCode( arg3->evaluate( varcache )->targetType( varcache ) ) + " operand" );
			}
		}




		vars::valueptr evaluate::_unary( vars::operators::code op_code, vars::variables& varcache, const nodeptr& arg1 )
		{
			switch( op_code )
			{
				case vars::operators::code::_not:
					return vars::boolval::create( !arg1->evaluate( varcache )->targetBool( varcache ) );
				case vars::operators::code::sqrt:
				{
					auto val = arg1->evaluate( varcache );
					switch( val->targetType( varcache ) )
					{
						case vars::type_code::int_t:
							return vars::floatval::create(
								std::sqrt( static_cast<int>( arg1->evaluate( varcache )->targetInt( varcache ) ) ) );
						case vars::type_code::float_t:
							return vars::floatval::create(
								std::sqrt( arg1->evaluate( varcache )->targetFloat( varcache ) ) );
					}
					break;
				}
				case vars::operators::code::unary_minus:
				{
					auto val = arg1->evaluate( varcache );
					switch( val->targetType( varcache ) )
					{
						case vars::type_code::int_t:
							return vars::intval::create( -val->targetInt( varcache ) );
						case vars::type_code::float_t:
							return vars::floatval::create( -val->targetFloat( varcache ) );
					}
					break;
				}
				case vars::operators::code::exists:
					return vars::boolval::create(
						path( arg1->evaluate( varcache )->targetString( varcache ) ).exists() );
				case vars::operators::code::trim:
					return vars::stringval::create( arg1->evaluate( varcache )->targetString( varcache ).trim() );
				case vars::operators::code::rtrim:
					return vars::stringval::create(
						arg1->evaluate( varcache )->targetString( varcache ).trimTrailing() );
				case vars::operators::code::ltrim:
					return vars::stringval::create( arg1->evaluate( varcache )->targetString( varcache ).trimLeading() );
				case vars::operators::code::len:
				{
					auto val = arg1->evaluate( varcache );
					switch( val->targetType( varcache ) )
					{
						case vars::type_code::string_t:
							return vars::intval::create(
								static_cast<int64_t>( val->targetString( varcache ).numChars() ) );
						case vars::type_code::raw_t:
							return vars::intval::create(
								static_cast<int64_t>( val->targetRaw( varcache ).size() ) );
						case vars::type_code::binary_t:
							return vars::intval::create(
								static_cast<int64_t>( val->targetBinary( varcache ).size() / 2 ) );
						case vars::type_code::tuple_t:
							return vars::intval::create(
								static_cast<int64_t>( val->targetTuple( varcache ).numAttributes() ) );
						case vars::type_code::meta_t:
							return vars::intval::create(
								static_cast<int64_t>( val->targetMeta( varcache ).numAttributes() ) );
					}
					break;
				}
				case vars::operators::code::type_of:
					return vars::nameval::create(
						vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) );
				case vars::operators::code::isname:
					return vars::boolval::create( name::valid( arg1->evaluate( varcache )->targetString( varcache ) ) );
				case vars::operators::code::is_set:
					if( !arg1->isOperator() )
					{
						auto val = dynamic_cast<operandnode*>( &*arg1 )->value();
						if( val->isVariable() )
							return vars::boolval::create( static_cast<bool>( varcache.get( val->actualVariable() ) ) );
					}
					break;
				case vars::operators::code::topath:
					return vars::stringval::create(
						eon::path( arg1->evaluate( varcache )->targetString( varcache ) ).str() );
				case vars::operators::code::basename:
					return vars::stringval::create(
						eon::path( arg1->evaluate( varcache )->targetString( varcache ) ).base() );
				case vars::operators::code::tobool:
					return vars::boolval::create( arg1->evaluate( varcache )->convertTargetBool( varcache ) );
				case vars::operators::code::tochar:
					return vars::charval::create( arg1->evaluate( varcache )->convertTargetChar( varcache ) );
				case vars::operators::code::toint:
					return vars::intval::create( arg1->evaluate( varcache )->convertTargetInt( varcache ) );
				case vars::operators::code::tofloat:
					return vars::floatval::create(arg1->evaluate( varcache )->convertTargetFloat( varcache ) );
				case vars::operators::code::toname:
					return vars::nameval::create( arg1->evaluate( varcache )->convertTargetName( varcache ) );
				case vars::operators::code::tostring:
					return vars::stringval::create( arg1->evaluate( varcache )->convertTargetString( varcache ) );
				case vars::operators::code::toref:
					return vars::refval::create( arg1->evaluate( varcache )->convertTargetReference( varcache ), nullptr );
				case vars::operators::code::tovar:
					return vars::variableval::create( arg1->evaluate( varcache )->convertTargetVariable( varcache ) );
				case vars::operators::code::_return:
				{
					auto val = arg1->evaluate( varcache );
					return vars::controlval::create( vars::ctrl_t::_return, val->target( varcache, val ) );
				}
				case vars::operators::code::loadstr:
					return vars::stringval::create(
						file( arg1->evaluate( varcache )->targetString( varcache ) ).loadText() );
				case vars::operators::code::loadraw:
				{
					auto str = file( arg1->evaluate( varcache )->targetString( varcache ) ).loadText();
					return vars::rawval::create( str.splitSequential<std::vector<string>>( '\n' ) );
				}
				case vars::operators::code::loadbin:
					return vars::binaryval::create(
						file( arg1->evaluate( varcache )->targetString( varcache ) ).loadBinary() );
				case vars::operators::code::_delete:
					try
					{
						file( arg1->evaluate( varcache )->targetString( varcache ) ).remove();
					}
					catch( ... )
					{
						return vars::boolval::create( false );
					}
					return vars::boolval::create( true );
			}
			throw vars::UnsupportedOperand( "Unary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " operand" );
		}
		vars::valueptr evaluate::_binary( vars::operators::code op_code,
			vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			switch( op_code )
			{
				case vars::operators::code::_and:
					return vars::boolval::create( arg1->evaluate( varcache )->targetBool( varcache )
						&& arg2->evaluate( varcache )->targetBool( varcache ) );
				case vars::operators::code::_or:
					return vars::boolval::create( arg1->evaluate( varcache )->targetBool( varcache )
						|| arg2->evaluate( varcache )->targetBool( varcache ) );
				case vars::operators::code::call:
				{
					auto val1= arg1->evaluateAccurate( varcache );
					if( !val1 )
						throw vars::NotFound( "Could not find " + arg1->str() );
					vars::function* func{ nullptr };
					auto found = val1->target( varcache, val1 );
					if( found->isMeta() && found->actualMeta().containsUnnamedValue( name_function ) )
					{
						auto& meta = found->actualMeta();
						if( !meta.exists( name_target ) )
							throw vars::BadFunction( "Declared function is missing target!" );
						auto trg = meta.at( name_target );
						if( !trg->isReference() )
						{
							size_t pos{ 0 };
							throw vars::BadFunction( "Target \"" + trg->str( pos, 0 )
								+ "\" of declared function must be a reference!" );
						}
						auto target = trg->actualReference();
						func = varcache.getFunction( target );
						if( !func )
						{
							size_t pos{ 0 };
							throw vars::BadFunction( "Target \"" + trg->str( pos, 0 )
								+ "\" of declared function does not exist!" );
						}
					}
					else if( found->isFunction() )
						func = dynamic_cast<vars::function*>( &*found );
					auto args = arg2->evaluate( varcache )->targetTuple( varcache );
					if( func )
						return func->execute( varcache, args );
					else
						break;
				}
				case vars::operators::code::eq:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) == 0 );
				case vars::operators::code::ne:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) != 0 );
				case vars::operators::code::lt:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) < 0 );
				case vars::operators::code::le:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) <= 0 );
				case vars::operators::code::gt:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) > 0 );
				case vars::operators::code::ge:
					return vars::boolval::create(
						arg1->evaluate( varcache )->targetCompare( arg2->evaluate( varcache ), varcache ) >= 0 );
				case vars::operators::code::plus:
				{
					auto val1 = arg1->evaluate( varcache );
					auto val2 = arg2->evaluate( varcache );
					auto type = val1->targetType( varcache );
					if( val2->targetType( varcache ) != type )
						throw vars::WrongType();
					switch( type )
					{
						case vars::type_code::int_t:
							return vars::intval::create( val1->targetInt( varcache ) + val2->targetInt( varcache ) );
						case vars::type_code::float_t:
							return vars::floatval::create( val1->targetFloat( varcache ) + val2->targetFloat( varcache ) );
						case vars::type_code::string_t:
							return vars::stringval::create(
								val1->targetString( varcache ) + val2->targetString( varcache ) );
						case vars::type_code::binary_t:
							return vars::binaryval::create(
								val1->targetBinary( varcache ) + val2->targetBinary( varcache ) );
						case vars::type_code::raw_t:
						{
							auto a = val1->targetRaw( varcache );
							auto& b = val2->targetRaw( varcache );
							a.insert( a.end(), b.begin(), b.end() );
							return vars::rawval::create( a );
						}
						case vars::type_code::tuple_t:
						{
							auto a = val1->targetTuple( varcache );
							a.append( val2->targetTuple( varcache ), varcache );
							return vars::tupleval::create( std::move( a ) );
						}
						case vars::type_code::meta_t:
						{
							auto a = val1->targetMeta( varcache );
							a.append( val2->targetMeta( varcache ), varcache );
							return vars::metaval::create( std::move( a ) );
						}
						default:
							throw vars::WrongType();
					}
					break;
				}
				case vars::operators::code::minus:
				{
					auto val1 = arg1->evaluate( varcache );
					auto val2 = arg2->evaluate( varcache );
					auto type = val1->targetType( varcache );
					if( val2->targetType( varcache ) != type )
						throw vars::WrongType();
					switch( type )
					{
						case vars::type_code::int_t:
							return vars::intval::create( val1->targetInt( varcache ) - val2->targetInt( varcache ) );
						case vars::type_code::float_t:
							return vars::floatval::create(val1->targetFloat( varcache ) - val2->targetFloat( varcache ) );
						case vars::type_code::string_t:
							return vars::stringval::create(
								val1->targetString( varcache ).remove( val2->targetString( varcache ) ) );
						default:
							throw vars::WrongType();
					}
					break;
				}
				case vars::operators::code::multiply:
				{
					auto val1 = arg1->evaluate( varcache );
					auto val2 = arg2->evaluate( varcache );
					auto type = val1->targetType( varcache );
					if( val2->targetType( varcache ) != type )
						throw vars::WrongType();
					switch( type )
					{
						case vars::type_code::int_t:
							return vars::intval::create( val1->targetInt( varcache ) * val2->targetInt( varcache ) );
						case vars::type_code::float_t:
							return vars::floatval::create( val1->targetFloat( varcache ) * val2->targetFloat( varcache ) );
						default:
							throw vars::WrongType();
					}
					break;
				}
				case vars::operators::code::divide:
				{
					auto val1 = arg1->evaluate( varcache );
					auto val2 = arg2->evaluate( varcache );
					auto type = val1->targetType( varcache );
					if( val2->targetType( varcache ) != type )
						throw vars::WrongType();
					switch( type )
					{
						case vars::type_code::int_t:
						{
							auto divisor = val2->targetInt( varcache );
							if( divisor == 0 )
								throw DivisionByZero();
							return vars::intval::create(val1->targetInt( varcache ) / divisor );
						}
						case vars::type_code::float_t:
						{
							auto divisor = val2->targetFloat( varcache );
							if( divisor == 0.0 )
								throw DivisionByZero();
							return vars::floatval::create( val1->targetFloat( varcache ) / divisor );
						}
						default:
							throw vars::WrongType();
					}
					break;
				}
				case vars::operators::code::pow:
				{
					auto val1 = arg1->evaluate( varcache );
					auto val2 = arg2->evaluate( varcache );
					auto type1 = val1->targetType( varcache );
					auto type2 = val2->targetType( varcache );
					double v1{ 0.0 }, v2{ 0.0 };
					switch( type1 )
					{
						case vars::type_code::int_t:
						{
							v1 = static_cast<double>( val1->targetInt( varcache ) );
							switch( type2 )
							{
								case vars::type_code::int_t:
									v2 = static_cast<double>( val2->targetInt( varcache ) );
									break;
								case vars::type_code::float_t:
									v2 = val2->targetFloat( varcache );
									break;
								default:
									throw vars::WrongType();
							}
							break;
						}
						case vars::type_code::float_t:
						{
							v1 = val1->targetFloat( varcache );
							switch( type2 )
							{
								case vars::type_code::int_t:
									v2 = static_cast<double>( val2->targetInt( varcache ) );
									break;
								case vars::type_code::float_t:
									v2 = val2->targetFloat( varcache );
									break;
								default:
									throw vars::WrongType();
							}
							break;
						}
						default:
							throw vars::WrongType();
					}
					return vars::floatval::create( std::pow( v1, v2 ) );
				}
				case vars::operators::code::mod:
					return vars::intval::create( arg1->evaluate( varcache )->targetInt( varcache )
						% arg2->evaluate( varcache )->targetInt( varcache ) );
				case vars::operators::code::match:
				{
					auto match = arg2->evaluate( varcache )->targetRegex( varcache ).match(
						arg1->evaluate( varcache )->targetString( varcache ) );
					if( match )
					{
						for( auto& capt : match )
							varcache.set( capt.first, vars::stringval::create( capt.second ) );
						return vars::boolval::create( true );
					}
					return vars::boolval::create( false );
				}
				case vars::operators::code::assign:
				{
					auto val1 = arg1->isOperator() ? arg1->evaluate( varcache )
						: dynamic_cast<operandnode*>( &*arg1 )->value();
					auto val2 = arg2->isOperator() ? arg2->evaluate( varcache )
						: dynamic_cast<operandnode*>( &*arg2 )->value();

					if( val1->isVariable() )
					{
						if( val2->isVariable() )
						{
							auto val = varcache.getActual( val2->actualVariable() );
							if( !val )
								throw vars::NotFound( "No such variable: " + *val2->actualVariable() );
							varcache.set( val1->actualVariable(), val );
						}
						else if( val2->isReference() )
						{
							auto val = varcache.getActual( val2->actualReference(),
								dynamic_cast<vars::refval*>( &*val2 )->context() );
							if( !val )
								throw vars::NotFound( "No such reference: " + val2->actualReference().str() );
							varcache.set( val1->actualVariable(), val );
						}
						else
							varcache.set( val1->actualVariable(), val2 );
						return vars::variableval::create( val1->actualVariable() );
					}
					else if( val1->isReference() )
					{
						if( val2->isVariable() )
						{
							auto val = varcache.getActual( val2->actualVariable() );
							if( !val )
								throw vars::NotFound( "No such variable: " + *val2->actualVariable() );
							varcache.set( val1->actualReference(), dynamic_cast<vars::refval*>( &*val1 )->context(), val );
						}
						else if( val2->isReference() )
						{
							auto val = varcache.getActual( val2->actualReference(),
								dynamic_cast<vars::refval*>( &*val2 )->context() );
							if( !val )
								throw vars::NotFound( "No such reference: " + val2->actualReference().str() );
							varcache.set( val1->actualReference(), dynamic_cast<vars::refval*>( &*val1 )->context(), val );
						}
						else
							varcache.set( val1->actualReference(),
								dynamic_cast<vars::refval*>( &*val1 )->context(), val2->target( varcache, val2 ) );
						return vars::refval::create( val1->actualReference(),
							dynamic_cast<vars::refval*>( &*val1 )->context() );
					}
					else
						break;
				}
				case vars::operators::code::resize:
				{
					auto str = arg1->evaluate( varcache )->targetString( varcache );
					auto sz = arg2->evaluate( varcache )->targetInt( varcache );
					if( sz > 0 && str.numChars() > static_cast<size_t>( sz ) )
						str = str.substr( 0, sz );
					else
						str = str.padRight( sz, ' ' );
					return vars::stringval::create( str );
				}
				case vars::operators::code::saveto:
				{
					auto val_obj = arg1->evaluate( varcache );
					auto val = val_obj->target( varcache, val_obj );
					path filename = arg2->evaluate( varcache )->targetString( varcache );
					try
					{
						if( val->isString() )
							file( filename ).save( val->actualString() );
						else if( val->isRaw() )
							file( filename ).save( string( "\n" ).join( val->actualRaw() ) );
						else if( val->isBinary() )
							file( filename ).save( val->actualBinary().binary() );
						else
							return vars::boolval::create( false );
					}
					catch( ... )
					{
						return vars::boolval::create( false );
					}
					return vars::boolval::create( true );
				}
				case vars::operators::code::open_square:
				{
					auto a1 = arg1->evaluate( varcache );
					auto obj = a1->target( varcache, a1 );
					switch( obj->type() )
					{
						case vars::type_code::string_t:
							return vars::charval::create( *( obj->actualString().begin() + static_cast<size_t>(
								arg2->evaluate( varcache )->targetInt( varcache ) ) ) );
						case vars::type_code::raw_t:
							return vars::stringval::create(
								obj->actualRaw().at( arg2->evaluate( varcache )->targetInt( varcache ) ) );
						case vars::type_code::binary_t:
							return vars::intval::create( static_cast<int64_t>(
								obj->actualBinary().byte( arg2->evaluate( varcache )->targetInt( varcache ) ) ) );
						case vars::type_code::tuple_t:
						{
							auto a2 = arg2->evaluate( varcache );
							auto pos = a2->target( varcache, a2 );
							if( pos->isInt() )
								return obj->actualTuple().at( static_cast<size_t>( pos->actualInt() ) );
							else if( pos->isName() )
								return obj->actualTuple().at( pos->actualName() );
						}
					}
					break;
				}
			}
			throw vars::UnsupportedOperand( "Binary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " operand" );
		}
		vars::valueptr evaluate::_ternary( vars::operators::code op_code, vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			switch( op_code )
			{
				case vars::operators::code::if_else:
				{
					auto condition = arg2->evaluate( varcache );
					if( condition && condition->targetBool( varcache ) )
						return arg1->evaluate( varcache );
					else
						return arg3->evaluate( varcache );
				}
			}
			throw vars::UnsupportedOperand( "Ternary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg3->evaluate( varcache )->targetType( varcache ) ) + " operand" );
		}
	}
}
