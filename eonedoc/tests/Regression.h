#pragma once

#include <eontest/Test.h>
#include <eonsource/String.h>
#include <eonfilesys/FileSys.h>
#include <eontypes/Primitives.h>
#include <eontypes/BasicTupleActions.h>
#include <eontypes/StandardTypes.h>
#include <eonedoc/Edoc.h>
#include <eonedoc/ToHtml.h>

namespace eon
{
	class EdocTest : public eontest::EonTest
	{
	public:

		void prepare() override
		{
			type::Handler::init();
		}
	};
	class ToHtmlTest : public eontest::EonTest
	{
	public:

		void prepare() override
		{
			type::Handler::init();
		}
	};
	class ToHtmlFileTest : public eontest::EonTestSandbox
	{
	public:
		ToHtmlFileTest() : eontest::EonTestSandbox( "edoc", "tohtml" ) {}

		void prepare() override
		{
			type::Handler::init();
		}
	};
}
