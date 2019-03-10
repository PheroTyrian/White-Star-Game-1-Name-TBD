#pragma once

#include "UI\UITypes.h"
#include "UI\UISkin.h"
#include "CoreTypes\Surface.h"

class SkinEditor;

namespace HAPI_UI_SPACE
{
	class UIObject;
	class UIObjectMenu;
	class UIObjectButton;
	class UIObjectText;
	class UIObjectTextEntry;
	class UIObjectAdornments;	
	class UIObjectCanvas;
	class UIObjectListBox;
	class UIObjectDial;
	class UIObjectSlider;
	class UIObjectShape;
	class UIMultiSelection;

	class IWindowUserData
	{
	public:
		virtual ~IWindowUserData() {};
	};

	// A window
	class UIWindow
	{
	private:
		std::string m_name;

		std::shared_ptr<Skin> m_skin;

		// Changed to map from unordered as editor needs the order
		// Monitor this but it should not be much of a hit
		std::map<std::string, UIObject*> m_uiObjectMap;

		// Radio button collections
		// Set is ordered and fast to find in O(log N), insertion is fast
		// Since button itself needs to know if it is in a collection using shared pointers
		std::vector<std::shared_ptr<std::vector<std::string>>> m_radioButtonCollections;

		// For objects that should follow the mouse
		// Name, offset
		std::map<std::string, VectorI> m_followingMouse;

		VectorI m_windowPos{ 0,0 }; // top left
		VectorI m_preMinimisedPosition;
		std::string m_minimisedIntoWindow;
		int m_windowWidth{ 0 };
		int m_windowHeight{ 0 };

		// Client rect is now cached since I need to take into account any scrollbars
		RectangleI m_clientRect;

		// Used for resizing
		int m_originalWidth{ 0 };
		int m_originalHeight{ 0 };

		// Each window can have just one of these
		std::string m_rightClickMenuName;

		std::string m_objectWithFocus;

		// For handling multiple selections
		std::shared_ptr<UIMultiSelection> m_multiSelection;

		EWindowState m_state{ EWindowState::eClosed };

		// Tag added to each object that can be used in global operations, e.g. turn off, turn on so useful for a screen
		std::string m_currentTag;		

		// Other windows can be minimised in us and stacked accordingly
		VectorI m_lastStackedPosition{ -1 }; // magic
		
		// Docks other window into this
		void Dock(UIWindow& window,const RectangleI &minimisedSize);

		// Undocks from this and repositions all docked windows
		void Undock(UIWindow& window);

		// Windows docked in us
		std::list<std::string> m_dockedWindows;

		// User can specify an id for a named object for ease
		// This is a map holding a map between object names and ids
		std::map<std::string, int> m_userIdMappings;		

		bool m_windowIsBeingDragged{ false };

		// Debug usage
		// TODO: add a keypress ot toggle this
		bool m_showBoundingRectangles{ false };
		
		bool m_useHardware{ false };

		// Some functionality needs ot change if being edited in editor
		bool m_beingEdited{ false };

		// Every window has to have one adornment
		UIObjectAdornments* m_adornments{ nullptr};

		RectangleI WindowRectToScreenRect(RectangleI rect) const {
			return rect.Translate(m_windowPos);
		}

		RectangleI ScreenRectToWindowRect(RectangleI rect) const {
			return rect.Translate(-m_windowPos);
		}

		void CalculateClientRect();

		friend class UISystem;		

		// Opening and closing windows must be done via a UI call (so ordering can be maintained)
		void Open();
		void Close();

		// If there is a user id mapping this returns it else false
		bool GetUserIdMapping(const UIObject& object, int *id) const;
				
		// For user's use, just held and accessed here, nothing else
		std::shared_ptr<IWindowUserData> m_userData;

		// Position first object relative to a rectangle
		void PositionRelativeTo(const std::string& objectToPositionName, RectangleI relativeToRectangle,
			EDirection positioning, EAlignment alignment = EAlignment::eAlignMinToMin, EGap gap=EGap::eNormal,
			const VectorI& translate = VectorI(), EAxis axis = EAxis::eBoth);

		//friend class UIEditor;
		friend class UISystem;
		friend class UIMultiSelection;
		friend class UIEditorMainWindow;
		friend class UIEditorObjectCommonWindow;

