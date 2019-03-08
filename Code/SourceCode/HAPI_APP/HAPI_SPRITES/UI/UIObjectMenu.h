#pragma once

#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	// A menu item
	struct MenuItem
	{
		std::string text;
		bool enabled{ true };

		MenuItem(std::string itemText) : text(itemText) {}
	};

	struct Position
	{
		RectangleI rect;
		VectorI textPos;

		Position(RectangleI rct, VectorI pos) :rect(rct),textPos(pos){}
	};

	class UIObjectMenu final : public UIObject
	{
	public:
		// Not drawn - just an identifier
		static std::string Seperator() { return "---"; }
	private:
		std::vector<MenuItem> m_menuItems;

		// 1:1 with menuItems, as offsets
		std::vector<Position> m_positioning;

		bool m_inMenuBar{ true };

		std::string m_selectedItem;

		int m_highlightBarFrameNumber{ 0 };

		EMenuType m_type{ EMenuType::eDropDown };
		bool m_fullyExpanded{ false };

		RectangleI GetDropDownAreaBounds() const;
		bool HandleMouseOverMenu(bool pressed);

		friend class UIEditor;
		friend class UIEditorObjectMenuWindow;
		std::vector<std::string> GetItems() const;
		void ReplaceItems(const std::vector<std::string>& items);

	public:
		// Constructs a menu. If type is drop down then the first item is the menu title
		UIObjectMenu(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, const std::vector<std::string>& menuItems, EMenuType type = EMenuType::eDropDown, bool inMenuBar=true);
		// construct from XML node
		UIObjectMenu(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);
		~UIObjectMenu() = default;

		EMenuType GetMenuType() const { return m_type; }
		void EnableMenuItem(const std::string&item, bool enable);
		void RestrictToMenuBar(bool set) { m_inMenuBar = set; }
		bool IsRestrictedToMenuBar() const { return m_inMenuBar; }

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;

		// Overrides required by UIObject		
		void LostFocus() override final;
		void GainedFocus() override final;
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;		
		
		void ChangeActionState(EActionState newState) override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;		
		ERenderPriority GetRenderPriority() const override final { return ERenderPriority::eLast; }
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eMenu; }
		EObjectType GetType() const override final { return EObjectType::eMenu; }

		// Optional overrides
		// Some objects may need multiple rects to describe their 'hit' areas. 
		std::vector<RectangleI> GetWindowSpaceCollisionRectangles() const override final;
		RectangleI GetBoundingRectangleObjectSpace() const override final;
		RectangleI GetMaxBoundingRectangleObjectSpace() const override final;
		bool CanGiveUpFocus(EInputMappingAction action) const override final;
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}