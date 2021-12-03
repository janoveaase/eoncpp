#pragma once

#include "Types.h"
#include <eonexcept/Exception.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown when trying to add a new item with the same name as an
	//* existing one
	EONEXCEPT( DuplicateNameError );

	//* Exception thrown when trying to access a type that doesn't exist
	EONEXCEPT( TypeNotFound );



	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		// A frame declaration
		class FrameDecl
		{
		public:
			inline FrameDecl( name_t name ) { Name = name; };
			virtual ~FrameDecl() = default;

			inline void addVar( name_t name, const TypeDef* type ) {
				Types.push_back( type ); Names.push_back( name ); StackSize += type->stackSize(); }

			inline name_t name() const noexcept { return Name; }
			inline pos_t stackSize() const noexcept { return StackSize; }
			inline const std::vector<const TypeDef*>& types() const noexcept { return Types; }
			inline const std::vector<name_t>& names() const noexcept { return Names; }

		private:
			name_t Name{ no_name };
			std::vector<const TypeDef*> Types;
			pos_t StackSize{ 0 };
			std::vector<name_t> Names;
		};

		// A frame on the variable stack
		class Frame
		{
		public:
			Frame( const FrameDecl& decl );
			~Frame();

			// Get object for given name
			// Returns nullptr if not in this frame!
			inline Object* obj( name_t name ) noexcept {
				auto found = Names.find( name ); return found != Names.end() ? found->second : nullptr; }

		private:
			const FrameDecl* Decl{ nullptr };
			byte_t* Data{ nullptr };
			std::unordered_map<name_t, Object*> Names;
		};


		class VarStack
		{
		public:

			// Construct stack with the "global" frame at the bottom
			inline VarStack( const FrameDecl& global_decl ) { push( global_decl ); }
			inline virtual ~VarStack() { while( !Frames.empty() ) pop(); }




			/******************************************************************
			  Stack access
			******************************************************************/
		public:

			// Get an object based on name
			// Returns nullptr if not found!
			inline Object* obj( name_t name ) noexcept { Object* found{ nullptr };
				for( auto i = Frames.rbegin(); i != Frames.rend(); ++i ) {
					found = i->obj( name ); if( found ) break; } return found; }




			/******************************************************************
			  Direct frame access
			******************************************************************/
		public:

			// Push new frame
			inline Frame& push( const FrameDecl& decl ) { Frames.push_back( Frame( decl ) ); return top(); }

			// Pop the top frame
			inline void pop() { Frames.erase( Frames.end() - 1 ); }

			// Access the top frame
			inline Frame& top() { return Frames.at( Frames.size() - 1 ); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			std::vector<Frame> Frames;
		};
	}
}
