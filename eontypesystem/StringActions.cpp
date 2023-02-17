#include "StringActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		sig_t StringCopyConstruct::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringByteConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<byte_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringCharConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<char_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringIntConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<int_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringShortConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<short_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringLongConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<long_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringFloatConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<flt_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringLowConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<low_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringHighConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<high_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringIndexConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<index_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringNameConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<name_t>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( eon::str( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}

		sig_t StringBytesConstruct::operator()( ActionExeArgs& args ) const
		{
			auto a1 = args.values().top().value<std::string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( string( a1 ), name_string, type::Qualifier::_none ) );
			return sig_t::_normal;
		}


		sig_t StringCompare::operator()( ActionExeArgs& args ) const
		{
			auto& a2 = args.values().top().value<string>();
			args.values().pop();
			auto& a1 = args.values().top().value<string>();
			args.values().pop();
			args.values().push( Attribute::newExplicit( a1.compare( a2 ), name_int, type::Qualifier::_none ) );
			return sig_t::_normal;
		}


		sig_t StringReset::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<string>();
			a1.clear();
			return sig_t::_normal;
		}

		sig_t StringLength::operator()( ActionExeArgs& args ) const
		{
			auto& a1 = args.values().top().value<string>();
			a1.clear();
			return sig_t::_normal;
		}


		void registerString()
		{
			scope::global().registerType( name_string );
			scope::global().registerAction( new StringCopy() );
			scope::global().registerAction( new StringTake() );
			scope::global().registerAction( new StringConstruct() );
			scope::global().registerAction( new StringCopyConstruct() );
			scope::global().registerAction( new StringByteConstruct() );
			scope::global().registerAction( new StringCharConstruct() );
			scope::global().registerAction( new StringIntConstruct() );
			scope::global().registerAction( new StringShortConstruct() );
			scope::global().registerAction( new StringLongConstruct() );
			scope::global().registerAction( new StringFloatConstruct() );
			scope::global().registerAction( new StringLowConstruct() );
			scope::global().registerAction( new StringHighConstruct() );
			scope::global().registerAction( new StringIndexConstruct() );
			scope::global().registerAction( new StringNameConstruct() );
			scope::global().registerAction( new StringBytesConstruct() );
			scope::global().registerAction( new StringCompare() );
			scope::global().registerAction( new StringReset() );
		}
	}
}
