#pragma once

#include "Document.h"
#include <eonfile/File.h>
#include "Parser.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown when trying to access a non-existing document
	EONEXCEPT( NoSuchEDoc );



	/**************************************************************************
	  Eon Document Format Class - eon::edoc

	  Read EDOC data from file or string, or save to file or to string.
	  NOTE: You can read multiple documents into the same edoc object!
	**************************************************************************/
	class edoc
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		edoc() = default;
		edoc( const edoc& ) = delete;
		inline edoc( edoc&& other ) noexcept { *this = std::move( other ); }

		~edoc() = default;


		edoc& operator=( const edoc& ) = delete;
		inline edoc& operator=( edoc&& other ) noexcept {
			Docs = std::move( other.Docs ); return *this; }



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if there are any documents at all
		inline bool empty() const noexcept { return Docs.empty(); }

		//* Get number of documents
		inline size_t numDocs() const noexcept { return Docs.size(); }

		//* A 'false' edoc object is an empty one
		inline operator bool() const noexcept { return empty(); }

		//* Check if a document with a given name exists
		inline bool exists( const string& doc_name ) const noexcept {
			return static_cast<bool>( Docs.find( doc_name ) != Docs.end() ); }

		//* Get document by name
		//* Throws [eon::NoSuchEDoc] if not found.
		inline const doc::document& doc( const string& name ) const {
			auto found = Docs.find( name ); if( found != Docs.end() ) return
				found->second; else throw NoSuchEDoc( name ); }


		//* Get a document by name, formatted as an EDOC string (that can be
		//* saved to file or handled otherwise)
		//* Throws [eon::NoSuchEDoc] if not found.
		inline string str( const string& name ) const {
			auto found = Docs.find( name ); if( found != Docs.end() ) return
				found->second.str( 79, 0 ); else throw NoSuchEDoc( name ); }




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		//* Load a document from a file
		//* The file base name will be used as document name, unless an
		//* override name is specified.
		//* Throws [eon::edocBadSyntax] if formatting is incorrect.
		void load( const path& file, const string& override_name = "" );

		//* Parse a document from a string
		//* You must provide a name for the document!
		//* Throws [eon::edocBadSyntax] if formatting is incorrect.
		void parse( const string& str, const string& document_name = "" );

		//* Add an explicit document
		//* You must provide a name for the document!
		//* WARNING: If another document has been added with the same name,
		//* that document will be replaced!
		inline void add( doc::document&& doc, const string& name ) {
			Docs[ name ] = std::move( doc ); }

		//* Clear all documents
		inline void clear() noexcept { Docs.clear(); }




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		doc::document _parse( const string& name, source& src );




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		// All the documents of this object
		std::map<string, doc::document> Docs;
	};
}
