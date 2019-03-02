#pragma once
#include <utility>
#include <vector>

struct entity;
struct Tile;

class Map
{
private:
	std::pair<int, int> m_drawOffset;
	std::pair<int, int> m_mapDimensions;
	std::vector<Tile> m_data;
public:
	void drawMap();

	//Returns a pointer to a given tile, returns nullptr if there is no tile there
	Tile *getTile(std::pair<int, int> coordinate);
	//An n = 1 version of getTileRadius for use in pathfinding
	std::vector<Tile*> getAdjacentTiles(std::pair<int, int> coord);
	//TODO:Returns tiles in a radius around a given tile, skipping the tile itself
	std::vector<Tile*> getTileRadius(std::pair<int, int> coord, int range);
	//TODO: Returns tiles in a cone emanating from a given tile, skipping the tile itself
	std::vector<Tile*> getTileCone(std::pair<int, int> coordinate, int range, enum eDirection);

	//Moves an entitys position on the map, returns false if the position is already taken
	bool moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos);

	std::pair<int, int> getDrawOffset() const { return m_drawOffset; }
	void setDrawOffset(std::pair<int, int> newOffset) { m_drawOffset = newOffset; }
	std::pair<int, int> getTileScreenPos(std::pair<int, int> coord);

	//TODO: Get constructor working. Need tiled parser or load from xml set up
	Map();
	~Map();
};

