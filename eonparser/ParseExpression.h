#pragma once
#include "ToolBox.h"
#include <eontypes/BasicTuple.h>
#include <eontypes/Expression.h>
#include <eontypes/Primitives.h>


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

		private:
			ParseExpression( name_t context ) { Context = context; }




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

			bool _processElement();

			bool _processExpressionElement();

			bool _processNameElement();
			bool _processSymbolElement();

			bool _processDefineVar();

			bool _parseActionCall();
			bool _parseOperator( type::operators::code type );
			bool _pushOperatorAction( type::operators::code type );

			bool _parseExpression();
			
			bool _parseName();
			void _parseIndentation();

			bool _parseOpenTuple();
			bool _parsePlainTuple();
			bool _parseDataTuple();
			bool _parseDynamicTuple();
			bool _parseTuple( type::BasicTuple &tuple, name_t tuple_type, name_t end_of_tuple );
			bool _endOfTuple( name_t end_of_tuple );
			bool _parseTupleAttribute( type::BasicTuple& tuple, name_t tuple_type );
			name_t _parseAttributeQualifier( name_t tuple_type );
			name_t _parseAttributeName( source::Ref& name_src );
			bool _parseAttributeType( TypeTuple& output, bool have_name );
			bool _parseAttributeValue( type::Expression& value );
			bool _getTypeFromValue( TypeTuple& output, type::Object* value, source::Ref src );
			bool _valueMatchesType( type::Object* value, const TypeTuple& type, source::Ref src );
			
			bool _parseTypeTuple();
			bool __parseTypeTuple( TypeTuple& output, name_t name, name_t end_of_tuple );
			bool _endOfTypeTuple( name_t end_of_tuple );
			bool _parseTypeTupleAttribute( TypeTuple& tuple );
			bool _parseSubTypeTuple( TypeTuple& tuple, name_t sub_name = no_name );
			bool _parseTypeTupleNameAndType( TypeTuple& tuple );

//			type::Action* _findAction( name_t name );
//			inline type::Action* _findOperator( type::operators::code op_code ) {
//				return _findAction( name::compilerGet( "$op_" + type::operators::mapCode( op_code ) ) ); }
//			void _filterCandidateActions( std::list<type::Action*>& candidates );

			bool _popOperatorsUntil( type::operators::code op );
			bool _popOperatorsFor( type::operators::code op );
			bool _popOperatorArgs();
			std::list<type::Node> _getArgsInOrder( index_t num_args, type::operators::code op );
			type::Node _finalizeTuple( std::list<type::Node>&& args );
			type::Node _finalizeOperator( actions::OperatorAction* action, std::list<type::Node>&& args );

			string _args( const type::Node& node ) const;
			bool _checkOperator( const std::list<type::Action*>& operators, actions::OperatorAction& op,
				string args, const TypeTuple& return_type ) const;




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			name_t Context{ no_name };
			ToolBox* Tools{ nullptr };
		};
	}
}
