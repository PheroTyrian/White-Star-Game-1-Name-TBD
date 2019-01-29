#pragma once
/*

HAPISprites_lib.h
=================
All the includes required to use the basic HAPI Sprites library wrapped in the HAPISPACE namespace.

HAPI Sprites is a 2D game API, written by Keith Ditchburn and free to use for all.

To import HAPI Sprites code library names:
using namespace HAPISPACE;

To use the UI you need to include its header:
#include <HAPISprites_UI.h>

To import UI names:
using namespace HAPI_UI_SPACE;

For full details on using the library please see the online documentation and demos: (CTRL-Click to Open)
https://scma-intranet.tees.ac.uk/users/u0018197/Happier/ 

Last modified by Keith Ditchburn: January 2019

Version 0.80 - 28/01/19
- Distributable creation
- Bug fixes and improvements to the UI code
- Editors feature complete, limited bug checking
- Changed to debug symbol generation (Zi to Z7) means symbols embedded in debug lib making it bigger (but should get rid of the pdb warning).
- Added support for windows text clipboard and updated UI text entry controls to use it
	GetClipboardText
	SetClipboardText
- UI is now solely accessed from HAPI_SPRITES which handles creation and rendering
- You can now specify not to scale a UI window on load
*/

// C++ standard library types required by HAPI Sprites
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <list>

// HAPI Sprites specific types
#include "HAPISprites_Types.h"

namespace HAPI_UI_SPACE
{
	class UISystem;
}

// All of HAPI Sprites is wrapped inside the HAPISPACE namespace
namespace HAPISPACE {

	// Public HAPI Sprites Interface
	class IHapiSprites
	{
	public:
		// Initialise to required size, optionally can provide a window title and can combine some flags see InitialisationFlags
		virtual bool Initialise(int &width, int &height, const std::string &windowTitle = std::string("HAPI Sprites"),
			unsigned int flags = eHSDefaults) = 0;

		// Create a surface from a texture, returns a shared pointer
		virtual std::shared_ptr<Surface> LoadSurface(const std::string &filename) const = 0;

		// Create a surface from scratch, returns a shared pointer
		// If filename is empty one is generated, colour defaults to Zero (all channels 0)
		virtual std::shared_ptr<Surface> MakeSurface(int width, int height, std::string filename = std::string(), Colour255 colour = Colour255::ZERO) const = 0;

		// Creates a full copy of a surface, returns new shared pointer
		virtual std::shared_ptr<Surface> CopySurface(const std::shared_ptr<Surface> &other) const = 0;

		// Returns the screen surface
		virtual std::shared_ptr<Surface> GetScreenSurface() const = 0;

		// Add listener to get input events from mouse, keyboard or controller
		// Note: done automatically when you create a class derived from IHapiSpritesInputListener but here in case you want to remove and add again
		virtual bool AddInputListener(IHapiSpritesInputListener &listener) = 0;

		// Remove input event listener  done automatically at destruction of IHapiSpritesInputListener derived class.
		virtual bool RemoveInputListener(IHapiSpritesInputListener &listener) = 0;

		// Load a spritesheet and create a sprite for it
		// This one has the advantage of loading full frame set data and normals etc.
		virtual std::unique_ptr<Sprite> LoadSprite(const std::string& xmlFilename, const std::string& textureRootPath = std::string()) const = 0;

		// Shortcut to create a surface, spritesheet and sprite (returned) from a texture filename
		virtual std::unique_ptr<Sprite> MakeSprite(const std::string& textureFilename, int numFrames = 1, bool horizontal = true, const std::string& frameSetName = std::string()) const = 0;

		// Make a sprite from an existing spritesheet
		virtual std::unique_ptr<Sprite> MakeSprite(std::shared_ptr<SpriteSheet> spriteSheet) const = 0;

		// Make a sprite from a surface - creates a default spritesheet
		virtual std::unique_ptr<Sprite> MakeSprite(std::shared_ptr<Surface> surface) const = 0;

		// Loads a spritesheet from an xml file
		// If texture not in same directory you can provide a path to it
		virtual std::shared_ptr<SpriteSheet> LoadSpritesheet(const std::string& xmlFilename, const std::string& textureRootPath = std::string()) const = 0;

		// Create a spritesheet from a surface and creates a basic frame layout
		virtual std::shared_ptr<SpriteSheet> MakeSpritesheet(const std::string& textureFilename, int numFrames = 1, bool horizontal = true) const = 0;

		// Creates a full copy of a spritesheet, returns new shared pointer
		virtual std::shared_ptr<SpriteSheet> CopySpritesheet(const std::shared_ptr<SpriteSheet> &other) const = 0;

		// Causes HAPI Sprites to close the window and return false in the next update call
		virtual bool Close() = 0;

		// HAPI Sprites must be regularly updated via this call. Returns false when app is closing.
		virtual bool Update() = 0;

