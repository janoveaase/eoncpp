#pragma once
#include "ToolBox.h"
#include <eontypes/BasicTuple.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		/**************************************************************************
		  Eon ParseExpression Class - eon::ParseExpression

		  Parse an Æon Expression.
		**************************************************************************/
		class ParseExpression
		{
			/**********************************************************************
			  Construction
			**********************************************************************/
		public:

			ParseExpression() = default;
			virtual ~ParseExpression() = default;




			/**********************************************************************
			  Execution
			**********************************************************************/
		public:

			//* Parse expression
			//* Will report issues to the specified reporter
			//* Returns true if successfully parsed, false if there were errors
			bool operator()( ToolBox& tools, type::Node& result );




			///////////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			bool _parse( type::Node& result );
			bool _processToken();

			void _checkContext();
			bool _processExpressionToken();

			bool _parseTuple();
			bool _parseDataTuple();
			bool _parseMetaTuple();
			bool _parseTuple( type::BasicTuple &tuple, type::BasicTuple::Class cls, bool parenthesized );
			bool _parseTupleAttribute( type::BasicTuple& tuple );
			
			bool _parseTypeTuple();
			TypeTuple* __parseTypeTuple( name_t name = no_name );
			bool _parseTypeTupleAttribute( TypeTuple& tuple );
			bool _parseSubTypeTuple( TypeTuple& tuple, name_t sub_name = no_name );
			bool _parseTypeTupleNameAndType( TypeTuple& tuple );

			bool _parseOperator( type::operators::code type );
			bool _parseName();
			void _parseLiteralName();
			void _parseString();
			void _parseBytes();
			void _parsePath();
			void _parseChar();
			void _parseByte();
			void _parseIndentation();
			void _parseRegex();
			void _parseBool();
			void _parseFloat();
			void _parseNamePath();
			void _parseInt();

			void _addAction( type::Action* action );

			bool _popOpsUntil( type::operators::code op );
			bool _popOpsFor( type::operators::code op );
			bool _processPopArgs( type::operators::code op );




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			ToolBox* Tools{ nullptr };
			bool Literal{ false };
		};
	}
}