		// Only to be called from UISystem, otherwise go via UI.RenameWindow
		void ChangeNameDirect(const std::string& newName) { m_name = newName; } 
		void ChangeFocusObject(const std::string &newFocusObject);
		const std::string& GetFocusObject() const { return m_objectWithFocus; }
		// Used to find  last item added for editor
		bool FindEditorLastAdded(std::string &lastAdded) const;

		void AdjustWindowScrollBarsForNewWindowSize(UIObject* slider, int oldWidth, int oldHeight);

		bool CanGiveUpFocus() const;
		
		// This window has gained the focus
		// Optionally we may be provided with whcih of our objects should also take the focus.
		void GainedFocus(const std::string& newFocusObject = std::string());
		
		// This window has lost the focus
		void LostFocus();

		bool FocusObjectHandleInputAction(EInputMappingAction action, EInputMappingAction lastAction);
		
		// Names of all objects
		std::vector<std::string> GetAllObjectNames() const;
	public:
		UIWindow(std::shared_ptr<Skin> skin,std::string name,RectangleI windowRect,unsigned int featuresFlags, std::string titleText);
		// Construct a Window from XML
		UIWindow(std::shared_ptr<Skin> skin, CHapiXML &xml, CHapiXMLNode *rootNode);
		~UIWindow();

		// Cascade styles and recreate all objects
		void Refresh();

		// True to disable window and contents
		void Disable(bool set);

		// True to disable contents (excluding adornments)
		// They remember their previous state and return to it on passing false here
		void DisableContents(bool set);

		// Minimise window (if it has a minimise), if stackInWindow provided, asks window for a stack position to move to
		void Minimise(const std::string& stackInWindowName=std::string());

		// Maximise a minimised window, position will be changed to what it was when minimised
		void Maximise();		

		// Multiple selection functions

		// Select all objects within a specified area of the screen (if in window) and add to selection
		void MultipleObjectSelection(const RectangleI &screenArea);
		// Provides objects to select by name and adds to selection
		void MultipleObjectSelection(const std::vector<std::string> &objectNames);
		// Clears the whole collection
		void MultipleObjectSelectionClear();
		// Clears one from the collection
		void MultipleObjectSelectionRemove(const std::string& objectName);
		// Gets if anything is slected
		bool MultipleObjectHasObjects() const;
		// Get if this is selected
		bool MultipleObjectHasObject(const std::string& objectName) const;
		// Gets the collection names
		std::vector<std::string> MultipleObjectSelectionObjectNames();

		// Makes sure an object is completely on the window, takes into account margins
		// and optionally isd aligned to grid made from skin horizontal and vertical gaps
		void AdjustObjectPositionForWindow(const std::string& objectName, bool snapToGrid);

		void SetBeingEdited(bool set) { m_beingEdited = set; }
		bool GetBeingEdited() const { return m_beingEdited; }

		VectorI GetMousePosWindowSpace() const;

		// Every window has to have one adornment object
		// Name is window name + "_Adorn"

		const UIObjectAdornments& GetAdornments() const { return *m_adornments; }
		UIObjectAdornments& GetAdornments() { return *m_adornments; }

		// Returns null if does not exist or is not a list box
		UIObjectListBox* GetListBox(const std::string& listBoxName) const;

		// Returns null if does not exist or is not a menu
		UIObjectMenu* GetMenu(const std::string& menuName) const;

		// For user's use, just held and accessed here, nothing else
		void SetUserData(std::shared_ptr<IWindowUserData> newData) { m_userData = newData; }
		std::shared_ptr<IWindowUserData> GetUserData() const { return m_userData; }

		// Allows user to map object names to their own id for ease
		// Returns false if an object could not be found
		// Note: if the name already exists in the map its id will be overriden
		bool AddIDMappings(const std::map<std::string, int>& newMappings);

		void DebugShowBoundingRectangles(bool set=true) { m_showBoundingRectangles = set; }

		void SetObjectToFollowMouse(const std::string& objectName, VectorI offset);


		// Will force refresh of all objects
		void ChangeSkin(std::string newSkinName);
		void ChangeSkin(std::shared_ptr<Skin> skin);

		void ChangeState(EWindowState newState) { m_state = newState; }
		EWindowState GetState() const { return m_state; }

		// Gets the skin used by this window to read
		const Skin& GetSkin() const { return *m_skin; }
		// Gets the skin used by this window to write
		Skin& GetSkin() { return *m_skin; }
		// To share
		std::shared_ptr<Skin> GetSkinToShare() const { return m_skin; }

		// Gets the skin sizes used by this window to read
		const UiSkinSizes& GetSkinSizes() const;

