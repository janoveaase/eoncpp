#include "NameActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t NameCompare::operator()( ActionExeArgs& args ) const
		{
			auto& a2 = args.values().top().value<name_t>(); args.values().pop();
			auto& a1 = args.values().top().value<name_t>(); args.values().pop();
			args.values().push(
				Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::_none ) );
			return sig_t::_normal;
		}


		void registerName()
		{
			scope::global().registerType( name_name );
			scope::global().registerAction( new NameCopy() );
			scope::global().registerAction( new NameTake() );
			scope::global().registerAction( new NameConstruct() );
			scope::global().registerAction( new NameCopyConstruct() );
			scope::global().registerAction( new NameCompare() );
			scope::global().registerAction( new NameReset() );
		}
	}
}
