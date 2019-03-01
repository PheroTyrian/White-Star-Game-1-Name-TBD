////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	HAPISprites_Types.h
//
// summary:	Types used by HAPI Sprites
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

// Core HAPI library types
#include "HAPI\HAPI_Types.h"
#include "HAPI\HAPI_InputCodes.h"

// HAPI Sprites specific types
#include "Shapes\Shapes.h"
#include "CoreTypes\Colour.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace: HAPI_UI_SPACE
///
/// summary: The UI is wrapped inside the HAPI_UI_SPACE namespace.
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAPI_UI_SPACE
{
	class UISystem;
}

namespace HAPISPACE {

	/// <summary>	Float PI. </summary>
	constexpr float M_PI{ 3.1415927f };
	/// <summary>	Float PI * 2. </summary>
	constexpr float M_PI2{ 2.0f * M_PI };

	/// <summary>	Double PI. </summary>
	constexpr double M_PID{ 3.141592653589793 };
	/// <summary>	Double PI * 2. </summary>
	constexpr double M_PI2D{ 2.0 * M_PID };

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Convert degrees to radians. </summary>
	///
	/// <param name="x">	Value in degrees. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	#define DEGREES_TO_RADIANS(x) (x * M_PI/180.0f)

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Convert radians to degrees. </summary>
	///
	/// <param name="x">	Value in radians. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	#define RADIANS_TO_DEGREES(x) (x * 180.0f/M_PI)

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Round float to nearest int. </summary>
	///
	/// <param name="f">	A float. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	#define ROUND_FLOAT_TO_INT(f) ((int)(f >= 0.0f ? (f + 0.5f) : (f - 0.5f)))

	/// <summary>	Aliases to types from the underlying HAPI library. </summary>
	using EMessageButtonType = HAPI_ButtonType;
	/// <summary>	Information describing the mouse state. </summary>
	using MouseData = HAPI_TMouseData;
	/// <summary>	Information describing the keyboard. </summary>
	using KeyboardData = HAPI_TKeyboardData;
	/// <summary>	Information describing the controller state. </summary>
	using ControllerData = HAPI_TControllerData;
	/// <summary>	Options for controlling the sound. </summary>
	using SoundOptions = HAPI_TSoundOptions;
	/// <summary>	The sound format. </summary>
	using SoundFormat = HAPI_TSoundFormat;
	/// <summary>	The media status. </summary>
	using EMediaStatus = HAPI_MediaStatus;
	/// <summary>	Information describing the streamed media. </summary>
	using StreamedMediaInfo = HAPI_TStreamedMediaInfo;
	/// <summary>	The error codes. </summary>
	using EErrorCodes = HAPI_ErrorCodes;
	/// <summary>	The file dialog filters. </summary>
	using FileDialogFilters = HAPI_TFileDialogFilters;
	
	/// <summary>	User response from dialogs. </summary>
	enum class EUserResponse
	{
		eAffirmative,	// e.g. OK, YES
		eNegative,		// e.g. NO
		eCancel
	};

	/// <summary>	Initialisation flags. </summary>
	enum InitialisationFlags
	{
		eHSDefaults = 0,
		eHSFullscreen = 1 << 0,
		eHSSoftware = 1 << 1,		// SW rendering only - no HW supprot
		eHSEnableUI = 1 << 2,		// Enables the UI system
		eHSReserved2 = 1 << 3
	};

	/// <summary>	Each of the internal apps. </summary>
	enum class EInternalApp
	{
		eSpriteEditor,
		eSkinEditor,
		eUiEditor
	//	eMapEditor
	};
	
	/// <summary>	Used in many places to define relative directions. </summary>
	enum class EDirection
	{
		eUnmapped,
		eNorth,
		eNorthEast,
		eEast,
		eSouthEast,
		eSouth,
		eSouthWest,
		eWest,
		eNorthWest
	};

	/// <summary>	Describes a key event. </summary>
	enum class EKeyEvent
	{
		ePress,
		eRelease
	};

	/// <summary>	Describes a mouse event. </summary>
	enum class EMouseEvent
	{
		eLeftButtonDown,
		eLeftButtonUp,
		eMiddleButtonDown,
		eMiddleButtonUp,
		eRightButtonDown,
		eRightButtonUp,
		eWheelForward,
		eWheelBack,
		eXButton
	};

	/// <summary>	Describes a controller event. </summary>
	enum class EControllerEvent
	{
		eAttached,
		eUnattached,
		eDigitalPress,
		eDigitalRelease,
		eAnalogue
	};

