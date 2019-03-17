#include "BattleSystem.h"
////////////////////////////////////////////////////////
//move all code out of main into here
// build stage to here to place an entity 
//set move stage 
///////////////////////////////////////////////////////

BattleSystem::BattleSystem() : running{ true }
{
	mappy = new Map(40, 22);
}

BattleSystem::~BattleSystem()
{
	delete mappy;
}

void BattleSystem::initialize()
{// temp 
	if (!HAPI_Sprites.Initialise(width, height, "test Demo ", eHSEnableUI))//it lies
		return;
	HAPI_Sprites.SetShowFPS(true);

	Entity* testShip = new Entity("Data\\mouseCrossHair.xml");
	m_entities.push_back(
		std::pair<Entity*, std::pair<int, int> >(testShip,std::pair<int, int>(20,20)));

}

void BattleSystem::update()
{
	UIWIndowTest UIWIndowTest({ width,height });
	if (!UIWIndowTest.Initialise())
	{
		HAPI_Sprites.UserMessage("Could not initialise", "Error");
		return;
	}

	//temp map set entity to tile 
	////////
	mappy->insertEntity(m_entities[0].first,m_entities[0].second);
	////////



	while (HAPI_Sprites.Update())// work on adding entity tile switching is temp 
	{

		SCREEN_SURFACE->Clear();

		mappy->drawMap();
		m_entities[0].first->render();
		UIWIndowTest.Update();

		for (int x = 0; x < mappy->getMap()->size(); x++) // temp these 2 vectors not gonna be public had to get test working 
		{
			//need to talk to tristan about entitys
			UIWIndowTest.HandleCollision(*UIWIndowTest.storage[UIWIndowTest.storage.size() - 1], *mappy->getMap()->data()[x].m_sprite);
		}
	}
	running = false;
}

void BattleSystem::render()
{

	
}

void BattleSystem::run()
{
	initialize();
	while (running == true) //Why are there two while loops nested! (Here and one in update)
	{
		update();
		render();
	}
}