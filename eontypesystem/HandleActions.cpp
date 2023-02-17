#include "HandleActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t HandleCompare::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<handle_t>(); args.values().pop();
			auto a1 = args.values().top().value<handle_t>(); args.values().pop();
			args.values().push( Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::_none ) );
			return sig_t::_normal;
		}


		void registerHandle()
		{
			scope::global().registerType( name_handle );
			scope::global().registerAction( new HandleConstruct() );
			scope::global().registerAction( new HandleCopyConstruct() );
			scope::global().registerAction( new HandleCompare() );
		}
	}
}
