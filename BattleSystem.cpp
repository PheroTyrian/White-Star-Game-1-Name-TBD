#include "BattleSystem.h"

BattleSystem::BattleSystem() : running{true}
{

}

void BattleSystem::initialize()
{// temp 



}

void BattleSystem::update()
{

}

void BattleSystem::render()
{

}

void BattleSystem::run()
{
	initialize();
	while (running = true)
	{
		update();
		render();
	}
}