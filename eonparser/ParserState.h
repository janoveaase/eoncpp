#pragma once

#include <eontypesystem/Tuple.h>
#include <eontypesystem/TupleFactory.h>
#include <eontypesystem/Operators.h>
#include <eonexpression/Expression.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonfilesys/Path.h>
#include <eontokenizer/TokenParser.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eoninlinetest/InlineTest.h>
#ifdef EON_TEST_MODE
#	include <eontypesystem/BoolActions.h>
#	include <eontypesystem/ByteActions.h>
#	include <eontypesystem/CharActions.h>
#	include <eontypesystem/IntActions.h>
#	include <eontypesystem/FloatActions.h>
#	include <eontypesystem/IndexActions.h>
#	include <eontypesystem/NameActions.h>
#	include <eontypesystem/HandleActions.h>
#	include <eontypesystem/BytesActions.h>
#	include <eontypesystem/StringActions.h>
#	include <eontypesystem/BitsActions.h>
#	include <eontypesystem/TupleActions.h>
#endif



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'parser' namespace encloses all background parser functionality
	//
	namespace parser
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Parser State class - eon::parser::State.
		//
		// A data object used by parser to maintain state while parsing.
		//
		class State
		{
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//
		public:

			State() = default;

			inline explicit State( string&& input_string, source::Reporter& reporter ) {
				Source = std::make_shared<source::String>( "string", std::move( input_string ) ); _initialize( reporter ); }
			inline explicit State( path input_file, source::Reporter& reporter ) {
				Source = std::make_shared<source::File>( input_file.str() ); _initialize( reporter ); }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			void _initialize( source::Reporter& reporter );
			void _prepTokenizer( Tokenizer& tokenizer );
			void _prepReTokenizer( ReTokenizer& retokenizer );




			///////////////////////////////////////////////////////////////////////
			//
			// State
			//
		public:

			std::shared_ptr<source::Raw> Source;
			TokenParser Tokens;

			source::Reporter* Report{ nullptr };
			Tuple* Scope{ nullptr };
		};


#ifdef EON_TEST_MODE
		class TestState
		{
		public:
			TestState() = default;
			inline TestState( string&& input_string ) {
				resetGlobal(); Report.defaultTargets(); Stat = State( std::move( input_string ), Report ); }
			inline TestState( path input_file ) {
				resetGlobal(); Report.defaultTargets(); Stat = State( input_file, Report ); }

			inline operator State&() noexcept { return Stat; }
			inline State& state() noexcept { return Stat; }

			static void resetGlobal()
			{
				scope::global().clear();
				type::registerBool();
				type::registerByte();
				type::registerChar();
				type::registerIndex();
				type::registerName();
				type::registerHandle();
				type::registerInt<int_t>();
				type::registerInt<short_t>();
				type::registerInt<long_t>();
				type::registerFloat<flt_t>();
				type::registerFloat<low_t>();
				type::registerFloat<high_t>();
				type::registerBits<b8_t>();
				type::registerBits<b16_t>();
				type::registerBits<b32_t>();
				type::registerBits<b64_t>();
				type::registerBytes();
				type::registerString();
				type::registerTupleActions();
			}

		private:
			State Stat;
			source::Reporter Report;
		};
#endif
	}
}
