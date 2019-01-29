#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	// Canvas to draw to
	class UIObjectCanvas final :
		public UIObject
	{
	private:
		std::shared_ptr<Sprite> m_sprite;
		std::shared_ptr<Sprite> m_userRenderSprite;
		friend class UIEditorObjectCanvasWindow;
		std::shared_ptr<Sprite> GetSprite() const { return m_sprite; }
		void SetSprite(std::shared_ptr<Sprite> sprite);
		int m_frameNumber{ 0 };
		std::string m_frameSetName;
	public:
		// Constructs a canvas to rect size
		// Can optionally povide a sprite to be drawn prior to user draw callback being called

		UIObjectCanvas(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI rect, std::shared_ptr<Sprite> sprite);
		
		// Construct from XML
		UIObjectCanvas(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		~UIObjectCanvas() = default;

		void SetSpriteFrameSet(std::string frameSetName);
		
		// Returns false if beyond range
		bool SetSpriteFrameNumber(int newFrame);
		int GetSpriteFrameNumber() const { return m_frameNumber; }

		// Overrides required by UIObject		

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void LostFocus() override final {};
		void GainedFocus() override final {};	
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eCanvas; }
		EObjectType GetType() const override final { return EObjectType::eCanvas; }
		void ChangeActionState(EActionState newState) override final;
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}
