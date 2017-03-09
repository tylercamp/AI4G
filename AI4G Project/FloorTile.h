
#pragma once

#include "GameEntity.h"
#include "LevelLoader.h"


class FloorTile : public GameEntity
{
public:
				FloorTile( sf::Vector2f position, const std::string & spriteSource );
};