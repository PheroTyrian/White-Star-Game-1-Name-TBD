/*
	HAPI Sprites Start
	------------------
	This solution contains an already set up HAPI Sprites project and this main file with brief example code
	Release and Debug configurations for 64 bit builds are configured for you (32 bit is not supported).
	
	The directory structure and main files are:

	HAPISprites_Start - contains the Visual Studio solution file (HAPI_APP.sln)
		HAPI_APP - contains the Visual Studio HAPI_APP project file (HAPI_APP.vcxproj) and source code
			HAPI_SPRITES - the directory with all the HAPI library files including GLM
			Data - a place to put all your data files

		MakeDistributable.bat - a batch file for populating a Demo folder for stand alone running
		Clean.bat  - a batch file to remove all the Viz temporary files and hence reduce the size of the folder		
		Note: this should be run with Visual Studio closed

	Last updated: Keith Ditchburn 17/01/19
*/

// To use HAPI Sprites you need to include the header
#include <HAPISprites_Lib.h>
#include "Map.h"

// All of the core HAPI Sprites functionality is in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPISprites program must have a HAPI_Sprites_Main as an entry point
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	int width{ 1600 };
	int height{ 900 };

	// We must initialise HAPI Sprites before doing anything with it
	// This call allows us to specify the screen size and optionally the window title text and flags
	if (!HAPI_Sprites.Initialise(width, height, "HAPI Sprites Start Project"))
		return;

	/*
	Main Classes
	============
	HAPI Sprites has three main classes. A sprite class that contains a spritesheet that in turn contains a surface.

	1. Surface - this class represents an image. It can be a loaded texture or it could be one you write to (HW or SW).
	Every spritesheet has a surface but a surface can also be stand alone. The main screen is a hardware surface so
	all writing to it uses hardware acceleration (via OpenGL). Since this is used so often there is a shortcut to it
	via the macro SCREEN_SURFACE. It always exists.

	2. Spritesheet - this class holds information about the make up of a surface i.e. areas that define frames along
	with information about which frames make up an animation (via a named animation set). It can also create collision
	normals from the surface data.

	3. Sprite - this class handles all the rendering of a 2D image as well as maintaining position and collision handling etc.

	In practice
	===========
	Practially you do most of your work via the Sprite class
	A sprite can be created in a number of ways, the main two are:

	1. From scratch. This is created by loading a texture. Frames can be derived from some basic parameters but it does not create
	multiple frames of different sizes or framesets.

	2. From an xml file. The XML file format used is the one created for the Stirling framework. While there is no real standard
	format this one is used by a few sites that provide free 2D spritesheets. You can also use the HAPI Sprites built in editor
	to create one yourself. This supports frames and animation sets etc.
	*/

	/*
	Example of creating a Sprite from scratch

	This call specifies a texture filename and that it has 4 equally sized horizontally laid out frames
	HAPI Sprites returns a unique_ptr to the created sprite. You can keep it unique or convert it to shared (done here)
	*/

	/*
	Example of creating a Sprite from an XML file

	/*

	/*
	Sprite Operations
	There are a number of functions you can apply once a sprite is created

	For example the helicopter above does not have an alpha channel, however the black colour is used for transparency
	so we can convert it to alpha.
	*/
	Map* map = new Map(40, 20);

	/*
	Once loading is complete you can enter the main loop
	Update must be called for each frame
	HAPI Sprites will return true from Update until the user closes the window or you manually call close
	*/
	while (HAPI_Sprites.Update())
	{
		/*
		We need to clear the screen prior to drawing
		We can use the screen surface macro
		*/
		SCREEN_SURFACE->Clear();
		map->drawMap();
		map->setDrawScale(map->getDrawScale() + 0.01);


	}
	delete map;
}
