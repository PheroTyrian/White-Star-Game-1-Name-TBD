////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	UIInputMappings.h
//
// summary:	Declares the input mappings class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "UI\UITypes.h"

namespace HAPI_UI_SPACE
{
	/// <summary>	A macro that defines Number modifers. </summary>
	#define NUM_MODIFERS 3

	/*
		Realistically a user cannot hold more than 4 keys at a time
		All apart from one of those would need to be modifier keys
	*/
	/// <summary>	An input combi. </summary>
	struct InputCombi
	{
		// Main key is map key and is not a modifier

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Extra keys that need to be held at same time as the main one	e.g. shift, alt, control (can
		/// also be left shift only etc.)
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<EInputModifiers> modifiers;

		/// <summary>	The action. </summary>
		EInputMappingAction action{ EInputMappingAction::eUnmapped };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct. </summary>
		///
		/// <param name="actionIn">	The action in. </param>
		/// <param name="mods">	   	(Optional) The mods. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		InputCombi(EInputMappingAction actionIn, 
			std::vector<EInputModifiers> mods = std::vector<EInputModifiers>())
			: action(actionIn), modifiers(std::move(mods))
		{
			
		}					
	};	

	/// <summary>	An input mappings. </summary>
	class UIInputMappings
	{
	private:
		// Need to use multimap as may have duplicate keys
		/// <summary>	The key mappings. </summary>
		std::multimap<BYTE, InputCombi> m_keyMappings;		
		/// <summary>	Assign key mappings. </summary>
		void AssignKeyMappings();

		/// <summary>	The current key. </summary>
		BYTE m_currentKey{ 0 };
		/// <summary>	The current mods[ number modifers]. </summary>
		EInputModifiers m_currentMods[NUM_MODIFERS]{ EInputModifiers::eUnmapped };

		/// <summary>	The last action. </summary>
		EInputMappingAction m_lastAction;
		/// <summary>	The last time action triggered. </summary>
		DWORD m_lastTimeActionTriggered{ 0 };			

		/// <summary>	State of the last mouse. </summary>
		MouseData m_lastMouseState;
		/// <summary>	True to dragging. </summary>
		bool m_dragging{ false };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	No main key combi. </summary>
		///
		/// <returns>	A BYTE. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		constexpr BYTE NoMainKeyCombi() const noexcept { return 255; }

	public:
		/// <summary>	Default constructor. </summary>
		UIInputMappings();
		/// <summary>	Destructor. </summary>
		~UIInputMappings();

	//	void AddKeyInputMapping(BYTE keyCode, const std::vector<EInputModifiers> mods = std::vector<EInputModifiers>());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// For fixing a rare bug where mouse click stuck (after using FileOpen dialog)
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Clear();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets action from current state. </summary>
		///
		/// <returns>	The action from current state. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EInputMappingAction GetActionFromCurrentState();

		/// <summary>	Assign default mappings. </summary>
		void AssignDefaultMappings();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Key event happened. </summary>
		///
		/// <param name="keyEvent">	The key event. </param>
		/// <param name="keyCode"> 	The key code. </param>
		///
		/// <returns>	An EInputMappingAction. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EInputMappingAction KeyEventHappened(EKeyEvent keyEvent, BYTE keyCode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Mouse event happened. </summary>
		///
		/// <param name="mouseEvent">	The mouse event. </param>
		/// <param name="mouseData"> 	Information describing the mouse state. </param>
		///
		/// <returns>	An EInputMappingAction. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EInputMappingAction MouseEventHappened(EMouseEvent mouseEvent, const MouseData& mouseData);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Mouse move happened. </summary>
		///
		/// <param name="mouseData">	Information describing the mouse state. </param>
		///
		/// <returns>	An EInputMappingAction. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		EInputMappingAction MouseMoveHappened(const MouseData& mouseData);

	};

}

