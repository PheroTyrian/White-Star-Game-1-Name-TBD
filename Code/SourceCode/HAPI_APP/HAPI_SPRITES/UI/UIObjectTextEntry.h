#pragma once
#include "UI\UIObject.h"

namespace HAPI_UI_SPACE
{
	class UIEditorObjectTextEntryWindow;

	// Text entry
	class UIObjectTextEntry final : public UIObject
	{
	public:
		// Some default lambas
		static std::function<bool(char newChar, const std::string& newString)> kEverything;
		static std::function<bool(char newChar, const std::string& newString)> kAlphaNumeric;
		static std::function<bool(char newChar, const std::string& newString)> kNumbersOnly;
		static std::function<bool(char newChar, const std::string& newString)> kColourChannel;
		static std::function<bool(char newChar, const std::string& newString)> kDegrees;
		static std::function<bool(char newChar, const std::string& newString)> kPercent;
		
		static std::string sClipboard;
	private:
		bool m_highLighting{ false };
		bool m_controlDown{ false };
		bool m_isFloat{ false };
		int m_highlightStartCursorPos{ 0 };
		int m_highlightEndCursorPos{ 0 };
		int m_cursorPos{ 0 };
		int m_cursorFrameNumber{ 4 };

		// Since this takes time to calculate, caching the result to avoid calling GetTextRect each render
		RectangleI m_textRect;

		// An auto-generated style for the cursor
		UiWindowStyle m_cursorStyle;

		int m_numVisibleChars{ 1 };
		int m_renderTextCursorOffset{ 0 };

		std::string m_text;
		std::string m_lastText; // prior to edit
		std::string m_renderText;

		mutable DWORD m_lastCursorFlashTime{ 0 };
		mutable  bool m_showCursor{ true };
		
		// Lambda for validating an input char
		ETextEntryLambdas m_lambdaType;
		std::function<bool(char newChar, const std::string& newString)> m_validateLambda;

		// Calcs string to actually draw and the position of the cursor within it
		void CalculateStringToRender();
		int GetHorizontalTextGap() const;
		int GetVerticalTextGap() const;

	public:
		ETextEntryLambdas GetValidatingLambdaType() const { return m_lambdaType; }
		void SetNumVisibleChars(int numVisible);
		int GetNumVisibleChars() const { return m_numVisibleChars; }
		void ClearHighlighting();
	public:
		UIObjectTextEntry(UIWindow* owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleChars,
			ETextEntryLambdas validatingLambda,bool isFLoat);

		UIObjectTextEntry::UIObjectTextEntry(UIWindow*owner, std::string name, std::string tag, UiTextStyleGroup textStyle, int numVisibleChars,
			std::function<bool(char newChar, const std::string& newString)> validateLambda = UIObjectTextEntry::kEverything);		

		// Construct from XML
		UIObjectTextEntry(UIWindow* owner, CHapiXML &xml, CHapiXMLNode *root);

		// If true, text as a number is shown with decimal point and icrement decrement change to .1 
		// Does nothing if the text is a not a number
		void SetIsFloat(bool isFloat) { m_isFloat = true; }
		bool GetIsFloat() const { return m_isFloat; }

		void ChangeValidatingLambda(ETextEntryLambdas inbuiltType);
		void ChangeValidatingLambda(std::function<bool(char newChar, const std::string& newString)> validateLambda);

		// Allows setting text directly
		void SetText(const std::string &newText) override final;
		std::string GetText() const override final { return m_text; }

		// These come from mappings of key / mouse combinations e.g. CTRL-C is ePaste
		// Return true if handled
		bool HandleInputAction(EInputMappingAction action, EInputMappingAction lastAction) override final;

		bool CanGiveUpFocus(EInputMappingAction action) const override;
		void LostFocus() override final;
		void GainedFocus() override final;
		bool TextEntered(char c) override final;
		//bool KeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
		void Render(std::shared_ptr<Surface> &surface, VectorI translate) const override final;
		bool WriteXML(CHapiXMLNode *rootNode) override final;
		void ChangeActionState(EActionState newState) override final;
		ESkinStyle GetSkinStyle() const override final { return ESkinStyle::eTextEntry; }
		EObjectType GetType() const override final { return EObjectType::eTextEntry; }
	protected:
		// Renders to surfaces and uploads to HW for quick rendering
		void CreateSurfaces() override final;
		// Does the rendering
		void RenderToSurface(std::shared_ptr<Surface> &surface, RectangleI surfaceRect, ESkinSubStyle sub) const override final;
		// Calculates bounds and positioning of gfx etc.
		void CalculateSizes(const RectangleI& newSize, bool forceSize = false) override final;
	};
}