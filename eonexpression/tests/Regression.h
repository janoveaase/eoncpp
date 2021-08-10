#pragma once

#include <eontest/Test.h>
#include <eonexpression/Expression.h>


namespace eon
{
	class ExpressionTest : public eontest::EonTest {};
	class ReferenceTest : public eontest::EonTest, public vars::source
	{
	public:
		std::map<nameref, vars::valueptr> Vars;
		inline vars::valueptr _get( const nameref& ref ) const noexcept override
		{
			auto found = Vars.find( ref );
			return found != Vars.end() ? found->second : vars::valueptr();
		}
		inline void _set( const nameref& ref, const vars::valueptr& value ) override {
			Vars[ ref ] = value; }
	};
}
