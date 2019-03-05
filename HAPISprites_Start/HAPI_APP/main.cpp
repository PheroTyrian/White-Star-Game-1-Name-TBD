
#include "UIClass.h"

// need to set up so only one player for now press that then press where to move to update ;
//need to store previous sprite selecteed
void HAPI_Sprites_Main()
{
	
	int width{ 1280 };// does this want moving into ui
	int height{ 800 };
	if (!HAPI_Sprites.Initialise(width, height, "HAPI Sprites selection testing ", eHSEnableUI))
		return;

	UIWIndowTest UIWIndowTest({ width,height });
	if (!UIWIndowTest.Initialise())
	{
		HAPI_Sprites.UserMessage("Could not initialise selection test", "Error");
		return;
	}

	
	while (HAPI_Sprites.Update())
	{
		
		SCREEN_SURFACE->Clear();
		UIWIndowTest.Update();
	
	}
}



