#pragma once

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

	//Returns a pointer to a given tile
	Tile *getTile(std::pair<int, int> coordinate);
	//Returns tiles in a radius around a given tile, skipping the tile itself
	std::vector<Tile*> getTileRadius(std::pair<int, int> coordinate, int range);
	//TODO: Returns tiles in a cone emanating from a given tile, skipping the tile itself
	std::vector<Tile*> getTileCone(std::pair<int, int> coordinate, int range, enum eDirection);

	//Moves an entitys position on the map, returns false if the position is already taken
	bool moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos);

	std::pair<int, int> getDrawOffset() { return m_drawOffset; }
	void setDrawOffset(std::pair<int, int> newOffset) { m_drawOffset = newOffset; }
	std::pair<float, float> getTileScreenPos();

	//TODO: Get constructor working. Need tiled parser or load from xml set up
	Map();
	~Map();
};

