#pragma once

#include <eontypesystem/TupleFactory.h>



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
		// Eon Action finder - eon::parser::ActionFinder.
		//
		// Find an action.
		//
		class ActionFinder
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Structure to use when finding actions.
			class Details
			{
			public:
				inline Details( source::Reporter& reporter, source::Ref source ) noexcept {
					Report = &reporter; Source = source; }

				inline Details& actionName( name_t name ) noexcept { ActionName = name; return *this; }
				inline Details& category( name_t cls ) noexcept { Category = cls; return *this; }
				inline Details& typeName( name_t name ) noexcept { TypeName = name; return *this; }
				inline Details& args( TypeTuple args ) noexcept { Args = std::move( args ); return *this; }

				inline source::Reporter& reporter() const noexcept { return *Report; }
				inline const source::Ref& source() const noexcept { return Source; }
				inline name_t actionName() const noexcept { return ActionName; }
				inline name_t category() const noexcept { return Category; }
				inline name_t typeName() const noexcept { return TypeName; }
				inline const TypeTuple& args() const noexcept { return Args; }

			private:
				source::Reporter* Report{ nullptr };
				source::Ref Source;
				name_t ActionName{ no_name };
				name_t Category{ no_name };
				name_t TypeName{ no_name };
				TypeTuple Args;
			};

			// Find an action.
			// Paramters:
			//   details : Details about action to find.
			// Returns full type of found action, empty type for not found or too many found.
			TypeTuple find( Details& details ) const;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			TypeTuple _notFound( Details& details, std::set<TypeTuple>&& possible ) const;
		};
	}
}
