#pragma once

//#include "Definitions.h"
//#include "Action.h"
//#include "Operators.h"
#include <eontypesystem/Tuple.h>
#include "ExpressionNode.h"
//#include "DirectConversion.h"
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
	// Expression class - eon::Expression
	//
	class Expression
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct an empty expression.
		Expression() = default;

		// Construct by setting a root node.
		Expression( expression::Node&& root ) noexcept { Root = std::move( root ); }

		// Copy another expression
		inline Expression( const Expression& other ) { *this = other; }

		// Take ownership of an other expression's data
		inline Expression( Expression&& other ) noexcept { *this = std::move( other ); }

		~Expression() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Discard current details and copy those of another.
		inline Expression& operator=( const Expression& other ) { Root = other.Root; return *this; }

		// Discard current details and take ownership of those of another.
		inline Expression& operator=( Expression&& other ) noexcept { Root = std::move( other.Root ); return *this; }

		// Release root node (leaving the expression empty).
		inline expression::Node&& relaseRoot() noexcept { return std::move( Root ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check that the expression is something (not empty).
		inline operator bool() const noexcept { return Root; }

		// Get expression return type.
		inline TypeTuple type() const noexcept { return Root.type(); }

		// Get expression as an infix string representation.
		inline void str( Stringifier& strf ) const { return Root.str( strf ); }

		// Get expression as infix string representation.
		string str() const;




		///////////////////////////////////////////////////////////////////////
		//
		// Execution
		//
	public:

		// Run expression and get result.
		// Parameters:
		//   scope : A scope under which to run the expression.
		// The returned attribute will be empty if the expression is empty or does not return antyting.
		inline Attribute execute( Tuple& scope ) { return Root.evaluate( scope ); }




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		expression::Node Root;
	};
}
