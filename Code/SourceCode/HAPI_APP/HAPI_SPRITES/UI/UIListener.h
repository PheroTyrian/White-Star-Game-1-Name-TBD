#pragma once

namespace HAPI_UI_SPACE
{
	class UIWindow;
	class ListenerPimpl;

	enum class EWindowState;
	enum class EMouseButton;
	enum class EInputMappingAction;

	// Notification events
	enum class EUINotification
	{
		eGainedFocus,
		eLostFocus		
	};

	// An interface that can be used to receive UI notifications
	class IHapiSpritesUIListener
	{
	public:
		IHapiSpritesUIListener();
		virtual ~IHapiSpritesUIListener();
		std::unique_ptr<ListenerPimpl> m_pimpl;

		// By default a listener gets main window events, for others you need to specify
		void UI_AddWindowToListenTo(const std::string& windowName);

		// Can also remove a window from listening to
		bool UI_RemoveWindowToListenTo(const std::string& windowName);

		// Allows for turning on and off getting event messages altogether
		virtual bool UI_WantsEvents() const { return true; }

		// These will provide a user id if one has been found. Note that you should not keep the pointer it is only valid for this call

		// Menu menuName in window has had menu item item clicked on
		virtual void UI_MenuItemClicked(UIWindow& window, const std::string& menuName, const std::string& item, int* userId = nullptr) {}
		// Button buttonName in window has been clicked on
		virtual void UI_ButtonPressed(UIWindow& window, const std::string& buttonName, int* userId = nullptr) {}
		// Radio button buttonName in window has changed state between pressed or unpressed
		virtual void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId = nullptr) {}
		// List box listBoxName in window has had itemIndex item clicked on
		virtual void UI_ListBoxItemPressed(UIWindow& window, const std::string& listBoxName, int itemIndex, int* userId = nullptr) {}
		// Slider sliderName in window has a new value (0-1)
		virtual void UI_SliderValueChanged(UIWindow& window, const std::string& sliderName, float newValue, int* userId = nullptr) {}
		// Clickable text texName in window has been clicked on. Return true if handled.
		virtual bool UI_TextClicked(UIWindow& window, const std::string& textName, int* userId = nullptr) { return true; }
		// Canvas are has been clickd on. Return true if handled. mousePos is in canvas space.
		virtual bool UI_CanvasClicked(UIWindow& window, const std::string& textName, HAPISPACE::VectorI mousePos, int* userId = nullptr) { return true; }
		// A text entry textEntryName in window has been edited with newText. beingEdited is true if user still editing so best wait till goes false.
		virtual void UI_TextEntryChanged(UIWindow& window, const std::string& textEntryName, const std::string& newText, bool beingEdited, int* userId = nullptr) {}
		// The user has rotated dial dialName with new value ratioValue (0 to 1). If beingEdited is true they are still adjusting it.
		virtual void UI_DialChanged(UIWindow& window, const std::string& dialName, float ratioValue, bool beingEdited, int* userId = nullptr) {}

		// Window events

		// The window has changed state
		virtual void UI_WindowChangeState(UIWindow& window, EWindowState newState) {}
		// User is dragging or sizing the window - beingDragged goes false when dragging /sizing stopped.
		virtual void UI_WindowDraggedSized(UIWindow& window, bool beingDragged) {}
		// A UI object objectName has been clicked on with mouse button which. Note: does not mean UI focus has changed
		virtual void UI_MouseClickedOnObject(UIWindow& window, const std::string& objectName, EMouseButton which) {}
		// Only called if window is a UserDraw window (the main window is by default, others are not) and window wants events
		// OR it is a canvas that needs drawing. callerName is either window name or canvas name.
		// Surface has a clip area of the window (or canvas) applied i.e. you can only draw to window.GetScreenRect() area
		virtual void UI_UserDraw(UIWindow& window, std::shared_ptr<HAPISPACE::Surface> &renderSurface, const std::string& callerName) {}
		// Some user action was detected but not handled by any UI object or by a different callback
		virtual void UI_InputAction(UIWindow& window, EInputMappingAction action, EInputMappingAction lastAction) {}
	};

	// typedef for ease as a pain to write
	typedef std::list<std::reference_wrapper<IHapiSpritesUIListener>> ListenerList;
}