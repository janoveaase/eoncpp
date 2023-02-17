#include "CharActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t CharCompare::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<char_t>(); args.values().pop();
			auto a1 = args.values().top().value<char_t>(); args.values().pop();
			args.values().push(
				Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::_none ) );
			return sig_t::_normal;
		}


		void registerChar()
		{
			scope::global().registerType( name_char );
			scope::global().registerAction( new CharCopy() );
			scope::global().registerAction( new CharTake() );
			scope::global().registerAction( new CharConstruct() );
			scope::global().registerAction( new CharCopyConstruct() );
			scope::global().registerAction( new CharBoolConstruct() );
			scope::global().registerAction( new CharByteConstruct() );
			scope::global().registerAction( new CharIntConstruct() );
			scope::global().registerAction( new CharShortConstruct() );
			scope::global().registerAction( new CharLongConstruct() );
			scope::global().registerAction( new CharFloatConstruct() );
			scope::global().registerAction( new CharLowConstruct() );
			scope::global().registerAction( new CharHighConstruct() );
			scope::global().registerAction( new CharIndexConstruct() );
			scope::global().registerAction( new CharCompare() );
			scope::global().registerAction( new CharReset() );
		}
	}
}
