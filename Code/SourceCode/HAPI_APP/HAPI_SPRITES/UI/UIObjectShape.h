////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObjectShape.h
//
// summary:	Declares the object shape class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	// Shape
	class UIObjectShape final : public UIObject
	{
	private:	
		/// <summary>	The caption. </summary>
		std::string m_caption;
	public:
		// Constructs a shape to rect size

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		[in,out] If non-null, the owner. </param>
		/// <param name="name">			The name. </param>
		/// <param name="tag">			The tag. </param>
		/// <param name="textStyle">	The text style. </param>
		/// <param name="rect">			The rectangle. </param>
		/// <param name="caption">  	The caption. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectShape(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI rect, std::string caption);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from XML. </summary>
		///
		/// <param name="owner">	[in,out] If non-null, the owner. </param>
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectShape(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		/// <summary>	Destructor. </summary>
		~UIObjectShape() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste Return true if
		/// handled.
		/// </summary>
		///
		/// <param name="surface">  	[in,out] The surface. </param>
		/// <param name="translate">	The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////		
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change action state. </summary>
		///
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeActionState(EActionState newState) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handles the input action. </summary>
		///
		/// <param name="action">	 	The action. </param>
		/// <param name="lastAction">	The last action. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;
		/// <summary>	Lost focus. </summary>
		void LostFocus() override final {};
		/// <summary>	Gained focus. </summary>
		void GainedFocus() override final {};	

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin style. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eShape; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EObjectType GetType() const override final { return EObjectType::eShape; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows setting text directly (if object supports it) </summary>
		///
		/// <param name="newText">	The new text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetText(const std::string &newText) override final { if (newText == m_caption) return;  m_caption = newText; Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows getting text directly (if supported else empty) </summary>
		///
		/// <returns>	The text. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string GetText() const override final { return m_caption; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	priority 0 is rendered first. </summary>
		///
		/// <returns>	The render priority. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ERenderPriority GetRenderPriority() const override final { return ERenderPriority::eFirst; }
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
