#include "MapParser.h"
#include "Base64.h"
#include "tinyxml.h"
#include "../Map.h"
#include <assert.h>

std::vector<std::vector<int>> parseTileData(const TiXmlElement& rootElement, const std::pair<int, int> mapSize);
std::pair<int, int> parseMapSize(const TiXmlElement& rootElement);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, std::pair<int, int> mapSize);

Map MapParser::parseMap(const std::string& name)
{
	TiXmlDocument mapFile;
	bool mapLoaded = mapFile.LoadFile(name);
	assert(mapLoaded);

	const auto& rootElement = mapFile.RootElement();
	std::pair<int, int> mapSize = parseMapSize(*rootElement);

	return Map(mapSize, parseTileData(*rootElement, mapSize));
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, std::pair<int, int> mapSize)
{
	std::vector<std::vector<int>> tileData;
	tileData.reserve(mapSize.second);

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

	const std::vector<int> layerColumns(mapSize.first);
	for (int i = 0; i < mapSize.second; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < mapSize.second; ++rows)
	{
		for (int cols = 0; cols < mapSize.first; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * mapSize.first + cols) - 1;
		}
	}

	return tileData;
}

std::pair<int, int> parseMapSize(const TiXmlElement& rootElement)
{
	std::pair<int, int> mapSize;
	rootElement.Attribute("width", &mapSize.first);
	rootElement.Attribute("height", &mapSize.second);
	return mapSize;
}

std::vector<std::vector<int>> parseTileData(const TiXmlElement & rootElement, const std::pair<int, int> mapSize)
{
	std::vector<std::vector<int>> tileData;
	for (const auto* tileLayerElement = rootElement.FirstChildElement();
		tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}

		tileData = decodeTileLayer(*tileLayerElement, mapSize);
	}

	assert(!tileData.empty());
	return tileData;
}