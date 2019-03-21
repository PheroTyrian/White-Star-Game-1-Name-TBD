#include "OverworldUI.h"
// All of the core HAPI Sprites functionality is in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPISprites program must have a HAPI_Sprites_Main as an entry point
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1600, 900);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "test Demo ", eHSEnableUI))//it lies
		return;

	OverworldUIWIndowTest OverworldUIWIndowTest({ windowSize.first, windowSize.second });
	if (!OverworldUIWIndowTest.Initialise())
		return;

	HAPI_Sprites.SetShowFPS(true);

	OverworldUIWIndowTest.Run();
}