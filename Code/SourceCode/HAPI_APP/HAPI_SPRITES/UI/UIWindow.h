////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIWindow.h
//
// summary:	Declares the window class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "UI\UITypes.h"
#include "UI\UISkin.h"
#include "CoreTypes\Surface.h"

class SkinEditor;

namespace HAPI_UI_SPACE
{
	/// <summary>	An object. </summary>
	class UIObject;
	/// <summary>	An object menu. </summary>
	class UIObjectMenu;
	/// <summary>	An object button. </summary>
	class UIObjectButton;
	/// <summary>	An object text. </summary>
	class UIObjectText;
	/// <summary>	An object text entry. </summary>
	class UIObjectTextEntry;
	/// <summary>	An object adornments. </summary>
	class UIObjectAdornments;	
	/// <summary>	An object canvas. </summary>
	class UIObjectCanvas;
	/// <summary>	An object list box. </summary>
	class UIObjectListBox;
	/// <summary>	An object dial. </summary>
	class UIObjectDial;
	/// <summary>	An object slider. </summary>
	class UIObjectSlider;
	/// <summary>	An object shape. </summary>
	class UIObjectShape;
	/// <summary>	A multi selection. </summary>
	class UIMultiSelection;

	/// <summary>	A window user data. </summary>
	class IWindowUserData
	{
	public:
		/// <summary>	Destructor. </summary>
		virtual ~IWindowUserData() {};
	};

	/// <summary>	A window. </summary>
	class UIWindow
	{
	private:
		/// <summary>	The name. </summary>
		std::string m_name;

		/// <summary>	The skin. </summary>
		std::shared_ptr<Skin> m_skin;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Changed to map from unordered as editor needs the order Monitor this but it should not be
		/// much of a hit.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::map<std::string, UIObject*> m_uiObjectMap;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Radio button collections Set is ordered and fast to find in O(log N), insertion is fast Since
		/// button itself needs to know if it is in a collection using shared pointers.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::shared_ptr<std::vector<std::string>>> m_radioButtonCollections;

		/// <summary>	For objects that should follow the mouse Name, offset. </summary>
		std::map<std::string, VectorI> m_followingMouse;

		/// <summary>	top left. </summary>
		VectorI m_windowPos{ 0,0 };
		/// <summary>	The pre minimised position. </summary>
		VectorI m_preMinimisedPosition;
		RectangleI m_preMinimisedSize;
		/// <summary>	The minimised into window. </summary>
		std::string m_minimisedIntoWindow;
		/// <summary>	Width of the window. </summary>
		int m_windowWidth{ 0 };
		/// <summary>	Height of the window. </summary>
		int m_windowHeight{ 0 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Client rect is now cached since I need to take into account any scrollbars.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI m_clientRect;

		/// <summary>	Used for resizing. </summary>
		int m_originalWidth{ 0 };
		/// <summary>	Height of the original. </summary>
		int m_originalHeight{ 0 };

		/// <summary>	Each window can have just one of these. </summary>
		std::string m_rightClickMenuName;

		/// <summary>	The object with focus. </summary>
		std::string m_objectWithFocus;

		/// <summary>	For handling multiple selections. </summary>
		std::shared_ptr<UIMultiSelection> m_multiSelection;

		/// <summary>	The state. </summary>
		EWindowState m_state{ EWindowState::eClosed };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Tag added to each object that can be used in global operations, e.g. turn off, turn on so
		/// useful for a screen.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string m_currentTag;		

		/// <summary>	Other windows can be minimised in us and stacked accordingly. </summary>
		VectorI m_lastStackedPosition{ -1 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Docks other window into this. </summary>
		///
		/// <param name="window">			[in,out] The window. </param>
		/// <param name="minimisedSize">	Size of the minimised. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Dock(UIWindow& window,const RectangleI &minimisedSize);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Undocks from this and repositions all docked windows. </summary>
		///
		/// <param name="window">	[in,out] The window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Undock(UIWindow& window);

		/// <summary>	Windows docked in us. </summary>
		std::list<std::string> m_dockedWindows;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// User can specify an id for a named object for ease This is a map holding a map between object
		/// names and ids.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::map<std::string, int> m_userIdMappings;		

		/// <summary>	True if window is being dragged. </summary>
		bool m_windowIsBeingDragged{ false };

		/// <summary>	Debug usage TODO: add a keypress to toggle this. </summary>
		bool m_showBoundingRectangles{ false };
		
		/// <summary>	True to use hardware. </summary>
		bool m_useHardware{ false };

		/// <summary>	Some functionality needs ot change if being edited in editor. </summary>
		bool m_beingEdited{ false };

		/// <summary>	Every window has to have one adornment. </summary>
		UIObjectAdornments* m_adornments{ nullptr};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Window rectangle to screen rectangle. </summary>
		///
		/// <param name="rect">	The rectangle. </param>
		///
		/// <returns>	A RectangleI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI WindowRectToScreenRect(RectangleI rect) const {
			return rect.Translate(m_windowPos);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Screen rectangle to window rectangle. </summary>
		///
		/// <param name="rect">	The rectangle. </param>
		///
		/// <returns>	A RectangleI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI ScreenRectToWindowRect(RectangleI rect) const {
			return rect.Translate(-m_windowPos);
		}

		/// <summary>	Calculates the client rectangle. </summary>
		void CalculateClientRect();

		/// <summary>	A system. </summary>
		friend class UISystem;		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Opening and closing windows must be done via a UI call (so ordering can be maintained)
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Open();
		/// <summary>	Closes this object. </summary>
		void Close();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	If there is a user id mapping this returns it else false. </summary>
		///
		/// <param name="object">	The object. </param>
		/// <param name="id">	 	[in,out] If non-null, the identifier. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetUserIdMapping(const UIObject& object, int *id) const;
				
		/// <summary>	For user's use, just held and accessed here, nothing else. </summary>
		std::shared_ptr<IWindowUserData> m_userData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Position first object relative to a rectangle. </summary>
		///
		/// <param name="objectToPositionName">	Name of the object to position. </param>
		/// <param name="relativeToRectangle"> 	The relative to rectangle. </param>
		/// <param name="positioning">		   	The positioning. </param>
		/// <param name="alignment">		   	(Optional) The alignment. </param>
		/// <param name="gap">				   	(Optional) The gap. </param>
		/// <param name="translate">		   	(Optional) The translate. </param>
		/// <param name="axis">				   	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionRelativeTo(const std::string& objectToPositionName, RectangleI relativeToRectangle,
			EDirection positioning, EAlignment alignment = EAlignment::eAlignMinToMin, EGap gap=EGap::eNormal,
			const VectorI& translate = VectorI(), EAxis axis = EAxis::eBoth);

