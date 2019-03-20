#include "Map.h"
#include <memory>
#include <math.h>
#include <algorithm>
#include <HAPISprites_Lib.h>
#include <iostream> //For testing

constexpr int FRAME_HEIGHT = 28;

void Map::drawMap() const 
{
	std::pair<int, int> textureDimensions = std::pair<int, int>(
		m_data[0].m_sprite->FrameWidth(),
		FRAME_HEIGHT);
		//m_data[0].m_sprite->FrameHeight());

	int access{ 0 };
	for (int y = 0; y < m_mapDimensions.second; y++)
	{
		const float yPosEven = (float)(0.5 + y) * textureDimensions.second;
		const float yPosOdd = (float)y * textureDimensions.second;

		for (int x = 1; x < m_mapDimensions.first; x += 2)
		{
			const float xPos = (float)x * textureDimensions.first * 3 / 4;
			//Is Odd
			m_data[access + x].m_sprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
				xPos * m_drawScale - m_drawOffset.first,
				yPosOdd * m_drawScale - m_drawOffset.second));
			m_data[access + x].m_sprite->GetTransformComp().SetScaling(
				HAPISPACE::VectorF(m_drawScale, m_drawScale));
			m_data[access + x].m_sprite->Render(SCREEN_SURFACE);
		}
		for (int x = 0; x < m_mapDimensions.first; x += 2)
		{
			const float xPos = (float)x * textureDimensions.first * 3 / 4;
			//Is even
			m_data[access + x].m_sprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
				xPos * m_drawScale - m_drawOffset.first,
				yPosEven * m_drawScale - m_drawOffset.second));
			m_data[access + x].m_sprite->GetTransformComp().SetScaling(
				HAPISPACE::VectorF(m_drawScale, m_drawScale));
			m_data[access + x].m_sprite->Render(SCREEN_SURFACE);
		}
		access += m_mapDimensions.first;
	}
}

std::pair<int, int> Map::offsetToCube(std::pair<int, int> offset) const
{
	int cubeX = offset.first;
	int cubeY = - offset.first - (offset.second - (offset.first + (offset.first & 1)) / 2);
	int cubeZ = -cubeX - cubeY;
	//std::cout << cubeX << ", " << cubeY << ", " << cubeZ << std::endl;//For testing
	return std::pair<int, int>(cubeX, cubeY);
}

std::pair<int, int> Map::cubeToOffset(std::pair<int, int> cube) const
{
	int offsetX = cube.first;
	int offsetY = -cube.first - cube.second + (cube.first + (cube.first & 1)) / 2;
	//std::cout << offsetX << ", " << offsetY << std::endl;//For testing
	return std::pair<int, int>(offsetX, offsetY);
}

int Map::cubeDistance(std::pair<int, int> a, std::pair<int, int> b) const
{
	const int x = abs(a.first - b.first);
	const int y = abs(a.second - b.second);
	const int z = abs(a.first + a.second - b.first - b.second);
	return std::max(x, std::max(y, z));
}

bool Map::inCone(std::pair<int, int> orgHex, std::pair<int, int> testHex, eDirection dir) const//TODO
{
	std::pair<int, int> diff(testHex.first - orgHex.first, testHex.second - orgHex.second);
	if (dir == eNorth || dir == eSouth)//Axis x = 0
	{

	}
	else if (dir == eNorthEast || dir == eSouthWest)//Axis y = 0
	{

	}
	else if (dir == eNorthWest || dir == eSouthEast)//Axis z = 0
	{

	}
	return true;
}

Tile* Map::getTile(std::pair<int, int> coordinate)
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
			std::to_string(m_mapDimensions.first) +", "+ std::to_string(m_mapDimensions.second)),
		"Map error");
	*/
	return nullptr;
}

