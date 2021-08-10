#pragma once

#include <eontest/Test.h>
#include <eonof/Eof.h>


namespace eon
{
	class EtfTest : public eontest::EonTest {};

	class BoolTest : public eontest::EonTest {};
	class CharTest : public eontest::EonTest {};
	class IntTest : public eontest::EonTest {};
	class FloatTest : public eontest::EonTest {};
	class NameTest : public eontest::EonTest {};
	class StringTest : public eontest::EonTest {};
	class RawTest : public eontest::EonTest {};
	class BinaryTest : public eontest::EonTest {};
	class RegexTest : public eontest::EonTest {};
	class RefTest : public eontest::EonTest {};
	class VarTest : public eontest::EonTest {};
	class ExprTest : public eontest::EonTest {};

	class TupleTest : public eontest::EonTest {};
	class MetaTest : public eontest::EonTest {};
	class FunctionTest : public eontest::EonTest {};

	class ValidateTest : public eontest::EonTest {};



	class addInt : public vars::function
	{
	public:
		tuple Args{
			{ name::get( "a" ), vars::nameval::create( name_int ) },
			{ name::get( "b" ), vars::nameval::create( name_int ) } };

		addInt() = default;

		inline const tuple* args() const noexcept override { return &Args; }

		vars::valueptr execute( vars::variables& varcache, const tuple& args ) const override
		{
			auto a = args.at( name::get( "a" ) );
			auto b = args.at( name::get( "b" ) );
			if( a && b && a->isInt() && b->isInt() )
				return vars::intval::create( a->actualInt() + b->actualInt() );
			throw vars::WrongType();
		}
	};
}