		//friend class UIEditor;
		/// <summary>	A system. </summary>
		friend class UISystem;
		/// <summary>	A multi selection. </summary>
		friend class UIMultiSelection;
		/// <summary>	Form for viewing the user interface editor main. </summary>
		friend class UIEditorMainWindow;
		/// <summary>	Form for viewing the user interface editor object common. </summary>
		friend class UIEditorObjectCommonWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Only to be called from UISystem, otherwise go via UI.RenameWindow. </summary>
		///
		/// <param name="newName">	Name of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeNameDirect(const std::string& newName) { m_name = newName; } 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change focus object. </summary>
		///
		/// <param name="newFocusObject">	The new focus object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeFocusObject(const std::string &newFocusObject);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets focus object. </summary>
		///
		/// <returns>	The focus object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetFocusObject() const { return m_objectWithFocus; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Used to find  last item added for editor. </summary>
		///
		/// <param name="lastAdded">	[in,out] The last added. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool FindEditorLastAdded(std::string &lastAdded) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adjust window scroll bars for new window size. </summary>
		///
		/// <param name="slider">   	[in,out] If non-null, the slider. </param>
		/// <param name="oldWidth"> 	Width of the old. </param>
		/// <param name="oldHeight">	Height of the old. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void AdjustWindowScrollBarsForNewWindowSize(UIObject* slider, int oldWidth, int oldHeight);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if we can give up focus. </summary>
		///
		/// <returns>	True if we can give up focus, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CanGiveUpFocus() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// This window has gained the focus Optionally we may be provided with whcih of our objects
		/// should also take the focus.
		/// </summary>
		///
		/// <param name="newFocusObject">	(Optional) The new focus object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void GainedFocus(const std::string& newFocusObject = std::string());
		
