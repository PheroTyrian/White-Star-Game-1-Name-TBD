#ifndef BattleSystem_H
#define BattleSystem_H

#include<HAPISprites_lib.h>
#include<vector>
#include"entity.h"


class BattleSystem
{
private:
	//std::vector<std::pair<entity*, tile*>> m_entities; // need tile class
	bool running;
	void initialize();
	void update();
	void render();

public:
	BattleSystem();
	void run();
};

#endif
