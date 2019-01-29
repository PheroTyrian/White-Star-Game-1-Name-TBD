#pragma once

#include "UI\UITypes.h"
#include "UI\UISkin.h"

namespace HAPISPACE
{
	class UIandSkinPayloadData;
	class SizeUndo;
	class PositionUndo;
}

namespace HAPI_UI_SPACE
{	
	// UI Object base class
	class UIObject
	{
	private:
		// Local position within window
		VectorI m_position;

		// Object specific styles, default to inherit everything
		UiTextStyleGroup m_textStyle;
		UiWindowStyleGroup m_windowStyle;

		// Stores result in member vars, needs to be called when style changes
		void CalculateMaxBorderWidth();

		// Dangerous - do not use, instead do via window
		friend class UIWindow;
		void ChangeName(std::string newName) { m_name = std::move(newName); }
	
	protected:
		std::string m_name;
		RectangleI m_bounds;

		// Used when resizing
		RectangleI m_originalBounds;
		VectorI m_originalPosition;

		// It is best to cache these results
		int m_maxBorderWidth{ 0 };
		int m_maxShadowWidth{ 0 };

		bool m_visible{ true };

		EActionState m_actionState{ EActionState::eNormal };
		EActionState m_actionStatePriorToBeingDisabled{ EActionState::eNormal };

		// Always created now to hold images for rendering
		std::shared_ptr<Sprite> m_precreatedSprite;

		// Unfortunately cannot use a shared pointer for this
		UIWindow* m_owningWindow{ nullptr };

		// Tag for grouping e.g. into screens. 
		std::string m_tag;

		void RenderText(std::shared_ptr<Surface> &surface, VectorI p, const std::string& text, const UiTextStyle& style) const;

		void DrawWindowFilledRect(std::shared_ptr<Surface> &surface, const RectangleI &rect, const UiWindowStyle& style) const;

		// Arbitray line that uses window style border shader and width
		void DrawWindowLine(std::shared_ptr<Surface> &surface, const LineI &line, const UiWindowStyle& style) const;

		// setShadowWritesAlpha is special case for adornments which need to burn alpha
		void DrawWindowBorderRect(std::shared_ptr<Surface> &surface, const RectangleI &rect, const UiWindowStyle& style) const;
		// top is needed to know which way to send border
		void DrawHorizontalWindowBorder(std::shared_ptr<Surface> &surface, VectorI pos, int width, const UiWindowStyle& style,bool top) const;
		void DrawVerticalWindowBorder(std::shared_ptr<Surface> &surface, VectorI pos, int height, const UiWindowStyle& style,bool left) const;
		void DrawHorizontalWindowLine(std::shared_ptr<Surface> &surface, VectorI pos, int width, const UiWindowStyle& style) const;
		void DrawVerticalWindowLine(std::shared_ptr<Surface> &surface, VectorI pos, int height, const UiWindowStyle& style) const;
		void DrawShadow(std::shared_ptr<Surface> &surface, const RectangleF &rect, const UiWindowStyle& style) const;
		bool LoadBaseXML(CHapiXML &xml, CHapiXMLNode *root);

		// Solving issue when object first created and user has supplied a position in the rect
		RectangleI RemovePositionFromCreationRect(RectangleI dimensions);

		// Renders to surfaces and uploads to HW for quick rendering
		virtual void CreateSurfaces() = 0;
		// Calculates bounds and positioning of gfx etc.
		virtual void CalculateSizes(const RectangleI &newSize, bool forceSize = false) = 0;
		// Render anything that needs to be rendered to surface type
		virtual void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const = 0;
		/*
			All sizing and positioning operations must go via the owning window
		*/
		friend class UIWindow;
		// USed when resizing
		RectangleI GetOriginalBounds() const { assert(m_originalBounds.IsValid()); return m_originalBounds; }
		VectorI GetOriginalPosition() const { return m_originalPosition; }
		// Attempt to resize. May not be possible or there may be a minimum allowed
		// However if force flag is true they must use it
		// Passing by value to avoid aliasing issues when called with m_bounds
		friend class HAPISPACE::UIandSkinPayloadData;
		void Resize(RectangleI newSize, bool forceSize = false, bool setAsOriginal=true);
		
		// Gets largest of all subtypes in terms of bounding rect
		RectangleI GetTextRect(const std::string &text) const;

		// allows edit
		UiTextStyleGroup& GetTextStyleGroupSettings() { return m_textStyle; }
		UiWindowStyleGroup& GetWindowStyleGroupSettings() { return m_windowStyle; }

		// Access styles for this object
		//friend class UIEditorObjectStyleWindow;
		const UiTextStyle& GetTextStyleSettings(ESkinSubStyle sub) const { return m_textStyle.GetSub(sub); }
		const UiWindowStyle& GetWindowStyleSettings(ESkinSubStyle sub) const { return m_windowStyle.GetSub(sub); }

		const UiSkinSizes& GetSkinSizes() const;

		ESkinSubStyle GetSkinSubStyleFromActionState() const;

		friend class HAPISPACE::SizeUndo;
		friend class HAPISPACE::PositionUndo;

		//friend class UIEditorObjectCommonWindow;

		// Change the position of the object
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

		bool WriteBaseXML(CHapiXMLNode *rootNode) const;
				
