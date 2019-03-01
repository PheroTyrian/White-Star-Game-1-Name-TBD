#pragma once
/*
HAPI_lib.h
All the includes required to use HAPI.

For full details on using the library please see the online documentation (CTRL-Click to open):
https://scm-intranet.tees.ac.uk/users/u0018197/HAPI/HAPI-Reference/hapi-reference.html

Last modified by Keith Ditchburn: August 2019

Version 2.52 ON GOING
=====================



Version 2.51 28/01/19
=====================
- Changed to debug symbol generation (Zi to Z7) means symbols embedded in debug lib making it bigger
but should get rid of the pdb warning.

Version 2.50 20/08/18
=====================
- Removed video playback. It was not being used much and added a lot of dependant dlls. This means it is
much easier to distribute your game. The streamed media functions remain but now only support music playback.
- Removed other dependencies. This means HAPI now only needs OpenAL32.dll (and the Viz libs of course)
- Reworked streaming audio playback due to the above changes
- Rebuilt for latest Viz ready for new year
- Additional additions for HAPI Sprites support (mainly under the hood)

Version 2.35 05/02/18
=====================
- Completely altered the way text rendering is handled meaning it now renders direct to the surface
(previously it queued up the text and rendered it as the last thing before displaying)
This means that text is drawn in the correct order. It also means text can be drawn to any memory block
and a new RenderText function has been added to support this.
- Surface text rendering now takes a flag to determine if it writes alpha or not
- Added function: GetWorkingDirectory
- Added some further helper XML functions
- Includes fix for lab / home version mismatch for release mode (the other failed)
- Open dialog now uses result as initial filename (if not empty)

Older version changes are listed in the ReadMe.txt file
*/

// Standard library types required to use HAPI
#include <string>
#include <vector>
#include <iostream>

// Key and controller codes header
#include "HAPI_InputCodes.h"

// Enums and structs used with HAPI
#include "HAPI_Types.h"

// All of HAPI is inside the HAPISPACE namespace
namespace HAPISPACE {

	// Public HAPI interface
	class IHapi
	{
	public:
		// Initialise HAPI, pass in the required screen size, title and any init flags
		// Note: width and height values may be altered to a legal size by HAPI when running full screen
		virtual bool Initialise(int &width, int &height, const std::string &title=std::string(), unsigned int flags = eDefaults) = 0;

		// Causes HAPI to close the window and return false in the next update call
		virtual bool Close() = 0;

		// Tells HAPI to display (or not) the frames per second
		// Optionally can provide position and colour
		virtual bool SetShowFPS(bool set, int x=0, int y=0, HAPI_TColour& col=HAPI_TColour::WHITE) = 0;

		// Retrieve the current FPS as a value
		virtual int GetFPS() const = 0;

		// Tells HAPI to display or not the cursor (mouse pointer)
		virtual bool SetShowCursor(bool set) = 0;

		// Update HAPI, must be called each game loop. Returns false when user closes the window or an error occurs
		virtual bool Update() = 0;

		// Returns a pointer to the screen data. There are 4 bytes per pixel, in RGBA byte order
		// On error returns nullptr, use HAPI_GetLastError to determine error reason or look in output pane
		// Note: The screen pointer is guaranteed to be aligned to a 16 byte boundary (128 bit)
		virtual BYTE* GetScreenPointer() const = 0;

		// Returns a structure with the current mouse state
		virtual const HAPI_TMouseData& GetMouseData() const = 0;

		// Returns a structure with the current keyboard state
		virtual const HAPI_TKeyboardData& GetKeyboardData() const = 0;

		// Returns the maximum number of controller devices that could be installed (not how many actually are)
		virtual int GetMaxControllers() const = 0;

		// Returns a structure with the specified controller index (up to GetMaxControllers) state
		// Note: you must check that the isAttached member is true
		virtual const HAPI_TControllerData& GetControllerData(int controller) const = 0;

		// Sets the rumble motor speeds for the specified controller
		virtual bool SetControllerRumble(int controller, WORD leftMotor, WORD rightMotor) const = 0;

		// Load a sound file, supported fromats: ogg, wav, flac
		virtual bool LoadSound(const std::string &filename) = 0;

