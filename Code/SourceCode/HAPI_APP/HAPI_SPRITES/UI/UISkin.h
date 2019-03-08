#pragma once
#include "UI\UITypes.h"
#include "UI\UIStyle.h"

namespace HAPISPACE
{
	class UISkinEditorMainWindow;
	class UIEditorObjectStyleWindow;
	class UIEditorMainWindow;
	class UIandSkinPayloadData;
}

namespace HAPI_UI_SPACE
{	
	// Base sizes for this Skin
	struct UiSkinSizes
	{
		// Base text size, can be adjusted via specific style or even per object
		// In pixels high
		int m_textSize{ 16 };
		
		// Used in menu, text entry etc. when positioning
		int m_textHorizontalGap{ 4 };
		int m_textVerticalGap{ 2 };

		// Used when positioning relative to other objects
		int m_objectGapHorizontal{ 10 };
		int m_objectGapVertical{ 10 };

		// Used when positioniong against window edges as an offset
		int m_margin{ 6 };
		
		// Additions
		int m_shadowWidth{ 4 };
		int m_markerLengthPercentage{ 3 };

		bool SaveXML(CHapiXMLNode *parent) const;
		bool LoadXML(CHapiXMLNode *skinSizesNode);
	};

	// Class to hold and manipulate the skin colours and settings
	class Skin
	{
	private:
		// Master styles
		UiTextStyleGroup m_masterTextStyle;
		UiWindowStyleGroup m_masterWindowStyle;
	
		// Text styles. One per object type, each with sub types
		UiTextStyleGroup m_textStyles[(int)ESkinStyle::eNumSkinStyles];

		// Window styles e.g. for buttons, menu, 
		UiWindowStyleGroup m_windowStyles[(int)ESkinStyle::eNumSkinStyles];

		// Dont access direct because an instance might be being used
		UiSkinSizes m_sizes;

		// Runtime size instances, keyed by window. Only exist if different
		std::map<std::string, UiSkinSizes> m_sizeInstancesMap;

		// Skin name
		std::string m_name;

		// Colour / shader palette, want it to be ordered for skin editor
		std::map<std::string, std::shared_ptr<FillShader>> m_paletteMap;

		// Do not call directly, called via Scale or ScaleInst
		// Does the actual scaling work on an instance 
		bool ScaleInst(const VectorF& scaling, UiSkinSizes &inst);		

		// Any additions need addying to constructor that copies
		friend class HAPISPACE::UISkinEditorMainWindow;
		friend class HAPISPACE::UIEditorObjectStyleWindow;
		friend class HAPISPACE::UIEditorMainWindow;
		friend class HAPISPACE::UIandSkinPayloadData;
		
		// Note: any changes to these will require a cascade after
		UiTextStyleGroup& GetMasterTextStyle() { return m_masterTextStyle; }
		UiWindowStyleGroup& GetMasterWindowStyle() { return m_masterWindowStyle; }

		// Unlike the scale in public this does an in-place scale, not using instance data, so alters original
		// Returns false if cannot do e.g. font too small or too big		
		bool ScaleThis(const VectorF& scaling);

		// A call to this should be followed by a refresh / resize of window as will need values flushing through
		// Note: this does not include instanced size
		UiSkinSizes& GetSizesWrite() { return m_sizes; }		

		// Applies any instance data to the skin sizes and removes it
		// reports if more than one exists! Used by editor on load
		void ApplyInstanceData();

		friend class UISystem;
		// Careful with this! Does not change name in map, prefer ui RenameSkin fn
		void ChangeName(std::string newName) { m_name = newName; }

		void ReplaceShader(const std::string& toReplace, const std::string& newName);
	public:	
		// Constructor, sets default skin values
		Skin(std::string name);

		// Constructor from XML
		Skin(CHapiXML &xml, CHapiXMLNode *skinNode);

		// Construct by copying other and using new name
		Skin(const std::shared_ptr<Skin>& toCopy, std::string newName);

