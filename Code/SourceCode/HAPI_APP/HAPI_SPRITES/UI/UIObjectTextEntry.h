////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIObjectTextEntry.h
//
// summary:	Declares the object text entry class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Form for viewing the user interface editor object text entry. </summary>
	///
	/// <seealso cref="T:UIObject"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UIEditorObjectTextEntryWindow;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Text entry. </summary>
	///
	/// <seealso cref="T:UIObject"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class UIObjectTextEntry final : public UIObject
	{
	public:
		// Some default lambas
		/// <summary>	The everything. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kEverything;
		/// <summary>	The alpha numeric. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kAlphaNumeric;
		/// <summary>	The numbers only. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kNumbersOnly;
		/// <summary>	The colour channel. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kColourChannel;
		/// <summary>	The degrees. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kDegrees;
		/// <summary>	The percent. </summary>
		static std::function<bool(char newChar, const std::string& newString)> kPercent;
		
		/// <summary>	The clipboard. </summary>
		static std::string sClipboard;
	private:
		/// <summary>	True to high lighting. </summary>
		bool m_highLighting{ false };
		/// <summary>	True to control down. </summary>
		bool m_controlDown{ false };
		/// <summary>	True if is float, false if not. </summary>
		bool m_isFloat{ false };
		/// <summary>	The highlight start cursor position. </summary>
		int m_highlightStartCursorPos{ 0 };
		/// <summary>	The highlight end cursor position. </summary>
		int m_highlightEndCursorPos{ 0 };
		/// <summary>	The cursor position. </summary>
		int m_cursorPos{ 0 };
		/// <summary>	The cursor frame number. </summary>
		int m_cursorFrameNumber{ 4 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Since this takes time to calculate, caching the result to avoid calling GetTextRect each
		/// render.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI m_textRect;

		/// <summary>	An auto-generated style for the cursor. </summary>
		UiWindowStyle m_cursorStyle;

		/// <summary>	Number of visible characters. </summary>
		int m_numVisibleChars{ 1 };
		/// <summary>	The render text cursor offset. </summary>
		int m_renderTextCursorOffset{ 0 };

		/// <summary>	The text. </summary>
		std::string m_text;
		/// <summary>	prior to edit. </summary>
		std::string m_lastText;
		/// <summary>	The render text. </summary>
		std::string m_renderText;

		/// <summary>	The last cursor flash time. </summary>
		mutable DWORD m_lastCursorFlashTime{ 0 };
		/// <summary>	True to show, false to hide the cursor. </summary>
		mutable  bool m_showCursor{ true };
		
		/// <summary>	Lambda for validating an input char. </summary>
		ETextEntryLambdas m_lambdaType;
		/// <summary>	The validate lambda. </summary>
		std::function<bool(char newChar, const std::string& newString)> m_validateLambda;

		/// <summary>	Calcs string to actually draw and the position of the cursor within it. </summary>
		void CalculateStringToRender();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets horizontal text gap. </summary>
		///
		/// <returns>	The horizontal text gap. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetHorizontalTextGap() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vertical text gap. </summary>
		///
		/// <returns>	The vertical text gap. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetVerticalTextGap() const;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets validating lambda type. </summary>
		///
		/// <returns>	The validating lambda type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ETextEntryLambdas GetValidatingLambdaType() const { return m_lambdaType; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets number visible characters. </summary>
		///
		/// <param name="numVisible">	Number of visibles. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetNumVisibleChars(int numVisible);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets number visible characters. </summary>
		///
		/// <returns>	The number visible characters. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumVisibleChars() const { return m_numVisibleChars; }
		/// <summary>	Clears the highlighting. </summary>
		void ClearHighlighting();
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		   	[in,out] If non-null, the owner. </param>
		/// <param name="name">			   	The name. </param>
		/// <param name="tag">			   	The tag. </param>
		/// <param name="textStyle">	   	The text style. </param>
		/// <param name="numVisibleChars"> 	Number of visible characters. </param>
		/// <param name="validatingLambda">	The validating lambda. </param>
		/// <param name="isFLoat">		   	True if is f loat, false if not. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectTextEntry(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleChars,
			ETextEntryLambdas validatingLambda,bool isFLoat);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="owner">		  	[in,out] If non-null, the owner. </param>
		/// <param name="name">			  	The name. </param>
		/// <param name="tag">			  	The tag. </param>
		/// <param name="textStyle">	  	The text style. </param>
		/// <param name="numVisibleChars">	Number of visible characters. </param>
		/// <param name="validateLambda"> 	(Optional) The validate lambda. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectTextEntry::UIObjectTextEntry(UIWindow*owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleChars,
			std::function<bool(char newChar, const std::string& newString)> validateLambda = UIObjectTextEntry::kEverything);		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from XML. </summary>
		///
		/// <param name="owner">	[in,out] If non-null, the owner. </param>
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UIObjectTextEntry(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// If true, text as a number is shown with decimal point and icrement decrement change to .1
		/// Does nothing if the text is a not a number.
		/// </summary>
		///
		/// <param name="isFloat">	True if is float, false if not. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetIsFloat(bool isFloat) { m_isFloat = true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets is float. </summary>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetIsFloat() const { return m_isFloat; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change validating lambda. </summary>
		///
		/// <param name="inbuiltType">	Type of the inbuilt. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeValidatingLambda(ETextEntryLambdas inbuiltType);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change validating lambda. </summary>
		///
		/// <param name="validateLambda">	The validate lambda. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeValidatingLambda(std::function<bool(char newChar, const std::string& newString)> validateLambda);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows setting text directly. </summary>
		///
		/// <param name="newText">	The new text. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetText(const std::string &newText) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the text. </summary>
		///
		/// <returns>	The text. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string GetText() const override final { return m_text; }

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
		/// <summary>	Determine if we can give up focus. </summary>
		///
		/// <param name="action">	The action. </param>
		///
		/// <returns>	True if we can give up focus, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CanGiveUpFocus(EInputMappingAction action) const override;
		/// <summary>	Lost focus. </summary>
		void LostFocus() override final;
		/// <summary>	Gained focus. </summary>
		void GainedFocus() override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Text entered. </summary>
		///
		/// <param name="c">	The character. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool TextEntered(char c) override final;
		//bool KeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders this object. </summary>
		///
		/// <param name="surface">  	[in,out] The surface. </param>
		/// <param name="translate">	The translate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="rootNode">	[in,out] If non-null, the root node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool WriteXML(CHapiXMLNode *rootNode) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change action state. </summary>
		///
		/// <param name="newState">	New state. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeActionState(EActionState newState) override final;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets skin style. </summary>
		///
		/// <returns>	The skin style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eTextEntry; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EObjectType GetType() const override final { return EObjectType::eTextEntry; }
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