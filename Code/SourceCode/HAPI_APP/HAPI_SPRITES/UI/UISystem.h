#pragma once
/*
	Simple UI  library providing
		Windows
		Buttons (radio and normal)
		Static and clickable text
		Text entry
		Menus
		Dial
		Sliders and scroll bars
		Shape
	Supports saving to XML along with resolution scaling
*/

// If you use the UI you must get access to these

//#include "UI\UITypes.h"
//#include "UI\UISkin.h"
//#include "UI\UIObject.h"
//#include "UI\UIWindow.h"
//#include "UI\UIInputMappings.h"

//#define UI_VERBOSE

namespace HAPI_UI_SPACE
{	
	class UICommonDialogs;

	// Basic UI
	class UISystem : public IHapiSpritesInputListener
	{
	private:
		ListenerList m_listeners; // Runtime
		RectangleI m_screenRect; // Runtime
		std::string m_dataDirectory; // Runtime

		EInputMappingAction m_lastAction{ EInputMappingAction::eUnmapped };

		// Lassoo
		bool m_showLassoo{ false };

		// Done as two points to ease growing / shrinking. Point 0 is start, point 1 is end
		VectorI m_lasoo[2];
		Colour255 m_lassooColour{ Colour255::CYAN };
		
		// Hard coded skin, cannot be edited
		// Must be shared as used in multiple places
		std::shared_ptr<Skin> m_defaultSkin;

		// Common dialogs helper - created on demand so dont access directly
		// Call GetCommonDialogs
		std::shared_ptr<UICommonDialogs> m_commonDialogs;		

		// Input mappings to UI actions
		std::unique_ptr<UIInputMappings> m_inputMappings;

		// All the skins
		std::map<std::string, std::shared_ptr<Skin>> m_skins;

		// All windows sorted from furthest away to closest
		std::map<int, std::shared_ptr<UIWindow>> m_windowOrder;

		// Name of window with focus or empty if none
		std::string m_windowWithFocus;

		// If not empty nobody else gets any action
		std::string m_modalWindowOpen;

		// Which window is under screen point? Returns empty string if none.
		std::string FindWindowAtScreenPoint(VectorI screenPoint) const;

		// Current state of the mouse held for direct access
		MouseData m_mouseData;				

		// Created on demand - a transparent pattern
		std::shared_ptr<PatternFill> m_transparentPattern;

		// Gets the key one after the last window's in the order vecto's key
		int GetNextWindowOrder() const;
	
		// Add listener to get object events
		// You do not need to call this directly as the listener does it automatically
		// But do call UI_AddWindowToListenTo for all windows to listen to (Default is to gfet main window events only)
		friend class IHapiSpritesUIListener;
		void AddListener(IHapiSpritesUIListener& newListener);

		void OnMouseEvent(const MouseData& mouseData, EInputMappingAction action);		

		// Created on demand factory to create common dialogs - for internal use
		std::shared_ptr<UICommonDialogs> GetCommonDialogs();

		void FocusWindowHandleMouseAction(const MouseData& mouseData, EInputMappingAction action);

		friend class CHapiSpritesCore;
		// Cant be in constructor as needs to call fns requiring a valid this
		bool Initialise();

		// From 0.80 only available internally
		// Render all the active UI objects to the surface
		void Render(std::shared_ptr<Surface> &renderSurface);

		// Adds a skin to the XML tree, under rootNode
		bool SaveSkin(const std::string& skinName, CHapiXMLNode *rootNode) const;
	public:			
		// Must be created with screen size
		UISystem(int screenWidth, int screenHeight);
		~UISystem();

		// Created on demand first time - a transparent pattern
		std::shared_ptr<PatternFill> GetTransparentPatternShader();
		
		// Useful when debugging
		void ClearInputMappingValues() { m_inputMappings->Clear(); }

		// Note: can't get default skin this way, instead use GetDefaultSkin
		std::shared_ptr<Skin> GetSkin(const std::string& name) const;		

		// Provides direct access to latest mouse data
		const MouseData& GetMouseData() const { return m_mouseData; }

		// Helper, get mouse position in screen space
		VectorI GetMousePosition() const { return VectorI(m_mouseData.x, m_mouseData.y); }
		
		// Change focus window and object, leave object blank to use old one
		void ChangeFocus(const std::string &windowName, const std::string &newFocusObject=std::string());

