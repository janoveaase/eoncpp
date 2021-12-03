#pragma once
#include "Scope.h"
#include "Types.h"
#include <vector>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class instance;


	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Array Class - eon::tup::Array

		  Bindings for std::vector
		**********************************************************************/
		class Array
		{
		public:
			Array() = default;
			Array( const typespec* element_type, varscope& scope );
			virtual ~Array() = default;

			inline const typespec* elementType() const noexcept { return ElementType; }

			inline const std::vector<instance*>& elements() const noexcept { return Elements; }
			inline std::vector<instance*>& elements() noexcept { return Elements; }

			inline pos_t intToPos( int_t i ) const noexcept {
				return i >= 0 ? static_cast<pos_t>( i ) : Elements.size() - static_cast<pos_t>( -i ); }

			inline bool empty() const noexcept { return Elements.empty(); }
			inline pos_t size() const noexcept { return Elements.size(); }

			inline instance* at( pos_t pos ) const { return Elements.at( pos ); }
			inline instance* at( int_t pos ) const { return Elements.at( intToPos( pos ) ); }

			inline void append( instance* elm ) { Elements.push_back( elm ); }
			void append( const Array& other );
			inline void insert( pos_t pos, instance* elm ) { Elements.insert( Elements.begin() + pos, elm ); }
			inline void insert( int_t pos, instance* elm ) { Elements.insert( Elements.begin() + intToPos( pos ), elm ); }

			using iterator = std::vector<instance*>::iterator;
			using const_iterator = std::vector<instance*>::const_iterator;
			inline iterator begin() noexcept { return Elements.begin(); }
			inline iterator end() noexcept { return Elements.end(); }
			inline const_iterator begin() const noexcept { return Elements.begin(); }
			inline const_iterator end() const noexcept { return Elements.end(); }

			Array* copy() const;
			string str() const;
			int compare( const Array& other ) const;

			void registerActions( varscope& scope );

		private:
			std::vector<instance*> Elements;
			const typespec* ElementType{ nullptr };
		};


		struct Array_size : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
		struct Array_at_pos : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
		struct Array_at_int : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
		struct Array_append : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
		struct Array_insert_pos : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
		struct Array_insert_int : public action {
			instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) override; };
	}
}
