#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include<HAPISprites_lib.h>
#include<vector>
#include"entity.h"


class battleSystem
{
private:
	//std::vector<std::pair<entity*, tile*>> m_entities; // need tile class
	std::vector<entity> entities;
public:
	battleSystem();
	bool initualize();
	bool update();
	bool render();
	~battleSystem();
};

#endif
