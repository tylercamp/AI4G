

#include <SFML\Graphics.hpp>
#include <cstdarg>
#include <iostream>

#include "Debug.h"



			//	Text buffer to be rendered to the screen
std::string	g_DebugText;
			//	Font to be used for text rendering
sf::Font	g_RenderFont;



// Takes the given string reference and evaluates the formatting via the variadic parameters
//	that are assumed to follow it. Not suitable for arbitrary formatting.
std::string EvaluateFormattedText( std::string & format )
{
	static char printBuffer [0x40000];

	va_list args;
	va_start(args, format);

	vsprintf_s (printBuffer, format.c_str (), args);

	va_end(args);

	return printBuffer;
}



/*
=================
Debug::Error

Reports an error by displaying a message box for the user to interact with.
=================
 */
void Debug::Error( std::string format, ... )
{
	std::string errorString;
	errorString += "Debug::Error invoked:\n\n";
	errorString += EvaluateFormattedText(format);
	errorString += "\n\nWould you like to run a debug break?";

	//	Display the error to the user
	int result = MessageBoxA(NULL, errorString.c_str(), "Debug::Error", MB_APPLMODAL | MB_YESNOCANCEL);

	//	Act based on their intent
	switch (result)
	{
	case (IDYES):
		{
			__debugbreak();
			break;
		}

	case (IDNO):
		{
			break;
		}

	case (IDCANCEL):
		{
			exit (0);
			break;
		}

	default:
		break;
	}
}

/*
=================
Debug::Warning
=================
 */
void Debug::Warning( std::string format, ... )
{
	std::cout << EvaluateFormattedText(format) << std::endl;
}

/*
=================
Debug::SetFont

Sets the font file (TTF) to be used for text rendering.
=================
 */
void Debug::SetFont (std::string font)
{
	g_RenderFont.LoadFromFile (font, 14);
}

/*
=================
Debug::Printf

Adds the given text to the list of text to be drawn.
Reference: http://www.cplusplus.com/reference/cstdio/printf/
=================
 */
void Debug::Printf (std::string format, ...)
{
	if (g_DebugText.size () != 0)
		g_DebugText += "\n";

	g_DebugText += EvaluateFormattedText(format);
}

/*
=================
Debug::DrawDebugOutput

Draws the text sent by Debug::Printf at the specified position and clears the
internal text record.
=================
 */
void Debug::DrawDebugOutput (sf::RenderWindow * targetWindow, float x, float y)
{
	sf::String renderString ("-- DEBUG --\n" + g_DebugText);
	renderString.SetFont (g_RenderFont);
	renderString.SetColor (sf::Color::White);
	renderString.SetSize (14);

	renderString.SetPosition (x, y);

	targetWindow->Draw (renderString);

	g_DebugText = "";
}