#pragma once
#include <eontokenizer/ReTokenizer.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		class PercentOfRule : public ReTokenizer::RuleDef
		{
		public:
			inline PercentOfRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
		class AsPercentOfRule : public ReTokenizer::RuleDef
		{
		public:
			inline AsPercentOfRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
	}
}
