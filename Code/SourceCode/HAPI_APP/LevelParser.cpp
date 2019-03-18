#include "LevelParser.h"
#include <assert.h>

std::vector<TileLayer> parseTileLayers(const TiXmlElement& rootElement, const LevelDetails& levelDetails);
LevelDetails parseLevelDetails(const TiXmlElement& rootElement);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize);

Map LevelParser::parseMap(const std::string& name)
{
	TiXmlDocument mapFile;
	bool mapLoaded = mapFile.LoadFile(levelName);
	assert(mapLoaded);

	const auto& rootElement = mapFile.RootElement();
	LevelDetails levelDetails = parseLevelDetails(*rootElement);
	std::vector<TileLayer> tileLayers = parseTileLayers(*rootElement, levelDetails);

	return Level(levelDetails, tileLayers, tileSheets, std::move(collisionLayer), entities);
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize)
{
	std::vector<std::vector<int>> tileData;
	tileData.reserve(levelSize.y);

	std::string decodedIDs; //Base64 decoded information
	const TiXmlElement* dataNode = nullptr; //Store our node once we find it
	for (const TiXmlElement* e = tileLayerElement.FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			dataNode = e;
		}
	}
	assert(dataNode);

	Base64 base64;
	const TiXmlText* text = dataNode->FirstChild()->ToText();
	const std::string t = text->Value();
	decodedIDs = base64.base64_decode(t);

	const std::vector<int> layerColumns(levelSize.x);
	for (int i = 0; i < levelSize.y; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < levelSize.y; ++rows)
	{
		for (int cols = 0; cols < levelSize.x; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * levelSize.x + cols) - 1;
		}
	}

	return tileData;
}

LevelDetails parseLevelDetails(const TiXmlElement& rootElement)
{
	sf::Vector2i levelSize;
	int tileSize = 0;
	rootElement.Attribute("width", &levelSize.x);
	rootElement.Attribute("height", &levelSize.y);
	rootElement.Attribute("tilewidth", &tileSize);
	return LevelDetails(levelSize, tileSize);
}

std::vector<TileLayer> parseTileLayers(const TiXmlElement & rootElement, const LevelDetails& levelDetails)
{
	std::vector<TileLayer> tileLayers;
	for (const auto* tileLayerElement = rootElement.FirstChildElement();
		tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}

		auto tileMap = decodeTileLayer(*tileLayerElement, levelDetails.m_size);
		std::string name = tileLayerElement->Attribute("name");
		tileLayers.emplace_back(std::move(tileMap), name);
	}

	assert(!tileLayers.empty());
	return tileLayers;
}