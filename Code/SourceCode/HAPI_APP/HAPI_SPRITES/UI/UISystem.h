////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UISystem.h
//
// summary:	Declares the system class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//#define UI_VERBOSE

////////////////////////////////////////////////////////////////////////////////////////////////////
// namespace: HAPI_UI_SPACE
// 
// Simple UI  library providing Windows Buttons(radio and normal)
// Static and click-able text Text entry Menus Dial Sliders and scroll bars Shape Supports saving to
// XML along with resolution scaling summary:	Wraps all of the HAPI Sprites UI system. 
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAPI_UI_SPACE
{	
	class UICommonDialog_MessageBox;
	class UICommonDialog_SpriteViewer;
	class UICommonDialog_MultiChoice;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Basic UI.  Simple UI  library providing Windows Buttons(radio and normal)
	/// Static and click-able text Text entry Menus Dial Sliders and scroll bars Shape Supports
	/// saving to XML along with resolution scaling summary:	Wraps all of the HAPI Sprites UI system.
	/// </summary>
	///
	/// <seealso cref="T:IHapiSpritesInputListener"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UISystem : public IHapiSpritesInputListener
	{
	private:
		/// <summary>	Runtime. </summary>
		ListenerList m_listeners;
		/// <summary>	Runtime. </summary>
		RectangleI m_screenRect;
		/// <summary>	Runtime. </summary>
		std::string m_dataDirectory;

		/// <summary>	The last action. </summary>
		EInputMappingAction m_lastAction{ EInputMappingAction::eUnmapped };

		/// <summary>	Lassoo. </summary>
		bool m_showLassoo{ false };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Done as two points to ease growing / shrinking. Point 0 is start, point 1 is end.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI m_lasoo[2];
		/// <summary>	The lassoo colour. </summary>
		Colour255 m_lassooColour{ Colour255::CYAN };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Hard coded skin, cannot be edited Must be shared as used in multiple places.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Skin> m_defaultSkin;

		/// <summary>	helper - created on demand so dont access directly. </summary>
		std::shared_ptr<UICommonDialog_MessageBox> m_commonDialog_messageBox;		
		/// <summary>	The common dialog sprite viewer. </summary>
		std::shared_ptr<UICommonDialog_SpriteViewer> m_commonDialog_spriteViewer;
		/// <summary>	The common dialog multi choice dialog. </summary>
		std::shared_ptr<UICommonDialog_MultiChoice> m_commonDialog_multiChoice;

		/// <summary>	Input mappings to UI actions. </summary>
		std::unique_ptr<UIInputMappings> m_inputMappings;

		/// <summary>	All the skins. </summary>
		std::map<std::string, std::shared_ptr<Skin>> m_skins;

		/// <summary>	All windows sorted from furthest away to closest. </summary>
		std::map<int, std::shared_ptr<UIWindow>> m_windowOrder;

		/// <summary>	Name of window with focus or empty if none. </summary>
		std::string m_windowWithFocus;

		/// <summary>	If not empty nobody else gets any action. </summary>
		std::string m_modalWindowOpen;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Which window is under screen point? Returns empty string if none. </summary>
		///
		/// <param name="screenPoint">	The screen point. </param>
		///
		/// <returns>	The found window at screen point. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string FindWindowAtScreenPoint(VectorI screenPoint) const;

		/// <summary>	Current state of the mouse held for direct access. </summary>
		MouseData m_mouseData;				

		/// <summary>	Created on demand - a transparent pattern. </summary>
		std::shared_ptr<PatternFill> m_transparentPattern;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the key one after the last window's in the order vecto's key. </summary>
		///
		/// <returns>	The next window order. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNextWindowOrder() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Add listener to get object events You do not need to call this directly as the listener does
		/// it automatically But do call UI_AddWindowToListenTo for all windows to listen to (Default is
		/// to gfet main window events only)
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		friend class IHapiSpritesUIListener;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a listener. </summary>
		///
		/// <param name="newListener">	[in,out] The new listener. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void AddListener(IHapiSpritesUIListener& newListener);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the mouse event action. </summary>
		///
		/// <param name="mouseData">	Information describing the mouse state. </param>
		/// <param name="action">   	The action. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnMouseEvent(const MouseData& mouseData, EInputMappingAction action);		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Created on demand factory to create common dialogs - for internal use. </summary>
		///
		/// <returns>	The common dialog message box. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UICommonDialog_MessageBox> GetCommonDialog_MessageBox();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets common dialog sprite viewer. </summary>
		///
		/// <returns>	The common dialog sprite viewer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UICommonDialog_SpriteViewer> GetCommonDialog_SpriteViewer();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets common dialog multi choice. </summary>
		///
		/// <returns>	The common dialog multi choice. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UICommonDialog_MultiChoice> GetCommonDialog_MultiChoice();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Focus window handle mouse action. </summary>
		///
		/// <param name="mouseData">	Information describing the mouse state. </param>
		/// <param name="action">   	The action. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void FocusWindowHandleMouseAction(const MouseData& mouseData, EInputMappingAction action);

		/// <summary>	A hapi sprites core. </summary>
		friend class CHapiSpritesCore;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cant be in constructor as needs to call fns requiring a valid this. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Initialise();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// From 0.80 only available internally Render all the active UI objects to the surface.
		/// </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Render(std::shared_ptr<Surface> &renderSurface);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a skin to the XML tree, under rootNode. </summary>
		///
		/// <param name="skinName">	Name of the skin. </param>
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveSkin(const std::string& skinName, CHapiXMLNode *rootNode) const;
	public:			
		// Must be created with screen size

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="screenWidth"> 	Width of the screen. </param>
		/// <param name="screenHeight">	Height of the screen. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UISystem(int screenWidth, int screenHeight);
		/// <summary>	Destructor. </summary>
		~UISystem();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Created on demand first time - a transparent pattern. </summary>
		///
		/// <returns>	The transparent pattern shader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<PatternFill> GetTransparentPatternShader();
		
		/// <summary>	Useful when debugging. </summary>
		void ClearInputMappingValues() { m_inputMappings->Clear(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Note: can't get default skin this way, instead use GetDefaultSkin. </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	The skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Skin> GetSkin(const std::string& name) const;		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Provides direct access to latest mouse data. </summary>
		///
		/// <returns>	The mouse data. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const MouseData& GetMouseData() const { return m_mouseData; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Helper, get mouse position in screen space. </summary>
		///
		/// <returns>	The mouse position. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetMousePosition() const { return VectorI(m_mouseData.x, m_mouseData.y); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change focus window and object, leave object blank to use old one. </summary>
		///
		/// <param name="windowName">	 	Name of the window. </param>
		/// <param name="newFocusObject">	(Optional) The new focus object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeFocus(const std::string &windowName, const std::string &newFocusObject=std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A refresh causes all styles to cascade plus a recreation of all object graphics.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RefreshAllWindows();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Lassoo functions. </summary>
		///
		/// <param name="rect">  	The rectangle. </param>
		/// <param name="colour">	The colour. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ShowLassoo(RectangleI rect, Colour255 colour)
		{
			m_showLassoo = true;
			m_lassooColour = colour;
			m_lasoo[0] = rect.TopLeft();
			m_lasoo[1] = rect.BottomRight();
		}

		/// <summary>	Hides the lassoo. </summary>
		void HideLassoo() { m_showLassoo = false; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets lassoo rectangle. </summary>
		///
		/// <returns>	The lassoo rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetLassooRect() const {
			return RectangleI{
				std::min(m_lasoo[0].x,m_lasoo[1].x),
				std::max(m_lasoo[0].x,m_lasoo[1].x),
				std::min(m_lasoo[0].y,m_lasoo[1].y),
				std::max(m_lasoo[0].y,m_lasoo[1].y) };
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Expand lassoo. </summary>
		///
		/// <param name="pnt">	The point. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ExpandLassoo(const VectorI &pnt)
		{
			m_lasoo[1] = pnt;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Displays a modal message box. Function only returns when it is closed. Can optionally get
		/// info on button pressed via userResponse.
		/// </summary>
		///
		/// <param name="text">		   	The text. </param>
		/// <param name="title">	   	The title. </param>
		/// <param name="buttonType">  	(Optional) Type of the button. </param>
		/// <param name="userResponse">	[in,out] (Optional) If non-null, the user response. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void MessageBox(const std::string& text, const std::string& title,
			HAPI_ButtonType buttonType = HAPI_ButtonType::eButtonTypeOk, HAPI_UserResponse *userResponse = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows the viewing of a sprite and animation etc. TODO: add to documentation.
		/// </summary>
		///
		/// <param name="sprite">	 	The sprite. </param>
		/// <param name="startSet">  	(Optional) The start set. </param>
		/// <param name="startFrame">	(Optional) The start frame. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SpriteViewer(std::shared_ptr<Sprite> sprite, const std::string& startSet = std::string(), int startFrame = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Multi choice dialog with a set of buttons </summary>
		///
		/// <param name="choices">	The button choices. </param>
		/// <param name="title">  	The title. </param>
		///
		/// <returns>	A std::string with the selected choice. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string MultiChoiceDialog(const std::vector<std::string>& choices, const std::string& title);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Safe renaming of skin Note: you cannot rename the default skin. </summary>
		///
		/// <param name="oldSkinName">	Name of the old skin. </param>
		/// <param name="newSkinName">	Name of the new skin. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool RenameSkin(const std::string& oldSkinName, const std::string& newSkinName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Safe renaming of window. </summary>
		///
		/// <param name="oldWindowName">	Name of the old window. </param>
		/// <param name="newWindowName">	Name of the new window. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool RenameWindow(const std::string& oldWindowName, const std::string& newWindowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Saves skin to file Note you cannot change the filename, it must match the skin name for later
		/// window loads to work but you can provide the path. If left empty uses current working
		/// directory.
		/// </summary>
		///
		/// <param name="skinName">	Name of the skin. </param>
		/// <param name="path">	   	(Optional) Full pathname of the file. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveSkin(const std::string& skinName, const std::string& path=std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Loads skin from a file into the UI skin collection Returns skin name or empty string on error.
		/// </summary>
		///
		/// <param name="filename">	Filename of the file. </param>
		///
		/// <returns>	The skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string LoadSkin(const std::string& filename);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Loads skin from xml tree and inserts in collection. DOES NOT overwrite any with the same name
		/// Returns skin name or empty string on error.
		/// </summary>
		///
		/// <param name="xml">					[in,out] The XML. </param>
		/// <param name="skinNode">				[in,out] If non-null, the skin node. </param>
		/// <param name="overwriteExisting">	(Optional) True to overwrite, false to preserve the
		/// 									existing. </param>
		///
		/// <returns>	The skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string LoadSkin(CHapiXML &xml, CHapiXMLNode *skinNode,bool overwriteExisting=false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Make a copy of a skin and adds to collection and returns it Note: the newSkinName must be
		/// unique, if it is not it will be made unique so check.
		/// </summary>
		///
		/// <param name="nameOfskinToCopy">	The name ofskin to copy. </param>
		/// <param name="newSkinName">	   	Name of the new skin. </param>
		///
		/// <returns>	A std::shared_ptr&lt;Skin&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Skin> CopySkin(const std::string& nameOfskinToCopy, std::string newSkinName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Creates a skin using default skin settings, and adds to collection under 'skinName' If
		/// already have skin with that name will fail unless overwriteExisting is true.
		/// </summary>
		///
		/// <param name="skinName">				Name of the skin. </param>
		/// <param name="overwriteExisting">	(Optional) True to overwrite, false to preserve the
		/// 									existing. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CreateSkin(const std::string& skinName, bool overwriteExisting=false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Always have one fixed skin. </summary>
		///
		/// <returns>	The default skin name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static std::string GetDefaultSkinName() { return "SYS_Default_Skin"; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Always have one main window that is used for default things It is the size of the screen and
		/// is behind everything else (order=0)
		/// </summary>
		///
		/// <returns>	The default window name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static std::string GetDefaultWindowName() { return "SYS_Default_Window"; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets default skin. </summary>
		///
		/// <returns>	The default skin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Skin> GetDefaultSkin() const { return m_defaultSkin; }

		// Singleton instance
		//static UISystem& GetInstance() { assert(UISystem::kSingleton); return *UISystem::kSingleton; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get a particular window by name. </summary>
		///
		/// <param name="windowName"> 	Name of the window. </param>
		/// <param name="reportError">	(Optional) True to report error. </param>
		///
		/// <returns>	The window. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> GetWindow(const std::string& windowName, bool reportError=true) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The default. </summary>
		///
		/// <returns>	The default window. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> GetDefaultWindow() const { return GetWindow(GetDefaultWindowName()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the main window (always exists) </summary>
		///
		/// <returns>	A std::shared_ptr&lt;UIWindow&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> MainWindow() const { return GetWindow(UISystem::GetDefaultWindowName()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Alters window order. Back is last to get clicks etc. </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SendWindowToBack(const std::string &windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Screen width and height. </summary>
		///
		/// <returns>	The screen rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetScreenRect() const { return m_screenRect; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Data directory is norally working directory + //Data. </summary>
		///
		/// <returns>	The data directory. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetDataDirectory() const { return m_dataDirectory; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Can change working direction. </summary>
		///
		/// <param name="newDir">	The new dir. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeDataDirectory(std::string newDir);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constants - could go in data some time TODO. </summary>
		///
		/// <returns>	The cursor flash time. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		constexpr DWORD GetCursorFlashTime() const noexcept { return 600; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets keypress delay. </summary>
		///
		/// <returns>	The keypress delay. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		constexpr unsigned int GetKeypressDelay() const noexcept { return 400; } // was 100 but was too quick

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Since the UI editor and skin editor are built in, the user could be running with any
		/// resolution This can mean the editor windows are far too big so need to detect and alter
		/// accordingly We need a specific skin for the editor windows anyway since user may have changed
		/// the default.
		/// </summary>
		///
		/// <param name="screenWidth"> 	Width of the screen. </param>
		/// <param name="screenHeight">	Height of the screen. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CreateEditorSkin(int screenWidth, int screenHeight);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Open window, can optionally open modally Can also optionally position relative to another
		/// window.
		/// </summary>
		///
		/// <param name="windowName">	   	Name of the window. </param>
		/// <param name="openModally">	   	(Optional) True to open modally. </param>
		/// <param name="sendToFront">	   	(Optional) True to send to front. </param>
		/// <param name="relativeToWindow">	(Optional) The relative to window. </param>
		/// <param name="dir">			   	(Optional) The dir. </param>
		/// <param name="inside">		   	(Optional) True to inside. </param>
		/// <param name="gap">			   	(Optional) The gap. </param>
		/// <param name="translate">	   	(Optional) The translate. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool OpenWindow(const std::string& windowName,bool openModally=false, bool sendToFront = true,
			std::string relativeToWindow=std::string(),EDirection dir=EDirection::eNorthWest, bool inside=true,
			EGap gap = EGap::eNormal, const VectorI& translate = VectorI(0));

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Close window (does not delete it) </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CloseWindow(const std::string& windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Deletes a window. </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DeleteWindow(const std::string& windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove listener. </summary>
		///
		/// <param name="listener">	[in,out] The listener. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RemoveInputListener(IHapiSpritesUIListener &listener);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add a window with settings and a new skin. </summary>
		///
		/// <param name="windowName">   	Name of the window. </param>
		/// <param name="screenRect">   	The screen rectangle. </param>
		/// <param name="featuresFlags">	(Optional) The features flags. </param>
		/// <param name="titleText">		(Optional) The title text. </param>
		/// <param name="skin">				(Optional) The skin. </param>
		///
		/// <returns>	A std::shared_ptr&lt;UIWindow&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> AddWindow(const std::string& windowName, RectangleI screenRect,
			unsigned int featuresFlags = 0, std::string titleText = std::string(),
			std::shared_ptr<Skin> skin=nullptr);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add a window with settings and an existing skin. </summary>
		///
		/// <param name="windowName">   	Name of the window. </param>
		/// <param name="screenRect">   	The screen rectangle. </param>
		/// <param name="skinName">			Name of the skin. </param>
		/// <param name="featuresFlags">	(Optional) The features flags. </param>
		/// <param name="titleText">		(Optional) The title text. </param>
		///
		/// <returns>	A std::shared_ptr&lt;UIWindow&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> AddWindow(const std::string& windowName, RectangleI screenRect, const std::string& skinName,
			unsigned int featuresFlags = 0, std::string titleText = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add an already created window. </summary>
		///
		/// <param name="newWindow">	The new window. </param>
		///
		/// <returns>	A std::shared_ptr&lt;UIWindow&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<UIWindow> AddWindow(std::shared_ptr<UIWindow> newWindow);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Saves a UI window Note: you may also want to save the window's skin if it is not using the
		/// default, otherwise on load - if the skin does not exist a window will revert to using the
		/// system skin Can optionally provide user data to be saved alongside.
		/// </summary>
		///
		/// <param name="xmlFilename">	   	Filename of the XML file. </param>
		/// <param name="windowToSaveName">	Name of the window to save. </param>
		/// <param name="userData">		   	[in,out] (Optional) If non-null, information describing the
		/// 								user. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Save(const std::string& xmlFilename, const std::string windowToSaveName,CHapiXMLNode *userData=nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Loads a UI window If it uses a user skin and that is not already loaded it will attempt to
		/// load it If scale is true (default) and the resolution is different than what the UI window
		/// was created to, then it and its skin will be scaled To also load user data must use LoadXML
		/// instead of this.
		/// </summary>
		///
		/// <param name="xmlFilename">	   	Filename of the XML file. </param>
		/// <param name="loadedWindowName">	[in,out] Name of the loaded window. </param>
		/// <param name="scale">		   	(Optional) True to scale. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Load(const std::string& xmlFilename, std::string &loadedWindowName, bool scale=true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Load via XML If it uses a user skin and that is not already loaded it will attempt to load it
		/// Note: the name is fixed but you should provide a path.
		/// </summary>
		///
		/// <param name="xml">			   	[in,out] The XML. </param>
		/// <param name="loadedWindowName">	[in,out] Name of the loaded window. </param>
		/// <param name="skinPath">		   	Full pathname of the skin file. </param>
		/// <param name="userData">		   	[in,out] (Optional) If non-null, information describing the
		/// 								user. </param>
		/// <param name="scale">		   	(Optional) True to scale. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXML& xml, std::string &loadedWindowName, const std::string& skinPath, CHapiXMLNode **userData = nullptr, bool scale = true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	As above from a byte stream. </summary>
		///
		/// <param name="byteStream">	   	The byte stream. </param>
		/// <param name="loadedWindowName">	[in,out] Name of the loaded window. </param>
		/// <param name="userData">		   	[in,out] (Optional) If non-null, information describing the
		/// 								user. </param>
		/// <param name="scale">		   	(Optional) True to scale. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadBS(const std::vector<BYTE> &byteStream, std::string &loadedWindowName, CHapiXMLNode **userData = nullptr, bool scale = true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows user to know if they should ignore input because UI has an object using it.
		/// </summary>
		///
		/// <returns>	True if focus, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HasFocus() const;

		// Overrides required by IHapiSpritesInputListener:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI detects a key event. </summary>
		///
		/// <param name="keyEvent">	The key event. </param>
		/// <param name="keyCode"> 	The key code. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI detects a mouse event other than mouse move. </summary>
		///
		/// <param name="mouseEvent">	The mouse event. </param>
		/// <param name="mouseData"> 	Information describing the mouse state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnMouseEvent(EMouseEvent mouseEvent, const MouseData& mouseData) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI detects a mouse move. </summary>
		///
		/// <param name="mouseData">	Information describing the mouse state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnMouseMove(const MouseData& mouseData)  override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called when HAPI detects a controller event, currently not using. </summary>
		///
		/// <param name="controllerId">   	Identifier for the controller. </param>
		/// <param name="controllerEvent">	The controller event. </param>
		/// <param name="buttonIndex">	  	Zero-based index of the button. </param>
		/// <param name="controllerData"> 	Information describing the controller state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnControllerEvent(unsigned int controllerId, EControllerEvent controllerEvent, unsigned int buttonIndex,
			const ControllerData& controllerData)  override final {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called with interpreted key presses into character entry. </summary>
		///
		/// <param name="enteredText">	The entered text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OnTextEntered(char enteredText) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	These handle calls to IHapiSpritesUIListener and map to each function. </summary>
		///
		/// <param name="object">	The object. </param>
		/// <param name="item">  	The item. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_MenuItemClicked(const UIObject& object, const std::string& item);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener button pressed. </summary>
		///
		/// <param name="object">	The object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_ButtonPressed(const UIObject& object);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener slider value changed. </summary>
		///
		/// <param name="object">  	The object. </param>
		/// <param name="newValue">	The new value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_SliderValueChanged(const UIObject& object, float newValue);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener list box item pressed. </summary>
		///
		/// <param name="object">   	The object. </param>
		/// <param name="itemIndex">	Zero-based index of the item. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_ListBoxItemPressed(const UIObject& object,int itemIndex);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener radio button change state. </summary>
		///
		/// <param name="object"> 	The object. </param>
		/// <param name="pressed">	True if pressed. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_RadioButtonChangeState(const UIObject& object, bool pressed);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener text clicked. </summary>
		///
		/// <param name="object">	The object. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool TellListener_TextClicked(const UIObject& object);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener canvas clicked. </summary>
		///
		/// <param name="object">  	The object. </param>
		/// <param name="mousePos">	The mouse position. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool TellListener_CanvasClicked(const UIObject& object, VectorI mousePos);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener text entry changed. </summary>
		///
		/// <param name="object">	  	The object. </param>
		/// <param name="newText">	  	The new text. </param>
		/// <param name="beingEdited">	True if being edited. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_TextEntryChanged(const UIObject& object, const std::string& newText, bool beingEdited);		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener window change state. </summary>
		///
		/// <param name="window">  	[in,out] The window. </param>
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_WindowChangeState(UIWindow& window, EWindowState newState);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener window dragged sized. </summary>
		///
		/// <param name="window">	   	[in,out] The window. </param>
		/// <param name="beingDragged">	True if being dragged. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_WindowDraggedSized(UIWindow& window, bool beingDragged);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener mouse clicked on object. </summary>
		///
		/// <param name="window">	 	[in,out] The window. </param>
		/// <param name="objectName">	Name of the object. </param>
		/// <param name="which">	 	The which. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_MouseClickedOnObject(UIWindow& window, const std::string& objectName, EMouseButton which);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener render. </summary>
		///
		/// <param name="window">			[in,out] The window. </param>
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="callerName">   	Name of the caller. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_Render(UIWindow& window, std::shared_ptr<Surface> &renderSurface, const std::string& callerName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tell listener dial changed. </summary>
		///
		/// <param name="object">	The object. </param>
		/// <param name="ratio"> 	The ratio. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TellListener_DialChanged(const UIObject& object, float ratio);
		void TellListener_UnhandledAction(UIWindow& window, EInputMappingAction action,EInputMappingAction lastAction);		
	};
}