		void SetDragging(bool set) { m_windowIsBeingDragged = set; }
		bool GetDragging() const { return m_windowIsBeingDragged; }

		bool IsVisible() const { return (m_state != EWindowState::eClosed); }

		//__declspec(noinline)
		bool IsEnabled() const 
		{ 
			if (m_state == EWindowState::eOpen || m_state == EWindowState::eEditorDisabled || m_state==EWindowState::eMinimised) 
				return true; 
			return false; 
		}

		const std::string& GetName() const { return m_name; }

		RectangleI GetLocalRect() const;
		RectangleI GetScreenRect() const { return WindowRectToScreenRect(GetLocalRect()); }		
		const RectangleI& GetLocalClientRect() const { return m_clientRect; }
		RectangleI GetScreenSpaceClientRect() const { return WindowRectToScreenRect(GetLocalClientRect()); }

		RectangleI GetLocalMenuAreaRect() const;

		bool CoversScreenPoint(VectorI screenPoint) const { 
			return GetScreenRect().Contains(screenPoint); 
		}
		
		void SetScreenPosition(VectorI newPos){ m_windowPos = newPos; }
		VectorI GetScreenPosition() const {return m_windowPos;}

		std::string FindObjectAtScreenPoint(VectorI screenPoint) const;
		std::string FindObjectAtWindowPoint(VectorI windowPoint) const;

		// Scaling is dependant on feature flag set on creation
		void Resize(int newWidth,int newHeight);

		void SizeToFitContents();

		// Will return false if not possible e.g. window already has an object with that name
		bool ChangeObjectName(const std::string& objectName, const std::string& newName);

		// Get object pointer by name, can supress error message, returns nullptr if not found
		UIObject* GetObject(const std::string& objectName, bool reportError = true) const;

		// Get object pointer by user id, can supress error message, returns nullptr if not found
		UIObject* GetObject(int userId, bool reportError = true) const;

		// Makes a copy of an object
		// If duplicateName empty OR duplicateName already exists a new name is creasted from original name (number appended)
		// Note: you will want to change its position afterwards otherwise it will appear on the top of the old one
		// Returns null if fails
		UIObject* DuplicateObject(const std::string& objectName, std::string duplicateName = std::string());

		// Constructs a menu. If type is drop down first item is the menu title
		// If inMenuBar is true the y position is set automatically to menu bar but x is left to user
		UIObjectMenu* AddMenu(std::string name, const std::vector<std::string>& menuItems, EMenuType type = EMenuType::eDropDown, bool inMenuBar=true, int textSizeIncrement=0);

		// Constructs a button from scratch
		// When button type is one with text to left or right, size of button is determined by bounds width
		UIObjectButton* AddButton(std::string name, std::string text, EButtonType type = EButtonType::eToggle, int textSizeIncrement = 0);

		// Constructs a list box. Width is determined from widest item provided unless a numCharsWide is specified (!=0)
		UIObjectListBox* AddListBox(std::string name, int numVisibleRows, std::vector<std::string> items, 
			EListBoxType type=EListBoxType::eNormal, int numCharsWide=0, int textSizeIncrement = 0);

		// Constructs a canvas to rect size
		// Can optionally provide a sprite to render to canvas before user renders
		UIObjectCanvas* AddCanvas(std::string name, RectangleI rect, std::shared_ptr<Sprite> sprite=nullptr, int textSizeIncrement = 0);

		/*
			A helper that does a few common tasks:
				Ceates the buttons and sets them all to the same size 
				Sets them as a radio button collection
				Fills them into rows
				Optionally assigns an id range

				nameAndText is a pair , one for the id name and one for text on the button
				/When button type is one with text to left or right, size of button is determined by bounds width
		*/
		bool AddRadioButtons(const std::vector<std::pair<std::string, std::string>>& nameAndText,
			const std::string& relativeToObject, EDirection positioning, int maxInRow,
			EAlignment alignment = EAlignment::eAlignMinToMin,const int * const userIdStart = nullptr,
			bool initiallyVisible=true, EButtonType type=EButtonType::eRadio, int textSizeIncrement = 0);

		/*
		A helper that does a few common tasks:
		Ceates the toggle buttons and sets them all to the same size
		Fills them into rows
		Optionally assigns an id range
		nameAndText is a pair , one for the id name and one for text on the button
		*/
		bool AddButtons(const std::vector<std::pair<std::string, std::string>>& nameAndText,
			const std::string& relativeToObject, EDirection positioning, int maxInRow,
			EAlignment alignment = EAlignment::eAlignMinToMin, int *userIdStart = nullptr, int textSizeIncrement = 0);

