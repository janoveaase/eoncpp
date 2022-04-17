#pragma once

#include "TypeSystem.h"
#include "OperatorAction.h"
#include "Scope.h"



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
			opr,			// Operator
			action,			// Action call
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
			inline Node( NodeType type, Object* item, source::Ref source ) { Type = type; Item = item; Source = source; }
			inline Node( name_t name, source::Ref source, bool define = false ) {
				Type = NodeType::name_only; Name = name; Source = source; Define = define; }

		public:
			inline Node( bool define = false ) { Define = define; }
			inline Node( const Node& other ) { *this = other; }
			inline Node( Node&& other ) noexcept { *this = std::move( other ); }
			virtual ~Node() { if( Item ) Item->die(); }

			Node& operator=( const Node& other );
			Node& operator=( Node&& other ) noexcept;
			
			Node& claim( Node&& other ) noexcept;
			Node& copy( const Node& other );




			/******************************************************************
			  Factory
			******************************************************************/

			static inline Node newOperator( actions::OperatorAction* op_action ) {
				return Node( NodeType::opr, (Object*)op_action, op_action->source() ); }
			static inline Node newAction( Action* action ) {
				return Node( NodeType::action, (Object*)action, action->source() ); }
			static inline Node newValue( Object* value ) { return Node( NodeType::value, value, value->source() ); }
			static inline Node newName( name_t name, source::Ref source, bool define = false ) {
				return Node( name, source, define ); }
			static inline Node newNameDefine( name_t name, source::Ref source ) { return Node( name, source, true ); }
			static inline Node newEllipsis() { return Node( name_ellipsis, source::Ref() ); }




			/******************************************************************
			  Node type methods
			******************************************************************/
		public:

			// The resulting type after full evaluation
			inline EonType resultType() const noexcept { return isOperator() ? opr().returnType()
				: isAction() ? action().returnType() : isName() ? EonType( name_name )
				: isEllipsis() ? EonType( name_ellipsis ) : isValue() ? value()->type() : EonType(); }

			inline NodeType type() const noexcept { return Type; }
			inline bool isOperator() const noexcept { return Type == NodeType::opr; }
			inline bool isAction() const noexcept { return Type == NodeType::action; }
			inline bool isValue() const noexcept { return Type == NodeType::value; }
			inline bool isName() const noexcept { return Type == NodeType::name_only; }
			inline bool isEllipsis() const noexcept { return Type == NodeType::name_only && Name == name_ellipsis; }

			// Get as string
			inline string str() const { Stringifier strf; infixStr( strf ); return strf.str(); }

			// Format as infix string
			void infixStr( Stringifier& str ) const;




			/******************************************************************
			  Node item methods
			******************************************************************/
		public:

			//* Get operator action object
			//* Throws [eon::type::NotFound] if not operator or no action
			inline actions::OperatorAction& opr() {
				if( isOperator() ) return *(actions::OperatorAction*)Item; else throw NotFound(); }
			inline const actions::OperatorAction& opr() const {
				if( isOperator() ) return *(actions::OperatorAction*)Item; else throw NotFound(); }

			//* Get action object
			//* Throws [eon::type::NotFound] if not action or no action
			inline Action& action() { if( isAction() ) return *(Action*)Item; else throw NotFound(); }
			inline const Action& action() const { if( isAction() ) return *(Action*)Item; else throw NotFound(); }

			//* Set action object for operator/action
			inline void action( Action& action ) {
				if( ( isAction() && action.actionType() != actions::Type::_operator )
					|| ( isOperator() && action.actionType() == actions::Type::_operator ) ) Item = &action; }

			//* Get operand object
			//* Throws [eon::type::NotFound] if not operand or no object
			inline const Object* value() const { if( Type == NodeType::value ) return Item; else throw NotFound(); }
			inline Object* value() { if( Type == NodeType::value ) return Item; else throw NotFound(); }
			inline Object* consumeValue() {
				if( Type == NodeType::value ) { auto item = Item; Item = nullptr; return item; } else throw NotFound(); }

			//* Get name
			//* Throws [eon::type::NotFound] if no name
			inline name_t name() const { if( Type == NodeType::name_only ) return Name; else throw NotFound(); }

			//* Get source
			inline const source::Ref& source() const noexcept { return Source; }




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
			inline const Node& child( size_t num ) const {
				if( num < Children.size() ) return Children[ num ]; else throw NotFound(); }




			/******************************************************************
			  Comparison
			******************************************************************/
		public:

			int compare( const Node& other ) const noexcept;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			std::vector<bool> _operandsNeedingParenthesis( const actions::OperatorAction& opr ) const;

//			// Get as postfix string
//			void postfixStr( Stringifier& str ) const;

			void _infixInstanceStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixPreFirstStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixPreLastStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixGetStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixSetStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixHelperStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixTypeStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
			void _infixStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;

			void _infixOpStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const;
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
			bool Define{ false };		// The name is defined here

			// Child nodes (arguments) for actions
			std::vector<Node> Children;

			source::Ref Source;
		};
	}
}
