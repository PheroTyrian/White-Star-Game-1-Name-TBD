#pragma once
#include <utility>
#include <vector>
#include <string>
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "global.h"

struct entity;

enum eTileType
{
	eWater,
	eGrass,
	eSand
};

struct Tile
{
	enum eTileType m_type;
	entity* m_entityOnTile;
	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	const std::pair<int, int> m_tileCoordinate;

	Tile(eTileType type, std::string spriteName, std::string directory, std::pair<int, int> coord) :
		m_type(type), m_tileCoordinate(coord)
	{
		//HAPI's Sprite constructor takes two strings: the name of the file to load (append .xml) 
		//and the path to that file relative to the program
		m_sprite = HAPI_Sprites.LoadSprite(spriteName, directory);
	}
	Tile(const Tile &other) : m_tileCoordinate(std::pair<int, int>(other.m_tileCoordinate.first, other.m_tileCoordinate.second))
	{
		m_type = other.m_type;
		m_entityOnTile = other.m_entityOnTile;
		m_sprite = HAPI_Sprites.MakeSprite(other.m_sprite->GetSpritesheet());
	}
};

class Map
{
private:
	float m_windStrength;
	eDirection m_windDirection;
	float m_drawScale;
	std::pair<int, int> m_drawOffset;

	std::pair<int, int> m_mapDimensions;
	std::vector<Tile> m_data;
public:
	//Returns a pointer to a given tile, returns nullptr if there is no tile there
	Tile *getTile(std::pair<int, int> coordinate);
	//An n = 1 version of getTileRadius for use in pathfinding
	std::vector<Tile*> getAdjacentTiles(std::pair<int, int> coord);
	//TODO:Returns tiles in a radius around a given tile, skipping the tile itself
	std::vector<Tile*> getTileRadius(std::pair<int, int> coord, int range);
	//TODO: Returns tiles in a cone emanating from a given tile, skipping the tile itself
	std::vector<Tile*> getTileCone(std::pair<int, int> coord, int range, eDirection direction);

	std::pair<int, int> getTileScreenPos(std::pair<int, int> coord);

	//Moves an entitys position on the map, returns false if the position is already taken
	bool moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos);
	//Places a new entity on the map (no check for duplicates yet so try to avoid creating multiples)
	void insertEntity(entity* newEntity, std::pair<int, int> coord);

	void drawMap();
	std::pair<int, int> getDrawOffset() const { return m_drawOffset; }
	void setDrawOffset(std::pair<int, int> newOffset) { m_drawOffset = newOffset; }

	float getDrawScale() const { return m_drawScale; }
	void setDrawScale(float scale) { if (scale > 0.0) m_drawScale = scale; }

	float getWindStrength() const { return m_windStrength; }
	void setWindStrength(float strength) { if (strength > 0.0) m_windStrength = strength; }

	eDirection getWindDirection() const { return m_windDirection; }
	void setWindDirection(eDirection direction) { m_windDirection = direction; }

	//TODO: Get constructor working. Need tiled parser or load from xml set up
	Map(int width, int height);
	~Map();
};

