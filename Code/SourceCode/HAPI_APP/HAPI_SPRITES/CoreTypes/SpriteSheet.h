#pragma once

#include "Frame.h"

namespace HAPISPACE {

	// A spritesheet has a surface and a number of frame descriptions
	class SpriteSheet
	{
	private:
		// RULE: every frame must have a name and every frame name must be unique within the spritesheet
		std::vector<Frame> m_frames;
		std::shared_ptr<Surface> m_surface;

		// Key is an x,y position within the surface, value is the normal
		std::map<VectorI, VectorF> m_normals;

		// Used for PP collisions and also normal generation
		BYTE m_minAlphaForCollision{ 255 };

		// For editor internal access
		friend class SpriteEditor;
		friend class SpriteEditorMainWindow;
		friend class SpriteEditorPayloadData;
		friend class SpriteEditorCopyWindow;
		friend class SpriteEditorToolboxWindow;

		// Potentially slow. Returns null if none
		Frame* GetFrameAtPoint(VectorI pnt) ;

		// Direct write access to frame data for editor use
		// NOTE: if you add a frame or change position of one you must call m_shared->spriteSheet->SortFrames();
		std::vector<Frame>& EditorGetFrames() { return m_frames; }
		Frame* GetFrameByName(const std::string& name);

		// Check if pixel in frame has alpha >= minAlpha (for collisions)
		bool CheckPixel(const Frame& frame, VectorI pnt, BYTE minAlpha);

		// Fits a tight circle around data in the frame 
		void FitCircle(Circle &circle, Frame& frame, BYTE minAlpha);

		// Fits a tight rectangle around data in the frame 
		void FitRect(RectangleI &rect, BYTE minAlpha);

		// When using frame sets the frame index returned here != frame number
		int GetFrameNumberInSet(const std::string& frameSetName, int frameSetNumber) const;

		// Load
		void ParseXML(const std::string &xmlFilename);

		// Editor needs frames to always be sorted
		void SortFrames();

		std::vector<VectorI> GetNeighbours(VectorI pnt, int w, int h);

		mutable std::string m_filename{ "noname.xml" };
	public:
		// Allows creation from an equally spaced set of frames vertically or horizontally
		SpriteSheet(std::shared_ptr<Surface> surface, unsigned int numFrames = 1, bool layoutHorizontal = true, const std::string& frameSetName = std::string());

		// Allows creation from a provided vector of Frame representing the frames within the surface
		SpriteSheet(std::shared_ptr<Surface> surface, std::vector<Frame> frames);

		// Allows creation from an existing Surface and an XML file that describes the layout
		SpriteSheet(std::shared_ptr<Surface> surface, const std::string& xmlFilename);

		// Allows creation of a new surface and Sprite from an XML file that describes the layout
		SpriteSheet(const std::string& xmlFilename, const std::string& textureRootPath = std::string());

		// Copy constructor, allocates memory and copies raw data - deep copy
		SpriteSheet(const SpriteSheet &other);

		// Copy constructor, allocates memory and copies raw data - deep copy
		SpriteSheet(const std::shared_ptr<SpriteSheet> &other);

		~SpriteSheet() = default;

		// Retrieves original filename (if there is one) otherwise noname.xml
		const std::string& GetFilename() const { return m_filename; }

		// Checks integrity including checking rules
		// - every frame has a name
		// - every frame name is unique
		// - any frame sets have sequential numbering
		bool Validate() const;

		// min alpha defaults to 255 meaning collisions occur only when alpha is 255
		// This function allows you to lower that amount
		// NOTE: any surface normal calculations will need recalculating (not done for you)
		void SetMinimumAlphaForCollision(BYTE minAlpha) { m_minAlphaForCollision = minAlpha; }
		BYTE GetMinimumAlphaForCollision() const { return m_minAlphaForCollision; }

		// Note: uses current value of m_minAlphaForCollision
		void GenerateNormals();

		// If generated otherwise returns Zero vector
		VectorF GetNormal(VectorI surfacePosition) const;

		// Gets access to the surface the spritesheet uses
		std::shared_ptr<Surface> GetSurface() const { return m_surface; }

		// Returns the number of frames. Optionally can limit to the number in a frame set.
		int GetNumFrames(const std::string &setName = std::string()) const;

		// Does frame with this name exist?
		bool GetFrameExists(const std::string& frameName) const;

		// Does frame set with this name exist?
		bool GetFrameSetExists(const std::string& frameSetName) const;

		// Returns a vector of all frameset names
		std::vector<std::string> GetFrameSetNames() const;

