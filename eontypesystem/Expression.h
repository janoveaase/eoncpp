#pragma once

#include "Definitions.h"
#include "Action.h"
#include "Operators.h"
#include "Tuple.h"
#include "ExpressionNode.h"
#include "DirectConversion.h"
#include <eonsource/File.h>
#include <eonsource/String.h>
#include <eonsource/SourceRef.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eonstring/NamePath.h>
#include <eonfilesys/Path.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	EONEXCEPT( InvalidExpression );




	///////////////////////////////////////////////////////////////////////////
	//
	// Expression class - eon::Expression
	//
	class Expression
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct a void expression - a place-holder
		Expression() = default;

		// Construct for an infix expression
		// Specify the fully formed scope in which the expression will execute!
		// Issues will be reported to stdout and stderr!
		// Throws eon::InvalidExpression if not a valid expression!
		Expression( string&& infix, Tuple& scope );

		// Construct for an infix expression ready at current parser position
		// NOTE: For expression variables, the leading '(' must be the current token!
		// Specify the fully formed scope in which the expression will execute!
		// If there are issues, they will be reported using the specified
		// eon::source::Reporter.
		// Throws eon::InvalidExpression if not a valid expression
		Expression( TokenParser& parser, Tuple& scope, source::Reporter& reporter, std::set<string> terminate_on );

		// Copy another expression
		inline Expression( const Expression& other ) { *this = other; }

		// Take ownership of an other expression's data
		inline Expression( Expression&& other ) noexcept { *this = std::move( other ); }

		~Expression() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Copy another expression
		Expression& operator=( const Expression& other );

		// Take ownership of another expression's data
		Expression& operator=( Expression&& other ) noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if the expression is empty or not
		inline operator bool() const noexcept { return !Roots.empty(); }

		// Get object as infix string representation
		void str( Stringifier& strf ) const;

		// Get object as infix string representation
		string str() const;




		///////////////////////////////////////////////////////////////////////
		//
		// Execution
		//
	public:

		// Run expression and get zero or more values in return.
		// Throws:
		//   - eon::type::NotFound : Expression is void!
		std::vector<Attribute> execute();




		///////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	private:

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


		class ParseData
		{
		public:
			ParseData() = delete;
			inline ParseData( TokenParser& parser, source::Reporter& reporter, std::set<string> terminate_on ) {
				Parser = &parser; Reporter = &reporter; TerminateOn = terminate_on; }
			inline ParseData( ParseData& other, std::set<string> terminate_on ) {
				Parser = other.Parser; Reporter = other.Reporter; TerminateOn = terminate_on; }

			inline TokenParser& parser() noexcept { return *Parser; }
			inline source::Reporter& reporter() noexcept { return *Reporter; }

			inline void error() noexcept { Errors = true; }
			inline bool errors() const noexcept { return Errors; }

			inline stack<expression::Node>& operands() noexcept { return Operands; }
			inline stack<Operator>& operators() noexcept { return Operators; }

			inline void recordOperandAsLastSeen() noexcept { LastSeen = Seen::_operand; }
			inline void recordOperatorAsLastSeen() noexcept { LastSeen = Seen::_operator; }
			inline Seen lastSeen() const noexcept { return LastSeen; }

			inline bool terminateOn( string type ) const noexcept { return TerminateOn.find( type ) != TerminateOn.end(); }

		private:
			TokenParser* Parser{ nullptr };
			source::Reporter* Reporter{ nullptr };
			Seen LastSeen{ Seen::_operator };
			std::set<string> TerminateOn;
			stack<expression::Node> Operands;
			stack<Operator> Operators;
			bool Errors{ false };

		public:
			expression::DirectConversion Converter;
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _prepTokenizer( Tokenizer& tokenizer );
		void _prepReTokenizer( ReTokenizer& retokenizer );

		void _parseLargeExpression( ParseData& data );
		void _parseSmallExpression( ParseData& data );
		bool _parseExpressionDetails( ParseData& data );
		inline bool _endOfLargeExpression( ParseData& data ) {
			return !data.parser() || data.terminateOn( data.parser().viewed().str() ); }
		inline bool _endOfSmallExpression( ParseData& data ) {
			return _endOfLargeExpression( data ) || data.parser().viewed().is( symbol_semicolon ); }

		void _reportUnknownElementError( ParseData& data );

		bool _parseDetails( ParseData& data );

		bool _parseLiteral( ParseData& data );
		bool _parseBoolLiteral( ParseData& data );
		bool _parseByteLiteral( ParseData& data );
		bool _parseCharLiteral( ParseData& data );
		bool _parseIntLiteral( ParseData& data );
		bool _parseFloatLiteral( ParseData& data );
		bool _parseBytesLiteral( ParseData& data );
		bool _parseStringLiteral( ParseData& data );
		bool _parseRegexLiteral( ParseData& data );
		bool _parseNamepathLiteral( ParseData& data );
		bool _parsePathLiteral( ParseData& data );

		bool _parseOperator( ParseData& data );
		bool _processOperator( ParseData& data );
		bool _processOperatorName( ParseData& data, name_t name );
		bool _processOperator( ParseData& data, name_t op_name, source::Ref op_source );
		bool _processEndOfSmallExpression( ParseData& data );
		bool _processCloseParen( ParseData& data );
		bool _processPlainOperator( ParseData& data, name_t new_op_name, source::Ref new_op_source );
		bool _canBindOperator( ParseData& data, name_t new_op_name );

		void _finishExpressionDetails( ParseData& data );

		void _bindOperator( ParseData& data );
		void _bindAssign( ParseData& data );
		void _bindNormal( ParseData& data );
		void _bindOperands( ParseData& data, expression::Node& op );
		void _bindOperatorAction( ParseData& data, expression::Node& op );
		bool _getOperands( ParseData& data, std::initializer_list<expression::Node*> operands );
		bool _isLvalue( ParseData& data, const expression::Node& operand ) const;
		void _assignDirect( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const;
		TypeTuple _findAction( ParseData& data, name_t name, name_t classification, name_t type, TypeTuple args ) const;
		void _directAssignNotSupportedError( ParseData& data, name_t type ) const;
		void _directAssignMultipleOptionsError(
			ParseData& data, name_t name, name_t type, TypeTuple args, const std::set<TypeTuple>& action_types ) const;
		void _bindAction( ParseData& data, expression::Node& lhs, expression::Node& rhs, TypeTuple action_signature ) const;
		void _assignDirectByConversion( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const;
		void _assignTuple( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const;
		void _incompatibleTupleError( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const;
		void _assignByConstructor( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const;
		bool _canConvertDirectly( ParseData& data, const TypeTuple& lhs, const expression::Node& rhs ) const noexcept;
		void _resolveOperatorSequence( ParseData& data, name_t new_op_name, source::Ref new_op_source );
		name_t _resolveOperator( ParseData& data, name_t op_name );
		inline bool _singleOptionNoSequence( const std::vector<name_t>& all_options ) const noexcept {
			return all_options.size() == 1 && type::Operators::sequence( all_options[ 0 ] ).size() == 1 ; }
		name_t _resolveNonTrivalOperator( std::vector<name_t>& all_options, ParseData& data, name_t op_name );
		name_t _resolveNonTrivalOperator(
			std::vector<name_t>& all_options,
			ParseData& data,
			name_t op_name,
			std::vector<expression::Node>& operands,
			index_t seq_no );
		name_t _identifySingularOperator(
			ParseData& data, const std::vector<name_t>& options, name_t op_name, index_t op_no );
		std::vector<name_t> _discardPrefixOperatorOptions( const std::vector<name_t>& options, index_t op_no );
		name_t _operatorMissingElementSyntaxError( ParseData& data, std::vector<name_t>& all_options, index_t seq_no );
		std::set<string> _findStopOn( const std::vector<name_t>& options, index_t op_no );
		std::vector<name_t> _matchNextSequenceOperator(
			ParseData& data, const std::vector<name_t>& options, index_t op_no );
		name_t _processSequenceOperator( ParseData& data, name_t op_name, std::vector<expression::Node>&& operands );
		void _parseNextNonTrivalOperatorOperand(
			ParseData& data, const std::vector<name_t>& options, std::vector<expression::Node>& operands, index_t seq_no );
		name_t _incompleteOperatorError( ParseData& data, source::Ref orig_source );

		bool _parseTuple( ParseData& data );
		bool _parseTuple( ParseData& data, name_t tupletype );
		bool _badTupleInExpressionError( ParseData& data, name_t tupletype );
		std::vector<AttributePair> _parseTupleAttributes( ParseData& data );
		void _parseTupleAttribute( ParseData& data, std::vector<AttributePair>& attributes );
		name_t _parseTupleAttributeName( ParseData& data );

		template<typename T>
		T _operandNotProperlyEndedError( ParseData& data, string name, source::Ref source, T return_value )
		{
			data.error();
			data.reporter().error( name + " is not properly ended!", source );
			return return_value;
		}

		bool _parseTypeTuple( ParseData& data );
		TypeTuple _parseTypeTupleDetails( ParseData& data );
		void _parseTypeTupleAttributes( ParseData& data, TypeTuple& ttuple );
		void _parseTypeTupleAttributeValue( ParseData& data, name_t attrib_name, TypeTuple& ttuple );
		void _parseTypeTupleTupleAttributeValue( ParseData& data, name_t attrib_name, TypeTuple& ttuple );

		bool _parseName( ParseData& data );
		bool _parseNameLiteral( ParseData& data, name_t name = no_name );
		bool _misspelledPrefixError( ParseData& data, name_t prefix );
		bool _processVariable( ParseData& data, Attribute& variable, name_t name );

		bool _parseActionCall( ParseData& data, name_t action_name );
		struct CallArgs
		{
			std::vector<expression::Node> Args;
			TypeTuple ArgTypes;
		};
		CallArgs _parseActionCallArgs( ParseData& data );
		bool _processConstructorCall( ParseData& data, name_t action_name, CallArgs& args, source::Ref source );

		bool _parseExpression( ParseData& data );




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		Tuple* Scope{ nullptr };
		std::list<expression::Node> Roots;
	};
}
