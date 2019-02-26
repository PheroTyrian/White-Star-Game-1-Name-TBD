#include "Map.h"
#include <memory>
#include <string>
#include <math.h>
#include <algorithm>
#include <HAPISprites_Lib.h>

struct Tile
{
	enum eTileType;
	entity* m_entityOnTile;
	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	const std::pair<int, int> m_tileCoordinate;
};

void Map::drawMap()
{
	//TODO: Fill this in
}

Tile *Map::getTile(std::pair<int, int> coordinate)
{
	//Bounds check
	if (coordinate.first < m_mapDimensions.first &&
		coordinate.second < m_mapDimensions.second &&
		coordinate.first >= 0 &&
		coordinate.second >= 0)
	{
		Tile *result = &m_data[coordinate.first + coordinate.second * m_mapDimensions.first];
		return result;
	}

	HAPI_Sprites.UserMessage(
		std::string("getTile request out of bounds: " + std::to_string(coordinate.first) + 
			", " + std::to_string(coordinate.second) + " map dimensions are: " + 
			std::to_string(m_mapDimensions.first) + ", " + std::to_string(m_mapDimensions.second)),
		"Map error");
	return nullptr;
}

std::vector<Tile*> Map::getTileRadius(std::pair<int, int> coord, int range)
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileRadius range less than 1", "Map error");

	std::vector<Tile*> tileStore;
	int reserveSize{ 0 };
	for (int i = 1; i <= range; i++)
	{
		reserveSize += (int) pow(6, i);
	}
	tileStore.reserve((size_t)reserveSize);

	bool even{ false };
	if (2 % coord.first == 0)
		bool even = true;

	for (int x = std::max(0, coord.first - range); 
		x <= std::max(m_mapDimensions.first, coord.first + range); 
		x++)
	{
		for (int y = std::max(0, coord.second - ))//Point I stopped at
	}
	return tileStore;
}

std::vector<Tile*> Map::getTileCone(std::pair<int, int> coordinate, int range, eDirection)
{
	return std::vector<Tile*>();
}

bool Map::moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos)
{
	int newAddress{ newPos.first + newPos.second * m_mapDimensions.first };
	if (m_data[newAddress].m_entityOnTile != nullptr)
		return false;

}

std::pair<float, float> Map::getTileScreenPos()
{
	return std::pair<int, int>();
}

Map::Map()
{
}


Map::~Map()
{
}
