#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Entity.h"
#include "Map.h"
#include "UIClass.h"

using namespace HAPISPACE;

class BattleSystem
{
private:
	//Vector of a pair: ptr to entity, the tile coordinates of that entity (on the battle map)
	std::vector<std::pair<Entity*, std::pair<int, int> > > m_entities;

	bool running;
	void update();
	void render();
	int width{ 1280 };
	int height{ 800 };
	Map* map;

public:
	BattleSystem();
	~BattleSystem();
	void run();
};

