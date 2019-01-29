#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	// Slider or scrollbar
	class UIObjectSlider final :
		public UIObject
	{
	private:
		SliderLayout m_layout;
		RectangleI m_thumbRect;
		RectangleI m_trackRect;
		VectorI m_tlReduction;
		VectorI m_brReduction;
		int m_markerLength{ 0 };
		
		float m_value{ 0.0f };
		bool m_horizontal{ true };

		// When slider is used as a scrollbar we can scale the thumb graphic
		// User provides a ratio of the side of the thumb to the track
		// Ignores if 0
		float m_thumbToTrackRatio{ 0 };

		RectangleI m_maxLabelRect{ 0,0 };

		void RenderToSurfaceThumb(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const;
		void RenderMarkersToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const;
		void CalculateMarkerLength();

		friend class UIEditorObjectSliderWindow;
		const SliderLayout& GetLayout() const { return m_layout; }
		// Horrid but needed for editor
		PointsLabelsAndMarkers* GetLabelsAndMarkers() { return &m_layout.pntsLabsMarks; }
		void LabelsHaveChanged() { Recreate(); }

		// Hasd to be done this way as we need to force a resize
		void ChangeLayout(SliderLayout newLayout) { m_layout = std::move(newLayout); Recreate(); }
		void CalculateThumbTransform(Transform& renderTrans) const;
	public:
		// Constructs a slider dynamically
		UIObjectSlider(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI requestedSize, SliderLayout layout);
		
		// Construct from XML
		UIObjectSlider(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		~UIObjectSlider() = default;

		bool IsVertical() const { return (m_trackRect.Height() > m_trackRect.Width()); }

		// Overrides required by UIObject		
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		void LostFocus() override final;
		void GainedFocus() override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		// Some objects may want to hold on to focus e.g. text entry
	//	bool CanGiveUpFocus(const MouseData& mouseData) override final;
	// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
	// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;

		ESkinStyle GetSkinStyle() const override final { if (m_layout.asWindowScrollbar) return ESkinStyle::eScroller; return ESkinStyle::eSlider; }
		EObjectType GetType() const override final { return EObjectType::eSlider; }

		void ChangeActionState(EActionState newState) override final;

		// Allows setting value directly (if object supports it). Must be ratio between 0 and 1
		void SetValue(float ratioValue) override final;

		// When slider is used as a scrollbar we can scale the thumb graphic
		// User provides a ratio of the side of the thumb to the track
		// Ignores if 0 (uses original size)
		// Values beyone 1 are capped.
		void SetThumbToTrackRatio(float ratio) { m_thumbToTrackRatio = std::min(1.0f,ratio); }

		// Allows getting value directly (if object supports it). 
		// What the value means depends on object type but normally ranges from 0.0f to 1.0f
		float GetValue() const override final { return m_value; }

		bool ShouldObayWindowLayoutRules() const override final { return !m_layout.asWindowScrollbar; }

	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}
