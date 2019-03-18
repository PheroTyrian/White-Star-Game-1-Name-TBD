#include "BattleSystem.h"
////////////////////////////////////////////////////////
//move all code out of main into here
// build stage to here to place an entity 
//set move stage 
///////////////////////////////////////////////////////

BattleSystem::BattleSystem() : width(1280), height(800), 
	running{ true }
{
	
	map = new Map(40, 22);
	if (!HAPI_Sprites.Initialise(width, height, "test Demo ", eHSEnableUI))//it lies
		return;
	HAPI_Sprites.SetShowFPS(true);

	Entity* testShip = new Entity("Data\\mouseCrossHair.xml");
	m_entities.push_back(
		std::pair<Entity*, std::pair<int, int> >(testShip, std::pair<int, int>(4, 4)));

	UIWind = new UIWindowTest();
	UIWind->Initialise();
}

BattleSystem::~BattleSystem()
{
	delete map;
	delete UIWind;
}

void BattleSystem::update()
{
	//temp map set entity to tile 
	////////
	map->insertEntity(m_entities[0].first,m_entities[0].second);
	////////



	while (HAPI_Sprites.Update())// work on adding entity tile switching is temp 
	{


		SCREEN_SURFACE->Clear();

		map->drawMap();
		m_entities[0].first->render();
		UIWind->Update();

		for (int x = 0; x < map->getMap()->size(); x++) // temp these 2 vectors not gonna be public had to get test working 
		{
			//need to talk to tristan about entitys
			UIWind->HandleCollision(*UIWind->storage[UIWind->storage.size() - 1], *map->getMap()->data()[x].m_sprite);
		}
	}
	running = false;
}

void BattleSystem::render()
{

	
}

void BattleSystem::run()
{
	while (running == true) //Why are there two while loops nested! (Here and one in update)
	{
		update();
		render();
	}
}