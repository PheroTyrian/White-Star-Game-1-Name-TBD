#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	
	// A simple button
	class UIObjectButton final :
		public UIObject
	{
	private:
		EButtonType m_type{ EButtonType::eToggle };						
		std::string m_buttonText;

		// Calculated
		VectorI m_textPosOffset;
		// For radio buttons with text to the side only:
		RectangleI m_buttonOnlyBounds;

		// Added for buttons who may be part of radio button collections and need to notify others
		// Leaving in base as may be useful later for other controls
		// Moved here as not used by others so waste
		std::shared_ptr<std::vector<std::string>> m_collection;

		friend class UIEditorObjectButtonWindow;
		EButtonType GetButtonType() const { return m_type; }
		void ChangeButtonType(EButtonType newType);
		void CalculateSizesTextToSide(const RectangleI& requestedSize, bool forceSize);
	public:
		// Constructs a button dynamically
		UIObjectButton(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, std::string buttonText, EButtonType type=EButtonType::eToggle);
		
		// Construct from XML
		UIObjectButton(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		~UIObjectButton() = default;

		void SetCollection(std::shared_ptr<std::vector<std::string>> col) { m_collection = col; }

		// Overrides required by UIObject		
		void SetText(const std::string &newText) override final;
		std::string GetText() const override final { return m_buttonText; }

		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void LostFocus() override final;
		void GainedFocus() override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		// Varies on  type
		ESkinStyle GetSkinStyle() const override final {
			if (m_type == EButtonType::eRadioTextLeft || m_type==EButtonType::eRadioTextRight)
				return ESkinStyle::eButtonRadioSideText;
			return ESkinStyle::eButton;
		}

		EObjectType GetType() const override final { return EObjectType::eButton; }

		void ChangeActionState(EActionState newState) override final;

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;

		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	private:
		// To handle special case
		void ChangeActionState(EActionState newState, bool notifyListeners);
	};
}
