#pragma once
/*
	Surface type
	Holds the raw surface data and has functions used by Sprite along with some basic 2D drawing functions.
	Used to represent the screen via the special SCREEN_SURFACE macro as well as for textures
	Note: never changes width and height or numBytes.
*/

namespace HAPISPACE {

	// Allows stencilled drawing
	class Stencil;

	// Handles render target specific code for a HW surface
	class HWRenderTarget;

	class Surface final
	{
	private:
		// Dimensions in texels
		const int m_width;
		const int m_height;

		// Used to indicate to sprite when a HW update is required
		bool m_dirty{ true };

		// Problem is getting the top of the stack is very slow ,in debug at least, so:
		bool m_writeAlphaTop{ false };

		// Blending mode used for all draw operations apart from blit
		EBlendMode m_drawBlendModeTop{ EBlendMode::eAlphaBlend };

		// Type of surface
		ESurfaceType m_surfaceType{ ESurfaceType::eNormal };

		// Raw texel pointer to 32 bit RGBA - Guaranteed to be aligned to 128 bit				
		Colour255 *m_texels{ nullptr };

		// Provided or auto generated, used when saving
		std::string m_filename;

		// Only allocated if surface type is a render target
		std::shared_ptr<HWRenderTarget> m_hwRenderTarget{ nullptr };

		// If true, blend functions will write source alpha into dest
		// Done as a stack to handle nested calls
		std::stack<bool> m_writeAlphaStack;

		// Blend modes for draw operations (not blit)
		std::stack<EBlendMode> m_drawBlendModeStack;

		// A scratch pad surface of same size as this
		// Note: not created until first used
		std::shared_ptr<Surface> m_scratchPad;

		// Area to not draw to
		std::unique_ptr<Stencil> m_stencil;

		// Area to restrict drawing to
		RectangleI m_clipArea;

		// Used to indicate screen coord area that applies to fill
		// e.g. if using a texture this area is from 0,0 to 1,1 of the texture
		// but it is held in screen coordinates
		// When invalid it indicates no area is set. So first fn. to detect that needs to set it
		// Use the functions rather than access direct so errors can be detected
		RectangleF m_areaOfFill;

		// Used to handle nested clears
		int m_areaOfFillSetCounts{ 0 };

		// A number of internal classes that need access to implementation-only functions
		friend class CHapiSpritesCore;
		friend class HardwareSprite;
		friend class FillShader;
		friend class Sprite;
		friend class Stencil;

		// fill calls
		void SetAreaOfFill(const RectangleF &newArea);
		void ClearAreaOfFill();
		bool IsAreaOfFillSet() const { return m_areaOfFill.IsValid(); }
		const RectangleF& GetAreaOfFill() const { return m_areaOfFill; }

		// Direct access to raw data - prefer over m_texels
		Colour255* GetRawData() const { return m_texels; }

		// How many bytes used
		size_t GetNumBytes() const { return (size_t)m_width * m_height * 4; }

		// Used to indicate to sprite when a HW update is required
		bool IsDirty() const { return m_dirty; }
		void SetDirty(bool set);

		bool IsHardwareSurface() const;
		bool RoughlyEquals(Colour255 colour1, Colour255 colour2, int errorPercentage);
		void SetAsRenderTarget(ESurfaceType type);
		bool GetWriteAlpha() const { return m_writeAlphaTop; }
		EBlendMode GetBlendMode() const { return m_drawBlendModeTop; }
		bool IsStencilEnabled() const;

		// Blit onto this surface from other surface at top left = posX, posY and use 'area' of other. 
		// Note: optimised case for when there is no scaling or rotation, blending is determined by blendMode
		void BlitFast(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);
		void BlitRotatedNoScale(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);
		void BlitRotatedAndScaled(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);

