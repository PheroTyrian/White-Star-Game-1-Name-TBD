#include "BattleSystem.h"
////////////////////////////////////////////////////////
//move all code out of main into here
// build stage to here to place an entity 
//set move stage 
///////////////////////////////////////////////////////

BattleSystem::BattleSystem() : running{ true }
{

}

void BattleSystem::initialize()
{// temp 
	if (!HAPI_Sprites.Initialise(width, height, "HAPI Sprites selection testing ", eHSEnableUI))//it lies
		return;


}

void BattleSystem::update()
{

}

void BattleSystem::render()
{

	UIWIndowTest UIWIndowTest({ width,height });
	if (!UIWIndowTest.Initialise())
	{
		HAPI_Sprites.UserMessage("Could not initialise selection test", "Error");
		return;
	}
	Map mappy(22, 22);


	while (HAPI_Sprites.Update())// work on adding entity tile switching is temp 
	{

		SCREEN_SURFACE->Clear();

		mappy.drawMap();
		UIWIndowTest.Update();

		for (int x = 0; x < mappy.m_data.size(); x++) // temp these 2 vectors not gonna be public had to get test working 
		{
			//need to talk to tristan about enitys
			UIWIndowTest.HandleCollision(*UIWIndowTest.storage[UIWIndowTest.storage.size() - 1], *mappy.m_data[x].m_sprite);
		}
	}
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