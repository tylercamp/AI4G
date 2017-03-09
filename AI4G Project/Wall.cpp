
#include "Wall.h"
#include "Utility.h"

TextureAssetResponse(BuildWall)
void BuildWall( GameWorld & gameWorld, const PropertyStore & assetProperties, const LevelLoader::LevelAssetCollection & levelProperties)
{
	if (GetFileName(assetProperties.GetProperty<std::string>("assetname")) == "player")
	{
		gameWorld.AddEntity(new Wall(assetProperties.GetProperty<sf::Vector2f>("Position")));
	}
}

Wall::Wall (sf::Vector2f position)
{
	SetSpriteFromFileWithMask("resources/player.png");

	Position = position;
	Depth = -200;
}