		// A refresh causes all styles to cascade plus a recreation of all object graphics
		void RefreshAllWindows();

		// Lassoo functions

		void ShowLassoo(RectangleI rect, Colour255 colour)
		{
			m_showLassoo = true;
			m_lassooColour = colour;
			m_lasoo[0] = rect.TopLeft();
			m_lasoo[1] = rect.BottomRight();
		}

		void HideLassoo() { m_showLassoo = false; }

		RectangleI GetLassooRect() const {
			return RectangleI{
				std::min(m_lasoo[0].x,m_lasoo[1].x),
				std::max(m_lasoo[0].x,m_lasoo[1].x),
				std::min(m_lasoo[0].y,m_lasoo[1].y),
				std::max(m_lasoo[0].y,m_lasoo[1].y) };
		}

		void ExpandLassoo(const VectorI &pnt)
		{
			m_lasoo[1] = pnt;
		}

		// Displays a modal message box. Function only returns when it is closed.
		// Can optionally get info on button pressed via userResponse
		void MessageBox(const std::string& text, const std::string& title,
			HAPI_ButtonType buttonType = HAPI_ButtonType::eButtonTypeOk, HAPI_UserResponse *userResponse = 0);

		// Safe renaming of skin
		// Note: you cannot rename the default skin
		bool RenameSkin(const std::string& oldSkinName, const std::string& newSkinName);

		// Safe renaming of window
		bool RenameWindow(const std::string& oldWindowName, const std::string& newWindowName);

		// Saves skin to file
		// Note you cannot change the filename, it must match the skin name for later window loads to work
		// but you can provide the path. If left empty uses current working directory.
		bool SaveSkin(const std::string& skinName, const std::string& path=std::string());

		// Loads skin from a file into the UI skin collection
		// Returns skin name or empty string on error
		std::string LoadSkin(const std::string& filename);

		// Loads skin from xml tree and inserts in collection. DOES NOT overwrite any with the same name
		// Returns skin name or empty string on error
		std::string LoadSkin(CHapiXML &xml, CHapiXMLNode *skinNode,bool overwriteExisting=false);

		// Make a copy of a skin and adds to collection and returns it
		// Note: the newSkinName must be unique, if it is not it will be made unique so check
		std::shared_ptr<Skin> CopySkin(const std::string& nameOfskinToCopy, std::string newSkinName);

		// Creates a skin using default skin settings, and adds to collection under 'skinName'
		// If already have skin with that name will fail unless overwriteExisting is true
		bool CreateSkin(const std::string& skinName, bool overwriteExisting=false);

		// Always have one fixed skin
		static std::string GetDefaultSkinName() { return "SYS_Default_Skin"; }
		
		// Always have one main window that is used for default things
		// It is the size of the screen and is behind everything else (order=0)
		static std::string GetDefaultWindowName() { return "SYS_Default_Window"; }

		std::shared_ptr<Skin> GetDefaultSkin() const { return m_defaultSkin; }

		// Singleton instance
		//static UISystem& GetInstance() { assert(UISystem::kSingleton); return *UISystem::kSingleton; }

		// Get a particular window by name
		std::shared_ptr<UIWindow> GetWindow(const std::string& windowName, bool reportError=true) const;

		// The default 
		std::shared_ptr<UIWindow> GetDefaultWindow() const { return GetWindow(GetDefaultWindowName()); }

		// Get the main window (always exists)
		std::shared_ptr<UIWindow> MainWindow() const { return GetWindow(UISystem::GetDefaultWindowName()); }

		// Alters window order. Back is last to get clicks etc.
		void SendWindowToBack(const std::string &windowName);

		// Screen width and height
		RectangleI GetScreenRect() const { return m_screenRect; }

		// Data directory is norally working directory + //Data
		const std::string& GetDataDirectory() const { return m_dataDirectory; }

		// Can change working direction
		void ChangeDataDirectory(std::string newDir);

		// Constants - could go in data some time TODO
		constexpr DWORD GetCursorFlashTime() const noexcept { return 600; }
		constexpr unsigned int GetKeypressDelay() const noexcept { return 400; } // was 100 but was too quick

		// Since the UI editor and skin editor are built in, the user could be running with any resolution
		// This can mean the editor windows are far too big so need to detect and alter accordingly
		// We need a specific skin for the editor windows anyway since user may have changed the default
		void CreateEditorSkin(int screenWidth, int screenHeight);

