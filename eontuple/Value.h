#pragma once

#include <eonexcept/Exception.h>
#include <eonregex/RegEx.h>
#include <eonhex/Hex.h>
#include <eonexpression/Expression.h>
#include "Operators.h"
#include "Variables.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;
	class expression;




	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon tuple elements
	**************************************************************************/
	namespace tup
	{
		//* Exception throw when trying to access wrong type of value
		EONEXCEPT( WrongType );

		//* Exception thrown when a document or attribute name is already in
		//* use
		EONEXCEPT( DuplicateName );

		//* Exeption thrown when references end up in circles
		EONEXCEPT( CircularReferencing );

		//* Exception thrown when document validation fails
		EONEXCEPT( Invalid );

		//* Exception throw when dividing by zero
		EONEXCEPT( DivisionByZero );

		//* Exception throw when trying to use a variable that doesn't exist or
		//* a reference that doesn't lead anywhere.
		EONEXCEPT( NotFound );


		//* Basic tuple attribute types
		enum class basic_type
		{
			undef,
			bool_t,
			char_t,
			int_t,
			float_t,
			name_t,
			string_t,
			binary_t,
			raw_t,
			regex_t,
			ref_t,
			var_t,
			expr_t,
			tuple_t,
			meta_t
		};
		name_t mapBasicType( basic_type type );

		//* Formatting permissions heeded by value types that can span multiple
		//* lines.
		enum class perm
		{
			//* Allow the value to appear on a single line
			allow_oneliner = 0x01,

			//* Allow the value to appear/be split into multiple lines
			allow_multiliner = 0x02
		};
		inline perm operator|( perm a, perm b ) noexcept {
			return static_cast<perm>( static_cast<int>( a )
				| static_cast<int>( b ) ); }
		inline perm operator&( perm a, perm b ) noexcept {
			return static_cast<perm>( static_cast<int>( a )
				& static_cast<int>( b ) ); }

		class value;
		using valueptr = std::shared_ptr<value>;
		class path;
		class variables;




		/**********************************************************************
		  Eon Tuple Value Class - eon::tup::value

		  Each [eon::tup::basic_type] is represented by a different sub-
		  class of eon::tup::value.
		**********************************************************************/
		class value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		protected:

			//* Cannot construct directly
			value( basic_type type ) { Type = type; }
			value( tupleptr tuple_type ) {
				Type = basic_type::tuple_t; TupleType = tuple_type; }

		public:

			virtual ~value() = default;



			/******************************************************************
			  Type Methods
			******************************************************************/
		public:

			//* Check type
			inline basic_type type() const noexcept { return Type; }
			inline bool isBool() const noexcept {
				return Type == basic_type::bool_t; }
			inline bool isChar() const noexcept {
				return Type == basic_type::char_t; }
			inline bool isInt() const noexcept {
				return Type == basic_type::int_t; }
			inline bool isFloat() const noexcept {
				return Type == basic_type::float_t; }
			inline bool isName() const noexcept {
				return Type == basic_type::name_t; }
			inline bool isString() const noexcept {
				return Type == basic_type::string_t; }
			inline bool isBinary() const noexcept {
				return Type == basic_type::binary_t; }
			inline bool isRaw() const noexcept {
				return Type == basic_type::raw_t; }
			inline bool isRegex() const noexcept {
				return Type == basic_type::regex_t; }
			inline bool isRef() const noexcept {
				return Type == basic_type::ref_t; }
			inline bool isVar() const noexcept {
				return Type == basic_type::var_t; }
			inline bool isExpression() const noexcept {
				return Type == basic_type::expr_t; }
			inline bool isMeta() const noexcept {
				return Type == basic_type::meta_t; }
			inline bool isTuple() const noexcept {
				return Type == basic_type::tuple_t; }

			//* Get "soft" type - if variable then variable's value
			//* Throws [eon::tup::NotFound] if no such variable
			basic_type softType( tup::variables& vars ) const;

			//* If the type is 'tuple', then use this method to get the full
			//* type - as a tuple of names.
			inline tupleptr tupleType() const noexcept { return TupleType; }



			/******************************************************************
			  Value Access

			  Values can be accessed as "hard" or "soft", the difference being
			  that hard values will return the extact type asked for - or
			  throw [eon::tup::WrongType] - while soft will return variable's
			  values and try to convert values to the asked for type - if at
			  all possible. Throws [eon::tup::WrongType] or
			  [eon::tup::UnsupportedOperand] if not possible to get the value
			  as the asked for type.
			******************************************************************/
		public:

			virtual bool hardBool() const { throw WrongType(); }
			virtual bool softBool( variables& vars ) const {
				throw WrongType(); }
			virtual char_t hardChar() const { throw WrongType(); }
			virtual char_t softChar( variables& vars ) const {
				throw WrongType(); }
			virtual int64_t hardInt() const { throw WrongType(); }
			virtual int64_t softInt( variables& vars ) const {
				throw WrongType(); }
			virtual double hardFloat() const { throw WrongType(); }
			virtual double softFloat( variables& vars ) const {
				throw WrongType(); }
			virtual name_t hardName() const { throw WrongType(); }
			virtual name_t softName( variables& vars ) const {
				throw WrongType(); }
			virtual const string& hardString() const { throw WrongType(); }
			virtual const string& softString( variables& vars ) const {
				throw WrongType(); }
			virtual const hex& hardBinary() const { throw WrongType(); }
			virtual const hex& softBinary( variables& vars ) const {
				throw WrongType(); }
			virtual const std::vector<string>& hardRaw() const {
				throw WrongType(); }
			virtual const std::vector<string>& softRaw( variables& vars )
				const { throw WrongType(); }
			virtual const regex& hardRegex() const { throw WrongType(); }
			virtual const regex& softRegex( variables& vars ) const {
				throw WrongType(); }
			virtual const path& hardRef() const { throw WrongType(); }
			virtual const path& softRef( variables& vars ) const {
				throw WrongType(); }
			virtual name_t hardVar() const { throw WrongType(); }
			virtual name_t softVar( variables& vars ) const {
				throw WrongType(); }
			virtual const expression& hardExpression() const {
				throw WrongType(); }
			virtual const expression& softExpression( variables& vars )
				const { throw WrongType(); }
			virtual const tuple& hardTuple() const { throw WrongType(); }
			virtual const tuple& softTuple( variables& vars ) const {
				throw WrongType(); }
			virtual const tuple& hardMeta() const { throw WrongType(); }
			virtual const tuple& softMeta( variables& vars ) const {
				throw WrongType(); }




			/******************************************************************
			  Comparison

			  Values can be compared "hard" or "soft", the difference being
			  that hard compare will compare only the given values, resulting
			  in error if their types are different, while soft will try to
			  convert when possible and also obtain variable's values.
			  Throws [eon::tup::UnsupportedOperand] if comparison is not
			  possible.
			******************************************************************/
		public:

			virtual int hardCompare( const valueptr& other ) const = 0;
			virtual int softCompare( const valueptr& other, variables& vars )
				const = 0;




			/******************************************************************
			  Simple expression evaluation

			  For Æon and EOF, it is always the first operand that implements
			  the operators that are supported for that type. These methods
			  will evaluate an operator on 'this' (first operand) and zero or
			  more arguments.
			  Throws [eon::tup::UnsupportedOperand] if the operator isn't
			  supported for the value type of 'this'.
			******************************************************************/
		public:

			//* Evaluate a unary operator
			valueptr evaluate( operators::code op_code, variables& vars )
				const;

			//* Evaluate a binary operator
			//* Throws [eon::tup::DivisionByZero] if the operator is 'divide'
			//* and the second argument is zero!
			valueptr evaluate( operators::code op_code, const valueptr& arg2,
				variables& vars ) const;

			//* Evaluate a (the) ternary operator
			//* This is an exception to the rule of implementing operators for
			//* the first operand. Here, the second operand is the prominent
			//* one (the condition of the if-else operator).
			valueptr evaluate( operators::code op_code, const valueptr& arg1,
				const valueptr& arg3, variables& vars ) const;




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get an identical copy
			virtual valueptr copy() const = 0;


			//* Write value to string
			virtual string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept = 0;




			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* For a few types, it is possible to get a mutable reference to
			//* the value.
			//* Throws [eon::tup::WrongType] if you try to access the wrong
			//* type
			virtual string& string_value() { throw WrongType(); }
			virtual std::vector<eon::string>& raw_value() { throw WrongType(); }
			virtual hex& binary_value() { throw WrongType(); }
			virtual tuple& meta_value() { throw WrongType(); }
			virtual tuple& tuple_value() { throw WrongType(); }

			//* Clear value
			virtual void clear() noexcept = 0;




			/******************************************************************
			  Helper Methods
			******************************************************************/
		protected:

			inline void _nconv( basic_type to ) const { throw
				UnsupportedOperand( "Cannot convert from '" + *mapBasicType(
					Type ) + "' to '" + *mapBasicType( to ) + "'!" ); }





			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			basic_type Type{ basic_type::undef };
			tupleptr TupleType;
		};
	}
}
