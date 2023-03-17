#pragma once

#include "Definitions.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// Names related to operators

	EON_SYMBOL( open_round, "(" );
	EON_SYMBOL( close_round, ")" );
	EON_SYMBOL( open_square, "[" );
	EON_SYMBOL( close_square, "]" );

	EON_SYMBOL( plus, "+" );
	EON_SYMBOL( minus, "-" );
	EON_SYMBOL( multiply, "*" );
	EON_SYMBOL( divide, "/" );
	EON_NAME( mod );
	EON_SYMBOL( pow, "^" );

	EON_SYMBOL( cmp, "<=>" );
	EON_SYMBOL( eq, "==" );
	EON_SYMBOL( lt, "<" );
	EON_SYMBOL( le, "<=" );
	EON_SYMBOL( gt, ">" );
	EON_SYMBOL( ge, ">=" );
	EON_SYMBOL( ne, "!=" );

	EON_SYMBOL( plus_assign, "+=" );
	EON_SYMBOL( minus_assign, "-=" );
	EON_SYMBOL( multiply_assign, "*=" );
	EON_SYMBOL( divide_assign, "/=" );
	EON_SYMBOL( element, "[]" );
	EON_SYMBOL( slice, "[:]" );
	EON_SYMBOL( colon, ":" );

	EON_SYMBOL( member, "'s" );
	EON_COMPILERNAME( if_else, "if-else" );
	EON_COMPILERNAME( for_each, "for-each" );
	EON_COMPILERNAME( for_each_where, "for-each-where" );

	EON_SYMBOL( assign, "=" );
	EON_COMPILERNAME( is_not, "is-not" );
	EON_COMPILERNAME( size_of, "size-of" );
	EON_COMPILERNAME( swap_with, "swap-with" );
	EON_COMPILERNAME( find_in, "find-in" );

	EON_NAME( reset );
	EON_COMPILERNAME( unary_minus, "unary-minus" );
	EON_NAME( invert );			// Reciprocal, bitwise not, inverse
	EON_NAME( mask );			// Bitwise and, intersect
	EON_NAME( union );			// Bitwise or
	EON_NAME( diff );			// Bitwise xor, difference
	EON_SYMBOL( push, "<<" );	// Bitwise left shift
	EON_SYMBOL( pull, ">>" );	// Bitwise right shift

	EON_SYMBOL( pct_of, "%-of" );
	EON_SYMBOL( pct, "%" );
	EON_SYMBOL( as_pct_of, "as-%-of" );

	EON_COMPILERNAME( retblank, "return-nothing" );
	EON_COMPILERNAME( retvalue, "return-value" );

	EON_SYMBOL( semicolon, ";" );
	EON_SYMBOL( comma, "," );
	EON_SYMBOL( point, "." );




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'type' namespace enclosing all background type functionality
	//
	namespace type
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Operators class - eon::type::Operators
		//
		class Operators
		{
		public:

			Operators() = delete;




			///////////////////////////////////////////////////////////////////
			//
			// Definitions
			//
		public:

			// none  min   lmicr micro umicr llow  low   ulow  lmid  mid   umid  lhigh high  uhigh lultr ultra uultr max
			// none  min   pico  nano  micro milli centi deci  mid   deca  hecto kilo  mega  giga  tera  peta  exa   max

			// Operators have precedence, determining how some are to be
			// evaluated before others. Eon operates with the following levels:
			enum class Precedence
			{
				// Operators: return, bumpcall
				max_hi = 34, max_lo = 33,
				// Operators: [], type, uid, call, 's
				exa_hi = 32, exa_lo = 31,
				// Operators: is, -(unary), not, ~
				peta_hi = 30, peta_lo = 29,
				// Operators: [:], in, swap, pow
				tera_hi = 28, tera_lo = 27,
				// Operators: *, /, mod
				giga_hi = 26, giga_lo = 25,
				// Operators: +, -
				mega_hi = 24, mega_lo = 23,
				// Operators: <<, >>
				kilo_hi = 22, kilo_lo = 21,
				// Operators: <=>
				hecto_hi = 20, hecto_lo = 19,
				// Operators: <, <=, >, >=
				deca_hi = 18, deca_lo = 17,
				// Operators: ==, !=
				mid_hi = 16, mid_lo = 15,
				// Operators: &
				deci_hi = 14, deci_lo = 13,
				// Operators: ^
				centi_hi = 12, centi_lo = 11,
				// Operators: |
				milli_hi = 10, milli_lo = 9,
				// Operators: and
				micro_hi = 8, micro_lo = 7,
				// Operators: xor
				nano_hi = 6, nano_lo = 5,
				// Operators: or
				pico_hi = 4, pico_lo = 3,
				// Operators: =, +=, -=, *=, /=, =, raise, if, if-else, for-each, for-each-where, while, until, find,
				// continue, break, [, :
				min_hi = 2, min_lo = 1,

				none = 0
			};


			// Operator elements (symbols and names part of the operator) can appear before the next operand, or after -
			// they can be prefix or postfix.
			enum class Position : uint8_t
			{
				prefix,
				postfix
			};


			// Most operators consist of only a single operator symbol or name.
			// While multi-operand operators must have a symbol/name between each operand, unary may have both before,
			// after, and before + after. Multi-operand operators can also start with symbol/name before any operands,
			// and multiple symbol/names can occur in a row without operands inbetween.
			// A symbol/name that should not have an operand before it (first or other symbol/name before), is called a
			// prefix operator! (Unary operators are typically prefix!)
			// Note that while e.g., '+' techinically is an infix operator, in
			// this context it is a postfix operator (related only to the first operand).
			struct OperatorElement
			{
				OperatorElement() = default;
				inline OperatorElement( name_t op, Position pos = Position::postfix) { Op = op; Pos = pos; }
				inline bool prefix() const noexcept { return Pos == Position::prefix;	 }
				name_t Op{ no_name };
				Position Pos{ Position::postfix };
			};


			// Details about an operator
			struct OperatorDetails
			{
				OperatorDetails() = default;
				inline OperatorDetails( std::initializer_list<OperatorElement> op_sequence, int num_operands,
					Precedence input_precedence, Precedence stack_precedence ) noexcept {
					Sequence = op_sequence; NumOperands = num_operands;
					InputPrecedence = input_precedence; StackPrecedence = stack_precedence;
				}

				size_t NumOperands{ 0 };
				Precedence InputPrecedence{ Precedence::none };
				Precedence StackPrecedence{ Precedence::none };
				std::vector<OperatorElement> Sequence;
			};




			///////////////////////////////////////////////////////////////////
			//
			// Static methods
			//
		public:

			// Check if name is for an operator.
			static inline bool isOperator( name_t name ) { _prep(); return ElmToMain.find( name ) != ElmToMain.end(); }

			// Check if name is first in a sequence (and not later).
			static bool isFirstInSequence( name_t name );

			// Given an operator name, get the possible main operator names.
			static inline const std::vector<name_t>& mapOperator( name_t name ) {
				_prep(); auto found = ElmToMain.find( name ); return found != ElmToMain.end() ? found->second : NoMain; }

			// Get number of operands for specified operator.
			static inline size_t numOperands( name_t op_name ) { _prep(); return Details[ op_name ].NumOperands; }

			// Get operator precedence when reading input.
			static inline Precedence inputPrecedence( name_t op_name ) {
				_prep(); return Details[ op_name ].InputPrecedence; }

			// Get operator precedence when on stack.
			static inline Precedence stackPrecedence( name_t op_name ) {
				_prep(); return Details[ op_name ].StackPrecedence; }

			// Get sequence of operator elements for the named operator.
			static inline const std::vector<OperatorElement>& sequence( name_t op_name ) {
				_prep(); return Details[ op_name ].Sequence; }

			// Get the position in the operator element sequence for the specified operator name.
			static inline index_t posInSequence( const std::vector<OperatorElement>& sequence, name_t op_name ) noexcept {
				for( index_t i = 0; i < sequence.size(); ++i ) if( sequence[ i ].Op == op_name ) return i;
				return no_index; }

			// Check if named operator is type-implemented.
			static inline bool isTypeImplemented( name_t op_name ) noexcept {
				return TypeOperators.find( op_name ) != TypeOperators.end(); }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			static inline void _add( name_t op_name, OperatorDetails&& op_details ) {
				Details[ op_name ] = std::move( op_details ); }

			static void _prep();




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			// Mapping operator main name to operator details.
			static std::unordered_map<name_t, OperatorDetails> Details;

			// Mapping operator element name to operator main names.
			static std::unordered_map<name_t, std::vector<name_t>> ElmToMain;
			static const std::vector<name_t> NoMain;

			// Operators that must be implemented for types.
			static std::unordered_set<name_t> TypeOperators;
		};
	}
}