////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObject.h
//
// summary:	Declares the object class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "UI\UITypes.h"
#include "UI\UISkin.h"

namespace HAPISPACE
{
	/// <summary>	An iand skin payload data. </summary>
	class UIandSkinPayloadData;
	/// <summary>	A size undo. </summary>
	class SizeUndo;
	/// <summary>	A position undo. </summary>
	class PositionUndo;
}

namespace HAPI_UI_SPACE
{	
	/// <summary>	UI Object base class. </summary>
	class UIObject
	{
	private:
		// Local position within window
		/// <summary>	The position. </summary>
		VectorI m_position;

		/// <summary>	Object specific styles, default to inherit everything. </summary>
		UiTextStyleGroup m_textStyle;

		/// <summary>	The window style. </summary>
		UiWindowStyleGroup m_windowStyle;

		/// <summary>	Stores result in member vars, needs to be called when style changes. </summary>
		void CalculateMaxBorderWidth();

		/// <summary>	Dangerous - do not use, instead do via window. </summary>
		friend class UIWindow;

		/// <summary>	Change name. </summary>
		void ChangeName(std::string newName) { m_name = std::move(newName); }
	
	protected:
		/// <summary>	The name. </summary>
		std::string m_name;

		/// <summary>	The bounds. </summary>
		RectangleI m_bounds;

		/// <summary>	Used when resizing. </summary>
		RectangleI m_originalBounds;

		/// <summary>	The original position. </summary>
		VectorI m_originalPosition;

		/// <summary>	It is best to cache these results. </summary>
		int m_maxBorderWidth{ 0 };
		/// <summary>	The maximum shadow width. </summary>
		int m_maxShadowWidth{ 0 };

		/// <summary>	True to show, false to hide. </summary>
		bool m_visible{ true };

		/// <summary>	State of the action. </summary>
		EActionState m_actionState{ EActionState::eNormal };
		/// <summary>	The action state prior to being disabled. </summary>
		EActionState m_actionStatePriorToBeingDisabled{ EActionState::eNormal };

		/// <summary>	Always created now to hold images for rendering. </summary>
		std::shared_ptr<Sprite> m_precreatedSprite;

		/// <summary>	Unfortunately cannot use a shared pointer for this. </summary>
		UIWindow* m_owningWindow{ nullptr };

		/// <summary>	Tag for grouping e.g. into screens. </summary>
		std::string m_tag;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders the text. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="p">	  	A VectorI to process. </param>
		/// <param name="text">   	The text. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderText(std::shared_ptr<Surface> &surface, VectorI p, const std::string& text, const UiTextStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw window filled rectangle. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="rect">   	The rectangle. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawWindowFilledRect(std::shared_ptr<Surface> &surface, const RectangleI &rect, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Arbitray line that uses window style border shader and width. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="line">   	The line. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawWindowLine(std::shared_ptr<Surface> &surface, const LineI &line, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// setShadowWritesAlpha is special case for adornments which need to burn alpha.
		/// </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="rect">   	The rectangle. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawWindowBorderRect(std::shared_ptr<Surface> &surface, const RectangleI &rect, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	top is needed to know which way to send border. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="pos">	  	The position. </param>
		/// <param name="width">  	The width. </param>
		/// <param name="style">  	The style. </param>
		/// <param name="top">	  	True to top. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawHorizontalWindowBorder(std::shared_ptr<Surface> &surface, VectorI pos, int width, const UiWindowStyle& style,bool top) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw vertical window border. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="pos">	  	The position. </param>
		/// <param name="height"> 	The height. </param>
		/// <param name="style">  	The style. </param>
		/// <param name="left">   	True to left. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawVerticalWindowBorder(std::shared_ptr<Surface> &surface, VectorI pos, int height, const UiWindowStyle& style,bool left) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw horizontal window line. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="pos">	  	The position. </param>
		/// <param name="width">  	The width. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawHorizontalWindowLine(std::shared_ptr<Surface> &surface, VectorI pos, int width, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw vertical window line. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="pos">	  	The position. </param>
		/// <param name="height"> 	The height. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawVerticalWindowLine(std::shared_ptr<Surface> &surface, VectorI pos, int height, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw shadow. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		/// <param name="rect">   	The rectangle. </param>
		/// <param name="style">  	The style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawShadow(std::shared_ptr<Surface> &surface, const RectangleF &rect, const UiWindowStyle& style) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads base XML. </summary>
		///
		/// <param name="xml"> 	[in,out] The XML. </param>
		/// <param name="root">	[in,out] If non-null, the root. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadBaseXML(CHapiXML &xml, CHapiXMLNode *root);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Solving issue when object first created and user has supplied a position in the rect.
		/// </summary>
		///
		/// <param name="dimensions">	The dimensions. </param>
		///
		/// <returns>	A RectangleI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI RemovePositionFromCreationRect(RectangleI dimensions);

