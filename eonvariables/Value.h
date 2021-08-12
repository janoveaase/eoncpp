#pragma once

#include <eonexcept/Exception.h>
#include <eonregex/RegEx.h>
#include <eonhex/Hex.h>
#include <eonname/NameRef.h>
#include <eonexpression/Expression.h>
#include <eontuple/Tuple.h>
//#include "Operators.h"
#include "Variables.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::vars' namespace is for specific Æon Variables elements
	**************************************************************************/
	namespace vars
	{
		//* Exception throw when trying to access wrong type of value
		EONEXCEPT( WrongType );

		//* Exception thrown when requested value conversion is not possible
		EONEXCEPT( NoConversion );

		//* Exception throw when trying to use a variable that doesn't exist or
		//* a reference that doesn't lead anywhere.
		EONEXCEPT( NotFound );

		//* Exception thrown when trying to compare incompatible types
		EONEXCEPT( BadCompare );

		//* Exception thrown when trying to use and invalid/empty reference
		EONEXCEPT( InvalidReference );

		//* Exception thrown when trying to use a void valueptr object
		EONEXCEPT( NoValue );

		//* Exception thrown when trying to use a declared function that has
		//* incorrect declaration
		EONEXCEPT( BadFunction );


		//* Variable type codes
		enum class type_code
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
			meta_t,
			function_t,

			control_t
		};
		name_t mapTypeCode( type_code code );

		class value;
		using valueptr = std::shared_ptr<value>;




		/**********************************************************************
		  Eon Value Class - eon::vars::value

		  Each [eon::vars::type_code] is represented by a different sub-
		  class of eon::vars::value.
		**********************************************************************/
		class value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		protected:

			//* Cannot construct directly
			value( type_code type ) { Type = type; }
			value( tupleptr tuple_type ) { Type = type_code::tuple_t; TupleType = tuple_type; }

		public:

			virtual ~value() = default;




			/******************************************************************
			  Type Methods
			******************************************************************/
		public:

			//* Check type
			inline type_code type() const noexcept { return Type; }
			inline bool isBool() const noexcept { return Type == type_code::bool_t; }
			inline bool isChar() const noexcept { return Type == type_code::char_t; }
			inline bool isInt() const noexcept { return Type == type_code::int_t; }
			inline bool isFloat() const noexcept { return Type == type_code::float_t; }
			inline bool isName() const noexcept { return Type == type_code::name_t; }
			inline bool isString() const noexcept { return Type == type_code::string_t; }
			inline bool isBinary() const noexcept { return Type == type_code::binary_t; }
			inline bool isRaw() const noexcept { return Type == type_code::raw_t; }
			inline bool isRegex() const noexcept { return Type == type_code::regex_t; }
			inline bool isReference() const noexcept { return Type == type_code::ref_t; }
			inline bool isVariable() const noexcept { return Type == type_code::var_t; }
			inline bool isExpression() const noexcept { return Type == type_code::expr_t; }
			inline bool isTuple() const noexcept { return Type == type_code::tuple_t; }
			inline bool isMeta() const noexcept { return Type == type_code::meta_t; }
			inline bool isFunction() const noexcept { return Type == type_code::function_t; }
			inline bool isControl() const noexcept { return Type == type_code::control_t; }

			//* Get target's type (same as above except when variable or reference)
			//* Returns type_code::undef if not found
			type_code targetType( vars::variables& vars ) const;

			//* If the type is 'tuple', then use this method to get the full
			//* type - as a tuple of names.
			inline tupleptr tupleType() const noexcept { return TupleType; }



			/******************************************************************
			  Value Access - Actual value

			  Get the actual value, do not look up variables or references and
			  do not evaluate expressions!
			  Throws [eon::vars::WrongType] if trying to get the wrong type!
			******************************************************************/
		public:

			virtual bool actualBool() const { throw WrongType(); }
			virtual char_t actualChar() const { throw WrongType(); }
			virtual int64_t actualInt() const { throw WrongType(); }
			virtual double actualFloat() const { throw WrongType(); }
			
			virtual name_t actualName() const { throw WrongType(); }
			virtual const string& actualString() const { throw WrongType(); }
			virtual const std::vector<string>& actualRaw() const { throw WrongType(); }
			virtual const hex& actualBinary() const { throw WrongType(); }
			virtual const regex& actualRegex() const { throw WrongType(); }
			virtual const nameref& actualReference() const { throw WrongType(); }
			virtual name_t actualVariable() const { throw WrongType(); }
			virtual const expression& actualExpression() const { throw WrongType(); }

			virtual const tuple& actualTuple() const { throw WrongType(); }
			virtual const tuple& actualMeta() const { throw WrongType(); }
			virtual const tuple& actualFunction() const { throw WrongType(); }



			/******************************************************************
			  Value Access - Actual value conversion

			  Get the actual value, do not look up variables or references and
			  do not evaluate expressions - convert to the specified type (if
			  possible)!
			  Throws [eon::vars::WrongType] if trying to get the wrong type!
			  Throws [eon::vars::NoConversion] if value cannot be converted!
			******************************************************************/
		public:

			virtual bool convertBool() const { throw WrongType(); }
			virtual char_t convertChar() const { throw WrongType(); }
			virtual int64_t convertInt() const { throw WrongType(); }
			virtual double convertFloat() const { throw WrongType(); }

			virtual name_t convertName() const { throw WrongType(); }
			virtual string convertString() const { throw WrongType(); }
			virtual nameref convertReference() const { throw WrongType(); }
			virtual name_t convertVariable() const { throw WrongType(); }



			/******************************************************************
			  Value Access - Target value

			  If this value is variable or reference, then get the variable
			  value or referenced value respectively. If it is en expression,
			  evaluate the expression.
			  For all others, this is the same as getting actual value!
			  Throws [eon::vars::WrongType] if trying to get the wrong type!			  
			******************************************************************/
		public:

			virtual valueptr target( variables& vars, valueptr self ) const { return self; }

			virtual bool targetBool( variables& vars ) const { throw WrongType(); }
			virtual char_t targetChar( variables& vars ) const { throw WrongType(); }
			virtual int64_t targetInt( variables& vars ) const { throw WrongType(); }
			virtual double targetFloat( variables& vars ) const { throw WrongType(); }

			virtual name_t targetName( variables& vars ) const { throw WrongType(); }
			virtual const string& targetString( variables& vars ) const { throw WrongType(); }
			virtual const std::vector<string>& targetRaw( variables& vars ) const { throw WrongType(); }
			virtual const hex& targetBinary( variables& vars ) const { throw WrongType(); }
			virtual const regex& targetRegex( variables& vars ) const { throw WrongType(); }
			virtual const nameref& targetReference( variables& vars ) const { throw WrongType(); }
			virtual name_t targetVariable( variables& vars ) const { throw WrongType(); }
			virtual const expression& targetExpression( variables& vars ) const { throw WrongType(); }

			virtual const tuple& targetTuple( variables& vars ) const { throw WrongType(); }
			virtual const tuple& targetMeta( variables& vars ) const { throw WrongType(); }
			virtual const tuple& targetFunction( variables& vars ) const { throw WrongType(); }



			/******************************************************************
			  Value Access - Target value conversion

			  These methods will call the 'target' method and then return the
			  corresponding convert value.
			  Throws [eon::vars::WrongType] if trying to get the wrong type!
			******************************************************************/
		public:

			bool convertTargetBool( variables& vars ) const;
			char_t convertTargetChar( variables& vars ) const;
			int64_t convertTargetInt( variables& vars ) const;
			double convertTargetFloat( variables& vars ) const;

			name_t convertTargetName( variables& vars ) const;
			string convertTargetString( variables& vars ) const;
			nameref convertTargetReference( variables& vars ) const;
			name_t convertTargetVariable( variables& vars ) const;




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get an identical copy
			virtual valueptr copy() const = 0;


			//* Write value to string
			virtual string str( size_t& pos_on_line, size_t indentation_level ) const noexcept = 0;


			//* Compare
			virtual int actualCompare( const valueptr& other ) const {
				throw BadCompare( "Cannot compare " + *mapTypeCode( Type ) + " with "
					+ ( other ? *mapTypeCode( other->Type ) : "N/A" ) + "!" ); }
			virtual int targetCompare( const valueptr& other, variables& vars ) const {
				return actualCompare( other ); };




			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			virtual void setActual( valueptr new_value ) = 0;
			void setTarget( valueptr new_value, variables& vars );

			//* For a few types, it is possible to get a mutable reference to
			//* the value.
			//* Throws [eon::vars::WrongType] if you try to access the wrong
			//* type
			virtual string& string_value() { throw WrongType(); }
			virtual std::vector<eon::string>& raw_value() { throw WrongType(); }
			virtual hex& binary_value() { throw WrongType(); }
			virtual expression& expression_value() { throw WrongType(); }
			virtual tuple& meta_value() { throw WrongType(); }
			virtual tuple& tuple_value() { throw WrongType(); }
			virtual tuple& function_value() { throw WrongType(); }

			//* Clear value
			virtual void clear() noexcept = 0;




			/******************************************************************
			  Helper Methods
			******************************************************************/
		protected:

			inline void _nconv( type_code to ) const {
				throw UnsupportedOperand( "Cannot convert from '" + *mapTypeCode( Type )
					+ "' to '" + *mapTypeCode( to ) + "'!" ); }





			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			type_code Type{ type_code::undef };
			tupleptr TupleType;
		};
	}
}