	/// <summary>	Interface for input callbacks for when not using the UI. </summary>
	class IHapiSpritesInputListener
	{
	public:
		IHapiSpritesInputListener();
		virtual ~IHapiSpritesInputListener();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Called when HAPI Sprites detects a key event (repeatadly for key down, once for key up)
		/// </summary>
		///
		/// <param name="keyEvent">	The key event. </param>
		/// <param name="keyCode"> 	The key code. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI detects a mouse event other than mouse move. </summary>
		///
		/// <param name="mouseEvent">	The mouse event. </param>
		/// <param name="mouseData"> 	Information describing the mouse state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void OnMouseEvent(EMouseEvent mouseEvent, const MouseData& mouseData) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Non virtual as may not want to implement - gets a lot of calls. </summary>
		///
		/// <param name="mouseData">	Information describing the mouse state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void OnMouseMove(const MouseData& mouseData) {};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI Sprites detects a controller event - optional. </summary>
		///
		/// <param name="controllerId">   	Identifier for the controller. </param>
		/// <param name="controllerEvent">	The controller event. </param>
		/// <param name="buttonIndex">	  	Zero-based index of the button. </param>
		/// <param name="controllerData"> 	Information describing the controller state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void OnControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called with interpreted key presses into character entry. </summary>
		///
		/// <param name="enteredText">	The entered text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void OnTextEntered(char enteredText) {}
	};

	/// <summary>	Used for gradient shader. </summary>
	enum class EGradientShape
	{
		eLinear,
		eRadial,
		eRadialCone,
		eSquare,
		eRectangleL1,
		eRectangleL1inf,
		eGaussian,
		eNumGradientShapes
	};

	/// <summary>	Used for gradient shader. </summary>
	enum class EGradientProfile
	{
		eLinear,
		eWaves,
		eRingBar,
		eExponential,
		eVanishingWave,
		eVanishingingRingBar,
		eCircDiff, // circ diffraction (Airy) profile
		eRectDiff // rect diffraction (sinc2) profile
	};

	/// <summary>	Returned when calling SplitFilename. </summary>
	struct FilenameParts
	{
		/// <summary>	Full pathname of the file. </summary>
		std::string path;
		/// <summary>	The root. </summary>
		std::string root;
		/// <summary>	Pathname of the root directory. </summary>
		std::string rootDirectory;
		/// <summary>	Full pathname of the relative file. </summary>
		std::string relativePath;
		/// <summary>	Just the filename. </summary>
		std::string filename;
		/// <summary>	The stem. </summary>
		std::string stem;
		/// <summary>	The filename extension. </summary>
		std::string extension;
	};	
	
	/// <summary>	A surface can be one of these types. </summary>
	enum class ESurfaceType
	{		
		eNormal,			// when using draw calls it is done in SW, but when blitted to a hardware surface uses HW		
		eHWRenderSurface,	// draw calls done in HW. Specifically for doing special effects		
		eHWScreen			// draw calls done in HW and is a direct representation of the screen, only one of these
	};

	/// <summary>	Blending modes for draw operations (more will be added) </summary>
	enum class EBlendMode
	{
		eReplace,			// result = source
		eAlphaBlend,		// result = alpha * source + (1 - alpha) * dest, this is normal alpha blending
		eAlphaDestBlend,	// As above but source alpha is first modulated by destination alpha, useful for masks etc.
		eDifference,		// This is source colour - destination colour e.g. source as white inverts dest
		eUseLambda			// Use provided lambda
	};

	/// <summary>	Filters. </summary>
	enum class EFilter
	{
		eNearest,
		eBilinear
		// eBicubic = TODO
	};

	/// <summary>	Dial / listbox layout flags. </summary>
	enum class ELayout
	{
		eOff,		// Not shown
		eInside,	// Shown on inside of dial border. Rotates with dial.
		eOutside	// Shown out outisde of dial border. Fixed.
	};

	/// <summary>	Dial / listbox points and labels definition. </summary>
	struct PointsLabelsAndMarkers
	{
		/// <summary>	Gives resolution of dial. </summary>
		int numPoints{ 360 };

