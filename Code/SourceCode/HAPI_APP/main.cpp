#include "OverworldUI.h"
using namespace HAPISPACE;

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