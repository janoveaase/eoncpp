#pragma once
#include <eontokenizer/ReTokenizer.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		class NamePathRule : public ReTokenizer::RuleDef
		{
		public:
			inline NamePathRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
	}
}