		// Low level raw pixel data copy.No blending. No threading. No transform.
		// uses same area of each unless toPos provided
		// Maintains alpha from 'from'
		// Careful if calling using a shared pointer that a copy is not made (use *shared)
		static void RawCopy(const Surface& from, Surface& to, const RectangleI &area, const VectorI& toPos = VectorI());

		// Drawing helper functions
		// Internal version that does not check stencil, download etc.
		void SetPixelInt(const VectorI& pos, Colour255 colour);

		// Bezier implementation
		void DrawQuadBezierImp(int x0, int y0, int x1, int y1, int x2, int y2, const FillShader& fill, float width);
		void DrawQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, const FillShader& fill, float width);

		// Calculates (and optimises) the points of a super-ellipse
		std::vector<VectorF> CalculateSuperEllipsePoints(const RectangleF& rect, float curvature, float startAngle = 0, float endAngle = DEGREES_TO_RADIANS(360));
	public:
		// Construct from image file, most formats supported
		Surface(const std::string& filename);

		// Constructs a surface from scratch and fills with colour
		// If no filename provided then creates one automatically
		// Can also be used to create a HW render surface
		Surface(int width, int height, std::string filename = std::string(), Colour255 colour = Colour255::ZERO, ESurfaceType type = ESurfaceType::eNormal);

		// Construct from an area of another surface
		Surface(const Surface &other, const RectangleI& copyArea, ESurfaceType newSurfaceType = ESurfaceType::eNormal);

		// Copy constructor, allocates memory and copies raw data - deep copy
		Surface(const Surface &other, ESurfaceType newSurfaceType = ESurfaceType::eNormal);

		// Copy constructor from a shared pointer wrapped surface, allocates memory and copies raw data - deep copy
		Surface(const std::shared_ptr<Surface> &other, ESurfaceType newSurfaceType = ESurfaceType::eNormal) : Surface(*other.get(), newSurfaceType) {}

		// Construct from an already allocated memory aligned raw pointer
		Surface(BYTE *alignedMemoryPointer, int width, int height);

		// Destructor
		~Surface();

		// Makes a deep copy of this surface and returns shared pointer to new surface
		std::shared_ptr<Surface> MakeCopy() const { return std::make_shared<Surface>(*this); }

		// Used to check surface has been created succesfully
		bool HasData() const { return (m_width > 0 && m_height > 0 && m_texels); }

		// Width of the surface in texels
		int Width() const { return m_width; }

		// Height of the surface in texels
		int Height() const { return m_height; }

		// Number of texels in the surface
		int GetNumPixels() const { return m_width * m_height; }

		// Returns a rectangle  set to the dimensions of the surface
		RectangleI GetDimensions() const { return RectangleI(Width(), Height()); }

		// Returns the surface type
		ESurfaceType GetType() const { return m_surfaceType; }

		// Blend operations will not write source alpha to dest unless this is set
		void PushWriteAlpha(bool set) { m_writeAlphaStack.push(set); m_writeAlphaTop = set; }
		void PopWriteAlpha();

		// Blend mode for subsequent draw functions (not blit - that is passed into the function)
		void PushBlendMode(EBlendMode mode) { m_drawBlendModeStack.push(mode); m_drawBlendModeTop = mode; }
		void PopBlendMode();

		// A clip area defines the area outside of which no drawing takes place
		// Providing a null rectangle sets the clip area to the size of the surface (the default)
		// Returns the old clip area which should normally be used rather than null rect
		RectangleI SetClipArea(const RectangleI& newClipArea);

		// A stencil area defines the area inside of which no drawing takes place
		// Currenty does not work for DrawText or when rendering to a HW surface
		void EnableStencilArea(RectangleI area);
		void DisableStencilArea();

		// Returns a scratch pad surface of same size as this
		// Note: not created until first requested, then persists
		std::shared_ptr<Surface> GetScratchPad();

		// If this surface is a render target (other than screen) you must call this to carry out drawing
		void ProcessHWDraws();

