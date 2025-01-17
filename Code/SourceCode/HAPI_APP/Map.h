#pragma once
#include <utility>
#include <vector>
#include <string>
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "global.h"

class Entity;

struct Tile
{
	enum eTileType m_type;
	Entity* m_entityOnTile;
	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	const std::pair<int, int> m_tileCoordinate;

	Tile(eTileType type, const std::string& spriteName, std::pair<int, int> coord) :
		m_type(type), m_tileCoordinate(coord),
		m_entityOnTile(nullptr)
	{
		//HAPI's Sprite constructor takes two strings: the name of the file to load (append .xml) 
		//and the path to that file relative to the program
		m_sprite = HAPI_Sprites.LoadSprite(spriteName);
	}
	Tile(eTileType type, std::shared_ptr<HAPISPACE::SpriteSheet> spriteSheet, std::pair<int, int> coord) :
		m_type(type), m_tileCoordinate(coord),
		m_entityOnTile(nullptr)
	{
		//HAPI's make sprite takes a pointer to an existing spritesheet
		m_sprite = HAPI_Sprites.MakeSprite(spriteSheet);
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
	const std::pair<int, int> m_mapDimensions;
	float m_windStrength;
	eDirection m_windDirection;
	float m_drawScale;
	std::pair<int, int> m_drawOffset;
	std::unique_ptr<HAPISPACE::Sprite> motherSprite; //All tiles inherit from this sprite
	std::vector<Tile> m_data;

	std::pair<int, int> offsetToCube(std::pair<int, int> offset) const;
	std::pair<int, int> cubeToOffset(std::pair<int, int> cube) const;
	int cubeDistance(std::pair<int, int> a, std::pair<int, int> b) const;
	bool inCone(std::pair<int, int> orgHex, std::pair<int, int> testHex, eDirection dir) const;
public:
	//Returns a pointer to a given tile, returns nullptr if there is no tile there
	Tile *getTile(std::pair<int, int> coordinate);
	//An n = 1 version of getTileRadius for use in pathfinding, returns nullptr for 
	std::vector<Tile*> getAdjacentTiles(std::pair<int, int> coord);
	//TODO:Returns tiles in a radius around a given tile, skipping the tile itself
	std::vector<Tile*> getTileRadius(std::pair<int, int> coord, int range);
	//TODO: Returns tiles in a cone emanating from a given tile, skipping the tile itself
	std::vector<Tile*> getTileCone(std::pair<int, int> coord, int range, eDirection direction);

	std::pair<int, int> getTileScreenPos(std::pair<int, int> coord) const;

	//Moves an entitys position on the map, returns false if the position is already taken
	bool moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos);
	//Places a new entity on the map (no check for duplicates yet so try to avoid creating multiples)
	void insertEntity(Entity* newEntity, std::pair<int, int> coord);

	void drawMap() const;
	std::pair<int, int> getDrawOffset() const { return m_drawOffset; }
	void setDrawOffset(std::pair<int, int> newOffset) { m_drawOffset = newOffset; }

	float getDrawScale() const { return m_drawScale; }
	void setDrawScale(float scale) { if (scale > 0.0) m_drawScale = scale; }

	float getWindStrength() const { return m_windStrength; }
	void setWindStrength(float strength) { if (strength > 0.0) m_windStrength = strength; }

	eDirection getWindDirection() const { return m_windDirection; }
	void setWindDirection(eDirection direction) { m_windDirection = direction; }
	
	//TODO: remove later
	std::vector<Tile>* getMap() { return &m_data; }

	//TODO: Get constructor working. Need tiled parser or load from xml set up
	Map(std::pair<int, int> size, const std::vector<std::vector<int>>& tileData);
};

