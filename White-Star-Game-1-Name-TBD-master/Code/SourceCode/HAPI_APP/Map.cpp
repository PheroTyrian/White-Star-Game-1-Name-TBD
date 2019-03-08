#include "Map.h"
#include <memory>
#include <string>
#include <math.h>
#include <algorithm>
#include <HAPISprites_Lib.h>

struct Tile
{
	enum eTileType m_type;
	entity* m_entityOnTile;
	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	const std::pair<int, int> m_tileCoordinate;

	Tile(eTileType type, std::string spriteName, std::pair<int, int> coord) : m_type(type), m_tileCoordinate(coord)
	{
		m_sprite = HAPI_Sprites.LoadSprite(spriteName, std::string());
	}
};

void Map::drawMap()
{
	std::pair<int, int> textureDimensions = std::pair<int, int>(
		m_data[0].m_sprite->FrameWidth(),
		m_data[0].m_sprite->FrameHeight());

	float drawScale{ 1.0 };//For future zooming and resizing
	int access{ 0 };
	for (int y = 0; y < m_mapDimensions.second; y++)
	{
		float yPosEven = (float)(0.5 + y) * textureDimensions.second;
		float yPosOdd = (float)y * textureDimensions.second;
		for (int x = 0; x < m_mapDimensions.first; x++)
		{
			float xPos = (float)x * textureDimensions.first * 3 / 4;
			if (x % 2 == 0)//Is even
				m_data[access].m_sprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					xPos * drawScale + m_drawOffset.first, 
					yPosEven * drawScale + m_drawOffset.second));
			else//Is Odd
				m_data[access].m_sprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					xPos * drawScale + m_drawOffset.first,
					yPosOdd * drawScale + m_drawOffset.second));
			m_data[access].m_sprite->Render(SCREEN_SURFACE);
			access++;
		}
	}
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
	/*
	HAPI_Sprites.UserMessage(
		std::string("getTile request out of bounds: " + std::to_string(coordinate.first) + 
			", " + std::to_string(coordinate.second) + " map dimensions are: " + 
			std::to_string(m_mapDimensions.first) + ", " + std::to_string(m_mapDimensions.second)),
		"Map error");
	*/
	return nullptr;
}

std::vector<Tile*> Map::getAdjacentTiles(std::pair<int, int> coord)
{
	std::vector<Tile*> result;
	result.reserve(6);
	if (2 % coord.first == 1)//Is an odd tile
	{
		result.push_back(getTile(std::pair<int, int>(coord.first, coord.second - 1)));//N
		result.push_back(getTile(std::pair<int, int>(coord.first + 1, coord.second - 1)));//NE
		result.push_back(getTile(std::pair<int, int>(coord.first + 1, coord.second)));//SE
		result.push_back(getTile(std::pair<int, int>(coord.first, coord.second + 1)));//S
		result.push_back(getTile(std::pair<int, int>(coord.first - 1, coord.second)));//SW
		result.push_back(getTile(std::pair<int, int>(coord.first - 1, coord.second - 1)));//NW
	}
	else//Is even
	{
		result.push_back(getTile(std::pair<int, int>(coord.first, coord.second - 1)));//N
		result.push_back(getTile(std::pair<int, int>(coord.first + 1, coord.second)));//NE
		result.push_back(getTile(std::pair<int, int>(coord.first + 1, coord.second + 1)));//SE
		result.push_back(getTile(std::pair<int, int>(coord.first, coord.second + 1)));//S
		result.push_back(getTile(std::pair<int, int>(coord.first - 1, coord.second + 1)));//SW
		result.push_back(getTile(std::pair<int, int>(coord.first - 1, coord.second)));//NW
	}
	return result;
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
		//for (int y = std::max(0, coord.second - ))//Point I stopped at
	}
	return tileStore;
}

std::vector<Tile*> Map::getTileCone(std::pair<int, int> coord, int range, enum eDirection)
{
	return std::vector<Tile*>();
}

bool Map::moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos)
{
	auto tmpNew = getTile(newPos)->m_entityOnTile;
	auto tmpOld = getTile(originalPos)->m_entityOnTile;

	if (tmpNew != nullptr || tmpOld == nullptr)
		return false;

	tmpNew = tmpOld;
	tmpOld = nullptr;
	return true;
}

void Map::insertEntity(entity * newEntity, std::pair<int, int> coord)
{
	getTile(coord)->m_entityOnTile = newEntity;
}

std::pair<int, int> Map::getTileScreenPos(std::pair<int, int> coord)
{
	std::pair<int, int> textureDimensions = std::pair<int, int>(
		m_data[0].m_sprite->FrameWidth(),
		m_data[0].m_sprite->FrameHeight());
	
	float drawScale = 1.0;//For future zooming and resizing
	const int xPos = (coord.first * textureDimensions.first) * 3 / 4;
	const int yPos = ((((1 + coord.first) % 2) + 2 * coord.second) * textureDimensions.second) / 2;
	return std::pair<int, int>(
		xPos * drawScale + m_drawOffset.first, 
		yPos * drawScale + m_drawOffset.second);
}

Map::Map()
{
}


Map::~Map()
{
}
