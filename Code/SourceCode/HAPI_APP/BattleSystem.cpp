#include "BattleSystem.h"

battleSystem::battleSystem()
{

}

bool battleSystem::initualize()
{
	entity newEntity("Data\\thing.png");
	newEntity.setTileLocation(HAPISPACE::VectorI(50, 50));
	newEntity.getSprite().GetTransformComp().SetPosition(newEntity.getTileLocation());
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
		e.getSprite().Render(SCREEN_SURFACE);
	}
	return true;
}

battleSystem::~battleSystem()
{
}