		// Open window, can optionally open modally
		// Can also optionally position relative to another window
		bool OpenWindow(const std::string& windowName,bool openModally=false, bool sendToFront = true,
			std::string relativeToWindow=std::string(),EDirection dir=EDirection::eNorthWest, bool inside=true,
			EGap gap = EGap::eNormal, const VectorI& translate = VectorI(0));

		// Close window (does not delete it)
		bool CloseWindow(const std::string& windowName);

		// Deletes a window
		bool DeleteWindow(const std::string& windowName);

		// Remove listener
		void RemoveInputListener(IHapiSpritesUIListener &listener);

		// Add a window with settings and a new skin
		std::shared_ptr<UIWindow> AddWindow(const std::string& windowName, RectangleI screenRect,
			unsigned int featuresFlags = 0, std::string titleText = std::string(),
			std::shared_ptr<Skin> skin=nullptr);

		// Add a window with settings and an existing skin
		std::shared_ptr<UIWindow> AddWindow(const std::string& windowName, RectangleI screenRect, const std::string& skinName,
			unsigned int featuresFlags = 0, std::string titleText = std::string());

		// Add an already created window
		std::shared_ptr<UIWindow> AddWindow(std::shared_ptr<UIWindow> newWindow);

		// Saves a UI window
		// Note: you may also want to save the window's skin if it is not using the default, otherwise on load - if the skin does not exist
		// a window will revert to using the system skin
		// Can optionally provide user data to be saved alongside
		bool Save(const std::string& xmlFilename, const std::string windowToSaveName,CHapiXMLNode *userData=nullptr) const;

		// Loads a UI window
		// If it uses a user skin and that is not already loaded it will attempt to load it
		// If scale is true (default) and the resolution is different than what the UI window was created to, then it and its skin will be scaled
		// To also load user data must use LoadXML instead of this
		bool Load(const std::string& xmlFilename, std::string &loadedWindowName, bool scale=true);

		// Load via XML
		// If it uses a user skin and that is not already loaded it will attempt to load it
		// Note: the name is fixed but you should provide a path
		bool LoadXML(CHapiXML& xml, std::string &loadedWindowName, const std::string& skinPath, CHapiXMLNode **userData = nullptr, bool scale = true);

		// As above from a byte stream
		bool LoadBS(const std::vector<BYTE> &byteStream, std::string &loadedWindowName, CHapiXMLNode **userData = nullptr, bool scale = true);
		
		// Allows user to know if they should ignore input because UI has an object using it
		bool HasFocus() const;

		// Overrides required by IHapiSpritesInputListener:

		// Called when HAPI detects a key event
		void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
		
		// Called when HAPI detects a mouse event other than mouse move
		void OnMouseEvent(EMouseEvent mouseEvent, const MouseData& mouseData) override final;

		// Called when HAPI detects a mouse move
		void OnMouseMove(const MouseData& mouseData)  override final;

		// Called when HAPI detects a controller event, currently not using
		void OnControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData)  override final {}

		// Called with interpreted key presses into character entry
		void OnTextEntered(char enteredText) override final;

		// These handle calls to IHapiSpritesUIListener and map to each function
		void TellListener_MenuItemClicked(const UIObject& object, const std::string& item);
		void TellListener_ButtonPressed(const UIObject& object);
		void TellListener_SliderValueChanged(const UIObject& object, float newValue);
		void TellListener_ListBoxItemPressed(const UIObject& object,int itemIndex);
		void TellListener_RadioButtonChangeState(const UIObject& object, bool pressed);
		bool TellListener_TextClicked(const UIObject& object);
		bool TellListener_CanvasClicked(const UIObject& object, VectorI mousePos);
		void TellListener_TextEntryChanged(const UIObject& object, const std::string& newText, bool beingEdited);		
		void TellListener_WindowChangeState(UIWindow& window, EWindowState newState);
		void TellListener_WindowDraggedSized(UIWindow& window, bool beingDragged);
		void TellListener_MouseClickedOnObject(UIWindow& window, const std::string& objectName, EMouseButton which);
		void TellListener_Render(UIWindow& window, std::shared_ptr<Surface> &renderSurface, const std::string& callerName);
		void TellListener_DialChanged(const UIObject& object, float ratio);
		void TellListener_UnhandledAction(UIWindow& window, EInputMappingAction action,EInputMappingAction lastAction);		
	};
}
