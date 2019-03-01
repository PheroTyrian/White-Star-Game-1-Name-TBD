////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObjectListBox.h
//
// summary:	Declares the object list box class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Basic list box. </summary>
	///
	/// <seealso cref="T:UIObject"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UIObjectListBox final : public UIObject
	{
	private:	
		// Creation params
		/// <summary>	The type. </summary>
		EListBoxType m_type;
		/// <summary>	Number of visible rows. </summary>
		int m_numVisibleRows;
		/// <summary>	The items. </summary>
		std::vector<std::string> m_items;
		/// <summary>	if 0, sizes to content. </summary>
		int m_numCharsWide{ 0 };

		/// <summary>	Temps. </summary>
		VectorI m_firstRowPos;
		
		/// <summary>	Height of the row. </summary>
		int m_rowHeight;
		/// <summary>	The selected row. </summary>
		int m_selectedRow{ -1 };
		/// <summary>	The pressed row. </summary>
		int m_pressedRow{ -1 };
		/// <summary>	The rows scrolled. </summary>
		int m_rowsScrolled{ 0 };

		/// <summary>	Size of the slider. </summary>
		int m_sliderSize{ 0 };
		/// <summary>	The selected item background frame number. </summary>
		int m_selectedItemBackgroundFrameNum{ 0 };
		/// <summary>	The thumb frame number. </summary>
		int m_thumbFrameNum{ 0 };

		/// <summary>	The unexpanded bounds. </summary>
		RectangleI m_unexpandedBounds;
		/// <summary>	True if fully expanded. </summary>
		bool m_fullyExpanded{ true };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets row rectangle local space. </summary>
		///
		/// <param name="visibleRowNum">	The visible row number. </param>
		///
		/// <returns>	The row rectangle local space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetRowRectLocalSpace(int visibleRowNum) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets thumb y coordinate position from rows scrolled. </summary>
		///
		/// <param name="rowsScrolled">			The rows scrolled. </param>
		/// <param name="centreOfthumbYPos">	The centre ofthumb y coordinate position. </param>
		///
		/// <returns>	The thumb y coordinate position from rows scrolled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float GetThumbYPosFromRowsScrolled(int rowsScrolled, float centreOfthumbYPos) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets rows scrolled from thumb y coordinate position. </summary>
		///
		/// <param name="thumbYPos">	The thumb y coordinate position. </param>
		///
		/// <returns>	The rows scrolled from thumb y coordinate position. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetRowsScrolledFromThumbYPos(int thumbYPos) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handles the mouse over described by pressed. </summary>
		///
		/// <param name="pressed">	True if pressed. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool HandleMouseOver(bool pressed);

		/// <summary>	Form for viewing the user interface editor main. </summary>
		friend class UIEditorMainWindow;
		/// <summary>	Form for viewing the user interface editor object list box. </summary>
		friend class UIEditorObjectListBoxWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets list box type. </summary>
		///
		/// <returns>	The list box type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EListBoxType GetListBoxType() const { return m_type; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change list box type. </summary>
		///
		/// <param name="newType">	Type of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeListBoxType(EListBoxType newType) { m_type = newType; Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets number visible rows. </summary>
		///
		/// <returns>	The number visible rows. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumVisibleRows() const { return m_numVisibleRows; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets number characters wide. </summary>
		///
		/// <returns>	The number characters wide. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumCharsWide() const { return m_numCharsWide; }
		/// <summary>	Form for viewing the user interface editor object canvas. </summary>
		friend class UIEditorObjectCanvasWindow;
		/// <summary>	Form for viewing the user interface editor labels. </summary>
		friend class UIEditorLabelsWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the items. </summary>
		///
		/// <returns>	The items. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::vector<std::string>& GetItems() const { return m_items; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change number visible rows. </summary>
		///
		/// <param name="newNum">	The new number. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeNumVisibleRows(int newNum) { m_numVisibleRows = newNum; Recreate(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change number characters wide. </summary>
		///
		/// <param name="newNum">	The new number. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeNumCharsWide(int newNum) { m_numCharsWide = newNum; Recreate(); }		
	public:
		// Uses skin style for all colours

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		 	[in,out] If non-null, the owner. </param>
		/// <param name="name">			 	The name. </param>
		/// <param name="tag">			 	The tag. </param>
		/// <param name="textStyle">	 	The text style. </param>
		/// <param name="numVisibleRows">	Number of visible rows. </param>
		/// <param name="items">		 	The items. </param>
		/// <param name="type">			 	The type. </param>
		/// <param name="numCharsWide">  	Number of characters wides. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectListBox(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleRows, std::vector<std::string> items,EListBoxType type, int numCharsWide);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from XML. </summary>
		///
		/// <param name="owner">	[in,out] If non-null, the owner. </param>
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectListBox(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);
		/// <summary>	Destructor. </summary>
		~UIObjectListBox()=default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Special functions for list boxes. </summary>
		///
		/// <param name="items">   	The items. </param>
		/// <param name="resizeTo">	(Optional) True to resize to. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ReplaceItems(std::vector<std::string> items, bool resizeTo=false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Selects. </summary>
		///
		/// <param name="selectedIndex">	The selected index. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Select(int selectedIndex);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Selects the given text. </summary>
		///
		/// <param name="text">	The text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Select(const std::string& text);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets selected item index. </summary>
		///
		/// <returns>	The selected item index. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetSelectedItemIndex() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets selected item. </summary>
		///
		/// <returns>	The selected item. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetSelectedItem() const;

		/// <summary>	Overrides required by UIObject. </summary>
		void LostFocus() override final;
		/// <summary>	Gained focus. </summary>
		void GainedFocus() override final;

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
		/// <summary>	Gets skin style. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eListBox; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EObjectType GetType() const override final { return EObjectType::eListBox; }

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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Optional overrides. </summary>
		///
		/// <returns>	The bounding rectangle object space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetBoundingRectangleObjectSpace() const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets maximum bounding rectangle object space. </summary>
		///
		/// <returns>	The maximum bounding rectangle object space. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetMaxBoundingRectangleObjectSpace() const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Drop down needs priority. </summary>
		///
		/// <returns>	The render priority. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ERenderPriority GetRenderPriority() const override final { if (m_type == EListBoxType::eDropDown && m_fullyExpanded) return ERenderPriority::eLast; return ERenderPriority::eDontCare; }
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