		// Returns a unique frame name
		std::string GetUniqueName(const std::string root) const;

		// Get the rectangle for a frame and hence size
		RectangleI GetFrameRect(unsigned int frameNum, const std::string& frameSetName = std::string()) const;

		// Collider group for this frame
		const ColliderGroup& GetFrameCollider(unsigned int frameNum, const std::string& frameSetName = std::string()) const;

		// Read access to a frame
		const Frame& GetFrame(unsigned int frameNum, const std::string& frameSetName = std::string()) const;

		// Return the read only information for all frames	
		const std::vector<Frame>& GetAllFrames() const { return m_frames; }

		// Completely replace the frame information
		// Note: if sort set to true frames are sorted from top left to bottom right in row order
		void ModifyFrames(std::vector<Frame> newFrames, bool sort = false);

		// Deletes the first frame with this name (and collider data). Warning: this is slow
		void DeleteFrame(const std::string frameName);

		// Removes all frames and collider data
		void DeleteAllFrames();

		// Automatically finds frames in the image by finding solid blocks of colour (with alpha > minAlpha)
		// How well it works will depend if there are gaps between frames in the image
		// Warning: replaces all existing frame data if removeExistingFrames true
		void AutoFindFrames(bool removeExistingFrames = false, BYTE minAlpha = 1 );

		// Searches out from pos to find extents of a frame
		// Returns name of new frame or empty string if none
		// TODO: add to reference
		std::string AutoFindFrame(VectorI searchPoint,BYTE minAlpha = 1);

		// Collider names are not unique per framesheet so you need to specify the frame name as well (which is unique)
		bool DeleteCollider(const std::string& frameName, const std::string& colliderName);

		// Check if sprite is valid / loaded correctly
		bool HasData() const;

		// Saves an XML file describing the layout of the surface
		// Optionally saves the surface as well (in the same directory)
		bool SaveAsXML(const std::string& filename, bool saveSurface = false) const;

		// Creates a new spritesheet with a new surface which is made up solely of areas in the frame collection
		// Useful for trimming away dead areas or for making more cache friendly 
		// if alongX is true then frames are added left to right
		// if alongX is false then frames are added top to bottom (the default as this is best cache friendly method)
		// Can also limit to a frameset (or provide an empty string for all)
		// Note: does memory allocation so potentially slow
		std::shared_ptr<SpriteSheet> CreateCompacted(bool alongX = false, const std::string& frameSet = std::string()) const;

		// Joins two spritesheets together to create a third
		// if alongX is true then other sprite is added to the right of this
		// if alongX is false then other sprite is added below this
		// Note: does memory allocation so potentially slow
		std::shared_ptr<SpriteSheet> CreateCombined(const std::shared_ptr<SpriteSheet>& other, EDirection dir = EDirection::eEast) const;

		// Flips image data for each frame and creates a new frame with extension _flipHoriz
		// You can specify which frame set to process (empty string for all) and a frame set name for the new frames
		// Currently only supports horizontal flip (TODO)
		// Horizontal of true means frames laid out along x otherwise down y
		// Useful if you have a character graphic facing one way and need it to face another way as well
		// Not for realtime use, does lots of memory allocation and copying
		// Creates and returns a new spritesheet, does not modify existing
		std::shared_ptr<SpriteSheet> CreateFlippedFrames(const std::string& whichFrameSet = std::string(), const std::string& newFrameSetName = std::string(), bool horizontal = false) const;

		// Creates a new surface repeating the image num times, each time scaleX, scaleY different in size
		// Then creates a new sprite with frame data set to each of the repeats
		// if alongX is true then scaled surfaces are added horizontally
		// if alongX is false then scaled surfaces are added vertically
		// Be careful! The resulting surface could become enormous if your scale factors are too high
		std::shared_ptr<SpriteSheet> CreateSizes(float scaleX, float scaleY, int num, bool alongX = true, EFilter filter = EFilter::eBilinear) const;

		// Creates a new surface for each frame of this spritesheet
		// They are saved under a filename generated as: [surface name]_[Frame name].png
		// Returns a vector with all the new filenames
		// You can specify destination directory and file type (via the extension)
		std::vector<std::string> SaveFramesAsImages(const std::string &directory, std::string extension = ".png") const;

		// Reduce collider sizes to tightest fit
		// Note: uses current value of m_minAlphaForCollision
		void AutoFitColliders();

		// Sets colliders back to defaults (a bounding rectangle and a circle around it and loose fit)
		void ResetColliders();
	};
}