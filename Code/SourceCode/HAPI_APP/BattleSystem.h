#ifndef BattleSystem_H
#define BattleSystem_H

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
	std::vector<std::pair<Entity*, std::pair<int, int> > > m_entities;

	bool running;
	void initialize();
	void update();
	void render();
	int width{ 1280 };
	int height{ 800 };
	Map* mappy;

public:
	BattleSystem();
	~BattleSystem();
	void run();

};

#endif
