
#include "LevelLoader.h"
#include "rapidxml.hpp"
#include "Debug.h"
#include "Utility.h"
#include "PathManager.h"

#include <map>
#include <vector>
#include <cstdio>

using namespace rapidxml;

typedef std::map<LevelLoader::AssetType, std::vector<LevelLoader::AssetLoadHandler> > AssetHandlerMap;

AssetHandlerMap & GetHandlerMap()
{
	static AssetHandlerMap * handlerMap = nullptr;
	if (handlerMap == nullptr)
		handlerMap = new AssetHandlerMap();
	return *handlerMap;
}

AutoAssetResponseBuilder::AutoAssetResponseBuilder( LevelLoader::AssetType targetType, LevelLoader::AssetLoadHandler handler )
{
	LevelLoader::RegisterHandler(targetType, handler);
}

/*
=================
ProcessXmlFile
=================
*/
void LevelLoader::ProcessXmlFile( const std::string & fileName, GameWorld & targetWorld )
{
	FILE * fileHandle;
	if (fopen_s(&fileHandle, fileName.c_str(), "r") != 0)
	{
		Debug::Warning("Unable to open XML level file for reading: %s", fileName.c_str());
		return;
	}

	std::string fileContents;

	static char buffer[4096];
	int sizeRead = 0;
	do 
	{
		sizeRead = fread_s(buffer, sizeof(buffer), 1, sizeof(buffer), fileHandle);
		fileContents.append(buffer, sizeRead);

	} while (sizeRead == sizeof(buffer));

	ProcessXmlString(fileContents, targetWorld);
}

#pragma region Convenience Functions for ProcessXmlString

enum NodeDataType
{
	VECTOR_TYPE,
	COLOR_TYPE,
	REALVALUE_TYPE,
	BOOLEAN_TYPE,
	TEXT_TYPE
};

int GetNodeChildCount( xml_node<char> * node )
{
	if (node == nullptr)
		return 0;

	int count = 0;
	for (xml_node<char> * currentNode = node->first_node(); currentNode; currentNode = currentNode->next_sibling())
		count++;

	return count;
}

NodeDataType GetItemAttributeType( xml_node<char> * node )
{
	std::string value = node->value();
	if (value == "true" || value == "false")
		return NodeDataType::BOOLEAN_TYPE;
	
	if (GetNodeChildCount(node) == 4)
		return NodeDataType::COLOR_TYPE;

	if (GetNodeChildCount(node) == 2)
		return NodeDataType::VECTOR_TYPE;

	if (!StringContainsText(value))
		return NodeDataType::REALVALUE_TYPE;
	
	return NodeDataType::TEXT_TYPE;
}

void FillPropertiesForCustomProperties(
	xml_node<char> * customPropertiesNode,
	PropertyStore & targetStore,
	const std::string & namePrefix
	)
{
	auto currentNode = customPropertiesNode->first_node();
	while (currentNode)
	{
		std::string propertyName = currentNode->first_attribute("Name")->value();
		
		NodeDataType dataType = GetItemAttributeType( currentNode );

		switch (dataType)
		{
		case VECTOR_TYPE:
			{
				Debug::Warning("FillPropertiesForCustomProperties does not support the Vector type.");
				break;
			}

		case COLOR_TYPE:
			{
				Debug::Warning("FillPropertiesForCustomProperties does not support the Color type.");
				break;
			}

		case REALVALUE_TYPE:
			{
				float value = StringToReal<float>(currentNode->value());
				targetStore.SetProperty(namePrefix + propertyName, value);
				break;
			}

		case BOOLEAN_TYPE:
			{
				std::string textValue = currentNode->value();
				bool value = (textValue == "true");
				targetStore.SetProperty(namePrefix + propertyName, value);
				break;
			}

		case TEXT_TYPE:
			{
				std::string value = currentNode->value();
				targetStore.SetProperty(namePrefix + propertyName, value);
				break;
			}
		}

		currentNode = currentNode->next_sibling();
	}
}

void FillPropertiesForTextureData( xml_node<char> * node, PropertyStore & targetStore )
{
	/* Position */
	auto positionNode = node->first_node("Position");
	sf::Vector2f position;
	position.x = StringToReal<float>(positionNode->first_node("X")->value());
	position.y = StringToReal<float>(positionNode->first_node("Y")->value());
	targetStore.SetProperty("Position", position);

	/* Custom properties */
	auto propertiesNode = node->first_node("CustomProperties");
	FillPropertiesForCustomProperties(propertiesNode, targetStore, "Custom");

	/* Rotation */
	auto rotationNode = node->first_node("Rotation");
	float rotation = StringToReal<float>(rotationNode->value());
	targetStore.SetProperty("Rotation", rotation);

	/* Scale */
	auto scaleNode = node->first_node("Scale");
	sf::Vector2f scale;
	scale.x = StringToReal<float>(scaleNode->first_node("X")->value());
	scale.y = StringToReal<float>(scaleNode->first_node("Y")->value());
	targetStore.SetProperty("Scale", scale);

	/* Tint */
	auto tintNode = node->first_node("TintColor");
	sf::Color tint;
	tint.r = StringToReal<byte>(tintNode->first_node("R")->value());
	tint.g = StringToReal<byte>(tintNode->first_node("G")->value());
	tint.b = StringToReal<byte>(tintNode->first_node("B")->value());
	tint.a = StringToReal<byte>(tintNode->first_node("A")->value());
	targetStore.SetProperty("TintColor", tint);

	/* Texture Filename */
	auto textureFileNameNode = node->first_node("texture_filename");
	targetStore.SetProperty("TextureFilename", std::string (textureFileNameNode->value()));

	/* Texture Asset name */
	auto assetNameNode = node->first_node("asset_name");
	targetStore.SetProperty("AssetName", std::string (assetNameNode->value()));

	/* Origin */
	auto originNode = node->first_node("Origin");
	sf::Vector2f origin;
	origin.x = StringToReal<float>(originNode->first_node("X")->value());
	origin.y = StringToReal<float>(originNode->first_node("Y")->value());
	targetStore.SetProperty("Origin", origin);
}

