#pragma once
#include <eontokenizer/ReTokenizer.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		class ShortRule : public ReTokenizer::RuleDef
		{
		public:
			inline ShortRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
		class LongRule : public ReTokenizer::RuleDef
		{
		public:
			inline LongRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};

		class LowRule : public ReTokenizer::RuleDef
		{
		public:
			inline LowRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
		class HighRule : public ReTokenizer::RuleDef
		{
		public:
			inline HighRule( name_t name ) : ReTokenizer::RuleDef( name ) {}
			bool match( TokenParser& parser, std::vector<Token>& output ) const noexcept override;
		};
	}
}
