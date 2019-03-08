#pragma once
#include "UI\UITypes.h"

namespace HAPISPACE {
	class UIEditorObjectStyleWindow;
}

namespace HAPI_UI_SPACE
{
	// Style for text
	struct UiTextStyle
	{
		std::string textColour;									// Main text colour
		std::string textOutlineColour;							// Outline colour if thickness>0.0f
		float textOutlineThickness{ 0.0f };						// value of 0.0f means no outline
		int textSizeIncrement{ 0 };								// from UI default value
		unsigned int styleFlags{ 0 };							// see HAPI_TextStyle
																// Inheritance flags
		bool textColour_Inherited{ true };
		bool textOutlineColour_Inherited{ true };
		bool textOutlineThickness_Inherited{ true };
		bool textSizeIncrement_Inherited{ true };
		bool styleFlags_Inherited{ true };

		void SetAllInheritance(bool set);

		// Cascade inherited values into this
		void Cascade(const UiTextStyle& parent);
	};

	// Easiest to group rather than use 2D arrays everywhere
	class UiTextStyleGroup
	{
	private:
		friend class Skin;
		friend class UIObject;
		friend class HAPISPACE::UIEditorObjectStyleWindow;

		UiTextStyle style[(int)ESkinSubStyle::eNumSubStyles];

		// Remove any non-existant palette entries
		void Validate(UIWindow& window);
	public:
		// Optional tyext size increment is applied to all sub styles
		UiTextStyleGroup(int textSizeIncrement=0);
		
		const UiTextStyle& GetSub(ESkinSubStyle sub) const { return style[(size_t)sub]; }
		UiTextStyle& GetSub(ESkinSubStyle sub) { return style[(size_t)sub]; }

		void SetTextColour(ESkinSubStyle sub, const std::string& colour, bool inheritedFlag=false);
		void SetTextOutlineColour(ESkinSubStyle sub, const std::string& colour, bool inheritedFlag = false);
		void SetTextOutlineThickness(ESkinSubStyle sub, float thickness, bool inheritedFlag = false);
		void SetTextSizeIncrement(ESkinSubStyle sub, int increment, bool inheritedFlag = false);
		void SetTextStyleFlags(ESkinSubStyle sub, unsigned int styleFlags, bool inheritedFlag = false);		

		// Which just used for writing attribute
		bool SaveXML(CHapiXMLNode *parent, ESkinStyle which,bool asMaster=false) const;
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *root, ESkinStyle &style);
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *skinTextStyleNode);

		void SetAllInheritance(bool set)
		{
			for (size_t i = 0; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				style[i].SetAllInheritance(set);
		}

		// Cascade inherited values into this
		void Cascade(const UiTextStyleGroup& parent);
	};

	// Style for object backgrounds etc.
	struct UiWindowStyle
	{
		std::string borderShader;
		std::string backgroundShader;
		std::string shadowShader;
		int borderWidth{ 0 };			// max is 10
		float curvature{ 0.0f };
		bool hasShadow{ true };

		// Inheritance flags
		bool borderShader_Inherited{ true };
		bool backgroundShader_Inherited{ true };
		bool shadowShader_Inherited{ true };

		bool borderWidth_Inherited{ true };
		bool curvature_Inherited{ true };
		bool hasShadow_Inherited{ true };		

		void SetAllInheritance(bool set);

		// Cascade inherited values into this
		void Cascade(const UiWindowStyle& parent);
	};

	class UiWindowStyleGroup
	{
	private:
		friend class Skin;
		friend class UIObject;
		friend class UIEditorObjectStyleWindow;
		UiWindowStyle style[(int)ESkinSubStyle::eNumSubStyles];
		// Remove any non-existant palette entries
		void Validate(UIWindow& window);
	public:
		

		void SetBorderShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);
		void SetBackgroundShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);
		void SetBorderWidth(ESkinSubStyle sub, int width, bool inheritedFlag = false);
		void SetCurvature(ESkinSubStyle sub, float curvature, bool inheritedFlag = false);
		void SetHasShadow(ESkinSubStyle sub, bool hasShadow, bool inheritedFlag = false);
		void SetShadowShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);

		// Returns largest border of all substyles
		int GetLargestBorder() const
		{
			int ret{ style[0].borderWidth };
			for (size_t i = 1; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				ret = std::max(style[i].borderWidth, ret);
			return ret;
		}

		// Cascade inherited values into this group from parent
		void Cascade(const UiWindowStyleGroup& parent);

		const UiWindowStyle& GetSub(ESkinSubStyle sub) const { return style[(size_t)sub]; }
		UiWindowStyle& GetSub(ESkinSubStyle sub) { return style[(size_t)sub]; }

		bool SaveXML(CHapiXMLNode *parent, ESkinStyle which, bool asMaster=false) const;
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *root, ESkinStyle &style);
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *skinWindowStyleNode);

		void SetAllInheritance(bool set)
		{
			for (size_t i = 0; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				style[i].SetAllInheritance(set);
		}
	};
}