		/*
		A helper that adds a text entry and a label to go with it
		Assumes text entry to right of label and 
		Positioning refers to position of label
		*/
		bool AddTextEntryAndLabel(std::string textEntryName, std::string labelName, std::string labelText,
			int maxVisibleChars,
			const std::string& relativeToObject, EDirection positioning,
			EAlignment alignment = EAlignment::eAlignMinToMin,
			ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything, int textSizeIncrement = 0);

		// Adds a text line that can be static or clickable. Uses the current UI style skin
		UIObjectText* AddText(std::string name, std::string text, ETextType type = ETextType::eStatic, int textSizeIncrement = 0);

		// Adds a text line that can be static or clickable. This one allows text colour and size increment to be altered. SizeIncrement is offset from settings point size.
		UIObjectText* AddText(std::string name, std::string text, Colour255 textColour, ETextType type = ETextType::eStatic, int textSizeIncrement = 0);

		// Adds a text line that can be static or clickable. This one allows all text style settings to be overriden from the defaults.
		// Remember to set correct inherit flags in the style
		UIObjectText* AddText(std::string name, std::string text, UiTextStyleGroup style, ETextType type = ETextType::eStatic,int textSizeIncrement = 0);

		// Adds a text entry box with one of the provided lambdas to set dimensions
		//UIObjectTextEntry* AddTextEntry(std::string name, RectangleI dimensions,ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything);

		// Adds a text entry box with one of the provided lambdas to dimensions calculated from num chars
		UIObjectTextEntry* AddTextEntry(std::string name, int maxVisibleChars, ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything, bool isFloat=false, int textSizeIncrement = 0);

		// Adds a text entry box with a user supplied text validating lambda
		// Note: if used it cannot be saved in XML so will need post-applying after a load
		UIObjectTextEntry* AddTextEntry(std::string name, int maxVisibleChars, std::function<bool(char newChar, const std::string& newText)> validateLambda, int textSizeIncrement = 0);

		// Adds a dial which takes its dimensions from the rectangle
		UIObjectDial* AddDial(std::string name, RectangleI dimensions, DialLayout layout, int textSizeIncrement = 0);

		// Adds a slider which takes its dimensions from the rectangle and orientation
		UIObjectSlider* AddSlider(std::string name, RectangleI dimensions, SliderLayout layout, int textSizeIncrement = 0);

		// Adds a shape which is contained in the rectangle
		UIObjectShape* AddShape(std::string name, RectangleI dimensions, std::string caption, int textSizeIncrement = 0);

		// Safe downcasting. If object is not a slider or does not exist, returns nullptr
		UIObjectSlider* GetSlider(std::string name) const;

		// Adds scrollbars to this window. They are in the form of sliders but with no shadow and tumb region
		// stops against the lengths. Adds if object name is not empty.
		bool AddScrollbar(const std::string& verticalName, const std::string& horizontalName=std::string(), int textSizeIncrement = 0);

		// Sets window space position of an object directly. Best to let it be aligned to grid
		void PositionObject(const std::string& objectToPositionName, VectorI pos, bool alignToGrid = true);

		// Applies window offset to object window position
		VectorI GetObjectPositionScreenSpace(const std::string& objectName) const;

		// Shortcut to getting an objects screen space bounds
		RectangleI GetObjectBoundsScreenSpace(const std::string& objectName) const;

		// Useful for lining up menus in a window menu bar
		// Window must have been created with menu feature flag
		void PositionObjectsInMenuBar(const std::vector<std::string> &objects);	

		// Used ot position one menu in a menu bar. Position relates to any other menus 
		void PositionMenuInMenuBar(const std::string& menuName, int position);

		// Position first object relative to second object
		// Note: if positioning direction is a diagonal then alignment is ignored
		// translate is in terms of object gaps not actual pixels
		void PositionRelativeTo(const std::string& objectToPositionName, const std::string& relativeToObject,EDirection positioning,
			EAlignment alignment = EAlignment::eAlignMinToMin, EGap gap = EGap::eNormal, const VectorI& translate = VectorI(), EAxis axis=EAxis::eBoth);