void FillPropertiesForPathData( xml_node<char> * node, PropertyStore & targetStore )
{
	/* Position */
	auto positionNode = node->first_node("Position");
	sf::Vector2f position;
	position.x = StringToReal<float>(positionNode->first_node("X")->value());
	position.y = StringToReal<float>(positionNode->first_node("Y")->value());
	targetStore.SetProperty("Position", position);

	/* Custom properties */
	Debug::Warning("FillPropertiesForPathData does not process custom properties.");

	/* Local points */
	auto localNode = node->first_node("LocalPoints");
	auto currentLocalNode = localNode->first_node();

	PathManager localPath;

	while (currentLocalNode)
	{
		sf::Vector2f currentPosition;
		currentPosition.x = StringToReal<float>(currentLocalNode->first_node("X")->value());
		currentPosition.y = StringToReal<float>(currentLocalNode->first_node("Y")->value());
		localPath.Path.push_back(currentPosition);
		currentLocalNode = currentLocalNode->next_sibling();
	}

	targetStore.SetProperty("LocalPath", localPath);

	/* World points */
	auto worldNode = node->first_node("WorldPoints");
	auto currentWorldNode = worldNode->first_node();
	PathManager globalPath;
	while (currentWorldNode)
	{
		sf::Vector2f currentPosition;
		currentPosition.x = StringToReal<float>(currentWorldNode->first_node("X")->value());
		currentPosition.y = StringToReal<float>(currentWorldNode->first_node("Y")->value());
		globalPath.Path.push_back(currentPosition);
		currentWorldNode = currentWorldNode->next_sibling();
	}

	targetStore.SetProperty("GlobalPath", globalPath);

	/* IsPolygon */
	Debug::Warning("FillPropertiesForPathData does not process the IsPolygon property.");

	/* LineWidth */
	Debug::Warning("FillPropertiesForPathData does not process the LineWidth property.");

	/* Color */
	Debug::Warning("FillPropertiesForPathData does not process the LineColor property.");
}

LevelLoader::AssetType InterpretNodeAssetType( xml_node<char> * node )
{
	LevelLoader::AssetType assetType = LevelLoader::Invalid;
	std::string assetTypeString = node->first_attribute("xsi:type")->value();

	if (assetTypeString == "TextureItem")
		assetType = LevelLoader::Texture;
	if (assetTypeString == "PathItem")
		assetType = LevelLoader::Path;
	if (assetTypeString == "RectangleItem")
		assetType = LevelLoader::Rectangle;

	return assetType;
}

#pragma endregion

/*
=================
ProcessXmlString
=================
*/
void LevelLoader::ProcessXmlString( const std::string & xmlData, GameWorld & targetWorld )
{
	std::string mutableXmlData (xmlData);

	xml_document<char> document;
	document.parse<0>(const_cast<char *>(mutableXmlData.c_str()));

	xml_node<char> * itemsNode = document.first_node()->first_node()->first_node()->first_node();

	LevelAssetCollection assets;

	auto currentItem = itemsNode->first_node();
	while (currentItem)
	{
		AssetType assetType = InterpretNodeAssetType(currentItem);
		if (assetType == Invalid)
		{
			char * assetTypeString = currentItem->first_attribute("xsi:type")->value();
			Debug::Warning("Unable to identify appropriate AssetType for %s", assetTypeString);
			continue;
		}

		PropertyStore * itemData = new PropertyStore();
		itemData->SetProperty("ItemType", assetType);
		switch (assetType)
		{
		case LevelLoader::Texture:
			FillPropertiesForTextureData(currentItem, *itemData);
			break;
		case LevelLoader::Path:
			FillPropertiesForPathData(currentItem, *itemData);
			break;
		case LevelLoader::Rectangle:
			throw;
			break;
		}

		std::string itemName = currentItem->first_attribute("Name")->value();
		assets[itemName] = std::shared_ptr<PropertyStore>(itemData);

		currentItem = currentItem->next_sibling();
	}

	for (auto item : assets)
	{
		PropertyStore * currentStore = item.second.get();
		AssetType currentAssetType = currentStore->GetProperty<AssetType>("ItemType");

		for (auto currentItem : GetHandlerMap()[currentAssetType])
			currentItem(targetWorld, *currentStore, assets);
	}
}

/*
=================
RegisterHandler
=================
*/
void LevelLoader::RegisterHandler(LevelLoader::AssetType targetType, AssetLoadHandler handler)
{
	GetHandlerMap()[targetType].push_back(handler);
}


/*
=================
ClearHandlers
=================
*/
void LevelLoader::ClearHandlers( )
{
	GetHandlerMap().clear();
}