#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include<HAPISprites_lib.h>
#include<vector>
#include"entity.h"
#include "Rendering.h"

class battleSystem
{
private:
	//std::vector<std::pair<entity*, tile*>> m_entities; // need tile class
	bool running;
	void initialize();
	void update();
	void render();

public:
	battleSystem();
	void run();
};

#endif