		/// <summary>	Renders to surfaces and uploads to HW for quick rendering. </summary>
		virtual void CreateSurfaces() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates bounds and positioning of gfx etc. </summary>
		///
		/// <param name="newSize">  	Size of the new. </param>
		/// <param name="forceSize">	(Optional) True to force size. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void CalculateSizes(const RectangleI &newSize, bool forceSize = false) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Render anything that needs to be rendered to surface type. </summary>
		///
		/// <param name="surface">	  	[in,out] The surface. </param>
		/// <param name="surfaceRect">	The surface rectangle. </param>
		/// <param name="sub">		  	The sub. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const = 0;
		/*
			All sizing and positioning operations must go via the owning window
		*/
		/// <summary>	Form for viewing the user interface. </summary>
		friend class UIWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	USed when resizing. </summary>
		///
		/// <returns>	The original bounds. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetOriginalBounds() const { assert(m_originalBounds.IsValid()); return m_originalBounds; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets original position. </summary>
		///
		/// <returns>	The original position. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetOriginalPosition() const { return m_originalPosition; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Attempt to resize. May not be possible or there may be a minimum allowed However if force
		/// flag is true they must use it Passing by value to avoid aliasing issues when called with
		/// m_bounds.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		friend class HAPISPACE::UIandSkinPayloadData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resizes. </summary>
		///
		/// <param name="newSize">			Size of the new. </param>
		/// <param name="forceSize">		(Optional) True to force size. </param>
		/// <param name="setAsOriginal">	(Optional) True to set as original. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Resize(RectangleI newSize, bool forceSize = false, bool setAsOriginal=true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets largest of all subtypes in terms of bounding rect. </summary>
		///
		/// <param name="text">	The text. </param>
		///
		/// <returns>	The text rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetTextRect(const std::string &text) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	allows edit. </summary>
		///
		/// <returns>	The text style group settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiTextStyleGroup& GetTextStyleGroupSettings() { return m_textStyle; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style group settings. </summary>
		///
		/// <returns>	The window style group settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiWindowStyleGroup& GetWindowStyleGroupSettings() { return m_windowStyle; }

		// Access styles for this object
		//friend class UIEditorObjectStyleWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets text style settings. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The text style settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiTextStyle& GetTextStyleSettings(ESkinSubStyle sub) const { return m_textStyle.GetSub(sub); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style settings. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The window style settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiWindowStyle& GetWindowStyleSettings(ESkinSubStyle sub) const { return m_windowStyle.GetSub(sub); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin sizes. </summary>
		///
		/// <returns>	The skin sizes. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiSkinSizes& GetSkinSizes() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin sub style from action state. </summary>
		///
		/// <returns>	The skin sub style from action state. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinSubStyle GetSkinSubStyleFromActionState() const;

		/// <summary>	A size undo. </summary>
		friend class HAPISPACE::SizeUndo;
		/// <summary>	A position undo. </summary>
		friend class HAPISPACE::PositionUndo;

