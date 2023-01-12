#include "TupleActions.h"
#include "TupleFactory.h"


namespace eon
{
	namespace type
	{
		bool TupleCopy::canExecute( ActionExeArgs& args, source::Reporter& reporter ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			if( !lhs )
			{
				reporter.error( "Must be an existing variable!", args.values().top().source() );
				return false;
			}
			if( lhs->type() != value.type() )
			{
				reporter.error( "Types are not identical for direct assignment!",
					source::Ref(
						args.values().top().source().source(),
						args.values().top().source().start(),
						value.source().end() ) );
				return false;
			}
			lhs->value<Tuple>() = value.value<Tuple>();
			return true;
		}
		sig_t TupleCopy::operator()( ActionExeArgs& args ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			lhs->value<Tuple>() = value.value<Tuple>();
			return sig_t::norm;
		}



		bool TupleMove::canExecute( ActionExeArgs& args, source::Reporter& reporter ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			if( !lhs )
			{
				reporter.error( "Must be an existing variable!", args.values().top().source() );
				return false;
			}
			if( lhs->type() != value.type() )
			{
				reporter.error( "Types are not identical for direct assignment!",
					source::Ref(
						args.values().top().source().source(),
						args.values().top().source().start(),
						value.source().end() ) );
				return false;
			}
			lhs->value<Tuple>() = std::move( value.value<Tuple>() );
			return true;
		}
		sig_t TupleMove::operator()( ActionExeArgs& args ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			lhs->value<Tuple>() = std::move( value.value<Tuple>() );
			return sig_t::norm;
		}



		bool TupleAssign::canExecute( ActionExeArgs& args, source::Reporter& reporter ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			if( !lhs )
			{
				reporter.error( "Must be an existing variable!", args.values().top().source() );
				return false;
			}
			if( !value.type().compatibleWith( lhs->type() ) )
			{
				reporter.error( "Right side of assignment has type '" + value.type().str()
					+ "', which is not compatible with left side type '" + lhs->type().str() + "'!",
					source::Ref(
						args.values().top().source().source(),
						args.values().top().source().start(),
						value.source().end() ) );
				return false;
			}
			lhs->value<Tuple>().copyAttributes( value.value<Tuple>() );
			return true;
		}
		sig_t TupleAssign::operator()( ActionExeArgs& args ) const
		{
			Attribute value = std::move( args.values().top() );
			args.values().pop();
			name_t lhs_name = args.values().top().value<name_t>();
			auto lhs = args.scope().findAttributeIncludeParent( lhs_name );
			lhs->value<Tuple>().copyAttributes( value.value<Tuple>() );
			return sig_t::norm;
		}


		void registerTupleActions()
		{
			scope::global().registerAction( new TupleCopy() );
			scope::global().registerAction( new TupleMove() );
			scope::global().registerAction( new TupleAssign() );
		}
	}
}
