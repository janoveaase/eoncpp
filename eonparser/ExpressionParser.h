#pragma once

#include "ParserState.h"
#include "DirectConversion.h"
#include <eontypesystem/Tuple.h>
#include <eontypesystem/TupleFactory.h>
#include <eontypesystem/Operators.h>
#include <eonexpression/Expression.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonfilesys/Path.h>
#include <eontokenizer/TokenParser.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	EONEXCEPT( InvalidExpression );




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'parser' namespace encloses all background parser functionality
	//
	namespace parser
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Expression parser class - eon::parser::ExpressionParser.
		//
		// Parses a literal expression value.
		//
		class ExpressionParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal expression.
			// Parameters:
			//   state : Parser state for which the next token is the start of a literal expression.
			//   scope : Scope under which the expression is to be parsed.
			// Returns: Expression
			Expression parse( parser::State& state, Tuple& scope, std::set<string> terminate_on = std::set<string>() );

			// Parse literal expression.
			// Parameters:
			//   state : Parser state for which the next token is the start of a literal expression.
			//   scope : Scope under which the expression is to be parsed.
			// Returns: Root node of expression. (More efficient if the Expression wrapper object is not needed.)
			expression::Node parseNode(
				parser::State& state, Tuple& scope, std::set<string> terminate_on = std::set<string>() );





			///////////////////////////////////////////////////////////////////
			//
			// Definitions
			//
		PRIVATE:

			enum class Seen : uint8_t
			{
				_operand,
				_operator,
			};

			struct Operator
			{
				Operator() = default;
				inline Operator( name_t op, source::Ref source ) { Op = op; Source = source; }
				name_t Op{ no_name };
				source::Ref Source;
			};

			struct ParserData
			{
				inline ParserData( parser::State& state, Tuple& scope, std::set<string> terminate_on ) noexcept {
					State = &state; Scope = &scope; TerminateOn = std::move( terminate_on ); }
				inline ParserData( ParserData& other, std::set<string> terminate_on ) {
					State = other.State; Scope = other.Scope; TerminateOn = terminate_on; }

				inline bool terminateOn( string type ) const noexcept {
					return TerminateOn.find( type ) != TerminateOn.end(); }
				inline bool endOfLargeExpression() const noexcept {
					return State->Tokens.atEnd() || terminateOn( State->Tokens.viewed().str() ); }
				inline bool endOfSmallExpression() {
					return endOfLargeExpression() || State->Tokens.viewed().is( symbol_semicolon ); }

				parser::State* State{ nullptr };
				Tuple* Scope{ nullptr };

				Seen LastSeen{ Seen::_operator };
				std::set<string> TerminateOn;
				stack<expression::Node> Operands;
				stack<Operator> Operators;
				bool Errors{ false };
				expression::DirectConversion Converter;
			};





			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PRIVATE:

			bool _parseExpressionDetails( ParserData& data );

			bool _parseDetails( ParserData& data );

			bool _parseLiteral( ParserData& data );
			bool _parseBoolLiteral( ParserData& data );
			bool _parseByteLiteral( ParserData& data );
			bool _parseCharLiteral( ParserData& data );
			bool _parseIntLiteral( ParserData& data );
			bool _parseFloatLiteral( ParserData& data );
			bool _parseBytesLiteral( ParserData& data );
			bool _parseStringLiteral( ParserData& data );
			bool _parseRegexLiteral( ParserData& data );
			bool _parseNamepathLiteral( ParserData& data );
			bool _parsePathLiteral( ParserData& data );
			bool _parseNameLiteral( ParserData& data );

			bool _parseName( ParserData& data );
			bool _parseName( ParserData& data, name_t name );
			bool _parseActionCall( ParserData& data, name_t action_name );
			bool _misspelledPrefixError( ParserData& data, name_t prefix );
			bool _processVariable( ParserData& data, Attribute& variable, name_t name );

			bool _parseTuple( ParserData& data );
			bool _parseStaticTuple( ParserData& data );
			bool _parseTypeTuple( ParserData& data );

			bool _addOperand( ParserData& data, expression::Node&& node );

			bool _parseExpression( ParserData& data );

			void _reportUnknownElementError( ParserData& data );

			bool _parseOperator( ParserData& data );
			bool _processOperator( ParserData& data );
			bool _processOperatorName( ParserData& data, name_t name );
			bool _processOperator( ParserData& data, name_t op_name, source::Ref op_source );
			bool _processEndOfSmallExpression( ParserData& data );
			bool _processCloseParen( ParserData& data );
			bool _processPlainOperator( ParserData& data, name_t new_op_name, source::Ref new_op_source );
			bool _canBindOperator( ParserData& data, name_t new_op_name );

			void _finishExpressionDetails( ParserData& data );

			void _bindOperator( ParserData& data );
			void _bindAssign( ParserData& data );
			void _bindNormal( ParserData& data );
			void _bindOperands( ParserData& data, expression::Node& op );
			void _bindOperatorAction( ParserData& data, expression::Node& op );
			bool _getOperands( ParserData& data, std::initializer_list<expression::Node*> operands );
			bool _isLvalue( ParserData& data, const expression::Node& operand ) const;
			void _assignDirect( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const;
//			TypeTuple _findAction( ParserData& data, name_t name, name_t classification, name_t type, TypeTuple args ) const;
//			void _directAssignNotSupportedError( ParserData& data, name_t type ) const;
//			void _directAssignMultipleOptionsError(
//				ParserData& data, name_t name, name_t type, TypeTuple args, const std::set<TypeTuple>& action_types ) const;
			void _bindAction( ParserData& data, expression::Node& lhs, expression::Node& rhs, TypeTuple action_signature ) const;
			void _assignDirectByConversion( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const;
			void _assignTuple( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const;
			void _incompatibleTupleError( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const;
			void _assignByConstructor( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const;
			bool _canConvertDirectly( ParserData& data, const TypeTuple& lhs, const expression::Node& rhs ) const noexcept;
			void _resolveOperatorSequence( ParserData& data, name_t new_op_name, source::Ref new_op_source );
			name_t _resolveOperator( ParserData& data, name_t op_name );
			inline bool _singleOptionNoSequence( const std::vector<name_t>& all_options ) const noexcept {
				return all_options.size() == 1 && type::Operators::sequence( all_options[ 0 ] ).size() == 1; }
			name_t _resolveNonTrivalOperator( std::vector<name_t>& all_options, ParserData& data, name_t op_name );
			name_t _resolveNonTrivalOperator(
				std::vector<name_t>& all_options,
				ParserData& data,
				name_t op_name,
				std::vector<expression::Node>& operands,
				index_t seq_no );
			name_t _identifySingularOperator(
				ParserData& data, const std::vector<name_t>& options, name_t op_name, index_t seq_no );
			std::vector<name_t> _discardPrefixOperatorOptions( const std::vector<name_t>& options, index_t seq_no );
			name_t _operatorMissingElementSyntaxError( ParserData& data, std::vector<name_t>& all_options, index_t seq_no );
			std::set<string> _findStopOn( const std::vector<name_t>& options, index_t seq_no );
			std::vector<name_t> _matchNextSequenceOperator(
				ParserData& data, const std::vector<name_t>& options, index_t seq_no );
			name_t _processSequenceOperator( ParserData& data, name_t op_name, std::vector<expression::Node>&& operands );
			void _parseNextNonTrivalOperatorOperand(
				ParserData& data,
				const std::vector<name_t>& options,
				std::vector<expression::Node>& operands,
				index_t seq_no );
			name_t _incompleteOperatorError( ParserData& data, source::Ref orig_source );




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

		};
	}
}
