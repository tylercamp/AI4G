
#pragma once

#include <string>

#include <SFML\Graphics.hpp>

namespace Debug
{
	void				Error( std::string format, ... );
	void				Warning( std::string format, ... );

	void				Printf (std::string format, ...);

	void				SetFont (std::string fontName);
	void				DrawDebugOutput (sf::RenderWindow * targetWindow, float x, float y);
}