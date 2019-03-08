#pragma once

// Core HAPI library types
#include "HAPI\HAPI_Types.h"
#include "HAPI\HAPI_InputCodes.h"

// HAPI Sprites specific types
#include "Shapes\Shapes.h"
#include "CoreTypes\Colour.h"

namespace HAPISPACE {

	// Float PI
	constexpr float M_PI{ 3.1415927f };
	constexpr float M_PI2{ 2.0f * M_PI };

	// Double PI
	constexpr double M_PID{ 3.141592653589793 };
	constexpr double M_PI2D{ 2.0 * M_PID };

	// Useful macros
	#define DEGREES_TO_RADIANS(x) (x * M_PI/180.0f)
	#define RADIANS_TO_DEGREES(x) (x * 180.0f/M_PI)
	#define ROUND_FLOAT_TO_INT(f) ((int)(f >= 0.0f ? (f + 0.5f) : (f - 0.5f)))

	// Aliases to types from the underlying HAPI library
	using EMessageButtonType = HAPI_ButtonType;
	using MouseData = HAPI_TMouseData;
	using KeyboardData = HAPI_TKeyboardData;
	using ControllerData = HAPI_TControllerData;
	using SoundOptions = HAPI_TSoundOptions;
	using SoundFormat = HAPI_TSoundFormat;
	using EMediaStatus = HAPI_MediaStatus;
	using StreamedMediaInfo = HAPI_TStreamedMediaInfo;
	using EErrorCodes = HAPI_ErrorCodes;
	using FileDialogFilters = HAPI_TFileDialogFilters;
	
	// User response from dialogs
	enum class EUserResponse
	{
		eAffirmative,	// e.g. OK, YES
		eNegative,		// e.g. NO
		eCancel
	};

	// Initialisation flags
	enum InitialisationFlags
	{
		eHSDefaults = 0,
		eHSFullscreen = 1 << 0,
		eHSSoftware = 1 << 1,		// SW rendering only - no HW supprot
		eHSEnableUI = 1 << 2,		// Enables the UI system
		eHSReserved2 = 1 << 3
	};

	// For the internal apps
	enum class EInternalApp
	{
		eSpriteEditor,
		eSkinEditor,
		eUiEditor,
		eMapEditor
	};
	
	// Used in many places to define relative directions
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

	// Describes a key event
	enum class EKeyEvent
	{
		ePress,
		eRelease
	};

	// Describes a mouse event
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

	// Describes a controller event
	enum class EControllerEvent
	{
		eAttached,
		eUnattached,
		eDigitalPress,
		eDigitalRelease,
		eAnalogue
	};

	// Interface for input callbacks for when not using the UI
	class IHapiSpritesInputListener
	{
	public:
		IHapiSpritesInputListener();
		virtual ~IHapiSpritesInputListener();

		// Called when HAPI Sprites detects a key event (repeatadly for key down, once for key up)
		virtual void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) = 0;

		// Called when HAPI detects a mouse event other than mouse move
		virtual void OnMouseEvent(EMouseEvent mouseEvent, const MouseData& mouseData) = 0;

		// Non virtual as may not want to implement - gets a lot of calls 
		virtual void OnMouseMove(const MouseData& mouseData) {};

