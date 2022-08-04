#pragma once
#include <eontypes/DataTuple.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonfilesys/Path.h>
#include <eonfilesys/File.h>
#include <eonregex/RegEx.h>
#include <eonstring/Stringifier.h>


///////////////////////////////////////////////////////////////////////////////
//
//  The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	// Eon Data Tuple Class - eon::edt
	//
	// Load, save, and serialize DataTuple objects in EDT format + validate.
	//
	class edt
	{
		//////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		edt() = default;

		// Constructor for saving data tuple as EDT
		// User str() to save to string and save() to save to file
		inline edt( const DataTuple& tuple_to_save ) { ToSave = &tuple_to_save; }

		// Constructor for loading data tuple from EDT
		// Use load method to do the actual load
		inline edt( string source_name, string&& source ) {
			ToLoad = new source::String( source_name, std::move( source ) ); }
		inline edt( path source_path ) { ToLoad = new source::File( source_path.str() ); }

		virtual ~edt() { if( ToLoad ) delete ToLoad; };




		//////////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		// Save to string
		inline string str() { return ToSave ? _toStr( *ToSave ) : ""; }

		// Save to file
		inline void save( const path& target_file ) { file trg( target_file ); trg.save( str() ); }

		// Load data tuple
		// Optionally provide a reporter object to store issues that might be found in the source
		DataTuple load( source::Reporter& issues );
		inline DataTuple load() { source::Reporter issues; return load( issues ); }


		// Run validation on a data tuple
		// Parameters:
		//   target : Data tuple to validate
		//   source : Validation tuple
		//   issues : Data tuple in which to store issues when validation fails
		// Returns true if valid and false if not.
		bool validate( const DataTuple& target, const DataTuple& source, DataTuple* issues );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		inline string _toStr( const DataTuple& tuple ) {
			Stringifier str; _toStr( tuple, str, false ); return str.generateString(); }
		void _toStr( const DataTuple& tuple, Stringifier& str, bool comma_sep );

		void _load( source::Ref source, DataTuple& tuple, source::Reporter& reporter );

		void _loadTuple( DataTuple& tuple, TokenParser& parser, index_t indentation, bool end_by_close );
		void _loadAssignedChild( DataTuple& tuple, name_t child_name, TokenParser& parser );
		void _loadIndentedChild( DataTuple& tuple, name_t child_name, TokenParser& parser );
		void _loadFreeChild( DataTuple& tuple, TokenParser& parser );
		type::Object* _loadValue( TokenParser& parser );
		type::Object* _loadNegativeNumber( TokenParser& parser );
		type::Object* _loadTypeTuple( TokenParser& parser );
		type::Object* _loadTupleValue( TokenParser& parser );
		void _loadTypeTuple( EonType& tuple, TokenParser& parser );
		void _loadTypeTupleChild( EonType& tuple, name_t child_name, TokenParser& parser );

		index_t _indentation( TokenParser& parser );
		void _skipComments( TokenParser& parser );
		void _skipNewlines( TokenParser& parser );
		
		Tokenizer& _tokenizer();
		ReTokenizer& _retokenize();




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		const DataTuple* ToSave{ nullptr };
		source::Raw* ToLoad{ nullptr };

		source::Reporter* Reporter{ nullptr };
	};
};