		// Tells HAPI to display (or not) the frames per second, defaults to upper left (0,0)
		virtual bool SetShowFPS(bool set, int x = 0, int y = 0) = 0;

		// Retrieve the current FPS
		virtual int GetFPS() const = 0;

		// Returns a structure with the current mouse state
		virtual const MouseData& GetMouseData() const = 0;

		// Returns a structure with the current keyboard state
		virtual const KeyboardData& GetKeyboardData() const = 0;

		// Returns a structure with the specified controller index (up to GetMaxControllers) state
		// Note: you must check that the isAttached member is true
		virtual const ControllerData& GetControllerData(int controller) const = 0;

		// Returns the maximum number of controller devices that could be installed (not how many actually are)
		virtual int GetMaxControllers() const = 0;

		// Load a sound file, most formats are supported:
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool LoadSound(const std::string &filename) = 0;

		// Note: there is a hardware limit on how many sounds can be playing at once (1024 on PC but 32 on mobiles)
		virtual bool PlaySound(const std::string &filename) = 0;

		// As above but also allows volume, pan etc. to be changed from the defaults
		virtual bool PlaySound(const std::string &filename, const SoundOptions &options) = 0;

		// As above but also returns instanceId (for use when querying sound status)
		virtual bool PlaySound(const std::string &filename, const SoundOptions &options, int &instanceId) = 0;

		// Stop a sound early. Use the id retrieved from PlaySound
		virtual bool StopSound(int soundInstanceId, bool togglePause = false) = 0;

		// You can change things like pitch, volume and loop as the sound is playing
		virtual bool ChangePlayingSoundOptions(int &soundInstanceId, const SoundOptions &options) = 0;

		// Retrieve info on a sound instance e.g. to see if it is paused, finished etc.
		virtual const EMediaStatus GetSoundStatus(int soundInstanceId, float &playbackPosition) const = 0;

		// Plays streamed music using default options
		virtual bool PlayStreamedMedia(const std::string &filename, float startTimeOffset = 0.0f) = 0;

		// Plays streamed media and allows playback options to be set and instance obtained
		virtual bool PlayStreamedMedia(const std::string &filename, const SoundOptions& options, int &instanceId, float startTimeOffset = 0.0f) = 0;

		// Restarts streamed media that has already been played (more efficient than reloading it)
		virtual bool PlayStreamedMedia(int instanceId, float startTimeOffset = 0.0f) = 0;

		// Stops streaming media started via PlayStreamedMedia
		virtual bool StopStreamedMedia(int instanceId) const = 0;

		// Pause or unpause streaned media started with PlayStreamedMedia
		virtual bool PauseStreamedMedia(int instanceId, bool pause = true) const = 0;

		// Returns info about the media e.g. like current position
		virtual const StreamedMediaInfo& GetStreamedMediaInfo(int instanceId) const = 0;

		// Allows the font to be changed. This version uses fonts registered with the OS
		virtual bool ChangeFont(const std::string &fontName) = 0;

		// Allows the font to be changed. This version allows it to be loaded from a file.
		virtual bool ChangeFontFromFile(const std::string &fontFilename) = 0;

		// Allows text to be drawn on the screen at a position and with a specified fill colour with the current font
		// Text size and flags for bold, italic etc. are optional
		// Note: you can also do SCREEN_SURFACE.DrawText
		virtual bool RenderText(int x, int y, const Colour255 &fillColour, const std::string& text,
			int textSize = 12, unsigned int styleFlags = eRegular) = 0;

		// Version taking a vector
		// Note: you can also do SCREEN_SURFACE.DrawText
		virtual bool RenderText(VectorI p, const Colour255 &fillColour, const std::string& text,
			int textSize = 12, unsigned int styleFlags = eRegular) = 0;

		// As above but allows you to specify another colour to act as an outline to the text and how thick it should be
		// Note: you can also do SCREEN_SURFACE.DrawText
		// Can also supply a clip area
		virtual bool RenderText(int x, int y, const Colour255 &fillColour, const Colour255 &outlineColour,
			float outlineThickness, const std::string& text, int pointSize = 12, unsigned int styleFlags = eRegular,
			RectangleI clipArea = RectangleI()) = 0;

		// This one allows you to specify a memory buffer to write to
		// Assumes 32 bit and that surface points to surfaceWidth * surfaceHeight * 4 bytes
		// Can also supply a clip area
		virtual bool RenderText(int x, int y, Colour255 fillColour, Colour255 outlineColour,
			float outlineThickness, const std::string& text, BYTE *surface, int surfaceWidth, int surfaceHeight,
			int textSize = 12, unsigned int styleFlags = eRegular, bool writeAlpha = true, RectangleI clipArea = RectangleI()) = 0;

		// Allows the size of the text to be calculated. Returns the size in a rectangle.
		virtual RectangleI GetTextSize(const std::string& text, int pointSize, unsigned int styleFlags = eRegular, float outlineThickness = 0) = 0;

