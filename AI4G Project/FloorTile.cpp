
#include "FloorTile.h"
#include "Utility.h"

TextureAssetResponse(BuildFloor);
void BuildFloor( GameWorld & targetWorld, const PropertyStore & properties, const LevelLoader::LevelAssetCollection & assets )
{
	std::string assetName = GetFileName(properties.GetProperty<std::string>("AssetName"));
	if (assetName == "marbletile" || assetName == "marbletile2")
	{
		targetWorld.AddEntity(new FloorTile(properties.GetProperty<sf::Vector2f>("Position"), properties.GetProperty<std::string>("TextureFilename")));
	}
}

/*
=================
FloorTile::FloorTile
=================
 */
FloorTile::FloorTile( sf::Vector2f position, const std::string & spriteSource )
{
	Position = position;
	SetSpriteFromFile( spriteSource );

	Depth = 1000;
}