		// Makes on/off alpha by setting alpha to transparent wherever colour=colourKey otherwise opaque
		// If errorPercentage is 0 (the default) then only an exact match for colourKey will be used
		// If this is higher then rough matches are also included. Note a value of 100 means all colours 
		void MakeAlphaChannelFromColourKey(Colour255 colourKey, int errorPercentage = 0);

		// Allows a colour in the surface to be changed to another colour. 
		// If errorPercentage is 0 (the default) then only an exact match will be used
		// If this is higher then rough matches are also included. Note a value of 100 means all colours are changed.
		// Very quick for error=0, bit slower otherwise
		void Recolour(Colour255 oldColour, Colour255 newColour, int errorPercentage = 0);

		// Copies one channel to another
		// Channel numbers are 0=red, 1=green, 2=blue, 3=alpha
		void CopyChannel(int from, int to);

		// Blit onto this surface from other surface using transform (can be rotated and / or scaled)
		// If area invalid then whole 'other' surface used
		// Note: ignores surface wide blending mode and uses passed in one instead
		void Blit(const std::shared_ptr<Surface>& other, const Transform &trans = Transform(), RectangleI area = RectangleI(),
			const EBlendMode blendMode = EBlendMode::eAlphaBlend, const Colour255& mod = Colour255::ZERO,
			std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *blendLambda = nullptr);

		// Set a pixel to a colour, ignores if not on surface, blending based on colour alpha channel, checks write alpha
		// Potentially slow if you do a lot of them
		void SetPixel(const VectorI& pos, Colour255 colour);

		// Sets a pixel to colour. Does not check bounds, stencil or clip area. No blending. Alpha always written.
		// Use when you are sure of the above and want a faster call
		void SetPixelFast(const VectorI& pos, Colour255 colour);

		// Returns the colour of a pixel
		Colour255 GetPixel(const VectorI& pos) const;

		// Biliner filtering on non integer coordinate texel
		// Determine a colour for a texel, taking into account neighbours 
		// Specifically takes float vector for partial texel position
		// Currently just used for non-real-time scaling operations
		Colour255 BilinearInterpolate(const VectorF &pos) const;

		// Draws an arc from start on the edge of an arc with a defined centre and angle
		// The more numLines the better the quality but worse the performance
		void DrawArc(const VectorF &start, const VectorF &centre, float angle, int numLines, const FillShader& fill, float width);

		// Draws a single line
		void DrawLine(LineF line, const FillShader& fill, float width = 1.0f);

		// Draws a single line - alternative parameters
		void DrawLine(VectorF start, VectorF end, const FillShader& fill, float width = 1.0f) { DrawLine(LineF{ start,end }, fill, width); }

		// Draw multiple lines
		// widthOut of true means the line is the bottom part of the line with the line made wider 'above' it (uses winding)
		// false (normal) means the line width is expanded out from the centre
		void DrawLines(const std::vector<LineF>& lines, const FillShader& fill, float width = 1.0f, bool widthOut = false);

		// Same as DrawLines but using the Polygon type as parameter
		void DrawPolygon(const Polygon& poly, const FillShader& fill, float width = 1.0f, bool widthOut = false);

