#pragma once

#include <eontest/Test.h>
#include <eonexpression/Expression.h>


namespace eon
{
	class ExpressionTest : public eontest::EonTest {};
	class ReferenceTest : public eontest::EonTest, public vars::refsource
	{
	public:
		std::map<nameref, vars::valueptr> Vars;
		inline vars::valueptr _get( const nameref& ref, vars::valueptr context ) const noexcept override {
			auto found = Vars.find( ref ); return found != Vars.end() ? found->second : vars::valueptr(); }
		inline void _set( const nameref& ref, vars::valueptr context, vars::valueptr value ) override {
			Vars[ ref ] = value; }
	};
}
