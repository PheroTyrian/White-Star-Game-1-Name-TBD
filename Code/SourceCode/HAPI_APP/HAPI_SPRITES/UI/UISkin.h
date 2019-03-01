////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UISkin.h
//
// summary:	Declares the skin class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UI\UITypes.h"
#include "UI\UIStyle.h"

namespace HAPISPACE
{
	/// <summary>	Form for viewing the user interface skin editor main. </summary>
	class UISkinEditorMainWindow;
	/// <summary>	Form for viewing the user interface editor object style. </summary>
	class UIEditorObjectStyleWindow;
	/// <summary>	Form for viewing the user interface editor main. </summary>
	class UIEditorMainWindow;
	/// <summary>	An iand skin payload data. </summary>
	class UIandSkinPayloadData;
}

namespace HAPI_UI_SPACE
{	
	/// <summary>	Base sizes for this Skin. </summary>
	struct UiSkinSizes
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Base text size, can be adjusted via specific style or even per object In pixels high.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int m_textSize{ 16 };
		
		/// <summary>	Used in menu, text entry etc. when positioning. </summary>
		int m_textHorizontalGap{ 4 };
		/// <summary>	The text vertical gap. </summary>
		int m_textVerticalGap{ 2 };

		/// <summary>	Used when positioning relative to other objects. </summary>
		int m_objectGapHorizontal{ 10 };
		/// <summary>	The object gap vertical. </summary>
		int m_objectGapVertical{ 10 };

		/// <summary>	Used when positioniong against window edges as an offset. </summary>
		int m_margin{ 6 };
		