		/// <summary>	If markersForPoints true, determines how many points per marker. </summary>
		int numPointsPerMarker{ 45 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Can be derived from the above. </summary>
		///
		/// <returns>	The number of markers. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumMarkers() const { return numPoints / numPointsPerMarker; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Labels can be placed on markers, the point value is which point to label The key is the index of
		/// the marker and must be &lt; GetNumMarkers()
		/// Does not need to be one per marker could have just start and end etc.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::map<int, std::string> valueLables;

		/// <summary>	Markers use this colour rather than the skin colour. </summary>
		Colour255 markerColour{ Colour255::WHITE };

		// If changed - change Dial XML write / read
	};

	/// <summary>	Dial layout. </summary>
	struct DialLayout
	{
		/// <summary>	Determines visibility of pointer. </summary>
		ELayout pointerLayout{ ELayout::eInside };
		/// <summary>	Determines if marks should be placed on each numPointsPerMarker points. </summary>
		ELayout markersLayout{ ELayout::eInside };
		/// <summary>	Determines if labels should be placed. </summary>
		ELayout labelsLayout{ ELayout::eOutside };		
		/// <summary>	Rotation start angle. 0 is bottom. In Radians. </summary>
		float startAngle{ 0 };
		/// <summary>	End angle (needs to be bigger then startAngle). </summary>
		float endAngle{ DEGREES_TO_RADIANS(360) };
		/// <summary>	Width in pixels of the pointer, needs to be even number. </summary>
		int pointerWidth{ 4 };		
		/// <summary>	The points and labels marks data. </summary>
		PointsLabelsAndMarkers pntsLabsMarks;
	};

	/// <summary>	Slider layout. </summary>
	struct SliderLayout
	{
		/// <summary>	Determines if marks should be placed on each numPointsPerMarker points. </summary>
		bool displayMarkers{ true };
		/// <summary>	Determines if labels should be placed. </summary>
		bool displayLabels{ true };		
		/// <summary>	Size of the thumb (ratio) relative to the track (&gt;0 and &lt;1) </summary>
		float thumbRelativeSize{0.1f };
		/// <summary>	Details points labels and markers settings. </summary>
		PointsLabelsAndMarkers pntsLabsMarks;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// If true the thumb range stops at the end of the slider, thumb size ratio can be altered, and
		/// it uses the scrollbar style instead of the slider style.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool asWindowScrollbar{ false };
	};

	/// <summary>	Fill shaders. </summary>
	enum class FillShaderTypes
	{
		eColour,
		ePattern,
		eGradient,
		eImage,
		eLambda
	};

	class Surface;
	class FillShaderPIMPL;

	/// <summary>	For all draw functions you provide a fill shader. </summary>
	class FillShader
	{
	private:
		friend class Surface;

		/// <summary>	Gets the pimpl.For optisation purposes (caching) so mutable is acceptable. </summary>
		mutable std::shared_ptr<FillShaderPIMPL> m_pimpl;
	public:
		FillShader() noexcept = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Must be able to retrieve a colour from any position within bounds. Position is guaranteed to
		/// be within bounds. Returned colour is blended already with destination.
		/// </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A colour that is modulated with the calculated colour before being returned from every
		/// subsequent GetColour call (or a GetFilledSurface call)
		/// Note: only the last pushed colour is used.
		/// </summary>
		///
		/// <param name="mod">	The modulating colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PushModulate(Colour255 mod);

		/// <summary>	Pops the modulating colour. </summary>
		void PopModulate();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Modulates by the last modulation colour. If none then just returns col. </summary>
		///
		/// <param name="col">	The col. </param>
		///
		/// <returns>	A Colour255. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 Modulate(Colour255 col) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Fills a surface and returns it. Note: this may be slow first time but caches the result so
		/// subsequent calls will be quick.
		/// </summary>
		///
		/// <param name="width"> 	The width. </param>
		/// <param name="height">	The height. </param>
		///
		/// <returns>	The filled surface. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> GetFilledSurface(int width, int height) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Does not save modulation colour. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool SaveXML(CHapiXMLNode *parent) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes a deep copy of this object. </summary>
		///
		/// <returns>	A copy of this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual std::shared_ptr<FillShader> Clone() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fill shader type. </summary>
		///
		/// <returns>	The fill shader type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual FillShaderTypes GetFillShaderType() const noexcept = 0;