		// Quick alignment to another on an axis
		// translate is in terms of object gaps not actual pixels
		void AlignTo(const std::string& objectToPositionName, const std::string& relativeToObject, EAxis axis,
			EAlignment alignment = EAlignment::eAlignMinToMin, const VectorI& translate = VectorI(0));		

		// Position at the edge (or corner) of this window
		// translate is in terms of object gaps not actual pixels
		void PositionObjectAgainstWindowEdge(const std::string& toPositionName, EDirection edge, 
			EGap gap = EGap::eMargin, const VectorI& translate = VectorI(0), EAxis axis = EAxis::eBoth);

		// Allows 1 or more objects to be positioned centrally in a window, axis can be specified
		// Note that for multiple objects they are positioned as if they were one object with the rectangle 
		// bounding them all being centred
		void PositionObjectsInWindowCentre(const std::vector<std::string> &objects, EAxis axis = EAxis::eBoth);

		// For positioning a window relative to this one
		// Note that windows are not children of other windows so positioning is in screen space
		// if inside is true edge is the inside edge else outside edge
		// E.g. if you say east and inside then it will be positioned inside this window against left edge
		// But if you say east and !inside then it will be positioned to the left of the left edge
		// translate is in terms of object gaps not actual pixels
		void PositionWindowAgainstWindowEdge(const std::string& windowToPositionName, EDirection edge, 
			bool inside=false, EGap gap = EGap::eNormal, const VectorI& translate = VectorI(0));

		// Alters one edge of an object to match an edge of another object
		// Directions here can only be 1 of the four primary ones
		// whichEdge is the edge to alter, toWhichEdge is the edge of the relative object
		// This effectivley sizes an object. Note that some objects may alter their own size to a minimum
		void SetEdgeTo(const std::string& objectToSetName, const std::string& relativeToObject, EDirection whichEdge, EDirection toWhichEdge);

		// Sets the size of objectToSizeName to be the same as relativeToObject
		// width and height can be optionally set 
		void SizeTo(const std::string& objectToSizeName, const std::string& relativeToObject, EAxis axis=EAxis::eBoth);

		// Allows a group of objects to be sized to the largest dimension of any
		// adjust is in terms of object gaps not actual pixels and is added to size
		void SizeToLargest(const std::vector<std::string> &objectsToSize, EAxis axis = EAxis::eBoth, VectorI adjust = VectorI());

		// Allows a group of objects determined by tag to be sized to the largest dimension of any
		void SizeToLargestTag(const std::string &tag, EAxis axis = EAxis::eBoth);

		// Allows a group of buttons to behave like radio buttons 
		void SetAsRadioButtonCollection(const std::vector<std::string>& buttonVector);

		// Render this window to the surface
		void Render(std::shared_ptr<Surface> &renderSurface);

		// Called when HAPI detects a key event (via UI)
		// Return false if not handled
		// Additionally passed an input action that may have been detected from mappings
		bool HandleKeyEvent(EKeyEvent keyEvent, BYTE keyCode, EInputMappingAction action, EInputMappingAction lastAction);

		// Return false if not handled
		// Additionally passed an input action that may have been detected from mappings
		bool HandleMouseAction(const MouseData& mouseData, EInputMappingAction action, EInputMappingAction lastAction);

		// Called for a controller event, currently not using (via UI)
		// Return false if not handled
		bool HandleControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData);

		// Called with interpreted key presses into character entry (via UI)
		// Return false if not handled
		bool HandleTextEntered(char enteredText);

		// Tag added to each object that can be used in global operations, e.g. turn off, turn on so useful for a screen
		// Returns old tag
		std::string SetCurrentTag(std::string tag) { std::string oldTag{ m_currentTag }; m_currentTag = tag; return oldTag; }
		void ClearCurrentTag() { m_currentTag = std::string(); }

		std::vector<std::string> GetAllNamesOfObjectsWithTag(const std::string& tag) const;

		void SetTitleText(const std::string& newText);
		
		void SetTaggeObjectsVisibility(const std::string& tag, bool set);

		// Only call if you have completely finished with them
		void DeleteAllObjectsWithTag(const std::string& tag);

		// Only call if you have completely finished with it
		// Otherwise just set its visibility to false to disable it
		void DeleteObject(const std::string& name);

		// Saves this Window to XML
		bool WriteXML(CHapiXMLNode *windowNode) const;

		/*
			Saves all the object names to a text file for easy reference when building the UI backend code
			Uses current working directory and window name to save to, can optionally open in notepad
		*/
		bool SaveObjectNames(bool openInNotepad = false) const;
	};
}