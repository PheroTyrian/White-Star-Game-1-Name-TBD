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
	std::pair<int, int>oldPos;
	std::pair<int, int>newPos;
	bool running;
	Map m_map;
	UIWindowTest UIWind;
	std::pair<int, int>coord;
	bool select{ true };// temp boll used as trigger for the second selection
	int entityPositionInVector;
	
public:
	BattleSystem();
	~BattleSystem();

	void run();
};