		// Updates all style groups from master (where inherited)
		// Must be called when master or child changed
		void CascadeStyles();

		// Prefer to use the Set functions over these
		const std::map<std::string, std::shared_ptr<FillShader>>& GetPaletteMap() const { return m_paletteMap; }
		UiTextStyleGroup& GetTextStyleGroup(ESkinStyle which) { return m_textStyles[(int)which]; }
		UiWindowStyleGroup& GetWindowStyleGroup(ESkinStyle which) { return m_windowStyles[(int)which]; }

		VectorF GetScalingRatio(int oldWidth,int oldHeight,int newWidth, int newHeight) const;

		// Has light and dark grid
		//static std::unique_ptr<PatternFill> TransparentShader;

		const std::string& GetName() const { return m_name; }

		static std::shared_ptr<FillShader> kHorridPink;
		
		bool SaveXML(CHapiXMLNode *root) const;		

		template <typename T>
		void AddShaderToPalette(const std::string &name, std::shared_ptr<T> shader)
		{
			// Replace if required
			if (m_paletteMap.find(name) != m_paletteMap.end())
				m_paletteMap.erase(name);

			m_paletteMap[name] = shader;
		}

		bool DoesShaderExist(const std::string& name) const;

		// Retrieves a shader by name
		// If does not exist returns horrid pink and emits error, use above to check for existance
		FillShader* GetShader(const std::string& name) const;
		
		// Retrieves a colour shader by name
		// If the shader under than name is not a colour (or does not exist)
		// returns nullptr and raises an error
		ColourFill* GetColourshader(const std::string &name) const;

		// Retrieves a gradient shader by name
		// If the shader under than name is not a gradeint (or does not exist)
		// returns nullptr and raises an error
		GradientFill* GetGradientShader(const std::string &name) const;

		// Retrieves a pattern shader by name
		// If the shader under than name is not a pattern (or does not exist)
		// returns nullptr and raises an error
		PatternFill* GetPatternShader(const std::string &name) const;

		// Retrieves an image shader by name
		// If the shader under than name is not a image (or does not exist)
		// returns nullptr and raises an error
		ImageFill* GetImageShader(const std::string &name) const;

		// These could cause issues if done on default skin
		bool DeletePaletteShader(const std::string &name);
		void RenamePaletteShader(const std::string &oldName, const std::string &newName);
		void DuplicatePaletteShader(const std::string &toCopyName, const std::string &newCopyame);

		// If window has no instance (not scaled) then returns normal sizes otherwise a scaled version
		const UiSkinSizes& GetSizes(const std::string& windowName) const;

		// Read access
		const UiTextStyleGroup& GetTextStyleGroup(ESkinStyle which) const { return m_textStyles[(int)which]; }
		const UiWindowStyleGroup& GetWindowStyleGroup(ESkinStyle which) const { return m_windowStyles[(int)which]; }
		const UiTextStyle& GetTextStyle(ESkinStyle which, ESkinSubStyle sub) const { return GetTextStyleGroup(which).GetSub(sub); }
		const UiWindowStyle& GetWindowStyle(ESkinStyle which, ESkinSubStyle sub) const { return GetWindowStyleGroup(which).GetSub(sub);}

		// Creates a scaled skin sizes instance for the window
		// Returns false if cannot do e.g. font too small or too big		
		bool Scale(const std::string &windowName, const VectorF& scaling);

		// Removes window scaling instance
		void RemoveInstance(const std::string &windowName);

		// Gets text baseline as height from top
		int GetBaseline(const std::string& windowName, ESkinStyle which, ESkinSubStyle sub) const;

		// Gets largest of all subtypes in terms of bounding rect
		// Note: rect can have non zero left and top
		RectangleI GetLargestTextRect(const std::string& windowName,ESkinStyle which, const std::string &text) const;

		// As above when you have access to a group
		// Note: rect can have non zero left and top
		RectangleI GetLargestTextRect(const std::string& windowName, const UiTextStyleGroup& group, const std::string &text) const;
	};
}