#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{	
	// Basic list box
	class UIObjectListBox final : public UIObject
	{
	private:	
		// Creation params
		EListBoxType m_type;
		int m_numVisibleRows;
		std::vector<std::string> m_items;
		// if 0, sizes to content
		int m_numCharsWide{ 0 };

		// Temps
		VectorI m_firstRowPos;
		
		int m_rowHeight;
		int m_selectedRow{ -1 };
		int m_pressedRow{ -1 };
		int m_rowsScrolled{ 0 };

		int m_sliderSize{ 0 };
		int m_selectedItemBackgroundFrameNum{ 0 };
		int m_thumbFrameNum{ 0 };

		RectangleI m_unexpandedBounds;
		bool m_fullyExpanded{ true };

		RectangleI GetRowRectLocalSpace(int visibleRowNum) const;
		float GetThumbYPosFromRowsScrolled(int rowsScrolled, float centreOfthumbYPos) const;
		int GetRowsScrolledFromThumbYPos(int thumbYPos) const;

		bool HandleMouseOver(bool pressed);

		friend class UIEditorMainWindow;
		friend class UIEditorObjectListBoxWindow;
		EListBoxType GetListBoxType() const { return m_type; }
		void ChangeListBoxType(EListBoxType newType) { m_type = newType; Recreate(); }
		int GetNumVisibleRows() const { return m_numVisibleRows; }
		int GetNumCharsWide() const { return m_numCharsWide; }
		friend class UIEditorObjectCanvasWindow;
		friend class UIEditorLabelsWindow;
		const std::vector<std::string>& GetItems() const { return m_items; }
		void ChangeNumVisibleRows(int newNum) { m_numVisibleRows = newNum; Recreate(); }
		void ChangeNumCharsWide(int newNum) { m_numCharsWide = newNum; Recreate(); }		
	public:
		// Uses skin style for all colours
		UIObjectListBox(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleRows, std::vector<std::string> items,EListBoxType type, int numCharsWide);
		// Construct from XML
		UIObjectListBox(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);
		~UIObjectListBox()=default;

		// Special functions for list boxes
		void ReplaceItems(std::vector<std::string> items, bool resizeTo=false);
		void Select(int selectedIndex);
		void Select(const std::string& text);
		int GetSelectedItemIndex() const;
		const std::string& GetSelectedItem() const;

		// Overrides required by UIObject
		void LostFocus() override final;
		void GainedFocus() override final;
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void ChangeActionState(EActionState newState) override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eListBox; }
		EObjectType GetType() const override final { return EObjectType::eListBox; }

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
		// Optional overrides
		RectangleI GetBoundingRectangleObjectSpace() const override final;
		RectangleI GetMaxBoundingRectangleObjectSpace() const override final;

		// Drop down needs priority
		ERenderPriority GetRenderPriority() const override final { if (m_type == EListBoxType::eDropDown && m_fullyExpanded) return ERenderPriority::eLast; return ERenderPriority::eDontCare; }
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}