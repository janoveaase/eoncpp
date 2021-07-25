#pragma once

#include <eontuple/Tuple.h>
#include <eontuple/TupleValue.h>
#include <eonfilesys/FileSys.h>
#include <eonsource/Source.h>
#include "Parser.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception throw if an EFT document does not pass validation
	EONEXCEPT( EtfInvalidation );

	//* Exception thrown when trying to access a non-existing document
	EONEXCEPT( NoSuchEtfDoc );



	/**************************************************************************
	  Eon Object Format Class - eon::eonof

	  Read tuple data from file or string, or save to file or to string.
	  Note that all ETF 'documents' (top level tuples) you load or parse, are
	  stored within the same eonof object and can reference each other.
	**************************************************************************/
	class eonof
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		eonof() = default;
		inline eonof( const eonof& other ) { *this = other; }
		inline eonof( eonof&& other ) noexcept { *this = std::move( other ); }

		~eonof() = default;




		/**********************************************************************
		  Copy and Move
		**********************************************************************/
	public:

		inline eonof& operator=( const eonof& other ) {
			Docs = other.Docs; return *this; }
		inline eonof& operator=( eonof&& other ) noexcept {
			Docs = std::move( other.Docs ); return *this; }




		/**********************************************************************
		  Loading Documents
		**********************************************************************/
	public:

		//* Load one or more documents from a file
		//* Note that if the file contains only a single unnamed document and
		//* the base file name (without extenstion) is a valid �on name, that
		//* name will be used for the document!
		//* Exclusive: Existing documents with same name will be replaced!
		//* Merge    : New documents with same name as existing will be merged
		//*            into existing, adding new elements and overwriting
		//*            existing common elements.
		//* Throws [eon::EtfBadSyntax] if formatting is incorrect.
		//* Throws [eon::EtfInvalid] if validation fails.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		void loadExclusive( const path& file );
		void loadMerge( const path& file );

		//* Parse one or more documents from a string
		//* If you provide a document name, that name will be used if the
		//* string contains only a single, unnamed document.
		//* Exclusive: Existing documents with same name will be replaced!
		//* Merge    : New documents with same name as existing will be merged
		//*            into existing, adding new elements and overwriting
		//*            existing common elements.
		//* Throws [eon::EtfBadSyntax] if formatting is incorrect.
		//* Throws [eon::EtfInvalid] if validation fails.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		void parseExclusive( const string& str,
			name_t document_name = no_name );
		void parseMerge( const string& str, name_t document_name = no_name );



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if there are any documents at all
		inline bool empty() const noexcept { return Docs.empty(); }

		//* Get number of documents
		inline size_t numDocs() const noexcept { return Docs.numAttributes(); }

		//* A 'false' eonof object is an empty one
		inline operator bool() const noexcept { return empty(); }

		//* Check if a document with a given name exists
		inline bool exists( name_t doc_name ) const noexcept {
			return static_cast<bool>( Docs.attribute( doc_name ) ); }

		//* Get a const document (top level tuple) by position (in order of
		//* appending)
		//* Throws [eon::NoSuchEtfDoc] if position >= [eon::eonof::numDocs()].
		inline const tuple& doc( size_t pos ) const {
			auto doc = Docs.attribute( pos ); if( doc ) return
				doc->hardTuple(); else throw NoSuchEtfDoc(); }

		//* Get a const document (top level tuple) by name
		//* Throws [eon::NoSuchEtfDoc] if no document has that name.
		inline const tuple& doc( name_t name ) const {
			auto dc = Docs.attribute( name ); if( dc ) return
				dc->hardTuple(); else throw NoSuchEtfDoc(); }

		//* Get name of document at given position
		//* Returns no_name if not named
		inline name_t name( size_t pos ) const { return Docs.name( pos ); }

		//* Get metadata for named document
		//* Returns 'false' tupleptr if not found or no metadata
		inline tupleptr metadata( name_t name ) const noexcept {
			return Docs.metadata( name ); }


		//* Run validation on a document, optionally specify a (loaded)
		//* 'pattern' document (for which all references in other pattern
		//* documents must also be loaded).
		//* Throws [eon::tup::Invalid] exception if one of 'max_depth',
		//* 'min_length', 'max_length', 'min', 'max', 'format, 'flags',
		//* or 'options' internal meta data element is violated, and if a
		//* 'pattern' document is violated.
		//* Returns true if the document was validated, false if the document
		//* could not be found!
		bool validate( name_t document, name_t pattern_document = no_name );

		//* Access the variables used by documents
		inline const tup::variables& variables() const noexcept {
			return Vars; }




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		//* Get a modifiable document (top level tuple) by position (in order
		//* of appending)
		//* Throws [eon::NoSuchEtfDoc] if position >= [eon::eonof::numDocs()].
		inline tuple& doc( size_t pos ) {
			auto doc = Docs.attribute( pos ); if( doc ) return *(tuple*)&doc
				->hardTuple(); else throw NoSuchEtfDoc(); }

		//* Get a modifiable document (top level tuple) by name
		//* Throws [eon::NoSuchEtfDoc] if no document has that name.
		inline tuple& doc( name_t name ) {
			auto dc = Docs.attribute( name ); if( dc ) return *(tuple*)&dc
				->hardTuple(); else throw NoSuchEtfDoc(); }


		//* Set document by position
		//* Replaces existing document, appends a new if given position is at
		//* the end.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		inline void set( size_t doc_pos, tuple&& document ) {
			Docs.set( doc_pos, tup::valueptr(
				new tup::tupleval( std::move( document ) ) ), Vars ); }

		//* Append a new document with or without a name
		//* Throws [eon::tuple::DuplicateName] if another document already
		//* exists with the same name.
		//* Throws [eon::tup::CircularReferencing] if circular referencing is
		//* detected!
		inline void append( tuple&& document, name_t name = no_name ) {
			Docs.append( tup::valueptr(
				new tup::tupleval( std::move( document ) ) ), Vars, name ); }

		//* Clear all documents
		inline void clear() noexcept { Docs.clear(); }

		//* Access the variables used by documents
		inline tup::variables& variables() noexcept { return Vars; }




		/**********************************************************************
		  Searching

		  NOTE: Finding an attribute that is a reference will result in the
		        target being found, not the reference itself. To access the
				reference and not the target, find the parent tuple of the
				reference attribute and then use the 'attribute' method to
				access the reference value.
		**********************************************************************/
	public:

		//* Find attribute at specified path
		//* Returns 'false' if the path does not lead to an existing attribute.
		inline tup::valueptr find( const tup::path& path ) const noexcept {
			return Docs.find( path ); }
		inline tup::valueptr find( std::initializer_list<name_t> path )
			const noexcept { return Docs.find( tup::path( path ) ); }




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _parse( name_t name, source& src, bool merge );




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		// All the documents of this object
		tuple Docs;

		// Pattern documents
		std::set<name_t> Patterns;

		// Variables
		tup::variables Vars;
	};
}
