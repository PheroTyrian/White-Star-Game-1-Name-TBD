#pragma once

#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	/*
		Can be used as background for a window
		Fill colour and border colour come from style unless overwritten
	*/	
	class UIObjectAdornments final : public UIObject
	{
	private:
		RectangleI m_titleBarRect;
		RectangleI m_menuBarRect;
		RectangleI m_sizingAreaRect;
		VectorI m_textPos;
		VectorI m_crossPos;
		VectorI m_minimisePos;
		VectorI m_dragStart;

		// For the minimise, maximise and close buttons
		VectorI m_iconSize;
		int m_closeFrameNum{ 0 };
		int m_minimiseFrameNum{ 0 };
		int m_maximiseFrameNum{ 0 };
		// Different when minimised
		RectangleI m_minimisedBounds;
		// So can restore after minimisation
		RectangleI m_oldBounds;

		bool m_sizing{ false };
		RectangleI m_newSizeRect;
		
		std::string m_titleText;

		unsigned int m_featuresFlags;
				
		bool m_previousMouseDownState{ false };		
		bool m_highlightCross{ false };
		bool m_highlightMinimise{ false };		
	public:	
		UIObjectAdornments(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI dimensions,
			unsigned int featuresFlags,std::string titleText);
		UIObjectAdornments(UIWindow* owner, std::string name, CHapiXML &xml, CHapiXMLNode *adornmentsNode);
		~UIObjectAdornments() = default;

		// Allows change after creation.
		void SetFeatureFlag(WindowFeatures::EAdornmentFeatures which, bool set);

		void OwningWindowIsMinimised(bool set);
		
		// For adornments refers to window title
		// Allows setting text directly (if object supports it)
		void SetText(const std::string &newText) override final { m_titleText = newText;  Recreate(); }
		// Allows getting text directly (if supported else empty)
		std::string GetText() const override final { return m_titleText; }

		RectangleI GetClientRect() const;
		RectangleI GetMenuArea() const { return m_menuBarRect; }

		unsigned int GetFeaturesFlags() const { return m_featuresFlags; }
		bool GetUserDraw() const { return (m_featuresFlags & WindowFeatures::eUserDraw || m_featuresFlags & WindowFeatures::eSystem); }
		bool GetShouldScaleOnSize() const { return (m_featuresFlags & WindowFeatures::eScaleWhenSized); }

		// Some objects may need multiple rects to describe their 'hit' areas. 
		std::vector<RectangleI> GetWindowSpaceCollisionRectangles() const override final
		{
			// For collision detection we only want to use the title bar (if there is one)
			// Note: adornment rects are already in window space			

			// Addition - also want sizing area to hit	
			// But it depends on mode if we have a title bar
			if (m_featuresFlags & WindowFeatures::eTitleText)
				return std::vector<RectangleI>{m_titleBarRect, m_sizingAreaRect};

			return std::vector<RectangleI>{m_sizingAreaRect};
		}

		// Overrides from UIObject

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
		void LostFocus() override final;
		void GainedFocus() override final;
		bool CanGiveUpFocus(EInputMappingAction action) const override;

		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void ChangeActionState(EActionState newState) override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		ERenderPriority GetRenderPriority() const override final { return ERenderPriority::eFirst; }
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eWindowNormal; }
		EObjectType GetType() const override final { return EObjectType::eAdornment; }

	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};

}