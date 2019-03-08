#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	// Shape
	class UIObjectShape final :
		public UIObject
	{
	private:
		//RectangleI m_rect;		
		std::string m_caption;
	public:
		// Constructs a shape to rect size
		UIObjectShape(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI rect, std::string caption);
		
		// Construct from XML
		UIObjectShape(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		~UIObjectShape() = default;
		
		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;

		void ChangeActionState(EActionState newState) override final;
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
		void LostFocus() override final {};
		void GainedFocus() override final {};	

		bool WriteXML(CHapiXMLNode *rootNode) override final;
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eShape; }
		EObjectType GetType() const override final { return EObjectType::eShape; }

		// Allows setting text directly (if object supports it)
		void SetText(const std::string &newText) override final { if (newText == m_caption) return;  m_caption = newText; Recreate(); }

		// Allows getting text directly (if supported else empty)
		std::string GetText() const override final { return m_caption; }
		
		// priority 0 is rendered first
		ERenderPriority GetRenderPriority() const override final { return ERenderPriority::eFirst; }
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}
