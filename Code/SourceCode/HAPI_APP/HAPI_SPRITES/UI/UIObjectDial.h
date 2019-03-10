#pragma once
#include "UI\UIObject.h"

namespace HAPISPACE {
	class UIEditorObjectDialWindow;
	class UIEditorObjectCommonWindow;
}
namespace HAPI_UI_SPACE
{
	// dial
	class UIObjectDial final :
		public UIObject
	{
	private:
		DialLayout m_layout;
		RectangleI m_circleRect;
		// Value ranges from 0 to 1 across the dial range
		float m_value{ 0.0f };
		int m_sizeReduction{ 0 };
		int m_markerLength{ 0 };

		// Dial itself uses its own sprite for ease
		std::shared_ptr<Sprite> m_dialSprite;
		void CreateDialSprite();
		void RenderToSurfaceDial(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const;
		void CalculateValue(float angle);
		void RenderMarkersToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub, bool inside, bool showLabels) const;
		void CalculateMarkerLength();

		friend class HAPISPACE::UIEditorObjectDialWindow;
		friend class HAPISPACE::UIEditorObjectCommonWindow;

		const DialLayout& GetLayout() const { return m_layout; }
		// Horrid but needed for editor

		PointsLabelsAndMarkers* GetLabelsAndMarkers() { return &m_layout.pntsLabsMarks; }
		void LabelsHaveChanged() { Recreate(); }

		// Hasd to be done this way as we need to force a resize
		void ChangeLayout(DialLayout newLayout) { m_layout = std::move(newLayout); Recreate(); }		
	public:
		// Constructs a button dynamically
		UIObjectDial(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI requestedSize, DialLayout layout);
		
		// Construct from XML
		UIObjectDial(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		~UIObjectDial() = default;

		// Overrides required by UIObject		
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void LostFocus() override final;
		void GainedFocus() override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;

		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eDial; }
		EObjectType GetType() const override final { return EObjectType::eDial; }

		void ChangeActionState(EActionState newState) override final;

		// Allows setting value directly (if object supports it). Must be ratio between 0 and 1
		// Will be clamped between 0 and 1 and may be snapped to resolution.
		void SetValue(float ratioValue) override final;

		// Allows getting value directly (if object supports it). 
		// What the value means depends on object type but normally ranges from 0.0f to 1.0f
		float GetValue() const override final { return m_value; }

	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}
