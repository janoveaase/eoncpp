#include "ToolBox.h"
#include "BoolLiteralRule.h"
#include "NamePathRule.h"


namespace eon
{
	namespace parser
	{
		ToolBox& ToolBox::operator=( ToolBox&& other ) noexcept
		{
			Reporter = other.Reporter; other.Reporter = nullptr;
			Elements = std::move( other.Elements );
			CurElm = other.CurElm; other.CurElm = 0;

			ContextStack = std::move( other.ContextStack );
			IndentStack = std::move( other.IndentStack );

			OpStack = std::move( other.OpStack );
			TreeStack = std::move( other.TreeStack );

			Scope = other.Scope; other.Scope = nullptr;
			return *this;
		}




		ToolBox ToolBox::split()
		{
			ToolBox sub;
			sub.Reporter = Reporter;
			sub.Elements = std::move( Elements );
			sub.CurElm = CurElm;
			sub.Scope = Scope;
			return sub;
		}
		void ToolBox::sync( ToolBox&& other ) noexcept
		{
			Elements = std::move( other.Elements );
			CurElm = other.CurElm;
			other = ToolBox();
		}




		void ToolBox::reset() noexcept
		{
			while( !OpStack.empty() )
				OpStack.pop();
			while( TreeStack.empty() )
				TreeStack.pop();
		}
	}
}