		// Plays a sound. If filename not already loaded (via LoadSound) this loads it on demand
		// Note: there is a hardware limit on how many sounds can be playing at once (1024 on PC but 32 on mobiles)
		virtual bool PlaySound(const std::string &filename) = 0;

		// As PlaySound above but also allows volume, pan etc. to be changed from the defaults
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options) = 0;

		// As PlaySound above but also returns instanceId (for use when querying sound status)
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options, int &instanceId) = 0;

		// You can change things like pitch, volume and loop as the sound is playing
		virtual bool ChangePlayingSoundOptions(int &soundInstanceId, const HAPI_TSoundOptions &options) = 0;

		// Stop a sound early. Use the id retrieved from PlaySound
		virtual bool StopSound(int soundInstanceId, bool togglePause = false) = 0;

		// Retrieve info on a sound instance e.g. to see if it is paused, finished etc.
		virtual const HAPI_MediaStatus GetSoundStatus(int soundInstanceId, float &playbackPosition) = 0;

		// Allows access to the raw sample data of a sound
		// Note: to see how to interpret the raw data use the values from HAPI_TSoundFormat
		virtual bool GetSoundRawData(const std::string& filename, std::vector<SWORD>& rawData, HAPI_TSoundFormat &soundInfo) = 0;

		// Allows changing a sound's sample data. Note: if sound is currently playing it will be stopped.
		virtual bool SetSoundRawData(const std::string& filename, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Allows a sound to be created from scratch.
		virtual bool CreateSound(const std::string& name, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Saves a sound under a new filename
		// Supported formats (derived from filename extension) :
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool SaveSound(const std::string& name, const std::string& newFilename) const = 0;

		// Plays streamed music using default options
		// Supported formats: WAV(PCM only), OGG / Vorbis, FLAC.
		virtual bool PlayStreamedMedia(const std::string &filename, float startTimeOffset=0) = 0;

		// Plays streamed music and allows playback options to be set and instance obtained
		virtual bool PlayStreamedMedia(const std::string &filename, const HAPI_TSoundOptions& options, int &instanceId, float startTimeOffset=0) = 0;

		// Restarts streamed media that has already been played (more efficient than reloading it)
		virtual bool PlayStreamedMedia(int instanceId, float startTimeOffset=0) = 0;

		// Stops streaming media started via PlayStreamedMedia
		virtual bool StopStreamedMedia(int instanceId) = 0;

		// Pause or unpause streaned media started with PlayStreamedMedia
		virtual bool PauseStreamedMedia(int instanceId, bool pause = true) = 0;

		// Returns info about the media e.g. like current time position
		virtual const HAPI_TStreamedMediaInfo& GetStreamedMediaInfo(int instanceId) const = 0;

		// If a HAPI call fails this can be used to retrieve the error code and system the error occurred in
		// (see HAPI_ErrorCodes enum above and online documentation for more details on errors)
		virtual HAPI_ErrorCodes GetLastError(HAPI_System &system) const = 0;

		// Returns the time since HAPI was initialised in milliseconds
		virtual DWORD GetTime() const = 0;

		// Allows the font to be changed. This version uses fonts already registered with the OS e.g. "Arial"
		virtual bool ChangeFont(const std::string &fontName) = 0;

		// Allows the font to be changed. This version allows it to be loaded from a file.
		virtual bool ChangeFontFromFile(const std::string &fontFilename) = 0;

		// Allows text to be drawn on the screen at a position and with a specified fill colour with the current font
		// Text size and flags for bold, italic etc. are optional
		virtual bool RenderText(int x, int y, HAPI_TColour fillColour, const std::string& text,
			int textSize=12,unsigned int styleFlags=eRegular) = 0;

		// As RenderText but allows you to specify another colour to act as an outline to the text and how thick it should be
		// Can also provide a clip area
		virtual bool RenderText(int x, int y, HAPI_TColour fillColour, HAPI_TColour outlineColour,
			float outlineThickness, const std::string& text,
			int textSize = 12, unsigned int styleFlags = eRegular,
			int clipLeft = 0, int clipRight = 0, int clipTop = 0, int clipBottom = 0) = 0;

		// This one allows you to specify a different surface to write to than the screen
		// Assumes 32 bit RGBA and that surface points to a buffer of size: surfaceWidth * surfaceHeight * 4 bytes
		// Can also provide a clip area
		// If write alpha is true then the full colour is rendered but alpha set appropriately
		// - useful when subsequently blitting this surface to another one
		virtual bool RenderText(int x, int y, HAPI_TColour fillColour, HAPI_TColour outlineColour,
			float outlineThickness, const std::string& text, BYTE *surface, int surfaceWidth, int surfaceHeight,
			int textSize = 12, unsigned int styleFlags = eRegular,bool writeAlpha = true,
			int clipLeft=0, int clipRight=0,int clipTop=0, int clipBottom=0) = 0;

		// Allows the size of the text to be calculated. Returns the size in variables
		virtual bool GetTextSize(const std::string& text, int textSize,
			unsigned int styleFlags, int &left, int &right, int &top, int &bottom, float outlineThickness = 0) = 0;

		// Loads a texture, supports most PC formats. There are 4 bytes per texel in RGBA format
		// Caller is responsible for deleting the allocated data (use delete[])
		// Also returns width and height of texture
		virtual bool LoadTexture(const std::string &filename, BYTE** data, int &width, int &height) = 0;

		// As above but aligns texture to a 16 byte boundary (128 bit)
		// To delete this data you must call: _aligned_free(data);
		virtual bool LoadTextureAligned(const std::string &filename, BYTE** data, int &width, int &height) = 0;

		// Saves the data in rawImageData to a file
		// Width and height must be correct and assumes 4 bytes per pixel.
		// If no path information is in the filename it is saved to the working directory
		// supported formats are bmp, png, tga and jpg - format is derived from filename extension
		virtual bool SaveImage(BYTE *rawImageData, int width, int height, const std::string &filename) const = 0;

		// Display a message box to the user
		virtual bool UserMessage(const std::string &text, const std::string& title,
			HAPI_ButtonType buttonType = HAPI_ButtonType::eButtonTypeOk, HAPI_UserResponse *userResponse = nullptr) = 0;

		// Displays a modal dialog box to get input from the user
		virtual bool UserTextEntry(const std::string &question, std::string &result, 
			std::string initialText = std::string(), int dialogWidth = 0) = 0;

		// Display a file open or file save dialog
		// Opens at the programs current working directory, this can be changed if required via the optional parameter
		virtual bool FileDialog(const std::vector<HAPI_TFileDialogFilters>& filters, bool saveAsDialog,
			std::string &filename, HAPI_UserResponse &response, 
			const std::string& startDirectory=std::string(), bool selectFolderOnly=false) = 0;

		// As above but specifically to allow multiple file selection
		virtual bool FileDialogMultipleSelect(const std::vector<HAPI_TFileDialogFilters>& filters,
			std::vector<std::string> &filenames, HAPI_UserResponse &response,
			const std::string& startDirectory = std::string()) = 0;

		// Allows the icon shown in the window and task bar to be changed from the default HAPI logo
		virtual bool SetIcon(BYTE *rawImageData, int width, int height) const = 0;

		// Retrieve the directory the exe is running from
		virtual std::string GetExecutableDirectory() const = 0;

		// Retrieve the working directory - useful as a root for then finding data files
		// This is normally where the visual studio project file is, if running under viz,
		// otherwise it is where the executable directory is
		virtual std::string GetWorkingDirectory() const = 0;

		// Saves log to disk early. It always gets saved on exit but there are cases where you want to dump it sooner.
		virtual void WriteLogSoFar() const = 0;

		// Returns entered character as text (takes into account keyboard localisation and modifier keys etc).
		virtual char GetEnteredText() const = 0;

		// Useful if you are getting entered text from the user using GetEnteredText
		virtual void EnableKeyRepeat(bool enable) = 0;

		// Version as a string
		virtual std::string GetVersion() const = 0;

		// Deprecated: Use cout or cerr instead
		[[deprecated]]
		void DebugText(const std::string &str) {}
	};
}

// Retrieve the one HAPI instance
HAPISPACE::IHapi& GetHAPI();

// the HAPI instance. A shortcut to the above
#define HAPI GetHAPI()

