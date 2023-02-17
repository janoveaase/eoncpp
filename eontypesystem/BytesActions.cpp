#include "BytesActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t BytesCopyConstruct::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<std::string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesByteConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<byte_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::string( 1, a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesCharConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<char_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ).stdstr(), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesIntConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<int_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesShortConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<short_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesLongConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<long_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesFloatConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<flt_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesLowConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<low_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesHighConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<high_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesIndexConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<index_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( std::to_string( a1 ), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesNameConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<name_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( eon::str( a1 ).stdstr(), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t BytesStringConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( a1.stdstr(), name_bytes, type::Qualifier::_none ) );
			return sig_t::_normal;
		}


		sig_t BytesCompare::operator()( ActionExeArgs& args ) const
		{
			auto& a2 = args.values().top().value<std::string>();
			args.values().pop();
			auto& a1 = args.values().top().value<std::string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( a1.compare( a2 ), name_int, type::Qualifier::_none ) );
			return sig_t::_normal;
		}


		sig_t BytesReset::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<std::string>();
			a1.clear();
			return sig_t::_normal;
		}

		sig_t BytesLength::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<std::string>();
			a1.clear();
			return sig_t::_normal;
		}


		void registerBytes()
		{
			scope::global().registerType( name_bytes );
			scope::global().registerAction( new BytesCopy() );
			scope::global().registerAction( new BytesTake() );
			scope::global().registerAction( new BytesConstruct() );
			scope::global().registerAction( new BytesCopyConstruct() );
			scope::global().registerAction( new BytesByteConstruct() );
			scope::global().registerAction( new BytesCharConstruct() );
			scope::global().registerAction( new BytesIntConstruct() );
			scope::global().registerAction( new BytesShortConstruct() );
			scope::global().registerAction( new BytesLongConstruct() );
			scope::global().registerAction( new BytesFloatConstruct() );
			scope::global().registerAction( new BytesLowConstruct() );
			scope::global().registerAction( new BytesHighConstruct() );
			scope::global().registerAction( new BytesIndexConstruct() );
			scope::global().registerAction( new BytesNameConstruct() );
			scope::global().registerAction( new BytesStringConstruct() );
			scope::global().registerAction( new BytesCompare() );
			scope::global().registerAction( new BytesReset() );
		}
	}
}
