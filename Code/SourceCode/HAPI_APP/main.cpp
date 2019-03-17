// To use HAPI Sprites you need to include the header

#include "BattleSystem.h"

// All of the core HAPI Sprites functionality is in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPISprites program must have a HAPI_Sprites_Main as an entry point
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	BattleSystem world;
	world.run();

}