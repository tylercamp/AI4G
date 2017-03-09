
#pragma once

#include <SFML\System\Vector2.hpp>

#include "World.h"
#include "PropertyStore.h"

namespace LevelLoader
{
	enum AssetType
	{
		Invalid,
		Texture,
		Path,
		Rectangle
	};

	typedef std::map<std::string, std::shared_ptr<PropertyStore> > LevelAssetCollection;

	typedef void (* AssetLoadHandler)( GameWorld & targetWorld, const PropertyStore & assetProperties, const LevelAssetCollection & levelAssets );

	void ProcessXmlFile( const std::string & fileName, GameWorld & targetWorld );
	void ProcessXmlString( const std::string & xmlData, GameWorld & targetWorld );

	void RegisterHandler( AssetType targetType, AssetLoadHandler handler );
	void ClearHandlers( );
}



class AutoAssetResponseBuilder
{
public:
	AutoAssetResponseBuilder(
		LevelLoader::AssetType targetType,
		LevelLoader::AssetLoadHandler handler
		);
};




#define TextureAssetResponse(FunctionName)								\
	void FunctionName(													\
		GameWorld & targetWorld,										\
		const PropertyStore & assetProperties,							\
		const LevelLoader::LevelAssetCollection & levelProperties		\
		);																\
	AutoAssetResponseBuilder _##FunctionName##_TextureBuilder (			\
		LevelLoader::Texture,											\
		FunctionName													\
	);



#define PathAssetResponse(FunctionName)									\
	void FunctionName(													\
		GameWorld & targetWorld,										\
		const PropertyStore & assetProperties,							\
		const LevelLoader::LevelAssetCollection & levelProperties		\
		);																\
	AutoAssetResponseBuilder _##FunctionName##_TextureBuilder (			\
		LevelLoader::Texture,											\
		FunctionName													\
	);
