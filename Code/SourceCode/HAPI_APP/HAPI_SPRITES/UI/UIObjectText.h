#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{	
	// Note: text handles new lines (multi-line)
	class UIObjectText final : public UIObject
	{
	private:
		std::string m_text;
		ETextType m_type{ ETextType::eStatic };
		friend class UIEditorObjectTextWindow;
		ETextType GetTextType() const { return m_type; }
		void SetTextType(ETextType newType) { m_type = newType; Recreate(); }
	public:
		// Uses skin style for all colours
		UIObjectText(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, std::string text,
			ETextType type = ETextType::eStatic);
		// Construct from XML
		UIObjectText(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);
		~UIObjectText()=default;

		// Overrides required by UIObject
		void SetText(const std::string &newText) override final;
		std::string GetText() const override final { return m_text; }

		void LostFocus() override final;
		void GainedFocus() override final;
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void ChangeActionState(EActionState newState) override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;


		// Do we want / can  we take focus? Optional.
		bool WantsFocus() const override final;

		// Varies on text type
		ESkinStyle GetSkinStyle() const override final { 
			if (m_type == ETextType::eStatic) return ESkinStyle::eNormalText; 
			if (m_type == ETextType::eClickable) return ESkinStyle::eClickableText; return ESkinStyle::eHeadingText;}
		
 		EObjectType GetType() const override final { return EObjectType::eText; }
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}