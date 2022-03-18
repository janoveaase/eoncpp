#pragma once
#include <eontokenizer/ReTokenizer.h>
#include <eontokenizer/TokenMatcher.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		class DefineVarRule : public ReTokenizer::RuleDef
		{
		public:
			inline DefineVarRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
	}
}
