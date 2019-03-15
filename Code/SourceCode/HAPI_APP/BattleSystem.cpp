#include "BattleSystem.h"

battleSystem::battleSystem()
{

}

bool battleSystem::initualize()
{
	entity newEntity("Data\\thing.png");
	newEntity.setTileLocation(HAPISPACE::VectorI(50, 50));
	entities.push_back(newEntity);
	return true;
}

bool battleSystem::update()
{
	return true;
}

bool battleSystem::render()
{
	for (auto &e : entities)
	{
		e.render();
	}
	return true;
}

battleSystem::~battleSystem()
{
}
