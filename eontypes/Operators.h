#pragma once
#include "TypeDefinitions.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::type' namespace encloses all hidden type functionality
	**************************************************************************/
	namespace type
	{
		//* Exception thrown when an operand is tried evaluated on an
		//* unsupported operand.
		EONEXCEPT( UnsupportedOperand );




		/**********************************************************************
		  Eon Operators Class - eon::operators

		  This class defines the operators (and their precedence) supported by
		  attribute types.
		**********************************************************************/
		class operators
		{
			/******************************************************************
			  Definitions
			******************************************************************/
		public:

			//* Operator codes
			enum class code
			{
				undef,				// Undefined

				_beg,
				open_brace = _beg,	// (
				close_brace,		// )
				comma,				// ,

				plus,				// a + b
				minus,				// a - b
				multiply,			// a * b
				divide,				// a / b
				cmp,				// a <=> b
				eq,					// a == b
				ne,					// a != b
				lt,					// a < b
				le,					// a <= b
				gt,					// a > b
				ge,					// a >= b
				plus_assign,		// a += b
				minus_assign,		// a -= b
				multiply_assign,	// a *= b
				divide_assign,		// a /= b
				element,			// a's[b]
				in,					// a in b
				slice,				// a[b:c]

				member,				// a's b
				true_cond,			// a if b
				if_else,			// a if b else c
				for_each,			// a for each b in c
				for_each_where,		// a for each b in c where d
				prefix_loop,		// a while b
				postfix_loop,		// a until b

				assign,				// a = b
				type,				// type a
				uid,				// uid a
				is,					// a is b
				isnot,				// a is not b
				size_of,			// size of a
				swap,				// swap a and b
				find,				// find a in b

				unary_minus,		// -a
				not,				// not a
				and,				// a and b
				or,					// a or b
				xor,				// a xor b
				bit_not,			// ~a
				bit_and,			// a & b
				bit_or,				// a | b
				bit_xor,			// a ^ b
				bit_lsh,			// a << b
				bit_rsh,			// a >> b

				pct_of,				// a % of b
				as_pct_of,			// a as % of b

				return_nul,			// return
				return_val,			// return a
				next_iteration,		// continue
				end_loop,			// break
				bumpcall,			// bumpcall a
				raise,				// raise a, raise a<b>
				call,				// An action call

				element2,			// The ']' part of element
				slice2,				// The ':' part of slice
				slice3,				// The ']' part of slice
				if_else2,			// The "else" part of if_else
				for_each2,			// The 'each' part of for_each
				for_each3,			// The 'in' part of for_each
				for_each_where2,	// The 'each' part of for_each_where
				for_each_where3,	// The 'in' part of for_each_where
				for_each_where4,	// The 'where" part of for_each_where
				isnot2,				// The 'not' part of isnot
				size_of2,			// The 'of' part of size
				swap2,				// The 'and' part of swap
				find2,				// The 'in' part of find
				pct_of2,			// The 'of' part of pct_of
				as_pct_of2,			// The '%' part of as_pct_of
				as_pct_of3,			// The 'of' part of as_pct_of

				_end
			};
/*			static std::unordered_map<code, name_t> CodeToName;
			static std::unordered_map<code, string> CodeToSymbol;
			static std::unordered_map<string, code> SymbolToCode;
			static name_t codeToName( code op_code );
			static const string& codeToSymbol( code op_code );
			static code symbolToCode( const string& symbol, code context = code::undef );*/
			static code mapCode( const string& str, code context = code::undef );
			static string mapCode( code op_code );


			//* Most operators consist of only a single operator symbol or name
			//* While multi-operand operators must have a symbol/name between
			//* each operand, unary may have both before, after, and before +
			//* after. Multi-operand operators can also start with symbol/name
			//* before any operands, and multiple symbol/names can occur in a
			//* row without operands inbetween.
			//* A symbol/name that should not have an operand before it (first
			//* or other symbol/name before), is called a prefix operator!
			//* (Unary operators are typically prefix!)
			//* Note that while e.g. '+' techinically is an infix operator, in
			//* this context it is a postfix operator (related only to the
			//* first operand).
			struct Seq
			{
				Seq() = default;
				Seq( code op, bool prefix = false ) { Op = op; Prefix = prefix; }
				code Op{ code::undef };
				bool Prefix{ false };
			};

			//* Details about an operator
			//* Code
			//* Number of operands
			//* Precedence when read from infix expression
			//* Precedence when on the operator stack
			struct OperatorDetails
			{
				OperatorDetails() = default;
				inline OperatorDetails( std::initializer_list<Seq> op_sequence, int num_operands,
					Precedence input_precedence, Precedence stack_precedence ) noexcept {
					Sequence = op_sequence; NumOperands = num_operands;
					InputPrecedence = input_precedence; StackPrecedence = stack_precedence; }

				size_t NumOperands{ 0 };
				Precedence InputPrecedence{ Precedence::none };
				Precedence StackPrecedence{ Precedence::none };
				std::vector<Seq> Sequence;
			};




			/******************************************************************
			  Construction
			******************************************************************/
		public:

			operators() = delete;




			/******************************************************************
			  Static Methods
			******************************************************************/
		public:

			static inline size_t numOperands( code op_code ) {
				_prep(); return Ops[ static_cast<size_t>( op_code ) ].NumOperands; }

			static inline Precedence inputPrecedence( code op_code ) {
				_prep(); return Ops[ static_cast<size_t>( op_code ) ].InputPrecedence; }
			static inline Precedence stackPrecedence( code op_code ) {
				_prep(); return Ops[ static_cast<size_t>( op_code ) ].StackPrecedence; }

			static inline const std::vector<Seq>& sequence( code op_code ) {
				_prep(); return Ops[ static_cast<size_t>( op_code ) ].Sequence; }
			static inline index_t posInSequence( const std::vector<Seq>& sequence, code op_code ) noexcept {
				for( index_t i = 0; i < sequence.size(); ++i ) if( sequence[ i ].Op == op_code ) return i;
				return no_index; }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			static inline void _add( OperatorDetails&& op ) { Ops.push_back( std::move( op ) ); }

			static void _prep();




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			static std::vector<OperatorDetails> Ops;
		};
	}
}