		// Called when HAPI Sprites detects a controller event - optional
		virtual void OnControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData) {}

		// Called with interpreted key presses into character entry
		virtual void OnTextEntered(char enteredText) {}
	};

	// Used for gradient shader
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

	// Used for gradient shader
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

	// Returned when calling SplitFilename
	struct FilenameParts
	{
		std::string path;
		std::string root;
		std::string rootDirectory;
		std::string relativePath;
		std::string filename;
		std::string stem;
		std::string extension;
	};	
	
	// A surface can be one of these types
	enum class ESurfaceType
	{
		// when using draw calls it is done in SW, but when blitted to a hardware surface uses HW
		eNormal,
		// draw calls done in HW. Specifically for doing special effects
		eHWRenderSurface,
		// draw calls done in HW and is a direct representation of the screen, only one of these
		eHWScreen
	};

	// Blending modes for draw operations (more will be added)
	enum class EBlendMode
	{
		eReplace,			// result = source
		eAlphaBlend,		// result = alpha * source + (1 - alpha) * dest, this is normal alpha blending
		eAlphaDestBlend,	// As above but source alpha is first modulated by destination alpha, useful for masks etc.
		eDifference,		// This is source colour - destination colour e.g. source as white inverts dest
		eUseLambda			// Use provided lambda
	};

	// Filters
	enum class EFilter
	{
		eNearest,
		eBilinear
		// eBicubic = TODO
	};

	// Dial / listbox layout flags
	enum class ELayout
	{
		eOff,		// Not shown
		eInside,	// Shown on inside of dial border. Rotates with dial.
		eOutside	// Shown out outisde of dial border. Fixed.
	};

	// Dial / listbox points and labels definition
	struct PointsLabelsAndMarkers
	{
		// Gives resolution of dial
		int numPoints{ 360 };
		// If markersForPoints true, determines how many points per marker
		int numPointsPerMarker{ 45 };
		// Can be derived from the above
		int GetNumMarkers() const { return numPoints / numPointsPerMarker; }
		// Labels can be placed on markers, the point value is which point to label
		// They is the index of the marker and must be < GetNumMarkers()
		// Does not need to be one per marker could have just start and end etc.
		std::map<int, std::string> valueLables;
		// Markers use this colour rather than the skin colour
		Colour255 markerColour{ Colour255::WHITE };
		// If changed - change Dial XML write / read
	};

	// Dial layout
	struct DialLayout
	{
		// Determines visibility of pointer
		ELayout pointerLayout{ ELayout::eInside };
		// Determines if marks should be placed on each numPointsPerMarker points
		ELayout markersLayout{ ELayout::eInside };
		// Determines if labels should be placed
		ELayout labelsLayout{ ELayout::eOutside };		
		// Rotation start angle. 0 is bottom. In Radians.
		float startAngle{ 0 };
		// End angle (needs to be bigger then startAngle).
		float endAngle{ DEGREES_TO_RADIANS(360) };
		// Width in pixels of the pointer, needs to be even number
		int pointerWidth{ 4 };		
		PointsLabelsAndMarkers pntsLabsMarks;
	};

	// Slider layout
	struct SliderLayout
	{
		// Determines if marks should be placed on each numPointsPerMarker points
		bool displayMarkers{ true };
		// Determines if labels should be placed
		bool displayLabels{ true };		
		// Size of the thumb (ratio) relative to the track (>0 and <1)
		float thumbRelativeSize{0.1f };
		// Details points labels and markers settings
		PointsLabelsAndMarkers pntsLabsMarks;
		// If true the thumb range stops at the end of the slider, thumb size ratio can be altered, and it uses the scrollbar style instead of the slider style
		bool asWindowScrollbar{ false };
	};

	// Fill shaders
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

	// For all draw functions you provide a fill shader
	class FillShader
	{
	private:
		// For optisation purposes (caching) so mutable is acceptable
		friend class Surface;
		mutable std::shared_ptr<FillShaderPIMPL> m_pimpl;
	public:
		FillShader() noexcept = default;

		// Must be able to retrieve a colour from any position within bounds
		// Position is guaranteed to be within bounds
		// Returned colour is blended already with destination
		virtual Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const = 0;
	
		// A colour that is modulated with the calculated colour before being returned from every subsequent GetColour call (or a GetFilledSurface call)
		// Note: only the last pushed colour is used
		void PushModulate(Colour255 mod);
		void PopModulate();

		// Modulates by the last modulation colour
		// If none then just returns col
		Colour255 Modulate(Colour255 col) const;

		// Fills a surface and returns it
		// Note: this may be slow first time but caches the result so subsequent calls will be quick
		std::shared_ptr<Surface> GetFilledSurface(int width, int height) const;

		// does not save modulation colour
		virtual bool SaveXML(CHapiXMLNode *parent) const = 0;
		virtual std::shared_ptr<FillShader> Clone() const = 0;

		virtual FillShaderTypes GetFillShaderType() const noexcept = 0;

		// For editor use when changed values directly
		void ClearCache();
	};

	template <class Derived>
	class DerivationHelper : public FillShader
	{
		std::shared_ptr<FillShader> Clone() const override final
		{
			return std::make_shared<Derived>(static_cast<const Derived&>(*this)); // call the copy ctor.
		}
	};

	// A colour fill shader
	class ColourFill : public DerivationHelper<ColourFill>
	{
	private:
		Colour255 m_colour{ Colour255::HORRID_PINK };
	public:
		ColourFill(Colour255 col) noexcept : m_colour(col) {}
		ColourFill(CHapiXML &xml, CHapiXMLNode *shaderNode);

		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;
		//Colour255 GetOneColour(Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;

		Colour255 GetColour() const { return m_colour; }
		void ChangeColour(Colour255 newCol) { m_colour = newCol; }

		bool SaveXML(CHapiXMLNode *parent) const override final;

		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eColour; }
	};

	// A lambda function fill shader
	class LambdaFill : public DerivationHelper<LambdaFill>
	{
	private:
		std::function<Colour255(const VectorF &pos, const RectangleI &bounds, Colour255 destColour)> &m_lambda;
	public:
		LambdaFill(std::function<Colour255(const VectorF &pos, const RectangleI &bounds, Colour255 destColour)> &lambda) : m_lambda(lambda) {}

		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final 
		{ 
			Colour255 col=(m_lambda)(position,bounds,destColour); 
			if (!writeAlpha)
				return Colour255(col.red, col.green, col.blue, destColour.alpha);
			return col;
		}
		bool SaveXML(CHapiXMLNode *parent) const override final;

		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eLambda; }
	};

	// Represents a pattern for drawing
	class PatternFill : public DerivationHelper<PatternFill>
	{
	private:
		friend class Surface;
		friend class UISkinEditorPattern;
		std::vector<int> m_grid;
		std::vector<Colour255> m_palette;
		VectorI m_repeats;
		int m_width{ 0 };
		int m_height{ 0 };
	public:
		// Creates a 4 by 4 striped pattern by default
		PatternFill();
		/*
		The vector is a 1D representation of a 2D grid with each row being 'width' long (number of columns)
		Each entry in the grid is an index into the palette. If this index does not exist a horrid pink is used
		Repeats allows for scaling of the pattern
		*/
		PatternFill(std::vector<int> grid, int width, int height, std::vector<Colour255> palette, VectorI repeats = VectorI(1, 1)) :
			m_grid(grid), m_width(width), m_height(height), m_palette(palette), m_repeats(repeats) {}
		PatternFill(CHapiXML &xml, CHapiXMLNode *patternNode);

		// Saves under "Pattern" node
		bool SaveXML(CHapiXMLNode *parent) const;

		// Position can be larger than grid - it just repeats pattern
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::ePattern; }
	};

	// Represents a gradient for drawing
	class GradientFill : public DerivationHelper<GradientFill>
	{
	private:
		friend class Surface;
		friend class UISkinEditorPalette;
		friend class UISkinEditorGradient;
		VectorF m_p1;	// Ratios across rect
		VectorF m_p2;	// Ratios across rect
		Colour255 m_colour1;
		Colour255 m_colour2;
		EGradientShape m_shape;
		EGradientProfile m_profile;
		float m_alpha = 1.0f;	
	public:
		GradientFill();
		// Applies a gradient across the surface using p1 and p2 control points (must range 0 to 1 only)
		// Can provide null colour for either but not both - null colour means gradient goes to transparency
		GradientFill(VectorF p1, VectorF p2, Colour255 colour1, Colour255 colour2, EGradientShape shape, EGradientProfile profile, float alpha = 1.0f) noexcept :
			m_p1(p1), m_p2(p2), m_colour1(colour1), m_colour2(colour2), m_shape(shape), m_profile(profile), m_alpha(alpha) {}
		GradientFill(CHapiXML &xml, CHapiXMLNode *gradientNode);
		
		// Saves under "Gradient" node
		bool SaveXML(CHapiXMLNode *parent) const;

		// Overrides required by FillShader
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eGradient; }
	};

	// For image fill shader
	enum class EImageFillMode
	{
		eTile,		// Tiles into the area being drawn
		eScale,		// Stretches or shrinks over the area
		eAsIs		// Drawn at a 1:1 size, once
	};

	class ImageCache;

	// Draws a bitmap as fill
	class ImageFill : public DerivationHelper<ImageFill>
	{
	private:		
		std::shared_ptr<Surface> m_surface;
		EImageFillMode m_mode = EImageFillMode::eAsIs;
		RectangleI m_area;
		bool m_useBilinear;
	public:
		// If area invalid whole surface used
		// Note: area must be smaller than surface size!
		ImageFill(const std::string &surfaceFilename, 
			EImageFillMode mode=EImageFillMode::eAsIs,bool useBilinear=true, RectangleI area = RectangleI());
		ImageFill(CHapiXML &xml, CHapiXMLNode *gradientNode);

		EImageFillMode GetMode() const { return m_mode; }
		void ChangeMode(EImageFillMode newMode) { m_mode = newMode; ClearCache();}

		void ChangeImage(std::shared_ptr<Surface> newImage) { m_surface = newImage; ClearCache(); }

		// Saves under "Image" node
		bool SaveXML(CHapiXMLNode *parent) const;
		
		// If bounds bigger than surface then what happens depends on mode:
		// eAsIs - returns ZERO colour
		// eScale - scales surface to bounds
		// eTile - tiles across bounds
		Colour255 GetColour(const VectorF &position, const RectangleI &bounds, Colour255 destColour, EBlendMode blendMode, bool writeAlpha) const override final;
		FillShaderTypes GetFillShaderType() const noexcept override final { return FillShaderTypes::eImage; }
	};	
}

// HAPI Sprites specific types in other files
#include "CoreTypes\Surface.h"
#include "CoreTypes\ColliderGroup.h"
#include "CoreTypes\SpriteSheet.h"
#include "CoreTypes\Sprite.h"

