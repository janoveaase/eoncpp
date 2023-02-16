#pragma once
#include "SourceRef.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	namespace source
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Message Severity enum - eon::source::severity
		//
		// Messages are marked with a severity level.
		//
		enum class severity : uint8_t
		{
			fatal,		// So severe that all source handling must be aborted
			error,		// So severe that handling of current source element must be aborted
			serious,	// Severe and will possibly lead to future error/fatal
			warning,	// Severe, but probably won't lead to future error/fatal
			note,		// Point of interest, but not wrong in its own right
			info,		// Mainly for debugging parsers/scanners/etc.
			none		// For filtering only
		};
		extern string severityToString( severity value );




		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Reporter Message Class - eon::source::Message
		//
		// A source reporter message contains a severity level, a text string,
		// and a [eon::source::Ref] object. The latter identifies in which
		// source and where in that source the message is concerning.
		//
		class Message
		{
		public:
			Message() = default;
			inline Message( severity level, string&& text, Ref src ) noexcept {
				Level = level; Text = std::move( text ); Source = src; }
			inline Message( severity level, const string& text, Ref src ) { Level = level; Text = text; Source = src; }
			~Message() = default;

			inline severity level() const noexcept { return Level; }
			inline const string& text() const noexcept { return Text; }
			inline const Ref& source() const noexcept { return Source; }

			// Get the message a formatted string
			string str() const;

		PRIVATE:
			string _formatSource() const;
			string _formatPosition() const;
			string _formatSourceIndicator() const;

			class SourceIndicator
			{
			public:
				SourceIndicator() = default;
				SourceIndicator( Ref source );
				string format();
			PRIVATE:
				bool _write( string& str );
				string _startLine();
				string _writeIndicator();
				size_t _selectLineLength();
			PRIVATE:
				Ref Source;
				bool Newline{ false };
				Pos Start;
				size_t LineNumDigits{ 0 };
				Ref Line;
				int I{ 0 };
			};

		PRIVATE:
			severity Level{ severity::none };
			string Text;
			Ref Source;
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Message Target Class - eon::source::Target
		//
		// Super class for targets of source messages
		//
		class MessageTarget
		{
		public:

			// Construct target allowing for all messages
			// This is usually too verbose!
			MessageTarget() = default;

			// Construct target allowing only for messages at specified
			// severity level range.
			inline MessageTarget( severity filter_high, severity filter_low = severity::note ) noexcept {
				FilterHigh = filter_high; FilterLow = filter_low; }

			virtual ~MessageTarget() = default;


			// This method is called whenever a message at or more severe than
			// the filter level
			virtual void report( const Message& message ) = 0;


			// Check the filter level
			inline severity filterHigh() const noexcept { return FilterHigh; }
			inline severity filterLow() const noexcept { return FilterLow; }


		protected:
			severity FilterHigh{ severity::fatal };
			severity FilterLow{ severity::info };
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Default [eon::source::MessageTarget] classes
		//
		class StdOut : public MessageTarget
		{
		public:
			StdOut( severity filter_high = severity::warning, severity filter_low = severity::note ) noexcept
				: MessageTarget( filter_high, filter_low ) {}
			void report( const Message& message ) override { std::cout << message.str() << "\n"; }
		};
		class StdErr : public MessageTarget
		{
		public:
			StdErr( severity filter_high = severity::fatal, severity filter_low = severity::serious ) noexcept
				: MessageTarget( filter_high, filter_low ) {}
			void report( const Message& message ) override { std::cerr << message.str() << "\n"; }
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Reporter Class - eon::source::Reporter
		//
		// A source reporter is used to report messages related to source data.
		//
		// The reporter can be configured to send messages directly to stdout
		// and/or stderr, or to some specified target (an object receiving the
		// messages). (Multiple targets are possible.)
		//
		// The reporter will store all fatal, error, serious and warning
		// messages internally for access at any time.
		//
		class Reporter
		{
		public:

			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//

			// Default constructor, no source reference
			Reporter() = default;


			// Default destructor
			~Reporter() = default;




			///////////////////////////////////////////////////////////////////
			//
			// Modifier Methods
			//

			// Register an externally owned object as new target
			// WARNING: No checking is done to see if the same target has
			//          already been added!
			inline Reporter& add( MessageTarget& target ) { Targets.push_back( Target( &target, false ) ); return *this; }

			// Register newly created object as new target and take ownership
			// WARNING: No checking is done to see if the same target has
			//          already been added!
			inline Reporter& add( MessageTarget* target ) { Targets.push_back( Target( target, true ) ); return *this; }

			// Set stdout as default target for notices and warnings, and
			// stderr for fatal, error and serious, ignoring info messages
			inline Reporter& defaultTargets() { return add( new StdOut() ).add( new StdErr() ); }




			///////////////////////////////////////////////////////////////////
			//
			// Reporting
			//

			// Report a message for a specific level
			inline void fatal( string&& text, Ref source ) { report( severity::fatal, std::move( text ), source ); }
			inline void error( string&& text, Ref source ) { report( severity::error, std::move( text ), source ); }
			inline void serious( string&& text, Ref source ) { report( severity::serious, std::move( text ), source ); }
			inline void warning( string&& text, Ref source ) { report( severity::warning, std::move( text ), source ); }
			inline void note( string&& text, Ref source ) { report( severity::note, std::move( text ), source ); }
			inline void info( string&& text, Ref source ) { report( severity::info, std::move( text ), source ); }


			// Report a message with all details specified
			void report( severity level, string&& text, Ref source );




		private:
			struct Target
			{
				Target() = default;
				Target( MessageTarget* trg, bool owned ) { Trg = trg; Owned = owned; }
				Target( const Target& ) = delete;
				Target( Target&& other ) noexcept { *this = std::move( other ); }
				virtual ~Target() { if( Trg && Owned ) delete Trg; }

				Target& operator=( const Target& ) = delete;
				inline Target& operator=( Target&& other ) noexcept {
					Trg = other.Trg; Owned = other.Owned; if( Owned ) { other.Trg = nullptr; other.Owned = false; }
					return *this; }
				MessageTarget* Trg{ nullptr };
				bool Owned{ false };
			};
			std::vector<Target> Targets;
			std::vector<Message> Issues;	// fatal, error, serious, warning
		};
	}
}
