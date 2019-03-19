// To use HAPI Sprites you need to include the header

#include "BattleSystem.h"

// All of the core HAPI Sprites functionality is in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPISprites program must have a HAPI_Sprites_Main as an entry point
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1280, 800);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "test Demo ", eHSEnableUI))//it lies
		return;
	HAPI_Sprites.SetShowFPS(true);

	BattleSystem world;
	world.run();

}