#include "Bool.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t BoolCompare::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<bool>(); args.values().pop();
			auto a1 = args.values().top().value<bool>(); args.values().pop();
			args.values().push(
				Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::none ) );
			return sig_t::norm;
		}

		sig_t BoolAnd::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<bool>(); args.values().pop();
			auto a1 = args.values().top().value<bool>(); args.values().pop();
			args.values().push( Attribute::newImplicit( a1 && a2, Qualifier::none ) );
			return sig_t::norm;
		}

		sig_t BoolOr::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<bool>(); args.values().pop();
			auto a1 = args.values().top().value<bool>(); args.values().pop();
			args.values().push( Attribute::newImplicit( a1 || a2, Qualifier::none ) );
			return sig_t::norm;
		}

		sig_t BoolXor::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<bool>(); args.values().pop();
			auto a1 = args.values().top().value<bool>(); args.values().pop();
			args.values().push( Attribute::newImplicit( a1 != a2, Qualifier::none ) );
			return sig_t::norm;
		}


		void registerBool()
		{
			scope::global().registerType( name_bool );
			scope::global().registerAction( new BoolCopy() );
			scope::global().registerAction( new BoolTake() );
			scope::global().registerAction( new BoolConstruct() );
			scope::global().registerAction( new BoolCopyConstruct() );
			scope::global().registerAction( new BoolByteConstruct() );
			scope::global().registerAction( new BoolCharConstruct() );
			scope::global().registerAction( new BoolIntConstruct() );
			scope::global().registerAction( new BoolShortConstruct() );
			scope::global().registerAction( new BoolLongConstruct() );
			scope::global().registerAction( new BoolFloatConstruct() );
			scope::global().registerAction( new BoolLowConstruct() );
			scope::global().registerAction( new BoolHighConstruct() );
			scope::global().registerAction( new BoolIndexConstruct() );
			scope::global().registerAction( new BoolCompare() );
			scope::global().registerAction( new BoolNot() );
			scope::global().registerAction( new BoolAnd() );
			scope::global().registerAction( new BoolOr() );
			scope::global().registerAction( new BoolXor() );
			scope::global().registerAction( new BoolReset() );
		}
	}
}
