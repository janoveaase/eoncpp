#pragma once
#include <eonstring/String.h>
#include <eonname/Name.h>
#include <set>
#ifdef EON_WINDOWS
using HANDLE = void*;
using WORD = unsigned short;
#endif


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	// Styles that can be used with the Terminal
	enum class style
	{
		                // Foreground   Background
		                // -------------------------
		normal,         // default      default
		strong,         // enhanced     default
		weak,           // diminished   default
		black,          // black        default
		dark,           // dark gray    default
		bright,         // light gray   default
		white,          // white        default
		red,            // red          default
		green,          // green        default
		yellow,         // yellow       default
		blue,           // blue         default
		magenta,        // magenta      default
		cyan,           // cyan         default
		note,           // white        dark gray
		warning,        // white        dark yellow
		error,          // white        light red
		success,        // white        dark green
		quote,          // dark blue    light yellow
		reference,      // dark yellow  default
		code            // dark red     light gray
	};





	/**************************************************************************
	  Eon Terminal Class - eon::Terminal

	  This class replaces std::cout and std::cerr, adding styles and colors
	**************************************************************************/
	class Terminal
	{
		/**********************************************************************
		  Definitions
		**********************************************************************/
	public:

		// Level of functionality
		enum class level
		{
			non_functioning,		// Terminal is not accessible
			basic,					// Text only
			full					// Text and styles are possible
		};

		// Available colors
		enum class color
		{
			normal,
			black,					// Darkest possible color
			dark,					// Not as dark as black
			red_dark,				// Darkest red
			red_bright,				// Brightest red
			red,					// Best red color based on background
			green_dark,				// Darkest green
			green_bright,			// Brightest green
			green,					// Best green color based on background
			yellow_dark,			// Darkest yellow
			yellow_bright,			// Brightest yellow
			yellow,					// Best yellow color based on background
			blue_dark,				// Darkest blue
			blue_bright,			// Brightest blue
			blue,					// Best blue color based on background
			magenta_dark,			// Darkest magenta
			magenta_bright,			// Brightest magenta
			magenta,				// Best magenta color based on background
			cyan_dark,				// Darkest cyan
			cyan_bright,			// Brightest cyan
			cyan,					// Best cyan color based on background
			bright,					// Not as bright as white
			white					// Brightest possible color
		};




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		Terminal() = default;
		virtual ~Terminal() = default;




		/**********************************************************************
		  Read-only methods
		**********************************************************************/
	public:

		inline operator bool() const noexcept { return Level != level::non_functioning; }
		inline bool isFunctioning() const noexcept { return Level != level::non_functioning; }

		// Terminal view port
		inline size_t width() noexcept { if( !Init ) _checkPlatform(); return Width; }
		inline size_t height() noexcept { if( !Init ) _checkPlatform(); return Height; }

		// Terminal buffer
		inline size_t maxWidth() noexcept { if( !Init ) _checkPlatform(); return MaxWidth; }
		inline size_t maxHeight() noexcept { if( !Init ) _checkPlatform(); return MaxHeight; }

		// Cursor position
		inline size_t line() const noexcept { return CY; }
		inline size_t pos() const noexcept { return CX; }




		/**********************************************************************
		  Streaming methods
		**********************************************************************/
	public:

		//* Output string using current style
		Terminal& operator<<( string data );
		inline Terminal& operator<<( name_t name ) { return *this << *name; }
		inline Terminal& operator<<( const char* data ) { return *this << eon::string( data ); }
		inline Terminal& operator<<( const unsigned char* data ) { return *this << eon::string( data ); }
		inline Terminal& operator<<( const std::string& data ) { return *this << eon::string( data ); }
		template<typename T>
		inline Terminal& operator<<( T data ) { return *this << eon::string( data ); }

		//* Change current style
		//* NOTE: Once a style is set, it applies until changed!
		Terminal& operator<<( style term_style );

		//* Clear current style (default to normal settings)
		inline void clear() { *this << style::normal; }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _checkPlatform() noexcept;
		void _setColors() noexcept;



		
		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		bool Init{ false };
		level Level{ level::non_functioning };
		size_t CX{ 0 }, CY{ 0 };
		size_t Width{ 0 }, Height{ 0 };
		size_t MaxWidth{ 0 }, MaxHeight{ 0 };
		color Foreground{ color::normal };
		color Background{ color::normal };
		color FGNormal{ color::normal };
		color BGNormal{ color::normal };
		std::map<style, std::pair<color, color>> Styles;
		bool Bold = false;
		bool Err = false;

#ifdef EON_WINDOWS
		HANDLE Console{ NULL };
		std::map<color, WORD> FGColors, BGColors;
		color _fg( WORD raw, WORD& out );
		color _bg( WORD raw, WORD& out );
#endif
	};

	extern Terminal term;
}
