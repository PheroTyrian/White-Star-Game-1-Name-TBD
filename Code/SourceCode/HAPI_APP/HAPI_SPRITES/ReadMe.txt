/*! /mainpage HAPI Sprites is a 2D game API, written by Keith Ditchburn and free to use for all.

To use the library link to HAPI_Sprites_Debug64.lib or HAPI_Sprites_Release64.lib. They require Visual Stuido 2017 and Windows 10.

The recent version changes are in the HAPI_Sprites_Lib.h file with just the older ones listed here.

Older Version History

 Version 0.82 - 12/02/19
 - SpriteEditor: BUG: changing the animation set was not saving
 - Massively reduced the debug library size 
 
 Version 0.81 - 11/02/19
 - New: Added a Sprite Viewer to allow testing of sprites. Sprite Editor now has a play button to
 call it but can also be called manually via the UI e.g. UI.SpriteViewer
 - UIEditor: Fixed a bug with the position / size dialog  where some buttons were not pressable
 - UIEditor: Fixed a bug when copying a marker collider, it was getting a unique name generated
 which was not what was wanted
 - UIEditor: Removed debug text and inserted user warnings when actions cannot be carried out
 - UIEditor: Added Save As option to menu
 - Made some debug output debug mode only, removed others
 - Small optimisations and code clean up
 - Removed old map editor code for efficiency. May reinstate if I decide to make one but Tiled is
 free and does the job.
 - Some useability improvements to editors
 - BUG: Fixed an error in SizeToFitContents that included the title bar twice
 - BUG: Fixed an issue where clearing a HW render surface did not use clip area
 - Improved UI message box to be modal but still render background correctly
 - Improved error messages
 - Reduced library file sizes a bit (~8%)  
 - Switched to using Doxygen comments for automatic documentation generation

Version 0.80 - 28/01/19
- Distributable creation
- Bug fixes and improvements to the UI code
- Editors feature complete, limited bug checking
- Changed to debug symbol generation (Zi to Z7) means symbols embedded in debug lib making it
 bigger (but should get rid of the pdb warning).
 - Added support for windows text clipboard and updated UI text entry controls to use it
 	GetClipboardText
 	SetClipboardText
- UI is now solely accessed from HAPI_SPRITES which handles creation and rendering
- You can now specify not to scale a UI window on load. 

Version 0.70 - 7/10/18
- Added gradient shading to the surface
- Changed structure and usage so sprite now contains simulation data, moving more toward a 2D engine than API.
- Added components for colliders and transform and restructured parts of the systems and where responsibility lies
e.g. a sprite now has a spritesheet which has a surface. A spritesheet can hold colliders per frame (and save and load them)
- Added expanded collider functionality (see collider demo)
- Text can now be drawn to any surface and is draw order dependant
- Added in built spritesheet editor including addition of collider editing and auto-fit functionality
- Expanded the XML file format to include collider data
- Improved UI
- UIWindow class that can be set to allow user drawing
- UI must be created prior to use (see examples)
- UI can now save objects
- UI supports tags to allow global positioning and settings changes etc.
- UI can be scaled and the editor uses this to reposition and resize UI objects for different screen sizes
- UI text entry supports highlighting text
- UI text entry now supports CTRL-C,V,X,A
- Improved the demo wrapper and added more demos
- SaveImageFile now correctly saves just an area if one is provided
- Many minor improvements and bug fixes
- Optimisations to rendering and collision detection

Version 0.60 - 04/04/17
- Added new function GetEnteredText to get text input without having to translate key pressed
- Added OnTextEntered to callback
- Added BlitRotatedAndScaled functions to Surface and in the process fixed a problem in the CreateScaled fn
- added operator* to Rectangle
- added a RenderText that takes a point
- OnKeyEvent now sends key pressed messages the whole time key is down and then 1 release message
- Sprite loading now looks in xml file path if cannot find image using normal path
- Fixed a problem with the right shift key in HAPI
- Added ToString to all basic types

Version 0.58 25/03/17
- Some rectangle functions (e.g. Translate) now also return *this for chaining
- Added Encompass to Rectangle
- Added == operator to Rectangle
- Made it so clipped rectangle outlines do not show edge against clipped

Version 0.57 - 17/03/17
- Removed the need to pass x,y values to GetTextSize - no idea why they were there! as well as
getting it to return a rect rather than the need to pass ints to it and fixed an issue with it
- Fixed a problem with CHapiXML HasData() sometimes returning true incorrectly
- Added a Sprite GetFrame function to get frame information by name
- When errors occur HAPI now immediately writes the log to disk
- You can also force the log to be written early via a new call: WriteLogSoFar()

Version 0.56 - 03/03/2017
- Added a LoadedOK function to the sprite to make it easier to check without going via the surface
- Fixed a rare crash when rendering a filled rectangle

Version 0.55 - 26/02/2017
- Added a default constructor for Frame
- Changed blitting lambdas to also pass the current source position
- Minor adjustment to MakeAlphaChannelFromColourKey so exact matches pass
- Added new optional param to Blit and BlitRotated to allow colour modulation
- And to sprite render calls
- Creating scaled sprites now stops if width or height are 0
- SetPixel now sets alpha so scaling sprites does as well
HAPI Core Changes (2.22)
- Added a * operator for Colour255 to allow modulation of one colour by another
- added XML GetByteStream and a byte stream constructor
- Fixed some logging output that was splitting lines incorrectly
- New function: GetExecutableDirectory - does what it says!

Version 0.50 - 05/02/2017
- Surface functions added:
- GetPixel
- CreateScaled
- Sprite functions added:
- CreateSizes
Added insertion operators for VectorFs, lines and Rectangles

Version 0.40 - 16/01/2017
- Testing & Optimisations
- Initial documentation

Version 0.40 - 09/01/2017
- Bug fixing
- Pixel perfect collisions implemented
- Further operators added
- Rotation working

Previous Versions
- Initial builds
*/