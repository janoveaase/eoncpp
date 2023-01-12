#include "Byte.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t ByteCompare::operator()( ActionExeArgs& args ) const
		{
			auto a2 = args.values().top().value<byte_t>(); args.values().pop();
			auto a1 = args.values().top().value<byte_t>(); args.values().pop();
			args.values().push(
				Attribute::newImplicit( static_cast<int_t>( a1 < a2 ? -1 : a2 < a1 ? 1 : 0 ), Qualifier::none ) );
			return sig_t::norm;
		}


		void registerByte()
		{
			scope::global().registerType( name_byte );
			scope::global().registerAction( new ByteCopy() );
			scope::global().registerAction( new ByteTake() );
			scope::global().registerAction( new ByteConstruct() );
			scope::global().registerAction( new ByteCopyConstruct() );
			scope::global().registerAction( new ByteBoolConstruct() );
			scope::global().registerAction( new ByteCharConstruct() );
			scope::global().registerAction( new ByteIntConstruct() );
			scope::global().registerAction( new ByteShortConstruct() );
			scope::global().registerAction( new ByteLongConstruct() );
			scope::global().registerAction( new ByteFloatConstruct() );
			scope::global().registerAction( new ByteLowConstruct() );
			scope::global().registerAction( new ByteHighConstruct() );
			scope::global().registerAction( new ByteIndexConstruct() );
			scope::global().registerAction( new ByteCompare() );
			scope::global().registerAction( new ByteReset() );
		}
	}
}