		/// <summary>	This window has lost the focus. </summary>
		void LostFocus();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Focus object handle input action. </summary>
		///
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool FocusObjectHandleInputAction(EInputMappingAction action, EInputMappingAction lastAction);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Names of all objects. </summary>
		///
		/// <returns>	all object names. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::string> GetAllObjectNames() const;
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="skin">				The skin. </param>
		/// <param name="name">				The name. </param>
		/// <param name="windowRect">   	The window rectangle. </param>
		/// <param name="featuresFlags">	The features flags. </param>
		/// <param name="titleText">		The title text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIWindow(std::shared_ptr<Skin> skin,std::string name,RectangleI windowRect,unsigned int featuresFlags, std::string titleText);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct a Window from XML. </summary>
		///
		/// <param name="skin">	   	The skin. </param>
		/// <param name="xml">	   	[in,out] The XML. </param>
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIWindow(std::shared_ptr<Skin> skin, CHapiXML &xml, CHapiXMLNode *rootNode);
		/// <summary>	Destructor. </summary>
		~UIWindow();

		/// <summary>	Cascade styles and recreate all objects. </summary>
		void Refresh();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	True to disable window and contents. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Disable(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// True to disable contents (excluding adornments)
		/// They remember their previous state and return to it on passing false here.
		/// </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DisableContents(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Minimise window (if it has a minimise), if stackInWindow provided, asks window for a stack
		/// position to move to.
		/// </summary>
		///
		/// <param name="stackInWindowName">	(Optional) Name of the stack in window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Minimise(const std::string& stackInWindowName=std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Maximise a minimised window, position will be changed to what it was when minimised.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Maximise();		

		// Multiple selection functions

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Select all objects within a specified area of the screen (if in window) and add to selection.
		/// </summary>
		///
		/// <param name="screenArea">	The screen area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void MultipleObjectSelection(const RectangleI &screenArea);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Provides objects to select by name and adds to selection. </summary>
		///
		/// <param name="objectNames">	List of names of the objects. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void MultipleObjectSelection(const std::vector<std::string> &objectNames);
		/// <summary>	Clears the whole collection. </summary>
		void MultipleObjectSelectionClear();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Clears one from the collection. </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void MultipleObjectSelectionRemove(const std::string& objectName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets if anything is slected. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool MultipleObjectHasObjects() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get if this is selected. </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool MultipleObjectHasObject(const std::string& objectName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the collection names. </summary>
		///
		/// <returns>	A std::vector&lt;std::string&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::string> MultipleObjectSelectionObjectNames();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Makes sure an object is completely on the window, takes into account margins and optionally
		/// isd aligned to grid made from skin horizontal and vertical gaps.
		/// </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		/// <param name="snapToGrid">	True to snap to grid. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void AdjustObjectPositionForWindow(const std::string& objectName, bool snapToGrid);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets being edited. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBeingEdited(bool set) { m_beingEdited = set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets being edited. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetBeingEdited() const { return m_beingEdited; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets mouse position window space. </summary>
		///
		/// <returns>	The mouse position window space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetMousePosWindowSpace() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Every window has to have one adornment object Name is window name + "_Adorn".
		/// </summary>
		///
		/// <returns>	The adornments. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UIObjectAdornments& GetAdornments() const { return *m_adornments; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the adornments. </summary>
		///
		/// <returns>	The adornments. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectAdornments& GetAdornments() { return *m_adornments; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns null if does not exist or is not a list box. </summary>
		///
		/// <param name="listBoxName">	Name of the list box. </param>
		///
		/// <returns>	Null if it fails, else the list box. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectListBox* GetListBox(const std::string& listBoxName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns null if does not exist or is not a menu. </summary>
		///
		/// <param name="menuName">	Name of the menu. </param>
		///
		/// <returns>	Null if it fails, else the menu. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectMenu* GetMenu(const std::string& menuName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	For user's use, just held and accessed here, nothing else. </summary>
		///
		/// <param name="newData">	Information describing the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetUserData(std::shared_ptr<IWindowUserData> newData) { m_userData = newData; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets user data. </summary>
		///
		/// <returns>	The user data. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<IWindowUserData> GetUserData() const { return m_userData; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows user to map object names to their own id for ease Returns false if an object could not
		/// be found Note: if the name already exists in the map its id will be overriden.
		/// </summary>
		///
		/// <param name="newMappings">	The new mappings. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AddIDMappings(const std::map<std::string, int>& newMappings);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Debug show bounding rectangles. </summary>
		///
		/// <param name="set">	(Optional) True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DebugShowBoundingRectangles(bool set=true) { m_showBoundingRectangles = set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets object to follow mouse. </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		/// <param name="offset">	 	The offset. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetObjectToFollowMouse(const std::string& objectName, VectorI offset);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Will force refresh of all objects. </summary>
		///
		/// <param name="newSkinName">	Name of the new skin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeSkin(std::string newSkinName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change skin. </summary>
		///
		/// <param name="skin">	The skin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeSkin(std::shared_ptr<Skin> skin);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change state. </summary>
		///
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeState(EWindowState newState) { m_state = newState; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the state. </summary>
		///
		/// <returns>	The state. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EWindowState GetState() const { return m_state; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the skin used by this window to read. </summary>
		///
		/// <returns>	The skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Skin& GetSkin() const { return *m_skin; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the skin used by this window to write. </summary>
		///
		/// <returns>	The skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skin& GetSkin() { return *m_skin; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	To share. </summary>
		///
		/// <returns>	The skin to share. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Skin> GetSkinToShare() const { return m_skin; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the skin sizes used by this window to read. </summary>
		///
		/// <returns>	The skin sizes. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiSkinSizes& GetSkinSizes() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a dragging. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetDragging(bool set) { m_windowIsBeingDragged = set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the dragging. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetDragging() const { return m_windowIsBeingDragged; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is visible. </summary>
		///
		/// <returns>	True if visible, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsVisible() const { return (m_state != EWindowState::eClosed); }

		//__declspec(noinline)

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is enabled. </summary>
		///
		/// <returns>	True if enabled, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsEnabled() const 
		{ 
			if (m_state == EWindowState::eOpen || m_state == EWindowState::eEditorDisabled || m_state==EWindowState::eMinimised) 
				return true; 
			return false; 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name. </summary>
		///
		/// <returns>	The name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetName() const { return m_name; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets local rectangle. </summary>
		///
		/// <returns>	The local rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetLocalRect() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets screen rectangle. </summary>
		///
		/// <returns>	The screen rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetScreenRect() const { return WindowRectToScreenRect(GetLocalRect()); }		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets local client rectangle. </summary>
		///
		/// <returns>	The local client rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const RectangleI& GetLocalClientRect() const { return m_clientRect; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets screen space client rectangle. </summary>
		///
		/// <returns>	The screen space client rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetScreenSpaceClientRect() const { return WindowRectToScreenRect(GetLocalClientRect()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets local menu area rectangle. </summary>
		///
		/// <returns>	The local menu area rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetLocalMenuAreaRect() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Covers screen point. </summary>
		///
		/// <param name="screenPoint">	The screen point. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CoversScreenPoint(VectorI screenPoint) const { 
			return GetScreenRect().Contains(screenPoint); 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets screen position. </summary>
		///
		/// <param name="newPos">	The new position. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetScreenPosition(VectorI newPos){ m_windowPos = newPos; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets screen position. </summary>
		///
		/// <returns>	The screen position. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetScreenPosition() const {return m_windowPos;}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for the first object at screen point. </summary>
		///
		/// <param name="screenPoint">	The screen point. </param>
		///
		/// <returns>	The found object at screen point. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string FindObjectAtScreenPoint(VectorI screenPoint) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for the first object at window point. </summary>
		///
		/// <param name="windowPoint">	The window point. </param>
		///
		/// <returns>	The found object at window point. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string FindObjectAtWindowPoint(VectorI windowPoint) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scaling is dependant on feature flag set on creation. </summary>
		///
		/// <param name="newWidth"> 	Width of the new. </param>
		/// <param name="newHeight">	Height of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Resize(int newWidth,int newHeight);

		/// <summary>	Size to fit contents. </summary>
		void SizeToFitContents();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Will return false if not possible e.g. window already has an object with that name.
		/// </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		/// <param name="newName">   	Name of the new. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeObjectName(const std::string& objectName, const std::string& newName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Get object pointer by name, can supress error message, returns nullptr if not found.
		/// </summary>
		///
		/// <param name="objectName"> 	Name of the object. </param>
		/// <param name="reportError">	(Optional) True to report error. </param>
		///
		/// <returns>	Null if it fails, else the object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObject* GetObject(const std::string& objectName, bool reportError = true) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Get object pointer by user id, can supress error message, returns nullptr if not found.
		/// </summary>
		///
		/// <param name="userId">	  	Identifier for the user. </param>
		/// <param name="reportError">	(Optional) True to report error. </param>
		///
		/// <returns>	Null if it fails, else the object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObject* GetObject(int userId, bool reportError = true) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Makes a copy of an object If duplicateName empty OR duplicateName already exists a new name
		/// is creasted from original name (number appended)
		/// Note: you will want to change its position afterwards otherwise it will appear on the top of
		/// the old one Returns null if fails.
		/// </summary>
		///
		/// <param name="objectName">   	Name of the object. </param>
		/// <param name="duplicateName">	(Optional) Name of the duplicate. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObject. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObject* DuplicateObject(const std::string& objectName, std::string duplicateName = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructs a menu. If type is drop down first item is the menu title If inMenuBar is true the
		/// y position is set automatically to menu bar but x is left to user.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="menuItems">			The menu items. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="inMenuBar">			(Optional) True to in menu bar. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectMenu. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectMenu* AddMenu(std::string name, const std::vector<std::string>& menuItems, EMenuType type = EMenuType::eDropDown, bool inMenuBar=true, int textSizeIncrement=0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructs a button from scratch When button type is one with text to left or right, size of
		/// button is determined by bounds width.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="text">					The text. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectButton. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectButton* AddButton(std::string name, std::string text, EButtonType type = EButtonType::eToggle, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructs a list box. Width is determined from widest item provided unless a numCharsWide is
		/// specified (!=0)
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="numVisibleRows">   	Number of visible rows. </param>
		/// <param name="items">				The items. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="numCharsWide">			(Optional) Number of characters wides. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a list of. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectListBox* AddListBox(std::string name, int numVisibleRows, std::vector<std::string> items, 
			EListBoxType type=EListBoxType::eNormal, int numCharsWide=0, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Constructs a canvas to rect size Can optionally provide a sprite to render to canvas before
		/// user renders.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="rect">					The rectangle. </param>
		/// <param name="sprite">				(Optional) The sprite. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to the UIObjectCanvas. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a radio buttons. </summary>
		///
		/// <param name="nameAndText">			The name and text. </param>
		/// <param name="relativeToObject"> 	The relative to object. </param>
		/// <param name="positioning">			The positioning. </param>
		/// <param name="maxInRow">				The maximum in row. </param>
		/// <param name="alignment">			(Optional) The alignment. </param>
		/// <param name="userIdStart">			(Optional) The user identifier start. </param>
		/// <param name="initiallyVisible"> 	(Optional) True to show, false to hide the initially. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		/// <param name="setAsCollection">  	(Optional) True to set as collection. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AddRadioButtons(const std::vector<std::pair<std::string, std::string>>& nameAndText,
			const std::string& relativeToObject, EDirection positioning, int maxInRow,
			EAlignment alignment = EAlignment::eAlignMinToMin,const int * const userIdStart = nullptr,
			bool initiallyVisible=true, EButtonType type=EButtonType::eRadio, int textSizeIncrement = 0, bool setAsCollection=true);

		/*
		A helper that does a few common tasks:
		Ceates the toggle buttons and sets them all to the same size
		Fills them into rows
		Optionally assigns an id range
		nameAndText is a pair , one for the id name and one for text on the button
		*/

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds the buttons. </summary>
		///
		/// <param name="nameAndText">			The name and text. </param>
		/// <param name="relativeToObject"> 	The relative to object. </param>
		/// <param name="positioning">			Positioning to relative. </param>
		/// <param name="maxInRow">				The maximum in row. </param>
		/// <param name="alignment">			(Optional) The alignment. </param>
		/// <param name="userIdStart">			[in,out] (Optional) If non-null, the user identifier
		/// 									start. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AddButtons(const std::vector<std::pair<std::string, std::string>>& nameAndText,
			const std::string& relativeToObject, EDirection positioning, int maxInRow,
			EAlignment alignment = EAlignment::eAlignMinToMin, int *userIdStart = nullptr, int textSizeIncrement = 0);

		/*
		A helper that adds a text entry and a label to go with it
		Assumes text entry to right of label and 
		Positioning refers to position of label
		*/

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a text entry and label. </summary>
		///
		/// <param name="textEntryName">		Name of the text entry. </param>
		/// <param name="labelName">			Name of the label. </param>
		/// <param name="labelText">			The label text. </param>
		/// <param name="maxVisibleChars">  	The maximum visible characters. </param>
		/// <param name="relativeToObject"> 	The relative to object. </param>
		/// <param name="positioning">			The positioning. </param>
		/// <param name="alignment">			(Optional) The alignment. </param>
		/// <param name="validatingLambda"> 	(Optional) The validating lambda. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AddTextEntryAndLabel(std::string textEntryName, std::string labelName, std::string labelText,
			int maxVisibleChars,
			const std::string& relativeToObject, EDirection positioning,
			EAlignment alignment = EAlignment::eAlignMinToMin,
			ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a text line that can be static or clickable. Uses the current UI style skin.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="text">					The text. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectText. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectText* AddText(std::string name, std::string text, ETextType type = ETextType::eStatic, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a text line that can be static or clickable. This one allows text colour and size
		/// increment to be altered. SizeIncrement is offset from settings point size.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="text">					The text. </param>
		/// <param name="textColour">			The text colour. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectText. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectText* AddText(std::string name, std::string text, Colour255 textColour, ETextType type = ETextType::eStatic, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a text line that can be static or clickable. This one allows all text style settings to
		/// be overriden from the defaults. Remember to set correct inherit flags in the style.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="text">					The text. </param>
		/// <param name="style">				The style. </param>
		/// <param name="type">					(Optional) The type. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectText. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectText* AddText(std::string name, std::string text, UiTextStyleGroup style, ETextType type = ETextType::eStatic,int textSizeIncrement = 0);

		// Adds a text entry box with one of the provided lambdas to set dimensions
		//UIObjectTextEntry* AddTextEntry(std::string name, RectangleI dimensions,ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a text entry box with one of the provided lambdas to dimensions calculated from num
		/// chars.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="maxVisibleChars">  	The maximum visible characters. </param>
		/// <param name="validatingLambda"> 	(Optional) The validating lambda. </param>
		/// <param name="isFloat">				(Optional) True if is float, false if not. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectTextEntry. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectTextEntry* AddTextEntry(std::string name, int maxVisibleChars, ETextEntryLambdas validatingLambda = ETextEntryLambdas::eEverything, bool isFloat=false, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a text entry box with a user supplied text validating lambda Note: if used it cannot be
		/// saved in XML so will need post-applying after a load.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="maxVisibleChars">  	The maximum visible characters. </param>
		/// <param name="validateLambda">   	The validate lambda. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectTextEntry. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectTextEntry* AddTextEntry(std::string name, int maxVisibleChars, std::function<bool(char newChar, const std::string& newText)> validateLambda, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a dial which takes its dimensions from the rectangle. </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="dimensions">			The dimensions. </param>
		/// <param name="layout">				The layout. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectDial. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectDial* AddDial(std::string name, RectangleI dimensions, DialLayout layout, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds a slider which takes its dimensions from the rectangle and orientation.
		/// </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="dimensions">			The dimensions. </param>
		/// <param name="layout">				The layout. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectSlider. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectSlider* AddSlider(std::string name, RectangleI dimensions, SliderLayout layout, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a shape which is contained in the rectangle. </summary>
		///
		/// <param name="name">					The name. </param>
		/// <param name="dimensions">			The dimensions. </param>
		/// <param name="caption">				The caption. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	Null if it fails, else a pointer to an UIObjectShape. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectShape* AddShape(std::string name, RectangleI dimensions, std::string caption, int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Safe downcasting. If object is not a slider or does not exist, returns nullptr.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the slider. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectSlider* GetSlider(std::string name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Adds scrollbars to this window. They are in the form of sliders but with no shadow and tumb
		/// region stops against the lengths. Adds if object name is not empty.
		/// </summary>
		///
		/// <param name="verticalName">			Name of the vertical. </param>
		/// <param name="horizontalName">   	(Optional) Name of the horizontal. </param>
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AddScrollbar(const std::string& verticalName, const std::string& horizontalName=std::string(), int textSizeIncrement = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Sets window space position of an object directly. Best to let it be aligned to grid.
		/// </summary>
		///
		/// <param name="objectToPositionName">	Name of the object to position. </param>
		/// <param name="pos">				   	The position. </param>
		/// <param name="alignToGrid">		   	(Optional) True to align to grid. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionObject(const std::string& objectToPositionName, VectorI pos, bool alignToGrid = true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Applies window offset to object window position. </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		///
		/// <returns>	The object position screen space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetObjectPositionScreenSpace(const std::string& objectName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shortcut to getting an objects screen space bounds. </summary>
		///
		/// <param name="objectName">	Name of the object. </param>
		///
		/// <returns>	The object bounds screen space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetObjectBoundsScreenSpace(const std::string& objectName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Useful for lining up menus in a window menu bar Window must have been created with menu
		/// feature flag.
		/// </summary>
		///
		/// <param name="objects">	The objects. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionObjectsInMenuBar(const std::vector<std::string> &objects);	

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Used ot position one menu in a menu bar. Position relates to any other menus.
		/// </summary>
		///
		/// <param name="menuName">	Name of the menu. </param>
		/// <param name="position">	The position. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionMenuInMenuBar(const std::string& menuName, int position);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Position first object relative to second object Note: if positioning direction is a diagonal
		/// then alignment is ignored translate is in terms of object gaps not actual pixels.
		/// </summary>
		///
		/// <param name="objectToPositionName">	Name of the object to position. </param>
		/// <param name="relativeToObject">	   	The relative to object. </param>
		/// <param name="positioning">		   	The positioning. </param>
		/// <param name="alignment">		   	(Optional) The alignment. </param>
		/// <param name="gap">				   	(Optional) The gap. </param>
		/// <param name="translate">		   	(Optional) The translate. </param>
		/// <param name="axis">				   	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionRelativeTo(const std::string& objectToPositionName, const std::string& relativeToObject,EDirection positioning,
			EAlignment alignment = EAlignment::eAlignMinToMin, EGap gap = EGap::eNormal, const VectorI& translate = VectorI(), EAxis axis=EAxis::eBoth);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Quick alignment to another on an axis translate is in terms of object gaps not actual pixels.
		/// </summary>
		///
		/// <param name="objectToPositionName">	Name of the object to position. </param>
		/// <param name="relativeToObject">	   	The relative to object. </param>
		/// <param name="axis">				   	The axis. </param>
		/// <param name="alignment">		   	(Optional) The alignment. </param>
		/// <param name="translate">		   	(Optional) The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void AlignTo(const std::string& objectToPositionName, const std::string& relativeToObject, EAxis axis,
			EAlignment alignment = EAlignment::eAlignMinToMin, const VectorI& translate = VectorI(0));		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Position at the edge (or corner) of this window translate is in terms of object gaps not
		/// actual pixels.
		/// </summary>
		///
		/// <param name="toPositionName">	Name of to position. </param>
		/// <param name="edge">			 	The edge. </param>
		/// <param name="gap">			 	(Optional) The gap. </param>
		/// <param name="translate">	 	(Optional) The translate. </param>
		/// <param name="axis">			 	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionObjectAgainstWindowEdge(const std::string& toPositionName, EDirection edge, 
			EGap gap = EGap::eMargin, const VectorI& translate = VectorI(0), EAxis axis = EAxis::eBoth);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows 1 or more objects to be positioned centrally in a window, axis can be specified Note
		/// that for multiple objects they are positioned as if they were one object with the rectangle
		/// bounding them all being centred.
		/// </summary>
		///
		/// <param name="objects">	The objects. </param>
		/// <param name="axis">   	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionObjectsInWindowCentre(const std::vector<std::string> &objects, EAxis axis = EAxis::eBoth);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// For positioning a window relative to this one Note that windows are not children of other
		/// windows so positioning is in screen space if inside is true edge is the inside edge else
		/// outside edge
		/// E.g. if you say east and inside then it will be positioned inside this window against left
		/// edge But if you say east and !inside then it will be positioned to the left of the left edge
		/// translate is in terms of object gaps not actual pixels.
		/// </summary>
		///
		/// <param name="windowToPositionName">	Name of the window to position. </param>
		/// <param name="edge">				   	The edge. </param>
		/// <param name="inside">			   	(Optional) True to inside. </param>
		/// <param name="gap">				   	(Optional) The gap. </param>
		/// <param name="translate">		   	(Optional) The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void PositionWindowAgainstWindowEdge(const std::string& windowToPositionName, EDirection edge, 
			bool inside=false, EGap gap = EGap::eNormal, const VectorI& translate = VectorI(0));

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Alters one edge of an object to match an edge of another object Directions here can only be 1
		/// of the four primary ones whichEdge is the edge to alter, toWhichEdge is the edge of the
		/// relative object This effectivley sizes an object. Note that some objects may alter their own
		/// size to a minimum.
		/// </summary>
		///
		/// <param name="objectToSetName"> 	Name of the object to set. </param>
		/// <param name="relativeToObject">	The relative to object. </param>
		/// <param name="whichEdge">	   	The which edge. </param>
		/// <param name="toWhichEdge">	   	to which edge. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetEdgeTo(const std::string& objectToSetName, const std::string& relativeToObject, EDirection whichEdge, EDirection toWhichEdge);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Sets the size of objectToSizeName to be the same as relativeToObject width and height can be
		/// optionally set.
		/// </summary>
		///
		/// <param name="objectToSizeName">	Name of the object to size. </param>
		/// <param name="relativeToObject">	The relative to object. </param>
		/// <param name="axis">			   	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SizeTo(const std::string& objectToSizeName, const std::string& relativeToObject, EAxis axis=EAxis::eBoth);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows a group of objects to be sized to the largest dimension of any adjust is in terms of
		/// object gaps not actual pixels and is added to size.
		/// </summary>
		///
		/// <param name="objectsToSize">	Size of the objects to. </param>
		/// <param name="axis">				(Optional) The axis. </param>
		/// <param name="adjust">			(Optional) The adjust. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SizeToLargest(const std::vector<std::string> &objectsToSize, EAxis axis = EAxis::eBoth, VectorI adjust = VectorI());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows a group of objects determined by tag to be sized to the largest dimension of any.
		/// </summary>
		///
		/// <param name="tag"> 	The tag. </param>
		/// <param name="axis">	(Optional) The axis. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SizeToLargestTag(const std::string &tag, EAxis axis = EAxis::eBoth);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows a group of buttons to behave like radio buttons. </summary>
		///
		/// <param name="buttonVector">	The button vector. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAsRadioButtonCollection(const std::vector<std::string>& buttonVector);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Render this window to the surface. </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Render(std::shared_ptr<Surface> &renderSurface);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Called when HAPI detects a key event (via UI)
		/// Return false if not handled Additionally passed an input action that may have been detected
		/// from mappings.
		/// </summary>
		///
		/// <param name="keyEvent">  	The key event. </param>
		/// <param name="keyCode">   	The key code. </param>
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleKeyEvent(EKeyEvent keyEvent, BYTE keyCode, EInputMappingAction action, EInputMappingAction lastAction);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Return false if not handled Additionally passed an input action that may have been detected
		/// from mappings.
		/// </summary>
		///
		/// <param name="mouseData"> 	Information describing the mouse state. </param>
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleMouseAction(const MouseData& mouseData, EInputMappingAction action, EInputMappingAction lastAction);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Called for a controller event, currently not using (via UI)
		/// Return false if not handled.
		/// </summary>
		///
		/// <param name="controllerId">   	Identifier for the controller. </param>
		/// <param name="controllerEvent">	The controller event. </param>
		/// <param name="buttonIndex">	  	Zero-based index of the button. </param>
		/// <param name="controllerData"> 	Information describing the controller state. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Called with interpreted key presses into character entry (via UI)
		/// Return false if not handled.
		/// </summary>
		///
		/// <param name="enteredText">	The entered text. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleTextEntered(char enteredText);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Tag added to each object that can be used in global operations, e.g. turn off, turn on so
		/// useful for a screen Returns old tag.
		/// </summary>
		///
		/// <param name="tag">	The tag. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string SetCurrentTag(std::string tag) { std::string oldTag{ m_currentTag }; m_currentTag = tag; return oldTag; }
		/// <summary>	Clears the current tag. </summary>
		void ClearCurrentTag() { m_currentTag = std::string(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets all names of objects with tag. </summary>
		///
		/// <param name="tag">	The tag. </param>
		///
		/// <returns>	all names of objects with tag. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::string> GetAllNamesOfObjectsWithTag(const std::string& tag) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets title text. </summary>
		///
		/// <param name="newText">	The new text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTitleText(const std::string& newText);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets tagge objects visibility. </summary>
		///
		/// <param name="tag">	The tag. </param>
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTaggeObjectsVisibility(const std::string& tag, bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Only call if you have completely finished with them. </summary>
		///
		/// <param name="tag">	The tag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DeleteAllObjectsWithTag(const std::string& tag);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Only call if you have completely finished with it Otherwise just set its visibility to false
		/// to disable it.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DeleteObject(const std::string& name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves this Window to XML. </summary>
		///
		/// <param name="windowNode">	[in,out] If non-null, the window node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteXML(CHapiXMLNode *windowNode) const;

		/*
			Saves all the object names to a text file for easy reference when building the UI backend code
			Uses current working directory and window name to save to, can optionally open in notepad
		*/

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an object names. </summary>
		///
		/// <param name="openInNotepad">	(Optional) True to open in notepad. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveObjectNames(bool openInNotepad = false) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Simulate button press. </summary>
		///
		/// <param name="buttonName"> 	Name of the button. </param>
		/// <param name="alsoRelease">	(Optional) True to also release. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SimulateButtonPress(const std::string& buttonName, bool alsoRelease = true);
	};
}