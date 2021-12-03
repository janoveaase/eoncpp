#pragma once

#include "TypeSpec.h"
#include "Tuple.h"
#include "Scope.h"
#include "Array.h"
#include <eonregex/RegEx.h>
#include <eonfilesys/Path.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class instance;

	//* Æon standard types (not defined elsewhere)
	// TODO: Remove!
	using list_t = std::list<instance*>;
	using set_t = std::set<instance*>;
	using heap_t = std::unordered_set<instance*>;

	namespace tup
	{
		class action;
	}
	namespace expr
	{
		class evaluate;
	}
	class expression;



	/**************************************************************************
	  Eon Instance Class - eon::instance

	  Instance of a type
	**************************************************************************/
	class instance
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		instance() = default;
		inline instance( tup::varscope& scope, bool value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_bool ); Value.Bool = value; References = refs; }
		inline instance( tup::varscope& scope, byte_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_byte ); Value.Byte = value; References = refs; }
		inline instance( tup::varscope& scope, char_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_char ); Value.Char = value; References = refs; }
		inline instance( tup::varscope& scope, int_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_int ); Value.Int = value; References = refs; }
		inline instance( tup::varscope& scope, int8_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_int8 ); Value.Int8 = value; References = refs; }
		inline instance( tup::varscope& scope, int16_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_int16 ); Value.Int16 = value; References = refs; }
		inline instance( tup::varscope& scope, int32_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_int32 ); Value.Int32 = value; References = refs; }
		inline instance( tup::varscope& scope, int64_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_int64 ); Value.Int64 = value; References = refs; }
		inline instance( tup::varscope& scope, flt_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_float ); Value.Float = value; References = refs; }
		inline instance( tup::varscope& scope, flt32_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_float32 ); Value.Float32 = value; References = refs; }
		inline instance( tup::varscope& scope, flt128_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_float128 ); Value.Float128 = value; References = refs; }
		inline instance( tup::varscope& scope, pos_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_position ); Value.Position = value; References = refs; }
		inline instance( tup::varscope& scope, name_t value, uint32_t refs = 0 ) {
			Type = scope.global().primitive( name_name ); Value.Name = value; References = refs; }

		inline instance( tup::varscope& scope, std::string* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_bytes ); Value.Bytes = value; References = refs; }
		inline instance( tup::varscope& scope, string* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_string ); Value.String = value; References = refs; }
		inline instance( tup::varscope& scope, regex* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_regex ); Value.Regex = value; References = refs; }
		inline instance( tup::varscope& scope, path* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_path ); Value.Path = value; References = refs; }
		inline instance( tup::varscope& scope, expression* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_expression ); Value.Expression = value; References = refs; }
		
		inline instance( tup::varscope& scope, tup::eonarray* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_array ); Value.Array = value; References = refs; }
		inline instance( tup::varscope& scope, list_t* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_list ); Value.List = value; References = refs; }
		inline instance( tup::varscope& scope, set_t* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_set ); Value.Set = value; References = refs; }
		inline instance( tup::varscope& scope, heap_t* value, uint32_t refs = 0 ) {
			Type = scope.global().standard( name_heap ); Value.Heap = value; References = refs; }

		instance( tup::varscope& scope, eon::tuple* value, uint32_t refs = 0 );

		inline instance( tup::varscope& scope, instance* reference ) {
			Type = scope.reference( reference->Type ); Value.Reference = reference; Value.Reference->addRef(); }

		inline instance( tup::varscope& scope, tup::action* value ) { Type = value->type(); Value.Action = value; }

		~instance() { _delete(); }




		/******************************************************************
		  Reference counting
		******************************************************************/
	public:

		//* Add a new reference
		inline void addRef() noexcept { ++References; }

		//* Remove a reference
		//* This will cause the object to self-delete if it reaches zero!
		inline void delRef() noexcept {
			if( --References == 0 )
				delete this;
		}




		/******************************************************************
		  Read-only Methods
		******************************************************************/
	public:

		const tup::typespec* type() const noexcept { return Type; }

		instance* copy() const;

		//* Get string representation
		string str() const;




		/******************************************************************
		  Value Access
		******************************************************************/
	public:

		EONEXCEPT( WrongType );
		EONEXCEPT( DivisionByZero );

		inline bool boolValue() const { if( Type->is( name_bool ) ) return Value.Bool; else throw WrongType(); }
		inline bool& boolValue() { if( Type->is( name_bool ) ) return Value.Bool; else throw WrongType(); }

		inline byte_t byteValue() const { if( Type->is( name_byte ) ) return Value.Byte; else throw WrongType(); }
		inline byte_t& byteValue() { if( Type->is( name_byte ) ) return Value.Byte; else throw WrongType(); }

		inline char_t charValue() const { if( Type->is( name_char ) ) return Value.Char; else throw WrongType(); }
		inline char_t& charValue() { if( Type->is( name_char ) ) return Value.Char; else throw WrongType(); }
		
		inline int_t intValue() const { if( Type->is( name_int ) ) return Value.Int; else throw WrongType(); }
		inline int_t& intValue() { if( Type->is( name_int ) ) return Value.Int; else throw WrongType(); }
		inline int_t intValueForDiv() const { if( Type->is( name_int ) ) {
			if( Value.Int != 0 ) return Value.Int; else throw DivisionByZero(); } else throw WrongType(); }

		inline int8_t int8Value() const { if( Type->is( name_int8 ) ) return Value.Int8; else throw WrongType(); }
		inline int8_t& int8Value() { if( Type->is( name_int8 ) ) return Value.Int8; else throw WrongType(); }
		inline int8_t int8ValueForDiv() const { if( Type->is( name_int8 ) ) {
			if( Value.Int8 != 0 ) return Value.Int8; else throw DivisionByZero(); } else throw WrongType(); }

		inline int16_t int16Value() const { if( Type->is( name_int16 ) ) return Value.Int16; else throw WrongType(); }
		inline int16_t& int16Value() { if( Type->is( name_int16 ) ) return Value.Int16; else throw WrongType(); }
		inline int16_t int16ValueForDiv() const { if( Type->is( name_int16 ) ) {
			if( Value.Int16 != 0 ) return Value.Int16; else throw DivisionByZero(); } else throw WrongType(); }

		inline int32_t int32Value() const { if( Type->is( name_int32 ) ) return Value.Int32; else throw WrongType(); }
		inline int32_t& int32Value() { if( Type->is( name_int32 ) ) return Value.Int32; else throw WrongType(); }
		inline int32_t int32ValueForDiv() const { if( Type->is( name_int32 ) ) {
			if( Value.Int32 != 0 ) return Value.Int32; else throw DivisionByZero(); } else throw WrongType(); }

		inline int64_t int64Value() const { if( Type->is( name_int64 ) ) return Value.Int64; else throw WrongType(); }
		inline int64_t& int64Value() { if( Type->is( name_int64 ) ) return Value.Int64; else throw WrongType(); }
		inline int64_t int64ValueForDiv() const { if( Type->is( name_int64 ) ) {
			if( Value.Int64 != 0 ) return Value.Int64; else throw DivisionByZero(); } else throw WrongType(); }

		inline flt_t floatValue() const { if( Type->is( name_float ) ) return Value.Float; else throw WrongType(); }
		inline flt_t& floatValue() { if( Type->is( name_float ) ) return Value.Float; else throw WrongType(); }
		inline flt_t floatValueForDiv() const { if( Type->is( name_float ) ) {
			if( Value.Float != 0.0 ) return Value.Float; else throw DivisionByZero(); } else throw WrongType(); }

		inline flt32_t float32Value() const { if( Type->is( name_float32 ) ) return Value.Float32; else throw WrongType(); }
		inline flt32_t& float32Value() { if( Type->is( name_float32 ) ) return Value.Float32; else throw WrongType(); }
		inline flt32_t float32ValueForDiv() const { if( Type->is( name_float32 ) ) {
			if( Value.Float32 != 0.0 ) return Value.Float32; else throw DivisionByZero(); } else throw WrongType(); }

		inline flt64_t float64Value() const { if( Type->is( name_float ) ) return Value.Float; else throw WrongType(); }
		inline flt64_t& float64Value() { if( Type->is( name_float ) ) return Value.Float; else throw WrongType(); }
		inline flt64_t float64ValueForDiv() const { if( Type->is( name_float ) ) {
			if( Value.Float != 0.0 ) return Value.Float; else throw DivisionByZero(); } else throw WrongType(); }

		inline flt128_t float128Value() const {
			if( Type->is( name_float128 ) ) return Value.Float128; else throw WrongType(); }
		inline flt128_t& float128Value() { if( Type->is( name_float128 ) ) return Value.Float128; else throw WrongType(); }
		inline flt128_t float128ValueForDiv() const { if( Type->is( name_float128 ) ) {
			if( Value.Float128 != 0.0 ) return Value.Float128; else throw DivisionByZero(); } else throw WrongType(); }

		inline pos_t positionValue() const {
			if( Type->is( name_position ) ) return Value.Position; else throw WrongType(); }
		inline pos_t& positionValue() { if( Type->is( name_position ) ) return Value.Position; else throw WrongType(); }
		inline pos_t positionValueForDiv() const { if( Type->is( name_position ) ) {
			if( Value.Position != 0.0 ) return Value.Position; else throw DivisionByZero(); } else throw WrongType(); }

		inline name_t nameValue() const { if( Type->is( name_name ) ) return Value.Name; else throw WrongType(); }
		inline void value( name_t value ) { if( Type->is( name_name ) ) Value.Name = value; else throw WrongType(); }


		inline const std::string* bytesValue() const {
			if( Type->is( name_bytes ) ) return Value.Bytes; else throw WrongType(); }
		inline std::string* bytesValue() { if( Type->is( name_bytes ) ) return Value.Bytes; else throw WrongType(); }
		inline const string* stringValue() const {
			if( Type->is( name_string ) ) return Value.String; else throw WrongType(); }
		inline string* stringValue() { if( Type->is( name_string ) ) return Value.String; else throw WrongType(); }
		inline const regex* regexValue() const {
			if( Type->is( name_regex ) ) return Value.Regex; else throw WrongType(); }
		inline regex* regexValue() { if( Type->is( name_regex ) ) return Value.Regex; else throw WrongType(); }
		inline const path* pathValue() const { if( Type->is( name_path ) ) return Value.Path; else throw WrongType(); }
		inline path* pathValue() { if( Type->is( name_path ) ) return Value.Path; else throw WrongType(); }
		inline const expression* expressionValue() const {
			if( Type->is( name_expression ) ) return Value.Expression; else throw WrongType(); }
		inline expression* expressionValue() {
			if( Type->is( name_expression ) ) return Value.Expression; else throw WrongType(); }
		
		inline const tup::eonarray* arrayValue() const {
			if( Type->is( name_array ) ) return Value.Array; else throw WrongType(); }
		inline tup::eonarray* arrayValue() {
			if( Type->is( name_array ) ) return Value.Array; else throw WrongType(); }
		inline const list_t* listValue() const {
			if( Type->is( name_list ) ) return Value.List; else throw WrongType(); }
		inline list_t* listValue() { if( Type->is( name_list ) ) return Value.List; else throw WrongType(); }
		inline const set_t* setValue() const {
			if( Type->is( name_set ) ) return Value.Set; else throw WrongType(); }
		inline set_t* setValue() { if( Type->is( name_set ) ) return Value.Set; else throw WrongType(); }
		inline const heap_t* heapValue() const {
			if( Type->is( name_heap ) ) return Value.Heap; else throw WrongType(); }
		inline heap_t* heapValue() { if( Type->is( name_heap ) ) return Value.Heap; else throw WrongType(); }


		inline const eon::tuple* tupleValue() const { if( Type->isTuple() ) return Value.Tuple; else throw WrongType(); }
		inline void value( eon::tuple* value ) { if( Type->isTuple() ) {
			_delete(); Value.Tuple = value; } else throw WrongType(); }


		inline const instance* referenceValue() const {
			if( Type->isReference() ) return Value.Reference; else throw WrongType(); }
		inline instance* referenceValue() {
			if( Type->isReference() ) return Value.Reference; else throw WrongType(); }

		
		inline const tup::action* actionValue() const { if( Type->isAction() ) return Value.Tuple; else throw WrongType(); }
		inline void value( eon::tuple* value ) { if( Type->isTuple() ) {
			_delete(); Value.Tuple = value; } else throw WrongType(); }


		//* Assign the value of another instance to this,
		//* either by copy or move
		//* WARNING: No type checking is done - if 'other' has a different type
		//* than 'this', 'this' will change type to that of 'other'!
		void copyAssign( const instance* other );
		void moveAssign( instance* other );




		/******************************************************************
		  Comparison
		******************************************************************/
	public:

		int compare( const instance& other ) const;

		inline bool operator<( const instance& other ) const { return compare( other ) < 0; }
		inline bool operator<=( const instance& other ) const { return compare( other ) <= 0; }
		inline bool operator>( const instance& other ) const { return compare( other ) > 0; }
		inline bool operator>=( const instance& other ) const { return compare( other ) >= 0; }
		inline bool operator==( const instance& other ) const { return compare( other ) == 0; }
		inline bool operator!=( const instance& other ) const { return compare( other ) != 0; }




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	protected:

		void _delete();




		///////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	protected:

		// The value of an instance can be one of these
		union InstValue
		{
			bool Bool;
			byte_t Byte;
			char_t Char;
			int_t Int;
			int8_t Int8;
			int16_t Int16;
			int32_t Int32;
			int64_t Int64;
			pos_t Position;
			flt_t Float;
			flt32_t Float32;
			flt64_t Float64;
			flt128_t Float128;
			name_t Name;

			std::string* Bytes;
			string* String;
			regex* Regex;
			expression* Expression;
			path* Path;

			tup::eonarray* Array;
			list_t* List;
			set_t* Set;
			heap_t* Heap;

			instance* Reference;

			tuple* Tuple;

			tup::action* Action;
		};




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:

		const tup::typespec* Type{ nullptr };
		InstValue Value;
		uint32_t References{ 0 };

		friend class expr::evaluate;
	};
}