		// Fills any shape, as long as it is convex. Poly points must be sorted clockwise.
		// A border can also be drawn (if borderWidth>0) and a fill provided
		// Note: colour fill is very quick, other fills will be slow
		void DrawFilledConvexPolygon(const Polygon& poly, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		// Draws a Rectangle outline
		void DrawRect(const RectangleF &rect, const FillShader& fill, float borderWidth = 1.0f);

		// Draws an Oriented Rectangle outline, assumes points are in clockwise order
		// New name since this is a lot slower than above (in SW, OK in HW)
		void DrawOrientedRect(const RectangleOrientedF& rect, const FillShader& fill, float borderWidth = 1.0f);

		// Draws an Oriented Rectangle filled, assumes points are in clockwise order
		void DrawFilledOrientedRect(const RectangleOrientedF& rect, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		// Draws a filled rectangle
		void DrawFilledRect(const RectangleF& rect, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		// Draws a circle outline
		void DrawCircle(const Circle& circle, const FillShader& borderFill, float borderWidth = 1.0f);

		// Draws a filled circle
		void DrawFilledCircle(const Circle& circle, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		// Draws a Bezier curve
		void DrawQuadBezier(VectorF pnt1, VectorF pnt2, VectorF pnt3, const FillShader& fill, float width = 1.0f);



		// Draws a super ellipse outline (something between a rectangle and a circle)
		// Good for rounded edges on UI buttons etc. but it is quite slow for realtime usage
		// Curvature of 1 produces a pure circle while 0 produces a pure rectangle
		// Optionally you can limit the part of the shape drawn using the angles
		void DrawSuperEllipse(const RectangleF& rect, float curvature, const FillShader& fill, float width = 1.0f,
			float startAngle = 0, float endAngle = DEGREES_TO_RADIANS(360));

		// Draws a filled super ellipse (something between a rectangle and a circle)
		// Good for rounded buttons etc. but it is quite slow for realtime usage
		// Curvature of 1 produces a pure circle while 0 produces a pure rectangle
		void DrawFilledSuperEllipse(const RectangleF& rect, float curvature, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		// Draws text to this surface using the current font (changed via HAPI_Sprites.ChangeFont..)
		// Note: unlike HAPI_Sprites.RenderText this uses a SW renderer to draw the text onto the surface
		// if it is not a HW surface
		void DrawText(VectorI pos, Colour255 colour, const std::string& text, int textSize = 12,
			unsigned int styleFlags = eRegular, Colour255 outlineColour = Colour255(), float outlineThickness = 0.0f);

		// Applies a gaussian blur to this surface with provided radius
		// Slow i.e. not foe use as a real time operation
		//To limit effect use cliparea
		void GaussianBlur(double sigma);

		// Black and greyscale clear - very quick
		// Note: clears alpha to grey as well
		void Clear(BYTE grey = 0);

		// Colour clear
		void Clear(const Colour255& colour);

		// Flips the image horizontally. Width and Height stay the same.
		void FlipHorizontal();

		// Joins two surfaces together to create a third. Note: does dynamic memory allocation.
		// Dir decides which side B is attached to A
		static std::shared_ptr<Surface> Combine(const std::shared_ptr<Surface>& surfaceA,
			const std::shared_ptr<Surface>& surfaceB, EDirection dir = EDirection::eEast);

		// Creates a new surface which is a scaled version of this. Note: does dynamic memory allocation.
		// The filter to use can also be provided, one of the enum EFilter
		// Alignment is useful if you want to keep the width and height divisible by a number e.g. for frames
		std::shared_ptr<Surface> CreateScaled(float scaleX, float scaleY, EFilter filter = EFilter::eNearest, int alignment = 1) const;

		// Creates a new surface which is a scaled version of this by fitting it into the provided height and width (resize)
		// The filter to use can also be provided, one of the enum EFilter
		std::shared_ptr<Surface> CreateScaled(int width, int height, EFilter filter = EFilter::eNearest) const;

		// Returns the filename used when creating the surface (or auto generated one)
		std::string GetFilename() const { return m_filename; }

		// Allows the filename to be changed
		// Note: this should exclude the path
		void ChangeFilename(std::string newFilename) { m_filename = newFilename; }

		// Saves the surface using the currently set filename
		// Supported formats are bmp, png, tga and jpg - format is derived from filename extension
		bool SaveImageFile() const;

		// Saves a copy of the surface with a new filename (does not change filename)
		// Supported formats are bmp, png, tga and jpg - format is derived from filename extension
		// Optionally can specify just an area to save
		bool SaveImageFile(const std::string& filename, const RectangleI& area = RectangleI()) const;
	};
}