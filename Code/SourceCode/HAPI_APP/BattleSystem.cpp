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
	Entity* testShip2 = new Entity("Data\\thingy.xml");
	m_entities.push_back(std::pair<Entity*, std::pair<int, int> >(testShip, std::pair<int, int>(4, 4)));
	m_entities.push_back(std::pair<Entity*, std::pair<int, int> >(testShip2, std::pair<int, int>(5, 5)));


	for (auto i : m_entities)
	{
		m_map.insertEntity(i.first, i.second);
	}
	
	
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

	{
	std::pair<float,float> tempTileLocation ;
	
	while (HAPI_Sprites.Update())
	{
		SCREEN_SURFACE->Clear();
		
		m_map.drawMap();
		
		UIWind.Update();
		
		for (int x = 0; x < m_map.getMap()->size(); x++) // temp these 2 vectors not gonna be public had to get test working 
		{
			UIWind.HandleCollision(*UIWind.storage[UIWind.storage.size() - 1], *m_map.getMap()->data()[x].m_sprite);
			tempTileLocation = std::pair<float, float>(m_map.getMap()->data()[x].m_sprite->GetTransformComp().GetPosition().x, m_map.getMap()->data()[x].m_sprite->GetTransformComp().GetPosition().y);
		
			if (tempTileLocation == UIWind.tilePos)
			{
				
				coord = m_map.getMap()->data()[x].m_tileCoordinate;
			}
			
			for (int x = 0; x < m_entities.size(); x++)
			{
				if (m_entities[x].second == coord)
				{
					entityPositionInVector = x;
				}
			}
			
			if (m_map.moveEntity(std::pair<int, int>(m_entities[entityPositionInVector].second), coord))
			{
				m_entities[entityPositionInVector].second = coord;
			}

			if (m_map.getMap()->data()[x].m_entityOnTile != nullptr)
			{
				m_map.getMap()->data()[x].m_entityOnTile->getSprite().GetTransformComp().SetPosition({ m_map.getMap()->data()[x].m_sprite->GetTransformComp().GetPosition().x + 30, m_map.getMap()->data()[x].m_sprite->GetTransformComp().GetPosition().y + 40 });
				m_map.getMap()->data()[x].m_entityOnTile->render();
	
			}
			
		}
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