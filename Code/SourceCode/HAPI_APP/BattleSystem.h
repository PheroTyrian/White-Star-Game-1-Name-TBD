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
	void update();

	std::vector<std::pair<Entity*, std::pair<int, int>>> m_entities;
	bool running;
	Map m_map;
	UIWindowTest UIWind;
	
public:
	BattleSystem();
	~BattleSystem();

	void run();
};