#pragma once
#include <eontypes/TypeSystem.h>
#include <eonsource/SourceRef.h>
#include <eontypes/Operators.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		/**********************************************************************
		  Eon Parser Element Class - eon::parser::Element

		  An element is identified by a type name, and may have a value
		  (name, type::Object, or number)
		**********************************************************************/
		class Element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			Element() = default;
			Element( const Element& ) = delete;
			Element( Element&& other ) noexcept { *this = std::move( other ); }
			Element( name_t new_type, Element&& other ) noexcept { *this = std::move( other ); Type = new_type; }
			inline Element( name_t type, source::Ref source ) noexcept { Type = type; Source = source; }
			inline Element( name_t type, name_t value, source::Ref source ) noexcept {
				Type = type; NameValue = value; Source = source; }
			inline Element( name_t type, size_t value, source::Ref source ) noexcept {
				Type = type; NumValue = value; Source = source; }
			inline Element( name_t type, char_t value, source::Ref source ) noexcept {
				Type = type; SymbValue = value; Source = source; }
			inline Element( name_t type, type::operators::code value, source::Ref source ) noexcept {
				Type = type; OpValue = value; Source = source; }
			inline Element( name_t type, type::Object* value ) noexcept {
				Type = type; ObjValue = value; Source = ObjValue->source(); }
			virtual ~Element() { _drop(); };


			Element& operator=( const Element& ) = delete;
			Element& operator=( Element&& other ) noexcept
			{
				_drop(); Type = other.Type;
				if( isName() )
					NameValue = other.NameValue;
				else if( isNumber() )
					NumValue = other.NumValue;
				else if( isSymbol() )
					SymbValue = other.SymbValue;
				else
					ObjValue = other.ObjValue;
				other.ObjValue = nullptr;
				other.Type = no_name;
				Source = other.Source;
				return *this;
			}




			/******************************************************************
			  Access
			******************************************************************/
		public:

			//* Get the element type
			inline name_t type() const noexcept { return Type; }

			//* Check if element type is a specified named value
			inline bool is( name_t type ) const noexcept { return Type == type; }

			//* Check which value this element contains
			inline bool isName() const noexcept { return Type == name_name || Type == name_brace; }
			inline bool isNumber() const noexcept { return Type == name_indentation; }
			inline bool isSymbol() const noexcept { return Type == name_symbol; }
			inline bool isOperator() const noexcept { return Type == name_operator; }
			inline bool isObject() const noexcept {
				return !isName() && !isNumber() && !isSymbol() && !isOperator() && ObjValue != nullptr; }

			//* Get value - NOTE: Must match the element's type!
			inline name_t nameValue() const noexcept { return NameValue; }
			inline size_t numValue() const noexcept { return NumValue; }
			inline char_t symbValue() const noexcept { return SymbValue; }
			inline type::operators::code opValue() const noexcept { return OpValue; }
			inline const type::Object* objValue() const noexcept { return ObjValue; }

			//* Take over ownership of the element object value
			//* (This leaves the element without a value!)
			inline type::Object* consumeObjValue() noexcept { auto val = ObjValue; ObjValue = nullptr; return val; }

			//* Get element source
			inline const source::Ref& source() const noexcept { return Source; }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			void _drop() noexcept { if( isObject() && ObjValue != nullptr ) delete ObjValue; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			name_t Type{ no_name };
			union
			{
				type::Object* ObjValue{ nullptr };
				name_t NameValue;
				size_t NumValue;
				char_t SymbValue;
				type::operators::code OpValue;
			};
			source::Ref Source;
		};
	}
}
