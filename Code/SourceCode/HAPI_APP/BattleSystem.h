#ifndef BattleSystem_H
#define BattleSystem_H

#include<HAPISprites_lib.h>
#include<vector>
#include"entity.h"
#include "Map.h"
#include "UIClass.h"
using namespace HAPISPACE;



class BattleSystem
{


private:
	//std::vector<std::pair<entity*, tile*>> m_entities; // need tile class

	bool running;
	void initialize();
	void update();
	void render();
	int width{ 1280 };
	int height{ 800 };


public:
	BattleSystem();
	void run();

};

#endif
