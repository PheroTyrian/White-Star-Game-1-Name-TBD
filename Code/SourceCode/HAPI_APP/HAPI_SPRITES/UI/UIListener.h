////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIListener.h
//
// summary:	Declares the listener class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPI_UI_SPACE
{
	class UIWindow;
	class ListenerPimpl;

	enum class EWindowState;
	enum class EMouseButton;
	enum class EInputMappingAction;

	/// <summary>	UI Notification events. </summary>
	enum class EUINotification
	{
		eGainedFocus,
		eLostFocus		
	};

	/// <summary>	An interface that can be used to receive UI notifications. </summary>
	class IHapiSpritesUIListener
	{
	public:
		IHapiSpritesUIListener();
		virtual ~IHapiSpritesUIListener();
		std::unique_ptr<ListenerPimpl> m_pimpl;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// By default a listener gets main window events, for others you need to specify.
		/// </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void UI_AddWindowToListenTo(const std::string& windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Can also remove a window from listening to. </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool UI_RemoveWindowToListenTo(const std::string& windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows for turning on and off getting event messages, from the UI, altogether. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool UI_WantsEvents() const { return true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Menu 'menuName' in window has had menu item item clicked on. Will provide a user id if one
		/// has been found. Note that you should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">  	[in,out] The window. </param>
		/// <param name="menuName">	Name of the menu. </param>
		/// <param name="item">	   	The item. </param>
		/// <param name="userId">  	[in,out] (Optional) If non-null, user identifier for the UI object.. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_MenuItemClicked(UIWindow& window, const std::string& menuName, const std::string& item, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Button buttonName in window has been clicked on. Will provide a user id if one has been
		/// found. Note that you should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="buttonName">	Name of the button. </param>
		/// <param name="userId">	 	[in,out] (Optional) If non-null, user identifier for the UI object.. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_ButtonPressed(UIWindow& window, const std::string& buttonName, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Radio button buttonName in window has changed state between pressed or unpressed.
		/// Will provide a user id if one has been found. Note that you should not keep the pointer it is only valid for this call
		/// </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="buttonName">	Name of the button. </param>
		/// <param name="pressed">   	True if pressed. </param>
		/// <param name="userId">	 	[in,out] (Optional) If non-null, user identifier for the UI object.. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// List box listBoxName in window has had itemIndex item clicked on. Will provide a user id if
		/// one has been found. Note that you should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">	  	[in,out] The window. </param>
		/// <param name="listBoxName">	Name of the list box. </param>
		/// <param name="itemIndex">  	Zero-based index of the item. </param>
		/// <param name="userId">	  	[in,out] (Optional) If non-null, user identifier for the UI
		/// 							object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_ListBoxItemPressed(UIWindow& window, const std::string& listBoxName, int itemIndex, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Slider sliderName in window has a new value (0-1)
		/// Will provide a user id if one has been found. Note that you should not keep the pointer it is
		/// only valid for this call.
		/// </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="sliderName">	Name of the slider. </param>
		/// <param name="newValue">  	The new value. </param>
		/// <param name="userId">	 	[in,out] (Optional) If non-null, user identifier for the UI
		/// 							object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_SliderValueChanged(UIWindow& window, const std::string& sliderName, float newValue, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Clickable text texName in window has been clicked on. Return true if handled.
		/// Will provide a user id if one has been found. Note that you should not keep the pointer it is only valid for this call
		/// </summary>
		///
		/// <param name="window">  	[in,out] The window. </param>
		/// <param name="textName">	Name of the text. </param>
		/// <param name="userId">  	[in,out] (Optional) If non-null, user identifier for the UI object.. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool UI_TextClicked(UIWindow& window, const std::string& textName, int* userId = nullptr) { return true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Canvas area has been clicked on. Return true if handled. Note: mousePos is in canvas space.
		/// Will provide a user id if one has been found. Note that you should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">  	[in,out] The window. </param>
		/// <param name="textName">	Name of the text. </param>
		/// <param name="mousePos">	The mouse position. </param>
		/// <param name="userId">  	[in,out] (Optional) If non-null, user identifier for the UI object.. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool UI_CanvasClicked(UIWindow& window, const std::string& textName, HAPISPACE::VectorI mousePos, int* userId = nullptr) { return true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A text entry textEntryName in 'window' has been edited with newText. beingEdited is true if
		/// user still editing so best wait till goes false. Will provide a user id if one has been
		/// found. Note that you should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">			[in,out] The window. </param>
		/// <param name="textEntryName">	Name of the text entry. </param>
		/// <param name="newText">			The new text. </param>
		/// <param name="beingEdited">  	True if being edited. </param>
		/// <param name="userId">			[in,out] (Optional) If non-null, user identifier for the UI
		/// 								object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_TextEntryChanged(UIWindow& window, const std::string& textEntryName, const std::string& newText, bool beingEdited, int* userId = nullptr) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// The user has rotated dial dialName with new value ratioValue (0 to 1). If beingEdited is true
		/// they are still adjusting it. Will provide a user id if one has been found. Note that you
		/// should not keep the pointer it is only valid for this call.
		/// </summary>
		///
		/// <param name="window">	  	[in,out] The window. </param>
		/// <param name="dialName">   	Name of the dial. </param>
		/// <param name="ratioValue"> 	The ratio value. </param>
		/// <param name="beingEdited">	True if being edited. </param>
		/// <param name="userId">	  	[in,out] (Optional) If non-null, user identifier for the UI
		/// 							object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_DialChanged(UIWindow& window, const std::string& dialName, float ratioValue, bool beingEdited, int* userId = nullptr) {}

		// Window events

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The window has changed state. </summary>
		///
		/// <param name="window">  	[in,out] The window. </param>
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_WindowChangeState(UIWindow& window, EWindowState newState) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// User is dragging or sizing the window - beingDragged goes false when dragging /sizing stopped.
		/// </summary>
		///
		/// <param name="window">	   	[in,out] The window. </param>
		/// <param name="beingDragged">	True if being dragged. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_WindowDraggedSized(UIWindow& window, bool beingDragged) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A UI object objectName has been clicked on with mouse button which. Note: does not mean UI
		/// focus has changed.
		/// </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="objectName">	Name of the object. </param>
		/// <param name="which">	 	Which mouse button. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_MouseClickedOnObject(UIWindow& window, const std::string& objectName, EMouseButton which) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Only called if window is a UserDraw window (the main window is by default, others are not)
		/// and window wants events OR it is a canvas that needs drawing. callerName is either window
		/// name or canvas name. Surface has a clip area of the window (or canvas) applied i.e. you can
		/// only draw to window.GetScreenRect() area.
		/// </summary>
		///
		/// <param name="window">			[in,out] The window. </param>
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="callerName">   	Name of the caller. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_UserDraw(UIWindow& window, std::shared_ptr<HAPISPACE::Surface> &renderSurface, const std::string& callerName) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Some user action was detected but not handled by any UI object or by a different callback.
		/// </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void UI_InputAction(UIWindow& window, EInputMappingAction action, EInputMappingAction lastAction) {}
	};

	/// <summary>	typedef for ease as a pain to write. </summary>
	typedef std::list<std::reference_wrapper<IHapiSpritesUIListener>> ListenerList;
}