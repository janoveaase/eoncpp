#pragma once
#include <locale>
#include "UniChar.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// The utf-8 local-spefication will vary on different platforms.
#ifdef EON_APPLE
#	define _UTF8 ".UTF-8"
#else
#	define _UTF8 ".utf8"
#endif

	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Locale is mostly identical std::locale, only including some
	// additional details.
	//
	// When Eon Locale is set, it applies to all Eon methods relying on locale
	// details. (Transformation of case, comparing, decimal separator, etc.)
	//
	// Default locale is "en_US.utf8"!
	//
	// NOTE: Eon will never set the 'global' locale, nor will it respect it either!
	//
	class locale
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		inline locale() { _set( "en_US" _UTF8 ); }
		inline locale( std::string name ) { _set( name ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Eon default locale
		//
		// Eon methods for which a locale can be specified will use a default
		// locale when no specific locale is given. This locale is thread-
		// local, meaning that it uses the default '"en_US" _UTF8' in every new
		// thread, unless explicitly set to something else.
		//
	public:

		// Set a (new) default locale for Eon.
		// Applies only to the current thread!
		static inline void set( locale new_default ) noexcept { CurLocale = std::move( new_default ); }

		// Get the current default locale (for the current thread).
		static const locale& get() noexcept { return CurLocale; }




		///////////////////////////////////////////////////////////////////////
		//
		// Generic access
		//
	public:

		inline std::string name() const noexcept { return Loc.name(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparing
		//
		// The C++ std::locale (std::collate) is not suited for comparing UTF-8
		// strings and therefore not suited for Eon strings - which are pre-
		// dominantly UTF-8. Instead, comparing is done in [eon::substring]
		// using binary that users requiring custom sorting can provide, while
		// default predicates are used otherwise.
		//
		// eon::locale therefore has no compare methods!
		//
		///////////////////////////////////////////////////////////////////////




		///////////////////////////////////////////////////////////////////////
		//
		// Numerical
		//
		// All of these methods are using standard C++ functionality.
		//
	public:

		// Get character separating the integer part from the fractional part
		// in general floating point numbers.
		inline char_t decimalSep() const { return Num->decimal_point(); }

		// Get character separating groups of thousands when expanding.
		// E.g., "1234567" to "1,234,567".
		inline char_t thousandsSep() const { return Num->thousands_sep(); }

		// Get digit groupings (typically for placing [thousandsSeperator] in-between).
		// The returned std::string will contain raw bytes of which each shall be interpreted as an integer.
		// The first integer applies to the right-most group, the second to the second right-most group, and so on.
		// The last integer (which may also be the first) applies to any subsequent groups.
		inline std::string grouping() const { return Num->grouping(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Transformations
		//
		// All of these methods are using standard C++ functionality.
		//
	public:

		// Get upper-case version of specified character.
		// Returns the same character if no upper-case version exists!
		inline wchar_t toUpper( wchar_t value ) const { return CType->toupper( value ); }

		// Get lower-case version of specified character.
		// Returns the same character if no lower-case version exists!
		inline wchar_t toLower( wchar_t value ) const { return CType->tolower( value ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Monetary
		//
		// All of these methods are using standard C++ functionality.
		//
	public:

		// Get character separating the integer part from the fractional part
		// in monetary floating point numbers.
		inline char_t decimalSepMoney() const { return Money->decimal_point(); }

		// Get character separating groups of thousands in monetary amounts
		// when expanding. E.g., "1234567" to "1,234,567".
		inline char_t thousandsSepMoney() const { return Money->thousands_sep(); }

		// Get digit groupings for monetary amounts (typically for placing [thousandsSeperator] in-between).
		// The returned std::string will contain raw bytes of which each shall be interpreted as an integer.
		// The first integer applies to the right-most group, the second to the second right-most group, and so on.
		// The last integer (which may also be the first) applies to any subsequent groups.
		inline std::string groupingMoney() const { return Money->grouping(); }

		// Get the currency symbol.
		inline std::wstring currencySymbol() const { return Money->curr_symbol(); }

		// Get the 'positive' sign for currency.
		inline std::wstring positiveSign() const { return Money->positive_sign(); }

		// Get the 'negative' sign for currency.
		inline std::wstring negativeSign() const { return Money->negative_sign(); }

		// Get the preferred number of fractional digits in monetary amounts.
		inline int fractionalDigits() const { return Money->frac_digits(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _set( std::string name );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::locale Loc;

		const std::collate<wchar_t>* Coll{ nullptr };
		const std::numpunct<wchar_t>* Num{ nullptr };
		const std::ctype<wchar_t>* CType{ nullptr };
		const std::moneypunct<wchar_t>* Money{ nullptr };

		static thread_local locale CurLocale;
	};
}
