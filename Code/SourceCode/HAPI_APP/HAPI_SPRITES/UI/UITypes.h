////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UITypes.h
//
// summary:	Types used throughout the UI and required headers
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <set>
#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include <cassert>
#include <queue>

#include "HAPISprites_Types.h"

using namespace HAPISPACE;

#include "UI\UIListener.h"

namespace HAPI_UI_SPACE
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Used for mapping input to actions Supports keys but will also support other control inputs.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class EInputMappingAction
	{
		eUnmapped,

		/* Default key mappings */
		ePress,			// Enter and Return and left click
		ePressAlt,		// Right mouse click. 
		eNorth,			// Up arrow or mouse scroll up
		eNorthFast,		// plus shift held
		eEast,			// Right arrow
		eEastFast,		// plus shift held
		eSouth,			// Down arrow or mouse scroll down
		eSouthFast,		// plus shift held
		eWest,			// Left arrow		
		eWestFast,		// plus shift held
		eEscape,		// ESC key
		eDeleteNext,	// Delete key
		eDeletePrevious,// Left arrow delete key
		eHome,			// HOME key
		eEnd,			// END key
		eTab,			// Tab key

		eZoomIn,		// Mouse scroll up + CTRL key
		eZoomOut,		// Mouse scroll down + CTRL key

		// Normally combinations
		eUndo,			// CTRL + Z
		eCut,			// CTRL + X
		eCopy,			// CTRL + C
		ePaste,			// CTRL + V
		eSelectAll,		// CTRL + A
		eSave,			// CTRL + S
		eHighlightStart, // SHIFT + HOME
		eHighlightEnd, // SHIFT + END

		// Involving mouse
		eShiftClick,
		eControlClick,
		eMouseDrag		// Cannot be altered		
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if 'action' is direction. </summary>
	///
	/// <param name="action">	The action. </param>
	///
	/// <returns>	True if direction, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool IsDirection(EInputMappingAction action) noexcept
	{
		if (action == EInputMappingAction::eNorth ||
			action == EInputMappingAction::eEast ||
			action == EInputMappingAction::eSouth ||
			action == EInputMappingAction::eWest)
			return true;
		return false;
	}

	/// <summary>	Values that represent input modifiers. </summary>
	enum class EInputModifiers
	{
		eUnmapped,
		eLeftShift,
		eRightShift,
		eLeftControl,
		eRightControl,
		eLeftAlt,
		eRightAlt,// Note: this also triggers a control key ALT-GR is Control - Alt
		eLeftMouseButton,
		eRightMouseButton,
		eMiddleMouseButton,
		eMouseScrollUp,
		eMouseScrollDown
	};

	/// <summary>	Values that represent mouse buttons. </summary>
	enum class EMouseButton
	{
		eLeft,
		eRight,
		eMiddle,
		eX1,
		eX2
	};

	/// <summary>	Values that represent gaps. </summary>
	enum class EGap
	{
		eUnmapped,		// Uses 0
		eMargin,	// Uses margin size from skin
		eTight,		// Uses half of skin object gap value
		eNormal,	// Uses skin object gap value e.g. either horizontalObjectGap or verticalObjectGap	
		eLoose		// Uses 2 * skin object gap value
	};

	/// <summary>	Values that represent axis. </summary>
	enum class EAxis
	{
		eX,
		eY,
		eBoth
	};

	/// <summary>	Values that represent alignments. </summary>
	enum class EAlignment
	{
		// Aligns minimum of object to other object
		eAlignMinToMin,	// E.g. if to the East means align top to the top, if South means align left to left		
		eAlignMinToMax,
		// Middles to middles
		eAlignMidToMid,
		// Aligns maximum of object to other object
		eAlignMaxToMin,	// E.g. if to the East means align bottom to the top, if South means align right to left
		eAlignMaxToMax
	};

	/// <summary>	Values that represent window states. </summary>
	enum class EWindowState
	{
		eClosed,
		eOpen,
		eDisabled, // visible but cannot interact with - has a grayed out appearance
		eMinimised, // just short bar visible
		eEditorDisabled // for use by editor only. Window is visible but not greyed out and object pressed listener messages still sent
	};

	/*
		OBJECTS
	*/

	/// <summary>	Object types. </summary>
	enum class EObjectType
	{
		eAdornment,
		eButton,
		eMenu,
		eText,
		eTextEntry,
		eListBox,
		eCanvas,
		eDial,
		eSlider,
		eShape
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Object type to string. </summary>
	///
	/// <param name="type">	The type. </param>
	///
	/// <returns>	A std::string. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline std::string EObjectTypeToString(EObjectType type)
	{
		switch (type)
		{
		case EObjectType::eAdornment:
			return "Adornment";
		case EObjectType::eButton:
			return "Button";
		case EObjectType::eMenu:
			return "Menu";
		case EObjectType::eText:
			return "Text";
		case EObjectType::eTextEntry:
			return "Text Entry";
		case EObjectType::eListBox:
			return "List Box";
		case EObjectType::eCanvas:
			return "Canvas";
		case EObjectType::eDial:
			return "Dial";
		case EObjectType::eSlider:
			return "Slider";
		case EObjectType::eShape:
			return "Shape";
		}

		return "ERROR: unknown type";
	}


	/// <summary>	Objects can be in one of these three states. </summary>
	enum class EActionState
	{
		eDisabled,
		eNormal,
		eSelected,
		ePressed
	};

	/// <summary>	Values that represent render priorities. </summary>
	enum class ERenderPriority
	{
		eFirst,
		eDontCare,
		eLast
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// namespace: WindowFeatures
	// 
	// Cannot be class as used for bit mask
	// so wrapped in sub namespace
	////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace WindowFeatures
	{
		/// <summary>	Values that represent adornment features. </summary>
		enum EAdornmentFeatures
		{
			eNormalWindow = 0,			 // Normal window style colours and text
			eBorder = 1 << 0,			 // Border
			eTitleText = 1 << 1,		 // If set creates a title bar
			eClose = 1 << 2,			 // If set creates a close box
			eMinimise = 1 << 3,
			eMenuBar = 1 << 4,			 // Adds room for a menu
			eUserDraw = 1 << 5,			 // If set listeners render gets called and no background colour drawn
			eSystem = 1 << 6,			 // This is owner draw and no adornments, close, size etc.
			eSizeable = 1 << 7,			 // Allows user to size window via bottom right handle
			eScaleWhenSized = 1 << 8	 // Overwrites the default so sizeing scales contents
			
		};
	}

	/// <summary>	Currently just two but could be expanded. </summary>
	enum class EButtonType
	{
		eToggle,		// Does not stay pressed
		eRadio,			// Stays pressed until pressed again
		eRadioTextLeft, // Behaviour as eRadio but text to left of button
		eRadioTextRight // Behaviour as eRadio but text to right of button
	};

	/// <summary>	Values that represent list box types. </summary>
	enum class EListBoxType
	{
		eNormal,
		eDropDown
	};

	/// <summary>	Currently two types of menu. </summary>
	enum class EMenuType
	{
		eDropDown,
		eRightClickMenu
	};

	/// <summary>	Values that represent text types. </summary>
	enum class ETextType
	{
		eStatic,	// Does nothing - just looks pretty
		eHeader,	// As above but uses different style
		eClickable	// Can be highlighted and clicked on where by it sends a notification
	};

	/// <summary>	Values that represent text entry lambdas. </summary>
	enum class ETextEntryLambdas
	{
		eEverything,
		eAlphaNumeric,
		eNumericOnly,
		eColourChannel, // 0-255
		eDegrees, // 0-360
		ePercent, // 0-100
		eUser
	};

	/*
		SKIN
	*/

	/// <summary>	A skin has a skin style for text and window. </summary>
	enum class ESkinStyle
	{
		// All use alt state for pressed unless stated
		eNormalText,
		eClickableText,
		eHeadingText,
		eButton,
		eButtonRadioSideText, // radio left text or right text
		eTextEntry,			// Cursor uses ALT text style colour which is also the pressed text colour
		eListBox,			// Highlighted items use window alt style for background, as does the thumb
		eMenu,				// Seperators use ALT style. Selected item has focussed window style.
		eCanvas,
		eDial,				// Markers use own supplied colour. Pointer uses ALT style.
		eSlider,			// Markers use own supplied colour. Thumb uses ALT style.		
		eScroller,			// Markers use own supplied colour. Thumb uses ALT style.
		eShape,				// Does not use focussed or alt styles
		eWindowNormal,		// Used by adornments. Alt window border shader does icons.
		eNumSkinStyles
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Skin style to string. </summary>
	///
	/// <param name="which">	The which. </param>
	///
	/// <returns>	A std::string. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline std::string SkinStyleToString(ESkinStyle which)
	{
		// If text changed here change SkinTextSringToStyle. Note it will break XML saves.
		switch (which)
		{
		case ESkinStyle::eNormalText:
			return "Normal Text";
		case ESkinStyle::eClickableText:
			return "Clickable Text";
		case ESkinStyle::eHeadingText:
			return "Heading Text";
		case ESkinStyle::eButton:
			return "Button";
		case ESkinStyle::eButtonRadioSideText:
			return "ButtonSideText";
		case ESkinStyle::eTextEntry:
			return "Text Entry";
		case ESkinStyle::eListBox:
			return "List Box";
		case ESkinStyle::eMenu:
			return "Menu";
		case ESkinStyle::eCanvas:
			return "Canvas";
		case ESkinStyle::eDial:
			return "Dial";
		case ESkinStyle::eSlider:
			return "Slider";
		case ESkinStyle::eWindowNormal:
			return "Normal Window";
		case ESkinStyle::eShape:
			return "Shape";
		case ESkinStyle::eScroller:
			return "Scroller";
		//case ESkinStyle::eWindowDialog:
		//	return "Dialog Window";
		default:
			break;
		}
		return "ERRROR: UNKNOWN SKIN STYLE";
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Skin string to style. </summary>
	///
	/// <param name="text">	The text. </param>
	///
	/// <returns>	An ESkinStyle. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ESkinStyle SkinStringToStyle(const std::string& text)
	{
		if (text == "Normal Text")
			return ESkinStyle::eNormalText;

		if (text == "Clickable Text")
			return ESkinStyle::eClickableText;

		if (text == "Heading Text")
			return ESkinStyle::eHeadingText;

		if (text == "Button")
			return ESkinStyle::eButton;

		if (text == "ButtonSideText")
			return ESkinStyle::eButtonRadioSideText;

		if (text == "Text Entry")
			return ESkinStyle::eTextEntry;

		if (text == "Menu")
			return ESkinStyle::eMenu;

		if (text == "Canvas")
			return ESkinStyle::eCanvas;

		if (text == "Dial")
			return ESkinStyle::eDial;

		if (text == "Slider")
			return ESkinStyle::eSlider;

		if (text == "Normal Window")
			return ESkinStyle::eWindowNormal;

		if (text == "List Box")
			return ESkinStyle::eListBox;

		if (text == "Shape")
			return ESkinStyle::eShape;

		if (text == "Scroller")
			return ESkinStyle::eScroller;


		std::cerr << "ERRROR: UNKNOWN SKIN STYLE: " << text << std::endl;
		return ESkinStyle::eNormalText;
	}

	/// <summary>	Values that represent skin sub styles. </summary>
	enum class ESkinSubStyle
	{
		eNormal,
		eDisabled,
		eFocussed,
		eAlt,	// what this defines depends on the object e.g. for a button it is pressed but for a dial it is colour iof pointer etc.
		eNumSubStyles
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Skin sub style to string. </summary>
	///
	/// <param name="which">	The which. </param>
	///
	/// <returns>	A std::string. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline std::string SkinSubStyleToString(ESkinSubStyle which)
	{
		// If text changed here change SkinSubStringToStyle. Note it will break XML saves.
		switch (which)
		{
		case ESkinSubStyle::eNormal:
			return "Normal";
		case ESkinSubStyle::eDisabled:
			return "Disabled";
		case ESkinSubStyle::eFocussed:
			return "Focussed";
		case ESkinSubStyle::eAlt:
			return "Alt";
		default:
			break;
		}
		return "ERRROR: UNKNOWN SKIN SUB STYLE";
	}
}