#include "BattleSystem.h"

battleSystem::battleSystem() : running{true}
{

}

void battleSystem::initialize()
{

}

void battleSystem::update()
{

}

void battleSystem::render()
{

}

void battleSystem::run()
{
	initialize();
	while (running = true)
	{
		update();
		render();
	}
}