std::vector<Tile*> Map::getAdjacentTiles(std::pair<int, int> coord)
{
	std::vector<Tile*> result;
	result.reserve(size_t(6));
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
	
	int reserveSize{ 0 };
	for (int i = 1; i <= range; i++)
	{
		reserveSize += 6 * i;
	}
	std::vector<Tile*> tileStore;

	tileStore.reserve((size_t)reserveSize);

	std::pair<int, int> cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range);
		y < std::min(m_mapDimensions.second, coord.second + range + 1);
		y++)
	{
		for (int x = std::max(0, coord.first - range);
			x < std::min(m_mapDimensions.first, coord.first + range + 1);
			x++)
		{
			if (coord.first != x && coord.second != y)
			{
				if (cubeDistance(cubeCoord, offsetToCube(std::pair<int, int>(x, y))) <= range)
				{
					tileStore.push_back(getTile(std::pair<int, int>(x, y)));
				}
			}
		}
	}
	return tileStore;
}

std::vector<Tile*> Map::getTileCone(std::pair<int, int> coord, int range, eDirection direction) //TODO
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileCone range less than 1", "Map error");

	int reserveSize{ 0 };
	for (int i = 2; i < range + 2; i++)
	{
		reserveSize += 2 * i;
	}
	std::vector<Tile*> tileStore;
	tileStore.reserve((size_t)reserveSize);

	std::pair<int, int> cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range);
		y < std::min(m_mapDimensions.second, coord.second + range + 1);
		y++)
	{
		for (int x = std::max(0, coord.first - range);
			x < std::min(m_mapDimensions.first, coord.first + range + 1);
			x++)
		{
			if (coord.first != x && coord.second != y)
			{
				if (cubeDistance(cubeCoord, offsetToCube(std::pair<int, int>(x, y))) <= range)
				{
					tileStore.push_back(getTile(std::pair<int, int>(x, y)));
				}
			}
		}
	}

	return tileStore;
}

bool Map::moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos)
{
	if (!getTile(newPos) || !getTile(originalPos))
		return false;

	Entity* tmpOld = getTile(originalPos)->m_entityOnTile;

	if (getTile(newPos)->m_entityOnTile != nullptr || tmpOld == nullptr)
		return false;

	getTile(newPos)->m_entityOnTile = tmpOld;
	getTile(originalPos)->m_entityOnTile = nullptr;
	return true;
}

void Map::insertEntity(Entity * newEntity, std::pair<int, int> coord)
{
	Tile* tile = getTile(coord);
	if (tile && !tile->m_entityOnTile)
	{
		tile->m_entityOnTile = newEntity;
	}
}

std::pair<int, int> Map::getTileScreenPos(std::pair<int, int> coord) const
{
	std::pair<int, int> textureDimensions = std::pair<int, int>(
		m_data[0].m_sprite->FrameWidth(),
		FRAME_HEIGHT);

	const float xPos = (float)(coord.first * textureDimensions.first) * 3 / 4;
	const float yPos = (float)((((1 + coord.first) % 2) + 2 * coord.second) 
		* textureDimensions.second) / 2;

	return std::pair<int, int>(
		xPos * m_drawScale - m_drawOffset.first,
		yPos * m_drawScale - m_drawOffset.second);
}

Map::Map(std::pair<int, int> size, const std::vector<std::vector<int>>& tileData) :
	m_mapDimensions(size),
	m_data(),
	m_drawOffset(std::pair<int, int>(50, 50)),
	m_windDirection(eNorth),
	m_windStrength(0.0),
	m_drawScale(2),
	motherSprite(nullptr)
{
	m_data.reserve(m_mapDimensions.first * m_mapDimensions.second);
	motherSprite = HAPI_Sprites.LoadSprite("Data\\hexTiles.xml");

	for (int y = 0; y < m_mapDimensions.second; y++)
	{
		for (int x = 0; x < m_mapDimensions.first; x++)
		{
			const int tileID = tileData[y][x];
			assert(tileID != -1);
			m_data.emplace_back(static_cast<eTileType>(tileID), 
				motherSprite->GetSpritesheet(), std::pair<int, int>(x, y));

			if (!m_data[x + y * m_mapDimensions.first].m_sprite)
			{
				HAPI_Sprites.UserMessage("Could not load tile spritesheet", "Error");
				return;
			}
			m_data[x + y * m_mapDimensions.first].m_sprite->SetFrameNumber(tileID);
			//cubeToOffset(offsetToCube(std::pair<int, int>(x, y)));
		}
	}
}