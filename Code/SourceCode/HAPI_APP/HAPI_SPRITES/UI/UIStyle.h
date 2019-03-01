////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIStyle.h
//
// summary:	Declares the style class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UITypes.h"

namespace HAPISPACE {
	/// <summary>	Form for viewing the user interface editor object style. </summary>
	class UIEditorObjectStyleWindow;
}

namespace HAPI_UI_SPACE
{
	/// <summary>	Style for text. </summary>
	struct UiTextStyle
	{
		/// <summary>	Main text colour. </summary>
		std::string textColour;
		/// <summary>	Outline colour if thickness>0.0f. </summary>
		std::string textOutlineColour;
		/// <summary>	value of 0.0f means no outline. </summary>
		float textOutlineThickness{ 0.0f };
		/// <summary>	from UI default value. </summary>
		int textSizeIncrement{ 0 };
		/// <summary>	see HAPI_TextStyle. </summary>
		unsigned int styleFlags{ 0 };
		/// <summary>	Inheritance flags. </summary>
		bool textColour_Inherited{ true };
		/// <summary>	True if text outline colour inherited. </summary>
		bool textOutlineColour_Inherited{ true };
		/// <summary>	True if text outline thickness inherited. </summary>
		bool textOutlineThickness_Inherited{ true };
		/// <summary>	True if text size increment inherited. </summary>
		bool textSizeIncrement_Inherited{ true };
		/// <summary>	True if style flags inherited. </summary>
		bool styleFlags_Inherited{ true };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets all inheritance. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAllInheritance(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cascade inherited values into this. </summary>
		///
		/// <param name="parent">	The parent. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Cascade(const UiTextStyle& parent);
	};

	/// <summary>	Easiest to group rather than use 2D arrays everywhere. </summary>
	class UiTextStyleGroup
	{
	private:
		/// <summary>	A skin. </summary>
		friend class Skin;
		/// <summary>	An object. </summary>
		friend class UIObject;
		/// <summary>	Form for viewing the user interface editor object style. </summary>
		friend class HAPISPACE::UIEditorObjectStyleWindow;

