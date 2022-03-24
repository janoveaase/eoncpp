#pragma once
#include <eonstring/String.h>
#include <eonstring/Name.h>
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
	extern string mapStyle( style s );





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
			black,
			white,
			red,
			green,
			yellow,
			blue,
			magenta,
			cyan
		};

		// Color intensity
		enum class intensity
		{
			bright,
			normal,
			dark
		};

		// Font style
		enum class fontstyle
		{
			normal,
			bold
		};

		// Color
		struct Color
		{
			Color() {}
			Color( color name, intensity intense = intensity::normal ) { Name = name; Intensity = intense; }
			color Name{ color::normal };
			intensity Intensity{ intensity::normal };
		};

		// Style details
		struct StyleDetails
		{
			StyleDetails() = default;
			StyleDetails( Color foreground, fontstyle fstyle = fontstyle::normal, Color background = Color() )
			{
				Foreground = foreground;
				FStyle = fstyle;
				Background = background;
			}
			Color Foreground;
			fontstyle FStyle{ fontstyle::normal };
			Color Background;
		};




		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		Terminal() = default;
		virtual ~Terminal() = default;


#ifdef EON_UNIX
		void setBackground( intensity style ) { Background = style; Init = false; }
#endif




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
		inline Terminal& operator<<( name_t name ) { return *this << eon::str( name ); }
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
		void _prepareStyles( intensity background );
		void _setStyle( const StyleDetails& details );



		
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
		color FGNormal{ color::normal };
		color BGNormal{ color::normal };
		std::map<style, StyleDetails> Styles;
		bool Bold = false;
		bool Err = false;

#ifdef EON_WINDOWS
		HANDLE Console{ NULL };
		std::map<color, WORD> FGColors, BGColors;
		Color _fg( WORD raw );
		Color _bg( WORD raw );
#else
		std::map<color, int> FGColors, BGColors;
		intensity Background{ intensity::bright };
#endif
	};

	extern Terminal term;
}
