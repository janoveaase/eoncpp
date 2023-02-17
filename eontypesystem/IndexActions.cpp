#include "IndexActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t IndexCompare::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto a1 = args.values().top().value<index_t>(); args.values().pop();
			args.values().push(
				Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::_none ) );
			return sig_t::_normal;
		}


		sig_t IndexPlusAssign::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto& a1 = args.values().top();
			a1.value<index_t>() += a2;
			return sig_t::_normal;
		}

		sig_t IndexMinusAssign::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto& a1 = args.values().top();
			a1.value<index_t>() -= a2;
			return sig_t::_normal;
		}

		sig_t IndexMultiplyAssign::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto& a1 = args.values().top();
			a1.value<index_t>() *= a2;
			return sig_t::_normal;
		}

		sig_t IndexDivideAssign::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			if( a2 == 0 )
			{
				args.values().push( Attribute::newImplicit( name( "division_by_zero" ), Qualifier::_none ) );
				return sig_t::_raise;
			}
			auto& a1 = args.values().top();
			a1.value<index_t>() /= a2;
			return sig_t::_normal;
		}

		sig_t IndexPlus::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto a1 = args.values().top().value<index_t>();
			args.values().push( Attribute::newImplicit( static_cast<int_t>( a1 + a2 ), Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t IndexMinus::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto a1 = args.values().top().value<index_t>();
			args.values().push( Attribute::newImplicit( static_cast<int_t>( a1 - a2 ), Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t IndexMultiply::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			auto a1 = args.values().top().value<index_t>();
			args.values().push( Attribute::newImplicit( static_cast<int_t>( a1 * a2 ), Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t IndexDivide::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<index_t>(); args.values().pop();
			if( a2 == 0 )
			{
				args.values().push( Attribute::newImplicit( name( "division_by_zero" ), Qualifier::_none ) );
				return sig_t::_raise;
			}
			auto a1 = args.values().top().value<index_t>();
			args.values().push( Attribute::newImplicit( static_cast<int_t>( a1 / a2 ), Qualifier::_none ) );
			return sig_t::_normal;
		}


		void registerIndex()
		{
			scope::global().registerType( name_index );
			scope::global().registerAction( new IndexCopy() );
			scope::global().registerAction( new IndexTake() );
			scope::global().registerAction( new IndexConstruct() );
			scope::global().registerAction( new IndexCopyConstruct() );
			scope::global().registerAction( new IndexBoolConstruct() );
			scope::global().registerAction( new IndexByteConstruct() );
			scope::global().registerAction( new IndexCharConstruct() );
			scope::global().registerAction( new IndexIntConstruct() );
			scope::global().registerAction( new IndexShortConstruct() );
			scope::global().registerAction( new IndexLongConstruct() );
			scope::global().registerAction( new IndexFloatConstruct() );
			scope::global().registerAction( new IndexLowConstruct() );
			scope::global().registerAction( new IndexHighConstruct() );
			scope::global().registerAction( new IndexB8Construct() );
			scope::global().registerAction( new IndexB16Construct() );
			scope::global().registerAction( new IndexB32Construct() );
			scope::global().registerAction( new IndexB64Construct() );
			scope::global().registerAction( new IndexCompare() );
			scope::global().registerAction( new IndexPlusAssign() );
			scope::global().registerAction( new IndexMinusAssign() );
			scope::global().registerAction( new IndexMultiplyAssign() );
			scope::global().registerAction( new IndexDivideAssign() );
			scope::global().registerAction( new IndexPlus() );
			scope::global().registerAction( new IndexMinus() );
			scope::global().registerAction( new IndexMultiply() );
			scope::global().registerAction( new IndexDivide() );
			scope::global().registerAction( new IndexReset() );
		}
	}
}
