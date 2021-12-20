#pragma once

#include "TypeSystem.h"
#include "Stringifier.h"
#include <eonscopes/Scope.h>



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
		//* Node types
		enum class NodeType
		{
			undef,
			action,			// Operator
			value,			// Single value operand
			name_only
		};




		//* Expression tree node
		class Node
		{
			/******************************************************************
			  Construction
			******************************************************************/
		private:
			inline Node( NodeType type, Object* item ) { Type = type; Item = item; }
			inline Node( name_t name ) { Type = NodeType::name_only; Name = name; }

		public:
			Node() = default;
			Node( const Node& ) = delete;

			inline Node( Node&& other ) noexcept { *this = std::move( other ); }
			virtual ~Node() { if( Item ) Item->die(); }

			Node& operator=( const Node& ) = delete;
			inline Node& operator=( Node&& other ) noexcept {
				Type = other.Type; other.Type = NodeType::undef; Name = other.Name; other.Name = no_name;					
				Item = other.Item; other.Item = nullptr; Children = std::move( other.Children ); return *this; }

			Node& copy( const Node& other, scope::Scope& scope );




			/******************************************************************
			  Factory
			******************************************************************/

			static inline Node newAction( Action* action ) { return Node( NodeType::action, (Object*)action ); }
			static inline Node newValue( Object* value ) { return Node( NodeType::value, value ); }
			static inline Node newName( name_t name ) { return Node( name ); }
			static inline Node newEllipsis() { return Node( name_ellipsis ); }




			/******************************************************************
			  Node type methods
			******************************************************************/
		public:

			inline NodeType type() const noexcept { return Type; }
			inline bool isAction() const noexcept { return Type == NodeType::action; }
			inline bool isValue() const noexcept { return Type == NodeType::value; }
			inline bool isName() const noexcept { return Type == NodeType::name_only; }
			inline bool isEllipsis() const noexcept { return Type == NodeType::name_only && Name == name_ellipsis; }

			// Get as infix string
			void infixStr( type::Stringifier& str ) const;




			/******************************************************************
			  Node item methods
			******************************************************************/
		public:

			//* Get operator action object
			//* Throws [eon::type::NotFound] if not operator or no action
			inline Action& action() { if( Type == NodeType::action ) return *(Action*)Item; else throw NotFound(); }
			inline const Action& action() const {
				if( Type == NodeType::action ) return *(Action*)Item; else throw NotFound(); }

			//* Get operand object
			//* Throws [eon::type::NotFound] if not operand or no object
			inline Object* value() { if( Type == NodeType::value ) return Item; else throw NotFound(); }
			inline Object* consumeValue() {
				if( Type == NodeType::value ) { auto item = Item; Item = nullptr; return item; } else throw NotFound(); }

			//* Get name
			//* Throws [eon::type::NotFound] if no name
			inline name_t name() const { if( Type == NodeType::name_only ) return Name; else throw NotFound(); }




			/******************************************************************
			  Child node methods
			******************************************************************/
		public:

			inline bool hasChildren() const noexcept { return !Children.empty(); }
			inline size_t numChildren() const noexcept { return Children.size(); }

			inline Node& add( Node&& child ) {
				Children.push_back( std::move( child ) ); return Children[ Children.size() - 1 ]; }

			//* Get a child node
			//* Throws [eon::type::NotFound] if invalid index
			inline Node& child( size_t num ) { if( num < Children.size() ) return Children[ num ]; else throw NotFound(); }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			std::vector<bool> _operandsNeedingParenthesis( const Action& action ) const;

			void _infixInstanceStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixPreFirstStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixPreLastStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixGetStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixSetStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixHelperStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixTypeStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;

			void _infixOpStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixOpArgStr( size_t arg_no, Stringifier& str, const std::vector<bool>& need_par ) const;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			NodeType Type{ NodeType::undef };

			// Either an action with child nodes or a value
			Object* Item{ nullptr };

			// Or, if we don't have an object, we must have a name
			name_t Name{ no_name };

			// Child nodes (arguments) for actions
			std::vector<Node> Children;
		};
	}
}