		//friend class UIEditorObjectCommonWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change the position of the object. </summary>
		///
		/// <param name="newPos">			The new position. </param>
		/// <param name="setAsOriginal">	(Optional) True to set as original. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetPositionWindowSpace(VectorI newPos, bool setAsOriginal = true) 
		{ 
			m_position = newPos; 
			if (setAsOriginal) m_originalPosition = newPos; 
			if (newPos.x != 0 || newPos.y != 0)
			{
				// trying to cacth a bug that changed an adornments position (it should always be at 0,0 in window space)
				// Note: think I have found it. Last occurence 28/10/18
				// No: happened again!
				assert(GetType() != EObjectType::eAdornment);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a base XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteBaseXML(CHapiXMLNode *rootNode) const;
				
	public:
		// Construct with a name and owning window

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		[in,out] If non-null, the owner. </param>
		/// <param name="name">			The name. </param>
		/// <param name="tag">			The tag. </param>
		/// <param name="textStyle">	The text style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObject(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle)  noexcept :
			m_owningWindow(owner), m_name(name), m_tag(tag), m_textStyle(textStyle) {}
		/// <summary>	Destructor. </summary>
		virtual ~UIObject() = default;		

		/// <summary>	Recreate all graphics (will also cascade styles) </summary>
		void Recreate();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change tag. </summary>
		///
		/// <param name="newTag">	The new tag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeTag(const std::string &newTag) { m_tag = newTag; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets text style group settings. </summary>
		///
		/// <returns>	The text style group settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiTextStyleGroup& GetTextStyleGroupSettings() const { return m_textStyle; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style group settings. </summary>
		///
		/// <returns>	The window style group settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiWindowStyleGroup& GetWindowStyleGroupSettings() const { return m_windowStyle; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows this object's instance data to be set for style Note: will cause a style cascade and a
		/// complete recreation of all object's graphics.
		/// </summary>
		///
		/// <param name="textStyle">	The text style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextStyleGroupSettings(const UiTextStyleGroup& textStyle) { m_textStyle = textStyle; Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows this object's instance data to be set for style Note: will cause a style cascade and a
		/// complete recreation of all object's graphics.
		/// </summary>
		///
		/// <param name="winStyle">	The window style. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetWindowStyleGroupSettings(const UiWindowStyleGroup& winStyle) { m_windowStyle = winStyle; Recreate();}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every object is owned by a window. </summary>
		///
		/// <returns>	The owning window. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIWindow& GetOwningWindow() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is visible. </summary>
		///
		/// <returns>	True if visible, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsVisible() const { return m_visible; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a visible. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetVisible(bool set) { m_visible = set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every object has a name unique to the window it is part of. </summary>
		///
		/// <returns>	The name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetName() const { return m_name; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Object's can optionally have a tag. </summary>
		///
		/// <returns>	The tag. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetTag() const { return m_tag; }		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	States. </summary>
		///
		/// <returns>	The action state. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EActionState GetActionState() const { return m_actionState; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Change state - updates listeners if relevant Prefer to use Disable if wanting to set to
		/// disabled.
		/// </summary>
		///
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void ChangeActionState(EActionState newState) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// True - Disables but remembers previous state to return to False - returns to previous state.
		/// </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Disable(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets position window space. </summary>
		///
		/// <returns>	The position window space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetPositionWindowSpace() const { return m_position; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets position screen space. </summary>
		///
		/// <param name="windowTopLeft">	The window top left. </param>
		///
		/// <returns>	The position screen space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetPositionScreenSpace(VectorI windowTopLeft) const { return GetPositionWindowSpace() + windowTopLeft; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Note this can vary dependant on object shape e.g. the menu can be expanded or just a title
		/// Therefore to get size when fully expanded call GetMaxBoundingRectangleObjectSpace This is
		/// virtual to allow things like menu to redefine.
		/// </summary>
		///
		/// <returns>	The bounding rectangle object space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual RectangleI GetBoundingRectangleObjectSpace() const { return m_bounds; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets maximum bounding rectangle object space. </summary>
		///
		/// <returns>	The maximum bounding rectangle object space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual RectangleI GetMaxBoundingRectangleObjectSpace() const { return GetBoundingRectangleObjectSpace(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets bounding rectangle window space. </summary>
		///
		/// <returns>	The bounding rectangle window space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetBoundingRectangleWindowSpace() const { return GetBoundingRectangleObjectSpace().Translated(m_position); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets bounding rectangle window space include border. </summary>
		///
		/// <returns>	The bounding rectangle window space include border. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetBoundingRectangleWindowSpaceIncludeBorder() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets bounding rectangle screen space. </summary>
		///
		/// <param name="windowTopLeft">	The window top left. </param>
		///
		/// <returns>	The bounding rectangle screen space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetBoundingRectangleScreenSpace(const VectorI& windowTopLeft) const { return GetBoundingRectangleWindowSpace().Translated(windowTopLeft); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets maximum border size. </summary>
		///
		/// <returns>	The maximum border size. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetMaxBorderSize() const { return m_maxBorderWidth; }

		// Virtuals

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Some objects may need multiple rects to describe their 'hit' areas. </summary>
		///
		/// <returns>	The window space collision rectangles. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual std::vector<RectangleI> GetWindowSpaceCollisionRectangles() const {
			// Default is just the one bounding
			return std::vector<RectangleI>{GetBoundingRectangleWindowSpace()};
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows setting text directly (if object supports it) </summary>
		///
		/// <param name="newText">	The new text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void SetText(const std::string &newText) { std::cerr << "SetText: Unsupported call" << std::endl; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows getting text directly (if supported else empty) </summary>
		///
		/// <returns>	The text. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual std::string GetText() const { return std::string(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows setting value directly (if object supports it). Will be clamped between 0 and 1 and
		/// may be snapped to resolution.
		/// </summary>
		///
		/// <param name="ratioValue">	The ratio value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void SetValue(float ratioValue) { std::cerr << "SetValue: Unsupported call" << std::endl; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows getting value directly (if object supports it). What the value means depends on object
		/// type but normally ranges from 0.0f to 1.0f.
		/// </summary>
		///
		/// <returns>	The value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual float GetValue() const { std::cerr << "GetValue: Unsuported call" << std::endl;  return -1.0f; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Interpreted key event occured when we have focus, return true if we handled it.
		/// </summary>
		///
		/// <param name="c">	The character. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool TextEntered(char c) { return false; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	priority 0 is rendered first TODO: allow this to be user set? </summary>
		///
		/// <returns>	The render priority. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ERenderPriority GetRenderPriority() const { return ERenderPriority::eDontCare; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Pure Virtuals. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual EObjectType GetType() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	These are hard coded in child to return an enum. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ESkinStyle GetSkinStyle() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Optional. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool ShouldObayWindowLayoutRules() const { return true; }
private:
		// Some objects may want to hold on to focus e.g. text entry
		// Some may want to hold on to it if mouse has just moved away and no button click e.g. right click menu

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if we can give up focus. </summary>
		///
		/// <param name="action">	The action. </param>
		///
		/// <returns>	True if we can give up focus, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool CanGiveUpFocus(EInputMappingAction action) const { return true; }

		/// <summary>	Object has lost focus (no argument) </summary>
		virtual void LostFocus() = 0;

		/// <summary>	UI wants to set this object as the one with focus (no argument) </summary>
		virtual void GainedFocus() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw this object to the surface. </summary>
		///
		/// <param name="surface">  	[in,out] The surface. </param>
		/// <param name="translate">	The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void Render(std::shared_ptr<Surface> &surface,VectorI translate) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool WriteXML(CHapiXMLNode *rootNode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Do we want / can  we take focus? Optional. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool WantsFocus() const
		{
			if (GetActionState() == EActionState::eDisabled)
				return false;
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste Return true if
		/// handled.
		/// </summary>
		///
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) = 0;		
	};
}