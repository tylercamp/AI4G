
#include <SFML\Graphics.hpp>
#include <gl/GL.h>

#include "Player.h"
#include "Debug.h"
#include "Wall.h"
#include "FloorTile.h"
#include "TestAI.h"
#include "FuzzyLogic.h"

#include "Utility.h"

#include "World.h"
#include "LevelLoader.h"

#include "ResourceManager.h"

#include "PropertyStore.h"


/* Reference SFML libraries */
#ifdef _DEBUG
#pragma comment (lib, "sfml-system-s-d.lib")
#pragma comment (lib, "sfml-window-s-d.lib")
#pragma comment (lib, "sfml-graphics-s-d.lib")
#else
#pragma comment (lib, "sfml-system-s.lib")
#pragma comment (lib, "sfml-window-s.lib")
#pragma comment (lib, "sfml-graphics-s.lib")
#endif

char * WCharTToCString(const wchar_t * text)
{
	int size = wcslen(text);
	char * result = new char[size];

	for (int i = 0; i < size; i++)
	{
		result[i] = (char)text[i];
	}

	return result;
}

void InvalidParameterCallback(
	const wchar_t * expression,
	const wchar_t * function, 
	const wchar_t * file, 
	unsigned int line,
	uintptr_t pReserved
	)
{
	char * cExpression = WCharTToCString(expression);
	char * cFunction = WCharTToCString(function);
	char * cFile = WCharTToCString(file);

	Debug::Error("%s\n\nIn function %s\nIn file%s\nOn line %i", cExpression, cFunction, cFile, line);
}

//	Just something I stitched together at the last moment
void DrawFloor(sf::RenderWindow & window)
{
	static sf::Image * image = nullptr;
	if (image == nullptr)
	{
		image = new sf::Image();
		image->LoadFromFile("resources/MarbleTile.png");
	}
}

/* Use the command-line window in Debug mode */
#ifdef _DEBUG
int main ()
#else
/* Don't include the command-line in Release */
int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShowCmd)
#endif
{
	//	Debugging
	_set_invalid_parameter_handler(InvalidParameterCallback);

	srand ((unsigned int)time (NULL));

	sf::RenderWindow window (sf::VideoMode (1280, 720), "");
	window.SetFramerateLimit (60);
	window.UseVerticalSync (false);

	Debug::SetFont ("Resources/SegoeUIMono-Regular.TTF");

	/* Initialize global objects */
	ResourceManager::Context = new ResourceManager ();

	GameWorld world (window);
	LevelLoader::ProcessXmlFile(TranslatePathToAbsolute("level.xml"), world);

	SceneGraph & worldGraph = world.GetSceneGraph();
	worldGraph.ClearGraphToSize(64, 36);
	worldGraph.AutoGenerateLinks();

	world.BuildWorldGraph();

	sf::View camera;
	camera.SetHalfSize(sf::Vector2f (window.GetWidth() / 2.0F, window.GetHeight() / 2.0f));

	while (window.IsOpened ())
	{
		/* Process basic input */
		sf::Event e;
		while (window.GetEvent (e))
		{
			if (e.Type == sf::Event::Closed)
				window.Close ();

			if (e.Type == sf::Event::KeyPressed)
			{
				if (e.Key.Code == sf::Key::Escape)
					window.Close ();
			}
		}

		window.Clear(sf::Color::White);

		Player * player = Player::Instance();
		camera.SetCenter(player->Position);

		window.SetView(camera);

		world.Update(window.GetFrameTime());

		DrawFloor(window);
		world.Draw();
		
		//world.GetGraphTranslator().Debug_Draw(window);

		window.SetView(window.GetDefaultView());

		window.Draw(sf::Shape::Rectangle(0.0F, 0.0F, 300.0F, (float)window.GetHeight(), sf::Color(0, 0, 0, 127)));

		Debug::DrawDebugOutput (&window, 5.0f, 5.0f);
		Debug::Printf("FPS: %i", (int)(1.0f / window.GetFrameTime()));

		window.Display ();
	}

	return 0;
}