#pragma once

#include <eontypes/TypeSystem.h>
#include <eontypes/Tuple.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::scope' namespace encloses all hidden scopes functionality
	**************************************************************************/
	namespace scope
	{
		/**********************************************************************
		  A frame is a fixed size pre-allocated memory area for local
		  variables, used by actions and blocks.

		  Frames for actions are named (after the actions), while plain blocks
		  are unnamed.
		**********************************************************************/
		class Frame
		{
		public:
			inline Frame() = default;
			inline Frame( index_t size, Frame* parent = nullptr ) { _allocate( size ); Parent = parent; }
			inline Frame( name_t name, index_t size, Frame* parent = nullptr ) {
				Name = name; _allocate( size ); Parent = parent; }
			virtual ~Frame() { for( auto object : Names ) object.second->callDestructor(); if( Data ) delete[] Data; }

			//* Allocate space for the next variable on the stack
			//* Returns pointer to start of allocated memory, nullptr if stack
			//* is full!
			template<typename T>
			inline T* allocate( name_t ) {
				if( DataFree + sizeof( T ) > DataEnd ) return nullptr; auto data = new ( DataFree ) byte_t[ object_size ];
				DataFree += sizeof( T ); Names[ name ] = (Object*)data; return (T*)data; }

			//* Get variable from this frame only, do not look in parent!
			//* Returns nullptr if not found
			inline type::Object* getLocal( name_t name ) const noexcept {
				auto found = Names.find( name ); return found != Names.end() ? found->second : nullptr; }

			//* Get variable
			//* If not found in this frame, the parent frame will be tried!
			//* Returns nullptr if not found
			inline type::Object* get( name_t name ) const noexcept { auto found = getLocal( name );
				return found != nullptr ? found : Parent != nullptr ? Parent->get( name ) : nullptr; }

		private:
			inline void _allocate( index_t size ) { Data = new byte_t[ size ]; DataFree = Data; DataEnd = Data + size; }

		private:
			Frame* Parent{ nullptr };
			name_t Name{ no_name };
			byte_t* Data{ nullptr };
			byte_t* DataFree{ nullptr };
			byte_t* DataEnd{ nullptr };
			std::unordered_map<name_t, type::Object*> Names;
		};
	}
}
