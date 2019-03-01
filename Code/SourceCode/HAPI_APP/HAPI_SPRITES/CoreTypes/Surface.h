////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Surface.h
//
// summary:	Declares the surface class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPISPACE {

	/// <summary>	Allows stencilled drawing. </summary>
	class Stencil;

	/// <summary>	Handles render target specific code for a HW surface. </summary>
	class HWRenderTarget;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Holds the raw surface data and has functions used by Sprite along with some basic 2D drawing
	/// functions. Used to represent the screen via the special SCREEN_SURFACE macro as well as for
	/// textures Note : never changes width and height or numBytes.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Surface final
	{
	private:
		const int m_width;
		const int m_height;
		int m_areaOfFillSetCounts{ 0 };
		bool m_dirty{ true };
		bool m_writeAlphaTop{ false };
		EBlendMode m_drawBlendModeTop{ EBlendMode::eAlphaBlend };
		ESurfaceType m_surfaceType{ ESurfaceType::eNormal };
		Colour255 *m_texels{ nullptr };
		std::string m_filename;
		std::shared_ptr<HWRenderTarget> m_hwRenderTarget{ nullptr };
		std::shared_ptr<Surface> m_scratchPad;
		std::unique_ptr<Stencil> m_stencil;
		std::stack<bool> m_writeAlphaStack;
		std::stack<EBlendMode> m_drawBlendModeStack;		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Used to indicate screen coord area that applies to fill
		/// e.g. if using a texture this area is from 0,0 to 1,1 of the texture but it is held in screen
		/// coordinates When invalid it indicates no area is set. So first fn. to detect that needs to
		/// set it Use the functions rather than access direct so errors can be detected.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleF m_areaOfFill;
		RectangleI m_clipArea;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A number of internal classes that need access to implementation-only functions.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		friend class CHapiSpritesCore;
		/// <summary>	A hardware sprite. </summary>
		friend class HardwareSprite;
		/// <summary>	A fill shader. </summary>
		friend class FillShader;
		/// <summary>	A sprite. </summary>
		friend class Sprite;
		/// <summary>	A stencil. </summary>
		friend class Stencil;

		void SetAreaOfFill(const RectangleF &newArea);
		void ClearAreaOfFill();
		bool IsAreaOfFillSet() const { return m_areaOfFill.IsValid(); }
		const RectangleF& GetAreaOfFill() const { return m_areaOfFill; }

		/// <summary>	Direct access to raw data - prefer over m_texels. </summary>
		Colour255* GetRawData() const { return m_texels; }

		size_t GetNumBytes() const { return (size_t)m_width * m_height * 4; }

		/// <summary>	Used to indicate to sprite when a HW update is required. </summary>
		bool IsDirty() const { return m_dirty; }
		void SetDirty(bool set) { m_dirty = set; }
		bool IsHardwareSurface() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Roughly equals. </summary>
		///
		/// <param name="colour1">		  	The first colour. </param>
		/// <param name="colour2">		  	The second colour. </param>
		/// <param name="errorPercentage">	The error percentage. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool RoughlyEquals(Colour255 colour1, Colour255 colour2, int errorPercentage);
		void SetAsRenderTarget(ESurfaceType type);
		bool GetWriteAlpha() const { return m_writeAlphaTop; }
		EBlendMode GetBlendMode() const { return m_drawBlendModeTop; }
		bool IsStencilEnabled() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Blit onto this surface from other surface at top left = posX, posY and use 'area' of other.
		/// Note: optimised case for when there is no scaling or rotation, blending is determined by
		/// blendMode.
		/// </summary>
		///
		/// <param name="other">	The other. </param>
		/// <param name="trans">	The transform. </param>
		/// <param name="area"> 	The area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void BlitFast(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Blit rotated no scale. </summary>
		///
		/// <param name="other">	The other. </param>
		/// <param name="trans">	The transform. </param>
		/// <param name="area"> 	The area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void BlitRotatedNoScale(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Blit rotated and scaled. </summary>
		///
		/// <param name="other">	The other. </param>
		/// <param name="trans">	The transform. </param>
		/// <param name="area"> 	The area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void BlitRotatedAndScaled(const std::shared_ptr<Surface> &other, const Transform &trans, const RectangleI& area);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Low level raw pixel data copy.No blending. No threading. No transform. uses same area of each
		/// unless toPos provided Maintains alpha from 'from' Careful if calling using a shared pointer
		/// that a copy is not made (use *shared)
		/// </summary>
		///
		/// <param name="from"> 	Source for the. </param>
		/// <param name="to">   	[in,out] to. </param>
		/// <param name="area"> 	The area. </param>
		/// <param name="toPos">	(Optional) To position. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void RawCopy(const Surface& from, Surface& to, const RectangleI &area, const VectorI& toPos = VectorI());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Drawing helper functions Internal version that does not check stencil, download etc.
		/// </summary>
		///
		/// <param name="pos">   	The position. </param>
		/// <param name="colour">	The colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetPixelInt(const VectorI& pos, Colour255 colour);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bezier implementation. </summary>
		///
		/// <param name="x0">   	The x coordinate 0. </param>
		/// <param name="y0">   	The y coordinate 0. </param>
		/// <param name="x1">   	The first x value. </param>
		/// <param name="y1">   	The first y value. </param>
		/// <param name="x2">   	The second x value. </param>
		/// <param name="y2">   	The second y value. </param>
		/// <param name="fill"> 	The fill. </param>
		/// <param name="width">	The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawQuadBezierImp(int x0, int y0, int x1, int y1, int x2, int y2, const FillShader& fill, float width);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw quad bezier segment. </summary>
		///
		/// <param name="x0">   	The x coordinate 0. </param>
		/// <param name="y0">   	The y coordinate 0. </param>
		/// <param name="x1">   	The first x value. </param>
		/// <param name="y1">   	The first y value. </param>
		/// <param name="x2">   	The second x value. </param>
		/// <param name="y2">   	The second y value. </param>
		/// <param name="fill"> 	The fill. </param>
		/// <param name="width">	The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, const FillShader& fill, float width);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates (and optimises) the points of a super-ellipse. </summary>
		///
		/// <param name="rect">		 	The rectangle. </param>
		/// <param name="curvature"> 	The curvature. </param>
		/// <param name="startAngle">	(Optional) The start angle. </param>
		/// <param name="endAngle">  	(Optional) The end angle. </param>
		///
		/// <returns>	The calculated super ellipse points. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<VectorF> CalculateSuperEllipsePoints(const RectangleF& rect, float curvature, float startAngle = 0, float endAngle = DEGREES_TO_RADIANS(360));
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. Construct from image file, most formats supported. </summary>
		///
		/// <param name="filename">	Filename of the file. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(const std::string& filename);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructs a surface from scratch and fills with colour. If no filename provided then creates
		/// one automatically Can also be used to create a HW render surface.
		/// </summary>
		///
		/// <param name="width">   	The width. </param>
		/// <param name="height">  	The height. </param>
		/// <param name="filename">	(Optional) Filename of the file. </param>
		/// <param name="colour">  	(Optional) The colour. </param>
		/// <param name="type">	   	(Optional) The type. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(int width, int height, std::string filename = std::string(), Colour255 colour = Colour255::ZERO, ESurfaceType type = ESurfaceType::eNormal);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from an area of another surface. </summary>
		///
		/// <param name="other">		 	The other surface. </param>
		/// <param name="copyArea">		 	The copy area. </param>
		/// <param name="newSurfaceType">	(Optional) Type of the new surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(const Surface &other, const RectangleI& copyArea, ESurfaceType newSurfaceType = ESurfaceType::eNormal);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor, allocates memory and copies raw data - deep copy. </summary>
		///
		/// <param name="other">		 	The other surface. </param>
		/// <param name="newSurfaceType">	(Optional) Type of the new surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(const Surface &other, ESurfaceType newSurfaceType = ESurfaceType::eNormal);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Copy constructor from a shared pointer wrapped surface, allocates memory and copies raw data -
		/// deep copy.
		/// </summary>
		///
		/// <param name="other">		 	The other surface. </param>
		/// <param name="newSurfaceType">	(Optional) Type of the new surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(const std::shared_ptr<Surface> &other, ESurfaceType newSurfaceType = ESurfaceType::eNormal) : Surface(*other.get(), newSurfaceType) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from an already allocated memory aligned raw pointer. </summary>
		///
		/// <param name="alignedMemoryPointer">	[in,out] If non-null, the aligned memory pointer. </param>
		/// <param name="width">			   	The width. </param>
		/// <param name="height">			   	The height. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Surface(BYTE *alignedMemoryPointer, int width, int height);

		/// <summary>	Destructor. </summary>
		~Surface();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Makes a deep copy of this surface and returns shared pointer to new surface New surface is a
		/// normal surface type.
		/// </summary>
		///
		/// <returns>	A std::shared_ptr&lt;Surface&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> MakeCopy() const { return std::make_shared<Surface>(*this); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Used to check surface has been created successfully. </summary>
		///
		/// <returns>	True if surface has data, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HasData() const { return (m_width > 0 && m_height > 0 && m_texels); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Width of the surface in texels. </summary>
		///
		/// <returns>	An int. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int Width() const noexcept { return m_width; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Height of the surface in texels. </summary>
		///
		/// <returns>	An int. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int Height() const noexcept { return m_height; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Number of texels in the surface. </summary>
		///
		/// <returns>	The number pixels. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumPixels() const noexcept { return m_width * m_height; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a rectangle  set to the dimensions of the surface. </summary>
		///
		/// <returns>	The dimensions. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetDimensions() const { return RectangleI(Width(), Height()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the surface type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESurfaceType GetType() const noexcept { return m_surfaceType; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Blend operations will not write source alpha to dest unless this is set. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PushWriteAlpha(bool set) { m_writeAlphaStack.push(set); m_writeAlphaTop = set; }
		/// <summary>	Pops the write alpha. </summary>
		void PopWriteAlpha();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Blend mode for subsequent draw functions (not blit - that is passed into the function)
		/// </summary>
		///
		/// <param name="mode">	The mode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PushBlendMode(EBlendMode mode) { m_drawBlendModeStack.push(mode); m_drawBlendModeTop = mode; }
		/// <summary>	Pops the blend mode. </summary>
		void PopBlendMode();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A clip area defines the area outside of which no drawing takes place. Providing a null
		/// rectangle sets the clip area to the size of the surface (the default).
		/// </summary>
		///
		/// <param name="newClipArea">	The new clip area. </param>
		///
		/// <returns>	Returns the old clip area which should normally be used rather than null rect. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI SetClipArea(const RectangleI& newClipArea);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A stencil area defines the area inside of which no drawing takes place. Currenty does not work
		/// for DrawText or when rendering to a HW surface.
		/// </summary>
		///
		/// <param name="area">	The area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void EnableStencilArea(RectangleI area);
		/// <summary>	Disables the stencil area. </summary>
		void DisableStencilArea();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Returns a scratch pad surface of same size as this Note: not created until first requested,
		/// then persists.
		/// </summary>
		///
		/// <returns>	The scratch pad. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> GetScratchPad();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// If this surface is a render target (other than screen) you must call this to carry out
		/// drawing.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ProcessHWDraws();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Makes on/off alpha by setting alpha to transparent wherever colour=colourKey otherwise opaque
		/// If errorPercentage is 0 (the default) then only an exact match for colourKey will be used. If
		/// this is higher then rough matches are also included. Note a value of 100 means all colours.
		/// </summary>
		///
		/// <param name="colourKey">	  	The colour key. </param>
		/// <param name="errorPercentage">	(Optional) The error percentage. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void MakeAlphaChannelFromColourKey(Colour255 colourKey, int errorPercentage = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows a colour in the surface to be changed to another colour. If errorPercentage is 0 (the
		/// default) then only an exact match will be used If this is higher then rough matches are also
		/// included. Note a value of 100 means all colours are changed. Very quick for error=0, bit
		/// slower otherwise.
		/// </summary>
		///
		/// <param name="oldColour">	  	The old colour. </param>
		/// <param name="newColour">	  	The new colour. </param>
		/// <param name="errorPercentage">	(Optional) The error percentage. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Recolour(Colour255 oldColour, Colour255 newColour, int errorPercentage = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Copies one channel to another Channel, numbers are 0=red, 1=green, 2=blue, 3=alpha.
		/// </summary>
		///
		/// <param name="from">	Source for the. </param>
		/// <param name="to">  	to. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CopyChannel(int from, int to);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Blit onto this surface from other surface using transform (can be rotated and / or scaled)
		/// If area invalid then whole 'other' surface used. Note: ignores surface wide blending mode and
		/// uses passed in one instead.
		/// </summary>
		///
		/// <param name="other">	  	The other. </param>
		/// <param name="trans">	  	(Optional) The transform. </param>
		/// <param name="area">		  	(Optional) The area. </param>
		/// <param name="blendMode">  	(Optional) The blend mode. </param>
		/// <param name="mod">		  	(Optional) The modifier. </param>
		/// <param name="blendLambda">	(Optional) The blend lambda. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Blit(const std::shared_ptr<Surface>& other, const Transform &trans = Transform(), RectangleI area = RectangleI(),
			const EBlendMode blendMode = EBlendMode::eAlphaBlend, const Colour255& mod = Colour255::ZERO,
			std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *blendLambda = nullptr);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Set a pixel to a colour, ignores if not on surface, blending based on colour alpha channel,
		/// checks write alpha. Potentially slow if you do a lot of them.
		/// </summary>
		///
		/// <param name="pos">   	The position. </param>
		/// <param name="colour">	The colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetPixel(const VectorI& pos, Colour255 colour);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Sets a pixel to colour. Does not check bounds, stencil or clip area. No blending. Alpha
		/// always written. Use when you are sure of the above and want a faster call.
		/// </summary>
		///
		/// <param name="pos">   	The position. </param>
		/// <param name="colour">	The colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetPixelFast(const VectorI& pos, Colour255 colour);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the colour of a pixel. </summary>
		///
		/// <param name="pos">	The position. </param>
		///
		/// <returns>	The pixel. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetPixel(const VectorI& pos) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Biliner filtering on non integer coordinate texel. Determine a colour for a texel, taking into
		/// account neighbours. Specifically takes float vector for partial texel position. Currently just
		/// used for non-real-time scaling operations.
		/// </summary>
		///
		/// <param name="pos">	The position. </param>
		///
		/// <returns>	A Colour255. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 BilinearInterpolate(const VectorF &pos) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draws an arc from start on the edge of an arc with a defined centre and angle. The more
		/// numLines the better the quality but worse the performance.
		/// </summary>
		///
		/// <param name="start">   	The start. </param>
		/// <param name="centre">  	The centre. </param>
		/// <param name="angle">   	The angle. </param>
		/// <param name="numLines">	Number of lines. </param>
		/// <param name="fill">	   	The fill. </param>
		/// <param name="width">   	The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawArc(const VectorF &start, const VectorF &centre, float angle, int numLines, const FillShader& fill, float width);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a single line. </summary>
		///
		/// <param name="line"> 	The line. </param>
		/// <param name="fill"> 	The fill. </param>
		/// <param name="width">	(Optional) The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawLine(LineF line, const FillShader& fill, float width = 1.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a single line - alternative parameters. </summary>
		///
		/// <param name="start">	The start. </param>
		/// <param name="end">  	The end. </param>
		/// <param name="fill"> 	The fill. </param>
		/// <param name="width">	(Optional) The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawLine(VectorF start, VectorF end, const FillShader& fill, float width = 1.0f) { DrawLine(LineF{ start,end }, fill, width); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draw multiple lines. widthOut of true means the line is the bottom part of the line with the
		/// line made wider 'above' it (uses winding)
		/// false (normal) means the line width is expanded out from the centre.
		/// </summary>
		///
		/// <param name="lines">   	The lines. </param>
		/// <param name="fill">	   	The fill. </param>
		/// <param name="width">   	(Optional) The width. </param>
		/// <param name="widthOut">	(Optional) True to width out. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawLines(const std::vector<LineF>& lines, const FillShader& fill, float width = 1.0f, bool widthOut = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Same as DrawLines but using the Polygon type as parameter. </summary>
		///
		/// <param name="poly">	   	The polygon. </param>
		/// <param name="fill">	   	The fill. </param>
		/// <param name="width">   	(Optional) The width. </param>
		/// <param name="widthOut">	(Optional) True to width out. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawPolygon(const Polygon& poly, const FillShader& fill, float width = 1.0f, bool widthOut = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Fills any shape, as long as it is convex. Poly points must be sorted clockwise. A border can
		/// also be drawn (if borderWidth>0) and a fill provided. Note: colour fill is very quick, other
		/// fills will be slower.
		/// </summary>
		///
		/// <param name="poly">		  	The polygon. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderFill"> 	(Optional) The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawFilledConvexPolygon(const Polygon& poly, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a Rectangle outline. </summary>
		///
		/// <param name="rect">		  	The rectangle. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawRect(const RectangleF &rect, const FillShader& fill, float borderWidth = 1.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draws an Oriented Rectangle outline, assumes points are in clockwise order. New name since
		/// this is a lot slower than above (in SW, OK in HW)
		/// </summary>
		///
		/// <param name="rect">		  	The rectangle. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawOrientedRect(const RectangleOrientedF& rect, const FillShader& fill, float borderWidth = 1.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a Filled Oriented Rectangle, assumes points are in clockwise order. </summary>
		///
		/// <param name="rect">		  	The rectangle. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderFill"> 	(Optional) The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawFilledOrientedRect(const RectangleOrientedF& rect, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a filled rectangle. </summary>
		///
		/// <param name="rect">		  	The rectangle. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderFill"> 	(Optional) The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawFilledRect(const RectangleF& rect, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a circle outline. </summary>
		///
		/// <param name="circle">	  	The circle. </param>
		/// <param name="borderFill"> 	The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawCircle(const Circle& circle, const FillShader& borderFill, float borderWidth = 1.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a filled circle. </summary>
		///
		/// <param name="circle">	  	The circle. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderFill"> 	(Optional) The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawFilledCircle(const Circle& circle, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draws a Bezier curve. </summary>
		///
		/// <param name="pnt1"> 	The first point. </param>
		/// <param name="pnt2"> 	The second point. </param>
		/// <param name="pnt3"> 	The third point. </param>
		/// <param name="fill"> 	The fill. </param>
		/// <param name="width">	(Optional) The width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawQuadBezier(VectorF pnt1, VectorF pnt2, VectorF pnt3, const FillShader& fill, float width = 1.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draws a super ellipse outline (something between a rectangle and a circle)
		/// Good for rounded edges on UI buttons etc. but it is quite slow for realtime usage. Curvature
		/// of 1 produces a pure circle while 0 produces a pure rectangle. Optionally you can limit the
		/// part of the shape drawn using the angles.
		/// </summary>
		///
		/// <param name="rect">		 	The rectangle. </param>
		/// <param name="curvature"> 	The curvature. </param>
		/// <param name="fill">		 	The fill. </param>
		/// <param name="width">	 	(Optional) The width. </param>
		/// <param name="startAngle">	(Optional) The start angle. </param>
		/// <param name="endAngle">  	(Optional) The end angle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawSuperEllipse(const RectangleF& rect, float curvature, const FillShader& fill, float width = 1.0f,
			float startAngle = 0, float endAngle = DEGREES_TO_RADIANS(360));

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draws a filled super ellipse (something between a rectangle and a circle)
		/// Good for rounded buttons etc. but it is quite slow for realtime usage Curvature of 1 produces
		/// a pure circle while 0 produces a pure rectangle.
		/// </summary>
		///
		/// <param name="rect">		  	The rectangle. </param>
		/// <param name="curvature">  	The curvature. </param>
		/// <param name="fill">		  	The fill. </param>
		/// <param name="borderFill"> 	(Optional) The border fill. </param>
		/// <param name="borderWidth">	(Optional) Width of the border. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawFilledSuperEllipse(const RectangleF& rect, float curvature, const FillShader& fill,
			const FillShader& borderFill = ColourFill(Colour255::HORRID_PINK), float borderWidth = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Draws text to this surface using the current font (changed via HAPI_Sprites.ChangeFont)
		/// Note: unlike HAPI_Sprites.RenderText this uses a SW renderer to draw the text onto the
		/// surface if it is not a HW surface.
		/// </summary>
		///
		/// <param name="pos">			   	The position. </param>
		/// <param name="colour">		   	The colour. </param>
		/// <param name="text">			   	The text. </param>
		/// <param name="textSize">		   	(Optional) Size of the text. </param>
		/// <param name="styleFlags">	   	(Optional) The style flags. </param>
		/// <param name="outlineColour">   	(Optional) The outline colour. </param>
		/// <param name="outlineThickness">	(Optional) The outline thickness. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawText(VectorI pos, Colour255 colour, const std::string& text, int textSize = 12,
			unsigned int styleFlags = eRegular, Colour255 outlineColour = Colour255(), float outlineThickness = 0.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Applies a gaussian blur to this surface with provided radius. Slow i.e. not for use as a real
		/// time operation. To limit effect use cliparea.
		/// </summary>
		///
		/// <param name="sigma">	The sigma. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void GaussianBlur(double sigma);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Black and greyscale clear - very quick .Note: clears alpha to grey as well, if you need alpha
		/// set differently use one below.
		/// </summary>
		///
		/// <param name="grey">	(Optional) The grey. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Clear(BYTE grey = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Colour clear. </summary>
		///
		/// <param name="colour">	The colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Clear(const Colour255& colour);

		/// <summary>	Flips the image horizontally. Width and Height stay the same. </summary>
		void FlipHorizontal();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Joins two surfaces together to create a third. Note: does dynamic memory allocation. Dir
		/// decides which side B is attached to A.
		/// </summary>
		///
		/// <param name="surfaceA">	The surface a. </param>
		/// <param name="surfaceB">	The surface b. </param>
		/// <param name="dir">	   	(Optional) The dir. </param>
		///
		/// <returns>	A std::shared_ptr&lt;Surface&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static std::shared_ptr<Surface> Combine(const std::shared_ptr<Surface>& surfaceA,
			const std::shared_ptr<Surface>& surfaceB, EDirection dir = EDirection::eEast);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Creates a new surface which is a scaled version of this. Note: does dynamic memory
		/// allocation. The filter to use can also be provided, one of the enum EFilter. Alignment is
		/// useful if you want to keep the width and height divisible by a number e.g. for frames.
		/// </summary>
		///
		/// <param name="scaleX">   	The scale x coordinate. </param>
		/// <param name="scaleY">   	The scale y coordinate. </param>
		/// <param name="filter">   	(Optional) Specifies the filter. </param>
		/// <param name="alignment">	(Optional) The alignment. </param>
		///
		/// <returns>	The new scaled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> CreateScaled(float scaleX, float scaleY, EFilter filter = EFilter::eNearest, int alignment = 1) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Creates a new surface which is a scaled version of this by fitting it into the provided
		/// height and width (resize)
		/// The filter to use can also be provided, one of the enum EFilter.
		/// </summary>
		///
		/// <param name="width"> 	The width. </param>
		/// <param name="height">	The height. </param>
		/// <param name="filter">	(Optional) Specifies the filter. </param>
		///
		/// <returns>	The new scaled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> CreateScaled(int width, int height, EFilter filter = EFilter::eNearest) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Returns the filename used when creating the surface (or auto generated one)
		/// </summary>
		///
		/// <returns>	The filename. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string GetFilename() const { return m_filename; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows the filename to be changed Note: this should exclude the path. </summary>
		///
		/// <param name="newFilename">	Filename of the new file. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeFilename(std::string newFilename) { m_filename = newFilename; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Saves the surface using the currently set filename. Supported formats are bmp, png, tga and
		/// jpg - format is derived from filename extension.
		/// </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveImageFile() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Saves a copy of the surface with a new filename (does not change filename)
		/// Supported formats are bmp, png, tga and jpg - format is derived from filename extension
		/// Optionally can specify just an area to save.
		/// </summary>
		///
		/// <param name="filename">	Filename of the file. </param>
		/// <param name="area">	   	(Optional) The area. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveImageFile(const std::string& filename, const RectangleI& area = RectangleI()) const;
	};
}