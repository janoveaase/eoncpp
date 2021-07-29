#pragma once
#include <eontokenizer/Tokenizer.h>
#include <eonexcept/Exception.h>
#include <map>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Exception used when attempting to access a non-existing node.
	**************************************************************************/
	EONEXCEPT( NoSuchNode );

	/**************************************************************************
	  Exception used when attempting to add a node with the same name as one
	  added before.
	**************************************************************************/
	EONEXCEPT( DuplicateNodeName );




	/**************************************************************************
	  Eon Documentation Node Class - eon::docnode

	  Each element to be documented is represented as a documentation node.
	  A node can have many sub-nodes, but only one parent node. A 'root' node
	  represents the highest (imlicit) level under which all documentation
	  nodes for a project resides.
	**************************************************************************/
	class docnode
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, void node
		docnode() = default;

		//* Copy and move are disabled
		docnode( const docnode& ) = delete;
		docnode( docnode&& ) = delete;

		//* Destruction
		virtual ~docnode() {
			for( auto& child : Children ) delete child.second; }




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Add a new child node to 'this'
		//* Returns the added node, or throws [eon::DuplicateNodeName] if
		//* another node with the same name already exists as child of 'this'.
		docnode& add( string&& name );




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
		
		//* Check if a named child node exists
		inline bool exists( const string& name ) const noexcept {
			return Children.find( name ) != Children.end(); }

		//* Get a named child node
		//* Throws [eon::NoSuchNode] if no child with that name.
		docnode& child( const string& name ) const;




	private:
		docnode* Parent{ nullptr };
		std::map<string, docnode*> Children;
		string TitleLine;
		string Documentation;
	};
};
