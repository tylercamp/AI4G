
#pragma once

#include "GameEntity.h"
#include "LevelLoader.h"

class Wall : public GameEntity
{
public:

				Wall (sf::Vector2f position = sf::Vector2f ());

private:

	typedef GameEntity	Base;
};