		// Helper to determine if a file exists on disk
		virtual bool DoesFileExist(const std::string &filename) const = 0;

		// Tells HAPI to display or not the cursor (mouse pointer)
		virtual bool SetShowCursor(bool set) = 0;

		// Text interpreted
		virtual char GetEnteredText() const = 0;

		// Brings up one of the internal apps. Note that all user code will now not operate until the app is closed
		// Current choices are eSpriteEditor, eSkinEditor, eUiEditor. Also can be accessed via SHIFT-F1 when running 
		virtual void LaunchInternalApp(EInternalApp which) = 0;

		// Sets the rumble motor speeds for the specified controller
		virtual bool SetControllerRumble(int controller, WORD leftMotor, WORD rightMotor) const = 0;

		// If a HAPI Sprites call fails this can be used to retrieve the error code and system the error occurred in
		// (see enum and documentation for more details on errors as well as the output pane in viz and the generated log)
		virtual EErrorCodes GetLastError(HAPI_System &system) const = 0;

		// Returns the time since HAPI Sprites was initialised in milliseconds
		virtual DWORD GetTime() const = 0;

		// Display a message box to the user, returns affirmative or not
		virtual EUserResponse UserMessage(const std::string &text, const std::string& title = "Message", EMessageButtonType buttonType = EMessageButtonType::eButtonTypeOk) = 0;

		// Displays a modal dialog box to get input from the user
		virtual bool UserTextEntry(const std::string &question, std::string &result,
			std::string initialText = std::string(), int dialogWidth = 0) = 0;

		// Display a file open or file save dialog
		// Opens at the programs current working directory, this can be changed if required via the optional parameter
		virtual bool FileDialog(const std::vector<FileDialogFilters>& filters, bool saveAsDialog,
			std::string &filename, HAPI_UserResponse &response,
			const std::string& startDirectory = std::string(), bool selectFolderOnly = false) = 0;

		// As above but specifically to allow multiple file selection
		virtual bool FileDialogMultipleSelect(const std::vector<FileDialogFilters>& filters,
			std::vector<std::string> &filenames, HAPI_UserResponse &response,
			const std::string& startDirectory = std::string()) = 0;

		// Allows the icon shown in the window and task bar to be changed from the default HAPI logo
		virtual bool SetIcon(const std::shared_ptr<Surface> &surface, int width, int height) const = 0;

		// Retrieve the directory the exe is running from
		virtual std::string GetExecutableDirectory() const = 0;

		// Retrieve the working directory
		// Useful as a root for then finding data files
		// This is normally where the visual studio project file is, if running under viz
		// otherwise it is where the executable directory is
		virtual std::string GetWorkingDirectory() const = 0;

		// Saves log to disk early. It always gets saved on exit but there are cases where you may want to dump it sooner.
		virtual void WriteLogSoFar() const = 0;

		// Helper function that simply divides a filename into its parts
		virtual FilenameParts SplitFilename(const std::string &filename) const = 0;

		// Enables the UI functionality in HAPI Sprites. Not needed if HS initialised with eHSEnableUI
		virtual bool EnableUI() = 0;

		// Returns the UI system. Note: if not already created will create on the fly.
		virtual HAPI_UI_SPACE::UISystem& GetUISystem() const = 0;

		// Get text in windows clipboard
		virtual std::string GetClipboardText() const = 0;

		// Set text in windows clipboard
		virtual void SetClipboardText(const std::string &newText) = 0;

		// HAPI Sprites Version as a string
		virtual std::string GetVersion() const = 0;

		// Allows access to the raw sample data of a sound
		// Note: to see how to interpret the raw data use the values from SoundFormat
		virtual bool GetSoundRawData(const std::string& filename, std::vector<SWORD>& rawData, SoundFormat &soundInfo) const = 0;

		// Allows changing a sound's sample data. Note: if sound is currently playing it will be stopped.
		virtual bool SetSoundRawData(const std::string& filename, const std::vector<SWORD>& rawData, const SoundFormat &soundInfo) = 0;

		// Allows a sound to be created from scratch.
		virtual bool CreateSound(const std::string& name, const std::vector<SWORD>& rawData, const SoundFormat &soundInfo) = 0;

		// Saves a sound under a new filename
		// Supported formats (derived from filename extension) :
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool SaveSound(const std::string& name, const std::string& newFilename) const = 0;
	};
}

// Returns the HAPI Sprites instance, prefer to use the HAPI_Sprites macro below
HAPISPACE::IHapiSprites& GetHAPI_Sprites();

// the HAPI Sprites instance macro. A shortcut to the above
#define HAPI_Sprites GetHAPI_Sprites()

// Another shortcut, this time to the, always present, screen surface
#define SCREEN_SURFACE HAPI_Sprites.GetScreenSurface()

// Provides access to the UI system. Make sure you have created it first either via init flag or direct call and included <HAPISprites_UI.h>
#define UI HAPI_Sprites.GetUISystem()