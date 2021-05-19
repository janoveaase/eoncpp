#pragma once

#include <eontuple/Tuple.h>
#include <eontuple/TupleValue.h>
#include <eonfile/File.h>
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
	EONEXCEPT( NoSuchDoc );



	/**************************************************************************
	  Eon Tuple Format Class - eon::etf

	  Read tuple data from file or string, or save to file convert to string
	  Note that all ETF 'documents' (top level tuples) you load or parse, are
	  stored within the same etf object and can reference each other.
	**************************************************************************/
	class etf
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		etf() = default;
		inline etf( const etf& other ) { *this = other; }
		inline etf( etf&& other ) noexcept { *this = std::move( other ); }

		~etf() = default;


		inline etf& operator=( const etf& other ) {
			Docs = other.Docs; return *this; }
		inline etf& operator=( etf&& other ) noexcept {
			Docs = std::move( other.Docs ); return *this; }



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		//* Check if there are any documents at all
		inline bool empty() const noexcept { return Docs.empty(); }

		//* Get number of documents
		inline size_t numDocs() const noexcept { return Docs.numAttributes(); }

		//* A 'false' etf object is an empty one
		inline operator bool() const noexcept { return empty(); }

		//* Check if a document with a given name exists
		inline bool exists( name_t doc_name ) const noexcept {
			return static_cast<bool>( Docs.attribute( doc_name ) ); }

		//* Get a const document (top level tuple) by position (in order of
		//* appending)
		//* Throws [eon::NoSuchDoc] if position >= [eon::etf::numDocs()].
		inline const tuple& doc( size_t pos ) const {
			auto doc = Docs.attribute( pos ); if( doc ) return
				doc->tuple_value(); else throw NoSuchDoc(); }

		//* Get a const document (top level tuple) by name
		//* Throws [eon::NoSuchDoc] if no document has that name.
		inline const tuple& doc( name_t name ) const {
			auto dc = Docs.attribute( name ); if( dc ) return
				dc->tuple_value(); else throw NoSuchDoc(); }

		//* Get a const value given a path (tuple with nameless 'name'
		//* attributes)
		// NOTE: Will not resolve references!
		//* Returns the value, or 'false' if the path does not lead to an
		//* attribute.
		const tup::valueptr get( const tup::path& path ) const noexcept;
		inline const tup::valueptr get( std::initializer_list<name_t> path )
			const { return get( tup::path( path ) ); }

		//* Get a const value given a path (tuple with nameless 'name'
		//* attributes)
		// NOTE: Will resolve references!
		//* Returns the value, or 'false' if the path does not lead to an
		//* attribute. If a reference is detected that does not lead to an
		//* attribute, the reference is returned unresolved. In case of
		//* circular referencing, the first reference seen before during the
		//* same resolve sequence will be returned.
		const tup::valueptr getFinal( const tup::path& path ) const noexcept;
		inline const tup::valueptr getFinal(
			std::initializer_list<name_t> path ) const {
				return getFinal( tup::path( path ) ); }


		//* Run validation on a document, optionally specify a (loaded)
		//* 'pattern' document (for which all references in other pattern
		//* documents must also be loaded).
		//* Throws [eon::tup::Invalid] exception if one of 'max_depth',
		//* 'min_length', 'max_length', 'min', 'max', 'format, 'flags',
		//* or 'options' internal meta data element is violated, and if a
		//* 'pattern' document is violated.
		//* Returns true if the document was validated, false if the document
		//* could not be found!
		bool validate( name_t document, name_t pattern_document = no_name )
			const;




		/**********************************************************************
		  Write Methods
		**********************************************************************/
	public:

		//* Load one or more documents from a file
		//* Note that if the file contains only a single unnamed document and
		//* the base file name (without extenstion) is a valid Æon name, that
		//* name will be used for the document!
		//* Throws [eon::EtfBadSyntax] if formatting is incorrect.
		//* Throws [eon::EtfInvalid] if validation fails.
		void load( const path& file );

		//* Parse one or more documents from a string
		//* If you provide a document name, that name will be used if the
		//* string contains only a single, unnamed document.
		//* Throws [eon::EtfBadSyntax] if formatting is incorrect.
		//* Throws [eon::EtfInvalid] if validation fails.
		void parse( const string& str, name_t document_name = no_name );


		//* Set document by position
		//* Replaces existing document, appends a new if given position is at
		//* the end.
		inline void set( size_t doc_pos, tuple&& document ) { Docs.set(
			doc_pos, tup::valueptr( new tup::tupleval( std::move(
				document ) ) ) ); }

		//* Append a new document with or without a name
		//* Throws [eon::tuple::DuplicateName] if another document already
		//* exists with the same name.
		inline void append( tuple&& document, name_t name = no_name ) {
			Docs.append( tup::valueptr( new tup::tupleval( std::move(
				document ) ) ), name ); }

		//* Get a modifiable document (tuple) by name
		//* Throws [eon::NoSuchDoc] if no document has that name.
		inline tuple& attribute( name_t name ) {
			auto doc = Docs.attribute( name ); if( doc ) return
				doc->tuple_value(); else throw NoSuchDoc(); }

		//* Get a modifiable value given a path (tuple with nameless 'name'
		//* attributes)
		// NOTE: Will not resolve references!
		//* Returns the value, or 'false' if the path does not lead to an
		//* attribute
		tup::valueptr get( const tup::path& path ) noexcept;
		inline tup::valueptr get( std::initializer_list<name_t> path ) {
			return get( tup::path( path ) ); }

		//* Clear all documents
		inline void clear() noexcept { Docs.clear(); }




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _parse( name_t name, source& src );

		// Get attribute and it's parent
		std::pair<tuple*, tup::valueptr> _get( const tuple& root,
			const tup::path& path_to_attribute, size_t first_path_elm ) const;




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		// All the documents of this object
		tuple Docs;
	};
}
