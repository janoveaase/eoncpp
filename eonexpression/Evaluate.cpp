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
#include <algorithm>


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
				throw unsupported( op_code, varcache, arg1 );
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
				throw unsupported( op_code, varcache, arg1, arg2 );
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
				throw unsupported( op_code, varcache, arg1, arg2, arg3 );
			}
		}




		vars::valueptr evaluate::_unary( vars::operators::code op_code, vars::variables& varcache, const nodeptr& arg1 )
		{
			switch( op_code )
			{
				case vars::operators::code::_not:
					return vars::boolval::create( !arg1->evaluate( varcache )->targetBool( varcache ) );
				case vars::operators::code::sqrt:
					return _sqrt( varcache, arg1 );
				case vars::operators::code::unary_minus:
					return _unary_minus( varcache, arg1 );
				case vars::operators::code::exists:
					return _exists( varcache, arg1 );
				case vars::operators::code::trim:
					return vars::stringval::create( arg1->evaluate( varcache )->targetString( varcache ).trim() );
				case vars::operators::code::rtrim:
					return vars::stringval::create(
						arg1->evaluate( varcache )->targetString( varcache ).trimTrailing() );
				case vars::operators::code::ltrim:
					return vars::stringval::create( arg1->evaluate( varcache )->targetString( varcache ).trimLeading() );
				case vars::operators::code::len:
					return _len( varcache, arg1 );
				case vars::operators::code::type_of:
					return vars::nameval::create( vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) );
				case vars::operators::code::isname:
					return vars::boolval::create( name::valid( arg1->evaluate( varcache )->targetString( varcache ) ) );
				case vars::operators::code::is_set:
					return _is_set( varcache, arg1 );
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
					return _delete( varcache, arg1 );
				default:
					throw unsupported( op_code, varcache, arg1 );
			}
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
					return _call( varcache, arg1, arg2 );
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
					return _plus( varcache, arg1, arg2 );
				case vars::operators::code::minus:
					return _minus( varcache, arg1, arg2 );
				case vars::operators::code::multiply:
					return _multiply( varcache, arg1, arg2 );
				case vars::operators::code::divide:
					return _divide( varcache, arg1, arg2 );
				case vars::operators::code::pow:
					return _pow( varcache, arg1, arg2 );
				case vars::operators::code::mod:
					return vars::intval::create( arg1->evaluate( varcache )->targetInt( varcache )
						% arg2->evaluate( varcache )->targetInt( varcache ) );
				case vars::operators::code::match:
					return _match( varcache, arg1, arg2 );
				case vars::operators::code::assign:
					return _assign( varcache, arg1, arg2 );
				case vars::operators::code::resize:
					return _resize( varcache, arg1, arg2 );
				case vars::operators::code::saveto:
					return _saveto( varcache, arg1, arg2 );
				case vars::operators::code::element:
					return _element( varcache, arg1, arg2 );
				case vars::operators::code::in:
					return _in( varcache, arg1, arg2 );
				default:
					throw unsupported( op_code, varcache, arg1, arg2 );
			}
		}
		vars::valueptr evaluate::_ternary( vars::operators::code op_code, vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			switch( op_code )
			{
				case vars::operators::code::if_else:
					return _if_else( varcache, arg1, arg2, arg3 );
				case vars::operators::code::slice:
					return _slice( varcache, arg1, arg2, arg3 );
				default:
					throw unsupported( op_code, varcache, arg1, arg2, arg3 );
			}			
		}


		vars::valueptr evaluate::_sqrt( vars::variables& varcache, const nodeptr& arg1 )
		{
			auto raw1 = arg1->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			auto val1 = raw1->target( varcache, raw1 );
			switch( val1->type() )
			{
				case vars::type_code::int_t:
					return vars::intval::create( static_cast<int64_t>( std::sqrt( val1->actualInt() ) ) );
				case vars::type_code::float_t:
					return vars::floatval::create( std::sqrt( val1->actualFloat() ) );
			}
			throw unsupported( vars::operators::code::sqrt, val1 );
		}
		vars::valueptr evaluate::_unary_minus( vars::variables& varcache, const nodeptr& arg1 )
		{
			auto raw1 = arg1->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			auto val1 = raw1->target( varcache, raw1 );
			switch( val1->type() )
			{
				case vars::type_code::int_t:
					return vars::intval::create( -val1->actualInt() );
				case vars::type_code::float_t:
					return vars::floatval::create( -val1->actualFloat() );
			}
			throw unsupported( vars::operators::code::unary_minus, val1 );
		}
		vars::valueptr evaluate::_exists( vars::variables& varcache, const nodeptr& arg1 )
		{
			return vars::boolval::create( path( arg1->evaluate( varcache )->targetString( varcache ) ).exists() );
		}
		vars::valueptr evaluate::_len( vars::variables& varcache, const nodeptr& arg1 )
		{
			auto raw1 = arg1->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			auto val1 = raw1->target( varcache, raw1 );
			switch( val1->type() )
			{
				case vars::type_code::string_t:
					return vars::intval::create(
						static_cast<int64_t>( val1->actualString().numChars() ) );
				case vars::type_code::raw_t:
					return vars::intval::create(
						static_cast<int64_t>( val1->actualRaw().size() ) );
				case vars::type_code::binary_t:
					return vars::intval::create(
						static_cast<int64_t>( val1->actualBinary().size() / 2 ) );
				case vars::type_code::tuple_t:
					return vars::intval::create(
						static_cast<int64_t>( val1->actualTuple().numAttributes() ) );
				case vars::type_code::meta_t:
					return vars::intval::create(
						static_cast<int64_t>( val1->actualMeta().numAttributes() ) );
			}
			throw unsupported( vars::operators::code::len, val1 );
		}
		vars::valueptr evaluate::_is_set( vars::variables& varcache, const nodeptr& arg1 )
		{
			if( !arg1->isOperator() )
			{
				auto val = dynamic_cast<operandnode*>( &*arg1 )->value();
				if( val->isVariable() )
					return vars::boolval::create( static_cast<bool>( varcache.get( val->actualVariable() ) ) );
			}
			throw unsupported( vars::operators::code::is_set, varcache, arg1 );
		}
		vars::valueptr evaluate::_delete( vars::variables& varcache, const nodeptr& arg1 )
		{
			auto raw1 = arg1->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			auto val1 = raw1->target( varcache, raw1 );
			if( !val1->isString() )
				throw unsupported( vars::operators::code::_delete, varcache, arg1 );
			bool success = false;
			string error;
			try
			{
				file( arg1->evaluate( varcache )->targetString( varcache ) ).remove();
				success = true;
			}
			catch( filesys::Failure& e )
			{
				error = e.details();
			}
			auto result = vars::tupleval::create();
			auto& res = result->tuple_value();
			res.addReplace( vars::boolval::create( success ), varcache, name_success );
			res.addReplace( vars::stringval::create( error ), varcache, name_error );
			return result;
		}

		vars::valueptr evaluate::_call( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			auto val1 = raw1->target( varcache, raw1 );

			vars::function* func{ nullptr };
			if( val1->isMeta() && val1->actualMeta().containsUnnamedValue( name_function ) )
			{
				auto& meta = val1->actualMeta();
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
			else if( val1->isFunction() )
				func = dynamic_cast<vars::function*>( &*val1 );
			else
				throw unsupported( vars::operators::code::call, varcache, arg1, arg2 );
			auto args = arg2->evaluate( varcache )->targetTuple( varcache );
			if( func )
				return func->execute( varcache, args );
			else
				throw vars::WrongType();		// Not possible!
		}
		vars::valueptr evaluate::_plus( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type1 != type2 )
				throw unsupported( vars::operators::code::plus, val1, val2 );
			switch( type1 )
			{
				case vars::type_code::int_t:
					return vars::intval::create( val1->actualInt() + val2->actualInt() );
				case vars::type_code::float_t:
					return vars::floatval::create( val1->actualFloat() + val2->actualFloat() );
				case vars::type_code::string_t:
					return vars::stringval::create(
						val1->actualString() + val2->actualString() );
				case vars::type_code::binary_t:
					return vars::binaryval::create(
						val1->actualBinary() + val2->actualBinary() );
				case vars::type_code::raw_t:
				{
					auto a = val1->actualRaw();
					auto& b = val2->actualRaw();
					a.insert( a.end(), b.begin(), b.end() );
					return vars::rawval::create( a );
				}
				case vars::type_code::tuple_t:
				{
					auto a = val1->actualTuple();
					a.append( val2->actualTuple(), varcache );
					return vars::tupleval::create( std::move( a ) );
				}
				case vars::type_code::meta_t:
				{
					auto a = val1->actualMeta();
					a.append( val2->actualMeta(), varcache );
					return vars::metaval::create( std::move( a ) );
				}
				default:
					throw unsupported( vars::operators::code::plus, val1, val2 );
			}
		}
		vars::valueptr evaluate::_minus( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type1 != type2 )
				throw unsupported( vars::operators::code::minus, val1, val2 );
			switch( type1 )
			{
				case vars::type_code::int_t:
					return vars::intval::create( val1->actualInt() - val2->actualInt() );
				case vars::type_code::float_t:
					return vars::floatval::create( val1->actualFloat() - val2->actualFloat() );
				case vars::type_code::string_t:
					return vars::stringval::create(
						val1->actualString().remove( val2->actualString() ) );
				default:
					throw unsupported( vars::operators::code::minus, val1, val2 );
			}
		}
		vars::valueptr evaluate::_multiply( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();

			if( type1 == vars::type_code::int_t && type2 == type1 )
				return vars::intval::create( val1->actualInt() * val2->actualInt() );
			else if( type1 == vars::type_code::float_t && type2 == type1 )
				return vars::floatval::create( val1->actualFloat() * val2->actualFloat() );
			else if( type1 == vars::type_code::string_t && type2 == vars::type_code::int_t )
				return vars::stringval::create( string( static_cast<size_t>( val2->actualInt() ), val1->actualString() ) );
			else
				throw unsupported( vars::operators::code::multiply, val1, val2 );
		}
		vars::valueptr evaluate::_divide( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type1 != type2 )
				throw unsupported( vars::operators::code::divide, val1, val2 );
			switch( type1 )
			{
				case vars::type_code::int_t:
				{
					auto divisor = val2->actualInt();
					if( divisor == 0 )
						throw DivisionByZero();
					return vars::intval::create( val1->actualInt() / divisor );
				}
				case vars::type_code::float_t:
				{
					auto divisor = val2->actualFloat();
					if( divisor == 0.0 )
						throw DivisionByZero();
					return vars::floatval::create( val1->actualFloat() / divisor );
				}
				default:
					throw unsupported( vars::operators::code::divide, val1, val2 );
			}
		}
		vars::valueptr evaluate::_pow( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type1 != type2 )
				throw unsupported( vars::operators::code::pow, val1, val2 );
			switch( type1 )
			{
				case vars::type_code::int_t:
					return vars::intval::create( static_cast<int64_t>( std::pow( val1->actualInt(), val2->actualInt() ) ) );
				case vars::type_code::float_t:
					return vars::floatval::create( std::pow( val1->actualFloat(), val2->actualFloat() ) );
				default:
					throw unsupported( vars::operators::code::pow, varcache, arg1, arg2 );
			}
		}
		vars::valueptr evaluate::_match( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type2 != vars::type_code::regex_t )
				throw unsupported( vars::operators::code::match, val1, val2 );
			rx::match match;
			switch( type1 )
			{
				case vars::type_code::string_t:
					match = val2->actualRegex().match( val1->actualString() );
					break;
				case vars::type_code::raw_t:
					match = val2->actualRegex().match( string( "\n" ).join( val1->actualRaw() ) );
					break;
				default:
					throw unsupported( vars::operators::code::match, varcache, arg1, arg2 );
			}
			if( match )
			{
				for( auto& capt : match )
					varcache.set( capt.first, vars::stringval::create( capt.second ) );
				return vars::boolval::create( true );
			}
			return vars::boolval::create( false );
		}
		vars::valueptr evaluate::_assign( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto val1 = arg1->isOperator() ? arg1->evaluate( varcache ) : dynamic_cast<operandnode*>( &*arg1 )->value();
			auto val2 = arg2->isOperator() ? arg2->evaluate( varcache ) : dynamic_cast<operandnode*>( &*arg2 )->value();

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
				throw unsupported( vars::operators::code::assign, val1, val2 );
		}
		vars::valueptr evaluate::_resize( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type2 != vars::type_code::int_t )
				throw unsupported( vars::operators::code::resize, val1, val2 );
			auto new_size = val2->actualInt();

			switch( type1 )
			{
				case vars::type_code::string_t:
					if( new_size >= 0 )
					{
						if( static_cast<size_t>( new_size ) > val1->actualString().numChars() )
							val1->string_value() = val1->actualString().padRight( new_size, ' ' );
						else
							val1->string_value() = val1->actualString().substr( 0, new_size );
					}
					return val1;
				case vars::type_code::raw_t:
					if( new_size >= 0 )
						val1->raw_value().resize( new_size );
					return val1;
				default:
					throw unsupported( vars::operators::code::resize, val1, val2 );
			}
		}
		vars::valueptr evaluate::_saveto( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type2 != vars::type_code::string_t )
				throw unsupported( vars::operators::code::saveto, val1, val2 );
			bool success = false;
			string error;
			try
			{
				switch( type1 )
				{
					case vars::type_code::string_t:
						file( path( val2->actualString() ) ).save( val1->actualString() );
						break;
					case vars::type_code::raw_t:
						file( path( val2->actualString() ) ).save( string( "\n" ).join( val1->actualRaw() ) );
						break;
					case vars::type_code::binary_t:
						file( path( val2->actualString() ) ).save( val2->actualBinary().binary() );
						break;
					default:
						throw unsupported( vars::operators::code::saveto, val1, val2 );
				}
				success = true;
			}
			catch( filesys::OpenError& e )
			{
				error = e.details();
			}
			catch( filesys::RWError& e )
			{
				error = e.details();
			}
			auto result = vars::tupleval::create();
			auto& res = result->tuple_value();
			res.addReplace( vars::boolval::create( success ), varcache, name_success );
			res.addReplace( vars::stringval::create( error ), varcache, name_error );
			return result;
		}
		vars::valueptr evaluate::_element( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			if( type2 != vars::type_code::int_t
				&& ( type1 != vars::type_code::tuple_t || type2 != vars::type_code::name_t ) )
				throw unsupported( vars::operators::code::open_square, val1, val2 );
			switch( type1 )
			{
				case vars::type_code::string_t:
					return vars::charval::create( *val1->actualString().slice(
						val2->actualInt(), val2->actualInt() ).begin() );
				case vars::type_code::raw_t:
				{
					auto pos = val2->actualInt();
					auto p = static_cast<size_t>( pos < 0 ? -pos : pos );
					if( pos < 0 )
						return vars::stringval::create( val1->actualRaw().at(
							p >= val1->actualRaw().size() ? 0 : val1->actualRaw().size() - p ) );
					else
						return vars::stringval::create( val1->actualRaw().at(
							p < val1->actualRaw().size() ? p : val1->actualRaw().size() - 1 ) );
				}
				case vars::type_code::binary_t:
				{
					auto pos = val2->actualInt();
					auto p = static_cast<size_t>( pos < 0 ? -pos : pos );
					auto size = val1->actualBinary().size() / 2;
					if( pos < 0 )
						return vars::intval::create( static_cast<int64_t>(
							val1->actualBinary().byte( p >= size ? 0 : val1->actualBinary().byte( size - p ) ) ) );
					else
						return vars::intval::create( static_cast<int64_t>(
							val1->actualBinary().byte( p < size ? p : size - 1 ) ) );
				}
				case vars::type_code::tuple_t:
					switch( type2 )
					{
						case vars::type_code::int_t:
						{
							auto pos = val2->actualInt();
							auto p = static_cast<size_t>( pos < 0 ? -pos : pos );
							if( pos < 0 )
								return val1->actualTuple().at(
									p >= val1->actualTuple().numAttributes() ? 0 : val1->actualTuple().numAttributes() - p );
							else
								return val1->actualTuple().at(
									p < val1->actualTuple().numAttributes() ? p : val1->actualTuple().numAttributes() - 1 );
						}
						case vars::type_code::name_t:
							return val1->actualTuple().at( val2->actualName() );
						default:
							; // Not possible!
					}
				default:
					throw unsupported( vars::operators::code::open_square, val1, val2 );
			}
		}
		vars::valueptr evaluate::_in( vars::variables& varcache, const nodeptr& arg1, const nodeptr& arg2 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			switch( type2 )
			{
				case vars::type_code::string_t:
					switch( type1 )
					{
						case vars::type_code::string_t:
							return vars::boolval::create( val2->actualString().contains( val1->actualString() ) );
						case vars::type_code::char_t:
							return vars::boolval::create( val2->actualString().contains( val1->actualChar() ) );
					}
					break;
				case vars::type_code::raw_t:
					switch( type1 )
					{
						case vars::type_code::string_t:
							return vars::boolval::create( std::find( val2->actualRaw().begin(), val2->actualRaw().end(),
								val1->actualString() ) != val2->actualRaw().end() );
					}
					break;
				case vars::type_code::tuple_t:
					switch( type1 )
					{
						case vars::type_code::name_t:
							return vars::boolval::create( val2->actualTuple().exists( val1->actualName() ) );
					}
					break;
			}
			throw unsupported( vars::operators::code::in, val1, val2 );
		}

		vars::valueptr evaluate::_if_else( vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			auto condition = arg2->evaluate( varcache );
			if( condition && condition->targetBool( varcache ) )
				return arg1->evaluate( varcache );
			else
				return arg3->evaluate( varcache );
		}
		vars::valueptr evaluate::_slice( vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			auto raw1 = arg1->evaluate( varcache ), raw2 = arg2->evaluate( varcache ), raw3 = arg3->evaluate( varcache );
			if( !raw1 )
				throw vars::NotFound( "Could not find " + arg1->str() );
			if( !raw2 )
				throw vars::NotFound( "Could not find " + arg2->str() );
			if( !raw3 )
				throw vars::NotFound( "Could not find " + arg3->str() );
			auto val1 = raw1->target( varcache, raw1 );
			auto val2 = raw2->target( varcache, raw2 );
			auto val3 = raw2->target( varcache, raw3 );

			auto type1 = val1->type();
			auto type2 = val2->type();
			auto type3 = val3->type();
			if( type2 != type3 || type2 != vars::type_code::int_t )
				throw unsupported( vars::operators::code::slice, val1, val2, val3 );
			switch( type1 )
			{
				case vars::type_code::string_t:
					return vars::stringval::create( val1->actualString().slice( val2->actualInt(), val3->actualInt() ) );
				case vars::type_code::raw_t:
					return vars::rawval::create( std::vector<string>(
						val1->actualRaw().begin() + static_cast<size_t>( val2->actualInt() ),
						val1->actualRaw().begin() + static_cast<size_t>( val3->actualInt() ) + 1 ) );
				default:
					throw unsupported( vars::operators::code::slice, val1, val2, val3 );
			}
		}

		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code,
			vars::variables& varcache, const nodeptr& arg1 )
		{
			return vars::UnsupportedOperand( "Unary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " operand!" );
		}
		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code, vars::valueptr arg1 )
		{
			return vars::UnsupportedOperand( "Unary operator '" + vars::operators::mapCode( op_code )
				+ "' not supported for " + *vars::mapTypeCode( arg1->type() ) + " operand!" );
		}
		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code, vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2 )
		{
			return vars::UnsupportedOperand( "Binary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " operands!" );
		}
		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code,
			vars::valueptr arg1, vars::valueptr arg2 )
		{
			return vars::UnsupportedOperand( "Unary operator '" + vars::operators::mapCode( op_code )
				+ "' not supported for " + *vars::mapTypeCode( arg1->type() ) + " vs. "
				+ *vars::mapTypeCode( arg2->type() ) + " operands!" );
		}
		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code, vars::variables& varcache,
			const nodeptr& arg1, const nodeptr& arg2, const nodeptr& arg3 )
		{
			return vars::UnsupportedOperand( "Ternary operator '"
				+ vars::operators::mapCode( op_code ) + "' not supported for "
				+ *vars::mapTypeCode( arg2->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg1->evaluate( varcache )->targetType( varcache ) ) + " vs. "
				+ *vars::mapTypeCode( arg3->evaluate( varcache )->targetType( varcache ) ) + " operands!" );
		}
		vars::UnsupportedOperand evaluate::unsupported( vars::operators::code op_code,
			vars::valueptr arg1, vars::valueptr arg2, vars::valueptr arg3 )
		{
			return vars::UnsupportedOperand( "Unary operator '" + vars::operators::mapCode( op_code )
				+ "' not supported for " + *vars::mapTypeCode( arg1->type() ) + " vs. "
				+ *vars::mapTypeCode( arg2->type() ) + " vs. " + *vars::mapTypeCode( arg3->type() ) + " operands!" );
		}
	}
}
