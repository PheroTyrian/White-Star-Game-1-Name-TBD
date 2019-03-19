#include "BattleSystem.h"
#include "Utilities/MapParser.h"
////////////////////////////////////////////////////////
//move all code out of main into here
// build stage to here to place an entity 
//set move stage 
///////////////////////////////////////////////////////

BattleSystem::BattleSystem() : 
	running{ true },
	m_map(MapParser::parseMap("Data\\Level1.tmx")),
	UIWind()
{
	Entity* testShip = new Entity("Data\\mouseCrossHair.xml");
	m_entities.push_back(
		std::pair<Entity*, std::pair<int, int> >(testShip, std::pair<int, int>(4, 4)));
}

BattleSystem::~BattleSystem()
{
	for (auto it : m_entities)
	{
		delete it.first;
	}
	m_entities.clear();
}

void BattleSystem::update()
{
	//temp map set entity to tile 
	m_map.insertEntity(m_entities[0].first,m_entities[0].second);

	while (HAPI_Sprites.Update())// work on adding entity tile switching is temp 
	{
		SCREEN_SURFACE->Clear();

		m_map.drawMap();
		m_entities[0].first->render();
		UIWind.Update();

		for (int x = 0; x < m_map.getMap()->size(); x++) // temp these 2 vectors not gonna be public had to get test working 
		{
			//need to talk to tristan about entitys
			UIWind.HandleCollision(*UIWind.storage[UIWind.storage.size() - 1], *m_map.getMap()->data()[x].m_sprite);
		}
	}
	running = false;
}

void BattleSystem::run()
{
	while (running) //Why are there two while loops nested! (Here and one in update)
	{
		update();
	}
}