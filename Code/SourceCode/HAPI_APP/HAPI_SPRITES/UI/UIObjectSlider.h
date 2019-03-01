////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObjectSlider.h
//
// summary:	Declares the object slider class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Slider or scrollbar. </summary>
	///
	/// <seealso cref="T:UIObject"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UIObjectSlider final :public UIObject
	{
	private:
		/// <summary>	The layout. </summary>
		SliderLayout m_layout;
		/// <summary>	The thumb rectangle. </summary>
		RectangleI m_thumbRect;
		/// <summary>	The track rectangle. </summary>
		RectangleI m_trackRect;
		/// <summary>	The tl reduction. </summary>
		VectorI m_tlReduction;
		/// <summary>	The line break reduction. </summary>
		VectorI m_brReduction;
		/// <summary>	Length of the marker. </summary>
		int m_markerLength{ 0 };
		
		/// <summary>	The value. </summary>
		float m_value{ 0.0f };
		/// <summary>	True to horizontal. </summary>
		bool m_horizontal{ true };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// When slider is used as a scrollbar we can scale the thumb graphic User provides a ratio of
		/// the side of the thumb to the track Ignores if 0.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float m_thumbToTrackRatio{ 0 };

		/// <summary>	The maximum label rectangle. </summary>
		RectangleI m_maxLabelRect{ 0,0 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders to surface thumb. </summary>
		///
		/// <param name="surface">	  	[in,out] The surface. </param>
		/// <param name="surfaceRect">	The surface rectangle. </param>
		/// <param name="sub">		  	The sub. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderToSurfaceThumb(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders the markers to surface. </summary>
		///
		/// <param name="surface">	  	[in,out] The surface. </param>
		/// <param name="surfaceRect">	The surface rectangle. </param>
		/// <param name="sub">		  	The sub. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderMarkersToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const;
		/// <summary>	Calculates the marker length. </summary>
		void CalculateMarkerLength();

		/// <summary>	Form for viewing the user interface editor object slider. </summary>
		friend class UIEditorObjectSliderWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the layout. </summary>
		///
		/// <returns>	The layout. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const SliderLayout& GetLayout() const { return m_layout; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Horrid but needed for editor. </summary>
		///
		/// <returns>	Null if it fails, else the labels and markers. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PointsLabelsAndMarkers* GetLabelsAndMarkers() { return &m_layout.pntsLabsMarks; }
		/// <summary>	Labels have changed. </summary>
		void LabelsHaveChanged() { Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Hasd to be done this way as we need to force a resize. </summary>
		///
		/// <param name="newLayout">	The new layout. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeLayout(SliderLayout newLayout);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates the thumb transform. </summary>
		///
		/// <param name="renderTrans">	[in,out] The render transaction. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CalculateThumbTransform(Transform& renderTrans) const;
	public:
		// Constructs a slider dynamically

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">			[in,out] If non-null, the owner. </param>
		/// <param name="name">				The name. </param>
		/// <param name="tag">				The tag. </param>
		/// <param name="textStyle">		The text style. </param>
		/// <param name="requestedSize">	Size of the requested. </param>
		/// <param name="layout">			The layout. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectSlider(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI requestedSize, SliderLayout layout);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from XML. </summary>
		///
		/// <param name="owner">	[in,out] If non-null, the owner. </param>
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectSlider(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		/// <summary>	Destructor. </summary>
		~UIObjectSlider() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is vertical. </summary>
		///
		/// <returns>	True if vertical, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsVertical() const { return (m_trackRect.Height() > m_trackRect.Width()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overrides required by UIObject. </summary>
		///
		/// <param name="surface">  	[in,out] The surface. </param>
		/// <param name="translate">	The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		/// <summary>	Lost focus. </summary>
		void LostFocus() override final;
		/// <summary>	Gained focus. </summary>
		void GainedFocus() override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Some objects may want to hold on to focus e.g. text entry bool CanGiveUpFocus(const
		/// MouseData& mouseData) override final;
		/// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste Return true if
		/// handled.
		/// </summary>
		///
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin style. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinStyle GetSkinStyle() const override final { if (m_layout.asWindowScrollbar) return ESkinStyle::eScroller; return ESkinStyle::eSlider; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EObjectType GetType() const override final { return EObjectType::eSlider; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change action state. </summary>
		///
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeActionState(EActionState newState) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows setting value directly (if object supports it). Must be ratio between 0 and 1.
		/// </summary>
		///
		/// <param name="ratioValue">	The ratio value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetValue(float ratioValue) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// When slider is used as a scrollbar we can scale the thumb graphic User provides a ratio of
		/// the side of the thumb to the track Ignores if 0 (uses original size)
		/// Values beyone 1 are capped.
		/// </summary>
		///
		/// <param name="ratio">	The ratio. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetThumbToTrackRatio(float ratio) { m_thumbToTrackRatio = std::min(1.0f,ratio); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows getting value directly (if object supports it). What the value means depends on object
		/// type but normally ranges from 0.0f to 1.0f.
		/// </summary>
		///
		/// <returns>	The value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float GetValue() const override final { return m_value; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if we should obay window layout rules. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ShouldObayWindowLayoutRules() const override final { return !m_layout.asWindowScrollbar; }

	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		/// <summary>	Creates the surfaces. </summary>
		void CreateSurfaces() override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Does the rendering. </summary>
		///
		/// <param name="surface">	  	[in,out] The surface. </param>
		/// <param name="surfaceRect">	The surface rectangle. </param>
		/// <param name="sub">		  	The sub. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}