		/// <summary>	For editor use when changed values directly. </summary>
		void ClearCache();
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A derivation helper. </summary>
	///
	/// <typeparam name="Derived">	Type of the derived. </typeparam>
	///
	/// <seealso cref="T:FillShader"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Derived>
	class DerivationHelper : public FillShader
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes a deep copy of this object. </summary>
		///
		/// <returns>	A copy of this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<FillShader> Clone() const override final
		{
			return std::make_shared<Derived>(static_cast<const Derived&>(*this)); // call the copy ctor.
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A colour fill shader. </summary>
	///
	/// <seealso cref="T:DerivationHelper{ColourFill}"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ColourFill : public DerivationHelper<ColourFill>
	{
	private:
		/// <summary>	The colour. </summary>
		Colour255 m_colour{ Colour255::HORRID_PINK };
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="col">	The col. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColourFill(Colour255 col) noexcept : m_colour(col) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="xml">		 	[in,out] The XML data. </param>
		/// <param name="shaderNode">	[in,out] If non-null, the shader node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColourFill(CHapiXML &xml, CHapiXMLNode *shaderNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a colour. </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the colour. </summary>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour() const { return m_colour; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change colour. </summary>
		///
		/// <param name="newCol">	The new colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeColour(Colour255 newCol) { m_colour = newCol; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an XML. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fill shader type. </summary>
		///
		/// <returns>	The fill shader type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eColour; }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A lambda function fill shader. </summary>
	///
	/// <seealso cref="T:DerivationHelper{LambdaFill}"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class LambdaFill : public DerivationHelper<LambdaFill>
	{
	private:
		/// <summary>	The lambda. </summary>
		std::function<Colour255(const VectorF &pos, const RectangleI &bounds, Colour255 destColour)> &m_lambda;
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="lambda">	The lambda. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		LambdaFill(std::function<Colour255(const VectorF &pos, const RectangleI &bounds, Colour255 destColour)> &lambda) : m_lambda(lambda) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a colour. </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final 
		{ 
			Colour255 col=(m_lambda)(position,bounds,destColour); 
			if (!writeAlpha)
				return Colour255(col.red, col.green, col.blue, destColour.alpha);
			return col;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an XML. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fill shader type. </summary>
		///
		/// <returns>	The fill shader type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eLambda; }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Represents a pattern for drawing. </summary>
	///
	/// <seealso cref="T:DerivationHelper{PatternFill}"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class PatternFill : public DerivationHelper<PatternFill>
	{
	private:
		/// <summary>	A surface. </summary>
		friend class Surface;
		/// <summary>	A skin editor pattern. </summary>
		friend class UISkinEditorPattern;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// The grid.The vector is a 1D representation of a 2D grid with each row being 'width' long
		/// (number of columns)
		/// Each entry in the grid is an index into the palette.If this index does not exist a horrid
		/// pink is used Repeats allows for scaling of the pattern.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<int> m_grid;
		/// <summary>	The palette. </summary>
		std::vector<Colour255> m_palette;
		/// <summary>	The repeats. </summary>
		VectorI m_repeats;
		/// <summary>	The width. </summary>
		int m_width{ 0 };
		/// <summary>	The height. </summary>
		int m_height{ 0 };
	public:
		/// <summary>	Default constructor. Creates a 4 by 4 striped pattern by default. </summary>
		PatternFill();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="grid">   	The grid. </param>
		/// <param name="width">  	The width. </param>
		/// <param name="height"> 	The height. </param>
		/// <param name="palette">	The palette. </param>
		/// <param name="repeats">	(Optional) The number of repeats. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PatternFill(std::vector<int> grid, int width, int height, std::vector<Colour255> palette, VectorI repeats = VectorI(1, 1)) :
			m_grid(grid), m_width(width), m_height(height), m_palette(palette), m_repeats(repeats) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="xml">		  	[in,out] The XML data. </param>
		/// <param name="patternNode">	[in,out] If non-null, the pattern node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PatternFill(CHapiXML &xml, CHapiXMLNode *patternNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves under "Pattern" node. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Position can be larger than grid - it just repeats pattern. </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fill shader type. </summary>
		///
		/// <returns>	The fill shader type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::ePattern; }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Represents a gradient for drawing. </summary>
	///
	/// <seealso cref="T:DerivationHelper{GradientFill}"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class GradientFill : public DerivationHelper<GradientFill>
	{
	private:
		friend class Surface;
		friend class UISkinEditorPalette;
		friend class UISkinEditorGradient;

		/// <summary>	Ratios across rect. </summary>
		VectorF m_p1;
		/// <summary>	Ratios across rect. </summary>
		VectorF m_p2;
		/// <summary>	The first colour. </summary>
		Colour255 m_colour1;
		/// <summary>	The second colour. </summary>
		Colour255 m_colour2;
		/// <summary>	The shape. </summary>
		EGradientShape m_shape;
		/// <summary>	The profile. </summary>
		EGradientProfile m_profile;
		/// <summary>	The alpha. </summary>
		float m_alpha = 1.0f;	
	public:
		/// <summary>	Default constructor. </summary>
		GradientFill();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Applies a gradient across the surface using p1 and p2 control points (must range 0 to 1 only)
		/// Can provide null colour for either but not both - null colour means gradient goes to
		/// transparency.
		/// </summary>
		///
		/// <param name="p1">	  	The first VectorF. </param>
		/// <param name="p2">	  	The second VectorF. </param>
		/// <param name="colour1">	The first colour. </param>
		/// <param name="colour2">	The second colour. </param>
		/// <param name="shape">  	The shape. </param>
		/// <param name="profile">	The profile. </param>
		/// <param name="alpha">  	(Optional) The alpha. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GradientFill(VectorF p1, VectorF p2, Colour255 colour1, Colour255 colour2, EGradientShape shape, EGradientProfile profile, float alpha = 1.0f) noexcept :
			m_p1(p1), m_p2(p2), m_colour1(colour1), m_colour2(colour2), m_shape(shape), m_profile(profile), m_alpha(alpha) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="xml">		   	[in,out] The XML. </param>
		/// <param name="gradientNode">	[in,out] If non-null, the gradient node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GradientFill(CHapiXML &xml, CHapiXMLNode *gradientNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves under "Gradient" node. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overrides required by FillShader. </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fill shader type. </summary>
		///
		/// <returns>	The fill shader type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eGradient; }
	};

	/// <summary>	For image fill shader. </summary>
	enum class EImageFillMode
	{
		eTile,		// Tiles into the area being drawn
		eScale,		// Stretches or shrinks over the area
		eAsIs		// Drawn at a 1:1 size, once
	};

	class ImageCache;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Draws a bitmap as fill. </summary>
	///
	/// <seealso cref="T:DerivationHelper{ImageFill}"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ImageFill : public DerivationHelper<ImageFill>
	{
	private:		
		/// <summary>	The surface. </summary>
		std::shared_ptr<Surface> m_surface;
		/// <summary>	The mode. </summary>
		EImageFillMode m_mode = EImageFillMode::eAsIs;
		/// <summary>	The area. </summary>
		RectangleI m_area;
		/// <summary>	True to use bilinear. </summary>
		bool m_useBilinear;
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. If area invalid whole surface used</summary>
		///
		/// <param name="surfaceFilename">	Filename of the surface file. </param>
		/// <param name="mode">			  	(Optional) The mode. </param>
		/// <param name="useBilinear">	  	(Optional) True to use bilinear. </param>
		/// <param name="area">			  	(Optional) The area. Note: area must be smaller than surface size!</param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ImageFill(const std::string &surfaceFilename, 
			EImageFillMode mode=EImageFillMode::eAsIs,bool useBilinear=true, RectangleI area = RectangleI());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="xml">		   	[in,out] The XML. </param>
		/// <param name="gradientNode">	[in,out] If non-null, the gradient node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ImageFill(CHapiXML &xml, CHapiXMLNode *gradientNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the mode. </summary>
		///
		/// <returns>	The mode. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EImageFillMode GetMode() const { return m_mode; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change mode. </summary>
		///
		/// <param name="newMode">	The new mode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeMode(EImageFillMode newMode) { m_mode = newMode; ClearCache();}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change image. </summary>
		///
		/// <param name="newImage">	The new image. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeImage(std::shared_ptr<Surface> newImage) { m_surface = newImage; ClearCache(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves under "Image" node. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// If bounds bigger than surface then what happens depends on mode: eAsIs - returns ZERO colour
		/// eScale - scales surface to bounds eTile - tiles across bounds.
		/// </summary>
		///
		/// <param name="position">  	The position. </param>
		/// <param name="bounds">	 	The bounds. </param>
		/// <param name="destColour">	The destination colour. </param>
		/// <param name="blendMode"> 	The blend mode. </param>
		/// <param name="writeAlpha">	True to write alpha. </param>
		///
		/// <returns>	The colour. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eImage; }
	};	
}

// HAPI Sprites specific types in other files
#include "CoreTypes\Surface.h"
#include "CoreTypes\ColliderGroup.h"
#include "CoreTypes\SpriteSheet.h"
#include "CoreTypes\Sprite.h"