	public:
		// Construct with a name and owning window
		UIObject(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle)  noexcept :
			m_owningWindow(owner), m_name(name), m_tag(tag), m_textStyle(textStyle) {}
		virtual ~UIObject() = default;		

		// Recreate all graphics (will also cascade styles)
		void Recreate();

		void ChangeTag(const std::string &newTag) { m_tag = newTag; }

		const UiTextStyleGroup& GetTextStyleGroupSettings() const { return m_textStyle; }
		const UiWindowStyleGroup& GetWindowStyleGroupSettings() const { return m_windowStyle; }

		// Allows this object's instance data to be set for style
		// Note: will cause a style cascade and a complete recreation of all object's graphics
		void SetTextStyleGroupSettings(const UiTextStyleGroup& textStyle) { m_textStyle = textStyle; Recreate(); }

		// Allows this object's instance data to be set for style
		// Note: will cause a style cascade and a complete recreation of all object's graphics
		void SetWindowStyleGroupSettings(const UiWindowStyleGroup& winStyle) { m_windowStyle = winStyle; Recreate();}

		// Every object is owned by a window
		UIWindow& GetOwningWindow() const;
				
		bool IsVisible() const { return m_visible; }
		void SetVisible(bool set) { m_visible = set; }

		// Every object has a name unique to the window it is part of
		const std::string& GetName() const { return m_name; }

		// Object's can optionally have a tag
		const std::string& GetTag() const { return m_tag; }		

		// States
		EActionState GetActionState() const { return m_actionState; }

		// Change state - updates listeners if relevant
		// Prefer to use Disable if wanting to set to disabled
		virtual void ChangeActionState(EActionState newState) = 0;

		// True - Disables but remembers previous state to return to
		// False - returns to previous state
		void Disable(bool set);

		VectorI GetPositionWindowSpace() const { return m_position; }
		VectorI GetPositionScreenSpace(VectorI windowTopLeft) const { return GetPositionWindowSpace() + windowTopLeft; }

		// Note this can vary dependant on object shape e.g. the menu can be expanded or just a title
		// Therefore to get size when fully expanded call GetMaxBoundingRectangleObjectSpace
		// This is virtual to allow things like menu to redefine		
		virtual RectangleI GetBoundingRectangleObjectSpace() const { return m_bounds; }
		virtual RectangleI GetMaxBoundingRectangleObjectSpace() const { return GetBoundingRectangleObjectSpace(); }
		RectangleI GetBoundingRectangleWindowSpace() const { return GetBoundingRectangleObjectSpace().Translated(m_position); }
		RectangleI GetBoundingRectangleWindowSpaceIncludeBorder() const;
		RectangleI GetBoundingRectangleScreenSpace(const VectorI& windowTopLeft) const { return GetBoundingRectangleWindowSpace().Translated(windowTopLeft); }
		int GetMaxBorderSize() const { return m_maxBorderWidth; }

		// Virtuals

		// Some objects may need multiple rects to describe their 'hit' areas. 
		virtual std::vector<RectangleI> GetWindowSpaceCollisionRectangles() const {
			// Default is just the one bounding
			return std::vector<RectangleI>{GetBoundingRectangleWindowSpace()};
		}
			
		// Allows setting text directly (if object supports it)
		virtual void SetText(const std::string &newText) { std::cout << "SetText: Unsupported call" << std::endl; }
		// Allows getting text directly (if supported else empty)
		virtual std::string GetText() const { return std::string(); }

		// Allows setting value directly (if object supports it). 
		// Will be clamped between 0 and 1 and may be snapped to resolution.
		virtual void SetValue(float ratioValue) { std::cout << "SetValue: Unsupported call" << std::endl; }

		// Allows getting value directly (if object supports it). 
		// What the value means depends on object type but normally ranges from 0.0f to 1.0f
		virtual float GetValue() const { std::cout << "GetValue: Unsuported call" << std::endl;  return -1.0f; }

		// Interpreted key event occured when we have focus, return true if we handled it
		virtual bool TextEntered(char c) { return false; }

		// priority 0 is rendered first
		// TODO: allow this to be user set?
		virtual ERenderPriority GetRenderPriority() const { return ERenderPriority::eDontCare; }

		// Pure Virtuals
		virtual EObjectType GetType() const = 0;

		// These are hard coded in child to return an enum
		virtual ESkinStyle GetSkinStyle() const = 0;

		// Optional
		virtual bool ShouldObayWindowLayoutRules() const { return true; }
private:
		// Some objects may want to hold on to focus e.g. text entry
		// Some may want to hold on to it if mouse has just moved away and no button click e.g. right click menu
		virtual bool CanGiveUpFocus(EInputMappingAction action) const { return true; }

		// Object has lost focus (no argument)
		virtual void LostFocus() = 0;

		// UI wants to set this object as the one with focus (no argument)
		virtual void GainedFocus() = 0;

		// Draw this object to the surface
		virtual void Render(std::shared_ptr<Surface> &surface,VectorI translate) const = 0;

		virtual bool WriteXML(CHapiXMLNode *rootNode) = 0;
		
		// Do we want / can  we take focus? Optional.
		virtual bool WantsFocus() const
		{
			if (GetActionState() == EActionState::eDisabled)
				return false;
			return true;
		}

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled	
		virtual bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) = 0;		
	};
}