		/// <summary>	Additions. </summary>
		int m_shadowWidth{ 4 };
		/// <summary>	The marker length percentage. </summary>
		int m_markerLengthPercentage{ 3 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an XML. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *parent) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads an XML. </summary>
		///
		/// <param name="skinSizesNode">	[in,out] If non-null, the skin sizes node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadXML(CHapiXMLNode *skinSizesNode);
	};

	/// <summary>	Class to hold and manipulate the skin colours and settings. </summary>
	class Skin
	{
	private:
		// Master styles
		/// <summary>	The master text style. </summary>
		UiTextStyleGroup m_masterTextStyle;
		/// <summary>	The master window style. </summary>
		UiWindowStyleGroup m_masterWindowStyle;
	
		/// <summary>	Text styles. One per object type, each with sub types. </summary>
		UiTextStyleGroup m_textStyles[(int)ESkinStyle::eNumSkinStyles];

		/// <summary>	Window styles e.g. for buttons, menu,. </summary>
		UiWindowStyleGroup m_windowStyles[(int)ESkinStyle::eNumSkinStyles];

		/// <summary>	Dont access direct because an instance might be being used. </summary>
		UiSkinSizes m_sizes;

		/// <summary>	Runtime size instances, keyed by window. Only exist if different. </summary>
		std::map<std::string, UiSkinSizes> m_sizeInstancesMap;

		/// <summary>	Skin name. </summary>
		std::string m_name;

		/// <summary>	Colour / shader palette, want it to be ordered for skin editor. </summary>
		std::map<std::string, std::shared_ptr<FillShader>> m_paletteMap;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Do not call directly, called via Scale or ScaleInst Does the actual scaling work on an
		/// instance.
		/// </summary>
		///
		/// <param name="scaling">	The scaling. </param>
		/// <param name="inst">   	[in,out] The instance. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ScaleInst(const VectorF& scaling, UiSkinSizes &inst);		

		/// <summary>	Any additions need addying to constructor that copies. </summary>
		friend class HAPISPACE::UISkinEditorMainWindow;
		/// <summary>	Form for viewing the user interface editor object style. </summary>
		friend class HAPISPACE::UIEditorObjectStyleWindow;
		/// <summary>	Form for viewing the user interface editor main. </summary>
		friend class HAPISPACE::UIEditorMainWindow;
		/// <summary>	An iand skin payload data. </summary>
		friend class HAPISPACE::UIandSkinPayloadData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Note: any changes to these will require a cascade after. </summary>
		///
		/// <returns>	The master text style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiTextStyleGroup& GetMasterTextStyle() { return m_masterTextStyle; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets master window style. </summary>
		///
		/// <returns>	The master window style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiWindowStyleGroup& GetMasterWindowStyle() { return m_masterWindowStyle; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Unlike the scale in public this does an in-place scale, not using instance data, so alters
		/// original Returns false if cannot do e.g. font too small or too big.
		/// </summary>
		///
		/// <param name="scaling">	The scaling. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ScaleThis(const VectorF& scaling);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A call to this should be followed by a refresh / resize of window as will need values
		/// flushing through Note: this does not include instanced size.
		/// </summary>
		///
		/// <returns>	The sizes write. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiSkinSizes& GetSizesWrite() { return m_sizes; }		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Applies any instance data to the skin sizes and removes it reports if more than one exists!
		/// Used by editor on load.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ApplyInstanceData();

		/// <summary>	A system. </summary>
		friend class UISystem;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Careful with this! Does not change name in map, prefer ui RenameSkin fn. </summary>
		///
		/// <param name="newName">	Name of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeName(std::string newName) { m_name = newName; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Replace shader. </summary>
		///
		/// <param name="toReplace">	to replace. </param>
		/// <param name="newName">  	Name of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ReplaceShader(const std::string& toReplace, const std::string& newName);
	public:	
		// Constructor, sets default skin values

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="name">	The name. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skin(std::string name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor from XML. </summary>
		///
		/// <param name="xml">	   	[in,out] The XML. </param>
		/// <param name="skinNode">	[in,out] If non-null, the skin node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skin(CHapiXML &xml, CHapiXMLNode *skinNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct by copying other and using new name. </summary>
		///
		/// <param name="toCopy"> 	to copy. </param>
		/// <param name="newName">	Name of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skin(const std::shared_ptr<Skin>& toCopy, std::string newName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Updates all style groups from master (where inherited)
		/// Must be called when master or child changed.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CascadeStyles();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Prefer to use the Set functions over these. </summary>
		///
		/// <returns>	The palette map. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::map<std::string, std::shared_ptr<FillShader>>& GetPaletteMap() const { return m_paletteMap; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets text style group. </summary>
		///
		/// <param name="which">	The which. </param>
		///
		/// <returns>	The text style group. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiTextStyleGroup& GetTextStyleGroup(ESkinStyle which) { return m_textStyles[(int)which]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style group. </summary>
		///
		/// <param name="which">	The which. </param>
		///
		/// <returns>	The window style group. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		UiWindowStyleGroup& GetWindowStyleGroup(ESkinStyle which) { return m_windowStyles[(int)which]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets scaling ratio. </summary>
		///
		/// <param name="oldWidth"> 	Width of the old. </param>
		/// <param name="oldHeight">	Height of the old. </param>
		/// <param name="newWidth"> 	Width of the new. </param>
		/// <param name="newHeight">	Height of the new. </param>
		///
		/// <returns>	The scaling ratio. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorF GetScalingRatio(int oldWidth,int oldHeight,int newWidth, int newHeight) const;

		// Has light and dark grid
		//static std::unique_ptr<PatternFill> TransparentShader;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name. </summary>
		///
		/// <returns>	The name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& GetName() const { return m_name; }

		/// <summary>	The horrid pink. </summary>
		static std::shared_ptr<FillShader> kHorridPink;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves an XML. </summary>
		///
		/// <param name="root">	[in,out] If non-null, the root. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SaveXML(CHapiXMLNode *root) const;		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a shader to palette to 'shader'. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="name">  	The name. </param>
		/// <param name="shader">	The shader. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		void AddShaderToPalette(const std::string &name, std::shared_ptr<T> shader)
		{
			// Replace if required
			if (m_paletteMap.find(name) != m_paletteMap.end())
				m_paletteMap.erase(name);

			m_paletteMap[name] = shader;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if 'name' does shader exist. </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DoesShaderExist(const std::string& name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves a shader by name If does not exist returns horrid pink and emits error, use above
		/// to check for existance.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the shader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		FillShader* GetShader(const std::string& name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves a colour shader by name If the shader under than name is not a colour (or does not
		/// exist)
		/// returns nullptr and raises an error.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the colourshader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColourFill* GetColourshader(const std::string &name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves a gradient shader by name If the shader under than name is not a gradeint (or does
		/// not exist)
		/// returns nullptr and raises an error.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the gradient shader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GradientFill* GetGradientShader(const std::string &name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves a pattern shader by name If the shader under than name is not a pattern (or does
		/// not exist)
		/// returns nullptr and raises an error.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the pattern shader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PatternFill* GetPatternShader(const std::string &name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves an image shader by name If the shader under than name is not a image (or does not
		/// exist)
		/// returns nullptr and raises an error.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	Null if it fails, else the image shader. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ImageFill* GetImageShader(const std::string &name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	These could cause issues if done on default skin. </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DeletePaletteShader(const std::string &name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Rename palette shader. </summary>
		///
		/// <param name="oldName">	Name of the old. </param>
		/// <param name="newName">	Name of the new. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenamePaletteShader(const std::string &oldName, const std::string &newName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Duplicate palette shader. </summary>
		///
		/// <param name="toCopyName">	Name of to copy. </param>
		/// <param name="newCopyame">	The new copyame. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DuplicatePaletteShader(const std::string &toCopyName, const std::string &newCopyame);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// If window has no instance (not scaled) then returns normal sizes otherwise a scaled version.
		/// </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		///
		/// <returns>	The sizes. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiSkinSizes& GetSizes(const std::string& windowName) const;

		// Create a new instance for this window with supplied sizes
		void SetSizes(const std::string& windowName, const UiSkinSizes& sizes);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Read access. </summary>
		///
		/// <param name="which">	The which. </param>
		///
		/// <returns>	The text style group. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiTextStyleGroup& GetTextStyleGroup(ESkinStyle which) const { return m_textStyles[(int)which]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style group. </summary>
		///
		/// <param name="which">	The which. </param>
		///
		/// <returns>	The window style group. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiWindowStyleGroup& GetWindowStyleGroup(ESkinStyle which) const { return m_windowStyles[(int)which]; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets text style. </summary>
		///
		/// <param name="which">	The which. </param>
		/// <param name="sub">  	The sub. </param>
		///
		/// <returns>	The text style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiTextStyle& GetTextStyle(ESkinStyle which, ESkinSubStyle sub) const { return GetTextStyleGroup(which).GetSub(sub); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets window style. </summary>
		///
		/// <param name="which">	The which. </param>
		/// <param name="sub">  	The sub. </param>
		///
		/// <returns>	The window style. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const UiWindowStyle& GetWindowStyle(ESkinStyle which, ESkinSubStyle sub) const { return GetWindowStyleGroup(which).GetSub(sub);}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Creates a scaled skin sizes instance for the window Returns false if cannot do e.g. font too
		/// small or too big.
		/// </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		/// <param name="scaling">   	The scaling. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Scale(const std::string &windowName, const VectorF& scaling);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes window scaling instance. </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RemoveInstance(const std::string &windowName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets text baseline as height from top. </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		/// <param name="which">	 	The which. </param>
		/// <param name="sub">		 	The sub. </param>
		///
		/// <returns>	The baseline. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetBaseline(const std::string& windowName, ESkinStyle which, ESkinSubStyle sub) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Gets largest of all subtypes in terms of bounding rect Note: rect can have non zero left and
		/// top.
		/// </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		/// <param name="which">	 	The which. </param>
		/// <param name="text">		 	The text. </param>
		///
		/// <returns>	The largest text rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetLargestTextRect(const std::string& windowName,ESkinStyle which, const std::string &text) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// As above when you have access to a group Note: rect can have non zero left and top.
		/// </summary>
		///
		/// <param name="windowName">	Name of the window. </param>
		/// <param name="group">	 	The group. </param>
		/// <param name="text">		 	The text. </param>
		///
		/// <returns>	The largest text rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetLargestTextRect(const std::string& windowName, const UiTextStyleGroup& group, const std::string &text) const;
	};
}