		/// <summary>	The style[(int) e skin sub style number sub styles]. </summary>
		UiTextStyle style[(int)ESkinSubStyle::eNumSubStyles];

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove any non-existant palette entries. </summary>
		///
		/// <param name="window">	[in,out] The window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Validate(UIWindow& window);
	public:
		// Optional tyext size increment is applied to all sub styles

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="textSizeIncrement">	(Optional) The text size increment. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiTextStyleGroup(int textSizeIncrement=0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a sub. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The sub. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiTextStyle& GetSub(ESkinSubStyle sub) const { return style[(size_t)sub]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a sub. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The sub. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiTextStyle& GetSub(ESkinSubStyle sub) { return style[(size_t)sub]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets text colour. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="colour">			The colour. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextColour(ESkinSubStyle sub, const std::string& colour, bool inheritedFlag=false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets text outline colour. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="colour">			The colour. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextOutlineColour(ESkinSubStyle sub, const std::string& colour, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets text outline thickness. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="thickness">		The thickness. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextOutlineThickness(ESkinSubStyle sub, float thickness, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets text size increment. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="increment">		Amount to increment by. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextSizeIncrement(ESkinSubStyle sub, int increment, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets text style flags. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="styleFlags">   	The style flags. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetTextStyleFlags(ESkinSubStyle sub, unsigned int styleFlags, bool inheritedFlag = false);		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Which just used for writing attribute. </summary>
		///
		/// <param name="parent">  	[in,out] If non-null, the parent. </param>
		/// <param name="which">   	The which. </param>
		/// <param name="asMaster">	(Optional) True to as master. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent, ESkinStyle which,bool asMaster=false) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads an XML. </summary>
		///
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		/// <param name="style">	[in,out] The style. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *root, ESkinStyle &style);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads an XML. </summary>
		///
		/// <param name="xml">					[in,out] The XML. </param>
		/// <param name="skinTextStyleNode">	[in,out] If non-null, the skin text style node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *skinTextStyleNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets all inheritance. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAllInheritance(bool set)
		{
			for (size_t i = 0; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				style[i].SetAllInheritance(set);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cascade inherited values into this. </summary>
		///
		/// <param name="parent">	The parent. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Cascade(const UiTextStyleGroup& parent);
	};

	/// <summary>	Style for object backgrounds etc. </summary>
	struct UiWindowStyle
	{
		/// <summary>	The border shader. </summary>
		std::string borderShader;
		/// <summary>	The background shader. </summary>
		std::string backgroundShader;
		/// <summary>	The shadow shader. </summary>
		std::string shadowShader;
		/// <summary>	max is 10. </summary>
		int borderWidth{ 0 };
		/// <summary>	The curvature. </summary>
		float curvature{ 0.0f };
		/// <summary>	True if has shadow, false if not. </summary>
		bool hasShadow{ true };

		/// <summary>	Inheritance flags. </summary>
		bool borderShader_Inherited{ true };
		/// <summary>	True if background shader inherited. </summary>
		bool backgroundShader_Inherited{ true };
		/// <summary>	True if shadow shader inherited. </summary>
		bool shadowShader_Inherited{ true };

		/// <summary>	True if border width inherited. </summary>
		bool borderWidth_Inherited{ true };
		/// <summary>	True if curvature inherited. </summary>
		bool curvature_Inherited{ true };
		/// <summary>	True if has shadow inherited, false if not. </summary>
		bool hasShadow_Inherited{ true };		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets all inheritance. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAllInheritance(bool set);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cascade inherited values into this. </summary>
		///
		/// <param name="parent">	The parent. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Cascade(const UiWindowStyle& parent);
	};

	/// <summary>	A window style group. </summary>
	class UiWindowStyleGroup
	{
	private:
		/// <summary>	A skin. </summary>
		friend class Skin;
		/// <summary>	An object. </summary>
		friend class UIObject;
		/// <summary>	Form for viewing the user interface editor object style. </summary>
		friend class UIEditorObjectStyleWindow;
		/// <summary>	The style[(int) e skin sub style number sub styles]. </summary>
		UiWindowStyle style[(int)ESkinSubStyle::eNumSubStyles];

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove any non-existant palette entries. </summary>
		///
		/// <param name="window">	[in,out] The window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Validate(UIWindow& window);
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets border shader. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="shader">			The shader. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBorderShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets background shader. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="shader">			The shader. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBackgroundShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets border width. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="width">			The width. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBorderWidth(ESkinSubStyle sub, int width, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a curvature. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="curvature">		The curvature. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetCurvature(ESkinSubStyle sub, float curvature, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets has shadow. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="hasShadow">		True if has shadow, false if not. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetHasShadow(ESkinSubStyle sub, bool hasShadow, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets shadow shader. </summary>
		///
		/// <param name="sub">				The sub. </param>
		/// <param name="shader">			The shader. </param>
		/// <param name="inheritedFlag">	(Optional) True to inherited flag. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetShadowShader(ESkinSubStyle sub, const std::string& shader, bool inheritedFlag = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns largest border of all substyles. </summary>
		///
		/// <returns>	The largest border. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetLargestBorder() const
		{
			int ret{ style[0].borderWidth };
			for (size_t i = 1; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				ret = std::max(style[i].borderWidth, ret);
			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cascade inherited values into this group from parent. </summary>
		///
		/// <param name="parent">	The parent. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Cascade(const UiWindowStyleGroup& parent);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a sub. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The sub. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiWindowStyle& GetSub(ESkinSubStyle sub) const { return style[(size_t)sub]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a sub. </summary>
		///
		/// <param name="sub">	The sub. </param>
		///
		/// <returns>	The sub. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiWindowStyle& GetSub(ESkinSubStyle sub) { return style[(size_t)sub]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an XML. </summary>
		///
		/// <param name="parent">  	[in,out] If non-null, the parent. </param>
		/// <param name="which">   	The which. </param>
		/// <param name="asMaster">	(Optional) True to as master. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent, ESkinStyle which, bool asMaster=false) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads an XML. </summary>
		///
		/// <param name="xml">  	[in,out] The XML. </param>
		/// <param name="root"> 	[in,out] If non-null, the root. </param>
		/// <param name="style">	[in,out] The style. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *root, ESkinStyle &style);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads an XML. </summary>
		///
		/// <param name="xml">				  	[in,out] The XML. </param>
		/// <param name="skinWindowStyleNode">	[in,out] If non-null, the skin window style node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXML &xml, CHapiXMLNode *skinWindowStyleNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets all inheritance. </summary>
		///
		/// <param name="set">	True to set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAllInheritance(bool set)
		{
			for (size_t i = 0; i < (int)ESkinSubStyle::eNumSubStyles; i++)
				style[i].SetAllInheritance(set);
		}
	};
}

