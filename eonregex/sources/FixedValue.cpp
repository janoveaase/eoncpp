#include "FixedValue.h"


namespace eon
{
	namespace rx
	{
		bool FixedValue::_match( RxData& data, size_t steps )
		{
			RxData param_b{ data };

			for( auto c : Value )
			{
				if( param_b() == c )
					param_b.advance();
				else
					return false;
			}
			data = std::move( param_b );
			return true;
		}

		string FixedValue::_strStruct() const
		{
			static const string special{ R"(|!{}?*+()^$[]\".@)" };
			string s;
			for( auto c : Value )
			{
				if( special.contains( c ) )
					s << "\\" << c;
				else
					s << c;
			}
			return s;
		}

		Node* FixedValue::_exposeLiterals()
		{
			if( Quant.minQ() == 1 && Quant.maxQ() == INDEX_MAX )
			{
				auto exposed = copy();
				exposed->resetQuantifier();
				Quant = Quantifier( 0, INDEX_MAX, Quant.greedy() );
				exposed->Next = this;
				return exposed;
			}
			else
				return this;
		}
	}
}
