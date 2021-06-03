#pragma once

#include <eonexcept/Exception.h>
#include <eonregex/RegEx.h>
#include <eonhex/Hex.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class tuple;
	using tupleptr = std::shared_ptr<tuple>;




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
			value( basic_type type ) { BasicType = type; }
			value( tupleptr tuple_type ) {
				BasicType = basic_type::tuple_t; TupleType = tuple_type; }

		public:

			virtual ~value() = default;



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Check basic type
			inline basic_type basicType() const noexcept { return BasicType; }
			inline bool isBool() const noexcept {
				return BasicType == basic_type::bool_t; }
			inline bool isChar() const noexcept {
				return BasicType == basic_type::char_t; }
			inline bool isInt() const noexcept {
				return BasicType == basic_type::int_t; }
			inline bool isFloat() const noexcept {
				return BasicType == basic_type::float_t; }
			inline bool isName() const noexcept {
				return BasicType == basic_type::name_t; }
			inline bool isString() const noexcept {
				return BasicType == basic_type::string_t; }
			inline bool isBinary() const noexcept {
				return BasicType == basic_type::binary_t; }
			inline bool isRaw() const noexcept {
				return BasicType == basic_type::raw_t; }
			inline bool isRegex() const noexcept {
				return BasicType == basic_type::regex_t; }
			inline bool isRef() const noexcept {
				return BasicType == basic_type::ref_t; }
			inline bool isMeta() const noexcept {
				return BasicType == basic_type::meta_t; }
			inline bool isTuple() const noexcept {
				return BasicType == basic_type::tuple_t; }

			//* If the type is 'tuple', then use this method to get the full
			//* type.
			inline tupleptr tupleType() const noexcept { return TupleType; }


			//* Get read-only value
			//* Throws [eon::tup::WrongType] if you try to access the wrong
			//* type
			virtual bool bool_value() const { throw WrongType(); }
			virtual char_t char_value() const { throw WrongType(); }
			virtual int64_t int_value() const { throw WrongType(); }
			virtual double float_value() const { throw WrongType(); }
			virtual name_t name_value() const { throw WrongType(); }
			virtual const string& string_value() const { throw WrongType(); }
			virtual const hex& binary_value() const { throw WrongType(); }
			virtual const std::vector<string>& raw_value() const {
				throw WrongType(); }
			virtual const regex& regex_value() const { throw WrongType(); }
			virtual const path& ref_value() const { throw WrongType(); }
			virtual const tuple& meta_value() const { throw WrongType(); }
			virtual const tuple& tuple_value() const { throw WrongType(); }

			
			//* Check if equal to another value of the same type
			virtual bool equal( const valueptr other ) const noexcept = 0;

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

			//* Get modifiable value
			//* Throws [eon::tup::WrongType] if you try to access the wrong
			//* type
			virtual bool& bool_value() { throw WrongType(); }
			virtual char_t& char_value() { throw WrongType(); }
			virtual int64_t& int_value() { throw WrongType(); }
			virtual double& float_value() { throw WrongType(); }
			virtual string& string_value() { throw WrongType(); }
			virtual hex& binary_value() { throw WrongType(); }
			virtual std::vector<string>& raw_value() { throw WrongType(); }
			virtual regex& regex_value() { throw WrongType(); }
			virtual path& ref_value() { throw WrongType(); }
			virtual tuple& type_value() { throw WrongType(); }
			virtual tuple& meta_value() { throw WrongType(); }
			virtual tuple& tuple_value() { throw WrongType(); }

			//* Clear value
			virtual void clear() noexcept = 0;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			basic_type BasicType{ basic_type::undef };
			tupleptr TupleType;
		};
	}
}
