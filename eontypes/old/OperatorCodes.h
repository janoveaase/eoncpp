#pragma once
#include <eonstring/String.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	 Eon Operators Class - eon::operators

	 This class defines the operators (and their precedence) supported by
	 attribute types.
	**************************************************************************/
	class operators
	{
		/**********************************************************************
		 Definitions
		**********************************************************************/
	public:

		//* Operator codes
		enum class code
		{
			undef,				// Undefined
			value,				// Not an operator
			open_brace,			// (
			close_brace,		// )

			plus,				// a + b
			minus,				// a - b
			multiply,			// a * b
			divide,				// a / b
			cmp,				// a cmp b
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
			element_assign,		// a's[b]=c
			in,					// a in b
			slice,				// a[b:c]

			member,				// a's b
			access,				// access a
			true_cond,			// a if b
			false_cond,			// a unless b
			if_else,			// a if b else c
			for_each,			// a for each b in c
			for_each_where,		// a for each b in c where d
			prefix_loop,		// a while b
			postfix_loop,		// a until b

			assign,				// a = b
			type,				// type a
			uid,				// uid a
			is,					// a is b
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
			separator,			// ';' and newline

			dummies,
			element2,			// The ']' part of element
			element_assign2,	// The ']' part of element_assign
			element_assign3,	// The '=' part of element_assign
			slice2,				// The ':' part of slice
			slice3,				// The ']' part of slice
			if_else2,			// The "else" part of if_else
			for_each2,			// The 'each' part of for_each
			for_each3,			// The 'in' part of for_each
			for_each_where2,	// The 'each' part of for_each_where
			for_each_where3,	// The 'in' part of for_each_where
			for_each_where4,	// The 'where" part of for_each_where
			is2,				// The optional 'not' part of is
			size_of2,			// The 'of' part of size
			swap2,				// The 'and' part of swap
			find2,				// The 'in' part of find
			pct_of2,			// The 'of' part of pct_of
			as_pct_of2,			// The '%' part of as_pct_of
			as_pct_of3,			// The 'of' part of as_pct_of

			call				// An action call
		};
		static code mapCode( const string& str, code context = code::undef );
		static string mapCode( code op_code );

		//* Details about an operator
		//* Code
		//* Number of operands
		//* Precedence when read from infix expression
		//* Precedence when on the operator stack
		struct OperatorDetails
		{
			OperatorDetails() = default;
			inline OperatorDetails( code op_code, int num_operands,
				int input_precedence, int stack_precedence ) noexcept {
				Type = op_code; NumOperands = num_operands;
				InputPrecedence = input_precedence;
				StackPrecedence = stack_precedence; }

			code Type{ code::undef };
			size_t NumOperands{ 0 };
			int InputPrecedence{ 0 };
			int StackPrecedence{ 0 };
		};




		/**********************************************************************
			Construction
		**********************************************************************/
	public:

		operators() = delete;




		/**********************************************************************
			Static Methods
		**********************************************************************/
	public:

		//* Populate the tokenizer with operator tokens
		static void populateTokenizer( Tokenizer& tokenizer );

		static inline size_t numOperands( code op_code ) {
			return Ops[ static_cast<size_t>( op_code ) ].NumOperands; }

		static inline int inputPrecedence( code op_code ) {
			return Ops[ static_cast<size_t>( op_code ) ].InputPrecedence; }
		static inline int stackPrecedence( code op_code ) {
			return Ops[ static_cast<size_t>( op_code ) ].StackPrecedence; }




		/**********************************************************************
			Helpers
		**********************************************************************/
	private:

		static inline void add( OperatorDetails&& op ) {
			Ops.push_back( std::move( op ) ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		static std::vector<OperatorDetails> Ops;
	};


	/**************************************************************************
	  The official list of operator codes
	**************************************************************************/
	enum class code
	{
		undef,				// Undefined
		value,				// Not an operator
		open_brace,			// (
		close_brace,		// )

		plus,				// a + b
		minus,				// a - b
		multiply,			// a * b
		divide,				// a / b
		cmp,				// a cmp b
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
		element,			// a[b]
//		element_assign,		// a[b]=c
		in,					// a in b
		slice,				// a[b:c]

		member,				// a's b
		access,				// access a
		_if,				// a if b
		unless,				// a unless b
		if_else,			// a if b else c
		for_each,			// a for each b in c
		for_each_where,		// a for each b in c where d
		_while,				// a while b
		until,				// a until b

		assign,				// a = b
		type,				// type a
		uid,				// uid a
		is,					// a is b
		_sizeof,			// sizeof a
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

		_return,			// return a
		separator,			// ';' and newline

		dummies,
		element2,			// The ']' part of element
		element_assign2,	// The ']' part of element_assign
		element_assign3,	// The '=' part of element_assign
		slice2,				// The ':' part of slice
		slice3,				// The ']' part of slice
		if_else2,			// The "else" part of if_else
		for_each2,			// The 'each' part of for_each
		for_each3,			// The 'in' part of for_each
		for_each_where2,	// The 'each' part of for_each_where
		for_each_where3,	// The 'in' part of for_each_where
		for_each_where4,	// The 'where" part of for_each_where
		is2,				// The optional 'not' part of is
		swap2,				// The 'and' part of swap
		find2,				// The 'in' part of find
		pct_of2,			// The 'of' part of pct_of
		as_pct_of2,			// The '%' part of as_pct_of
		as_pct_of3,			// The 'of' part of as_pct_of
	};
	extern code mapCode( const string& str );
	extern string mapCode( code op_code );
}
