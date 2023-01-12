#pragma once

#include "Definitions.h"
#include "Action.h"
#include "Operators.h"
#include "Tuple.h"
#include <eonsource/File.h>
#include <eonsource/String.h>
#include <eonsource/SourceRef.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eonstring/NamePath.h>
#include <eonfilesys/Path.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'expression' namespace encloses all private Expression functionality
	//
	namespace expression
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Expression Node Class - eon::expression::Node
		//
		class Node
		{
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//
		public:

			// Construct an empty node - a place-holder.
			Node() = default;

			// Copy another node.
			inline Node( const Node& other ) { *this = other; }

			// Take ownership of another node's details.
			inline Node( Node&& other ) noexcept { *this = std::move( other ); }

			// Default destruction
			~Node() = default;




			///////////////////////////////////////////////////////////////////
			//
			// Factory methods
			//
		public:

			// Construct a value node
			static Node newValue( Attribute&& value, name_t name = no_name );

			// Construct an operator node
			static Node newOperator( name_t name, source::Ref source );

			// Construct an action call node
			static Node newCall( const type::Action& action, source::Ref source );




			///////////////////////////////////////////////////////////////////
			//
			// Assignment operators
			//
		public:

			// Discard details and copy those of another node.
			inline Node& operator=( const Node& other ) {
				Name = other.Name; Type = other.Type; Value = other.Value; Source = other.Source;
				Executor = other.Executor; Children = other.Children; return *this; }

			// Discard details and take ownership of those of another node.
			inline Node& operator=( Node&& other ) noexcept {
				Name = other.Name; other.Name = no_name; Type = std::move( other.Type );
				Value = std::move( other.Value ); Source = other.Source;
				Executor = other.Executor; other.Executor = nullptr; Children = std::move( other.Children ); return *this; }




			///////////////////////////////////////////////////////////////////
			//
			// Basic read-only
			//
		public:

			// Check if the node has something (isn't empty).
			inline operator bool() const noexcept { return Name != no_name || static_cast<bool>( Value ); }

			// Check if this is an operator node.
			inline bool isOperator() const noexcept { return Name != no_name && Name != name_call && !Value; }

			// Check if this is a call node.
			inline bool isCall() const noexcept { return Name == name_call; }

			// Check if this is a value node.
			inline bool isValue() const noexcept { return static_cast<bool>( Value ); }


			// Get node name:
			//   - Name/symbol of operator
			//   - eon::name_call if action call
			//   - Name/eon::no_name if value
			//   - eon::no_name if nothing
			inline name_t name() const noexcept { return Name; }

			// Get node type (return type of operator/action call, value type otherwise)
			inline TypeTuple type() const noexcept { return Type; }

			// Get node value (void if not a value node).
			inline Attribute& value() noexcept { return Value; }

			// Get node value (void if not a value node).
			inline const Attribute& value() const noexcept { return Value; }

			// Get node source details.
			inline const source::Ref& source() const noexcept { return Source; }
			inline source::Ref& source() noexcept { return Source; }




			///////////////////////////////////////////////////////////////////
			//
			// Operations
			//
		public:

			// Set source
			inline void source( source::Ref source ) noexcept { Source = source; }

			// Add an operand/argument to operator/action call node.
			inline void addOperand( Node&& opr ) { Children.push_front( std::move( opr ) ); }

			// Find and bind operator action implementation to this operator node.
			// Returns true if bound and false if not, in which case an error is reported!
			inline bool bindOperator( Tuple& scope, source::Reporter& reporter ) {
				return Executor != nullptr ? true : _bindOperator( scope, reporter ); }

			// Evaluate operator/action call node and return value result.
			Attribute evaluate( Tuple& scope );

			// Express node in string format.
			void str( Stringifier& strf ) const;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			void _evaluateChildren( Tuple& scope, type::ActionExeArgs& args );
			Attribute _executionResult( type::ActionExeArgs& args );

			bool _bindOperator( Tuple& scope, source::Reporter& reporter );
			bool _wrongNumberOfOperands( source::Reporter& reporter );
			bool _childrenHaveOperatorsAndReturnTypes( Tuple& scope, source::Reporter& reporter );
			bool _findTypeImplementedOperator( Tuple& scope, source::Reporter& reporter );
			bool _isBinaryComparisonOperator();
//			TypeTuple _getInstanceType();
			TypeTuple _getOperatorArgs();
			void _resolveTrivialMatch( Tuple& scope, TypeTuple action );
			void _resolveOnReturnType( Tuple& scope, std::set<TypeTuple>& possible_actions );
			void _resolveBinaryComparisonOperator( Tuple& scope, TypeTuple instance_type, TypeTuple operator_args );
			void _handleAliasComparisonOperator( Tuple& scope, TypeTuple action );
			void _insertOperatorCmp( Tuple& scope, TypeTuple action );
			void _insertOriginalOperatorForInt( Tuple& scope );

			void _writeOperator( Stringifier& strf ) const;
			void _writeCall( Stringifier& strf ) const;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			name_t Name{ no_name };
			TypeTuple Type;
			Attribute Value;
			source::Ref Source;
			const type::Action* Executor{ nullptr };
			std::list<Node> Children;
		};
	}
}
