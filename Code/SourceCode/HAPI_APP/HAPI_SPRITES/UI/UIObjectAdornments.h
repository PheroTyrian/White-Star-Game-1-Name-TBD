////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObjectAdornments.h
//
// summary:	Declares the object adornments class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// An object adornments. Can be used as background for a window Fill colour and border colour
	/// come from style unless overwritten.
	/// </summary>
	///
	/// <seealso cref="T:UIObject"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UIObjectAdornments final : public UIObject
	{
	private:
		/// <summary>	The title bar rectangle. </summary>
		RectangleI m_titleBarRect;
		/// <summary>	The menu bar rectangle. </summary>
		RectangleI m_menuBarRect;
		/// <summary>	The sizing area rectangle. </summary>
		RectangleI m_sizingAreaRect;
		/// <summary>	The text position. </summary>
		VectorI m_textPos;
		/// <summary>	The cross position. </summary>
		VectorI m_crossPos;
		/// <summary>	The minimise position. </summary>
		VectorI m_minimisePos;
		/// <summary>	The drag start. </summary>
		VectorI m_dragStart;

		/// <summary>	For the minimise, maximise and close buttons. </summary>
		VectorI m_iconSize;
		/// <summary>	The close frame number. </summary>
		int m_closeFrameNum{ 0 };
		/// <summary>	The minimise frame number. </summary>
		int m_minimiseFrameNum{ 0 };
		/// <summary>	The maximise frame number. </summary>
		int m_maximiseFrameNum{ 0 };
		/// <summary>	Different when minimised. </summary>
		RectangleI m_minimisedBounds;
		/// <summary>	So can restore after minimisation. </summary>
		RectangleI m_oldBounds;

		/// <summary>	True to sizing. </summary>
		bool m_sizing{ false };
		/// <summary>	The new size rectangle. </summary>
		RectangleI m_newSizeRect;
		
		/// <summary>	The title text. </summary>
		std::string m_titleText;

		/// <summary>	The features flags. </summary>
		unsigned int m_featuresFlags;
				
		/// <summary>	True to previous mouse down state. </summary>
		bool m_previousMouseDownState{ false };		
		/// <summary>	True to highlight cross. </summary>
		bool m_highlightCross{ false };
		/// <summary>	True to highlight minimise. </summary>
		bool m_highlightMinimise{ false };		
	public:	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">			[in,out] If non-null, the owner. </param>
		/// <param name="name">				The name. </param>
		/// <param name="tag">				The tag. </param>
		/// <param name="textStyle">		The text style. </param>
		/// <param name="dimensions">   	The dimensions. </param>
		/// <param name="featuresFlags">	The features flags. </param>
		/// <param name="titleText">		The title text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectAdornments(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, RectangleI dimensions,
			unsigned int featuresFlags,std::string titleText);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		 	[in,out] If non-null, the owner. </param>
		/// <param name="name">			 	The name. </param>
		/// <param name="xml">			 	[in,out] The XML. </param>
		/// <param name="adornmentsNode">	[in,out] If non-null, the adornments node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectAdornments(UIWindow* owner, std::string name, CHapiXML &xml, CHapiXMLNode *adornmentsNode);
		/// <summary>	Destructor. </summary>
		~UIObjectAdornments() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows change after creation. </summary>
		///
		/// <param name="which">	The which. </param>
		/// <param name="set">  	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetFeatureFlag(WindowFeatures::EAdornmentFeatures which, bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Owning window is minimised. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void OwningWindowIsMinimised(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// For adornments refers to window title Allows setting text directly (if object supports it)
		/// </summary>
		///
		/// <param name="newText">	The new text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetText(const std::string &newText) override final { m_titleText = newText;  Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows getting text directly (if supported else empty) </summary>
		///
		/// <returns>	The text. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string GetText() const override final { return m_titleText; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets client rectangle. </summary>
		///
		/// <returns>	The client rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetClientRect() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets menu area. </summary>
		///
		/// <returns>	The menu area. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetMenuArea() const { return m_menuBarRect; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets features flags. </summary>
		///
		/// <returns>	The features flags. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int GetFeaturesFlags() const { return m_featuresFlags; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets user draw. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetUserDraw() const { return (m_featuresFlags & WindowFeatures::eUserDraw || m_featuresFlags & WindowFeatures::eSystem); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets should scale on size. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetShouldScaleOnSize() const { return (m_featuresFlags & WindowFeatures::eScaleWhenSized); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Some objects may need multiple rects to describe their 'hit' areas. </summary>
		///
		/// <returns>	The window space collision rectangles. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
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
		/// <summary>	Lost focus. </summary>
		void LostFocus() override final;
		/// <summary>	Gained focus. </summary>
		void GainedFocus() override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if we can give up focus. </summary>
		///
		/// <param name="action">	The action. </param>
		///
		/// <returns>	True if we can give up focus, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CanGiveUpFocus(EInputMappingAction action) const override;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders this object. </summary>
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
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets render priority. </summary>
		///
		/// <returns>	The render priority. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ERenderPriority GetRenderPriority() const override final { return ERenderPriority::eFirst; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin style. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eWindowNormal; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EObjectType GetType() const override final { return EObjectType::eAdornment; }

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