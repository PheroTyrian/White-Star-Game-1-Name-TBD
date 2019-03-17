#include "Map.h"
#include <memory>
#include <math.h>
#include <algorithm>
#include <HAPISprites_Lib.h>


void Map::drawMap()
{
	std::pair<int, int> textureDimensions = std::pair<int, int>(
		m_data[0].m_sprite->FrameWidth(),
		30);
		//m_data[0].m_sprite->FrameHeight());

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
					xPos * m_drawScale - m_drawOffset.first,
					yPosEven * m_drawScale - m_drawOffset.second));
			else//Is Odd
				m_data[access].m_sprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					xPos * m_drawScale - m_drawOffset.first,
					yPosOdd * m_drawScale - m_drawOffset.second));
			m_data[access].m_sprite->GetTransformComp().SetScaling(
				HAPISPACE::VectorF(m_drawScale, m_drawScale));
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
		reserveSize += (int)pow(6, i);
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

std::vector<Tile*> Map::getTileCone(std::pair<int, int> coord, int range, eDirection direction)
{
	return std::vector<Tile*>();
}

bool Map::moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos)
{
	entity* tmpNew = getTile(newPos)->m_entityOnTile;
	entity* tmpOld = getTile(originalPos)->m_entityOnTile;

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

	const int xPos = (coord.first * textureDimensions.first) * 3 / 4;
	const int yPos = ((((1 + coord.first) % 2) + 2 * coord.second) * textureDimensions.second) / 2;
	return std::pair<int, int>(
		xPos * m_drawScale - m_drawOffset.first,
		yPos * m_drawScale - m_drawOffset.second);
}

Map::Map(int width, int height) :
	m_mapDimensions(std::pair<int, int>(width, height)), m_data(), m_drawOffset(std::pair<int, int>(100, 100)),
	m_windDirection(eNorth), m_windStrength(0.0), m_drawScale(0.1)
{
	m_data.reserve(width * height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Tile tile(eWater, std::string("tilemapcurrent.xml"), std::string("Data\\"), std::pair<int, int>(x, y));
			m_data.push_back(tile);
			if (!m_data[x + y * m_mapDimensions.first].m_sprite)
			{
				HAPI_Sprites.UserMessage("Could not load tile spritesheet", "Error");
				return;
			}
			m_data[x + y * m_mapDimensions.first].m_sprite->SetFrameNumber(6);
		}
	}
}


Map::~Map()
{
}
