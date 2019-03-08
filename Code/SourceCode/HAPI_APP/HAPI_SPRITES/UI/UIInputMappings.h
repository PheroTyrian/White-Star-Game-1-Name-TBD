#pragma once

#include "UI\UITypes.h"

namespace HAPI_UI_SPACE
{
	#define NUM_MODIFERS 3


	/*
		Realistically a user cannot hold more than 4 keys at a time
		All apart from one of those would need to be modifier keys
	*/
	struct InputCombi
	{
		// Main key is map key and is not a modifier

		// Extra keys that need to be held at same time as the main one	e.g. shift, alt, control (can also be left shift only etc.)
		std::vector<EInputModifiers> modifiers;

		// The action
		EInputMappingAction action{ EInputMappingAction::eUnmapped };

		// Construct
		InputCombi(EInputMappingAction actionIn, 
			std::vector<EInputModifiers> mods = std::vector<EInputModifiers>())
			: action(actionIn), modifiers(std::move(mods))
		{
			
		}					
	};	

	class UIInputMappings
	{
	private:
		// Need to use multimap as may have duplicate keys
		std::multimap<BYTE, InputCombi> m_keyMappings;		
		void AssignKeyMappings();

		BYTE m_currentKey{ 0 };
		EInputModifiers m_currentMods[NUM_MODIFERS]{ EInputModifiers::eUnmapped };

		EInputMappingAction m_lastAction;
		DWORD m_lastTimeActionTriggered{ 0 };			

		MouseData m_lastMouseState;
		bool m_dragging{ false };

		constexpr BYTE NoMainKeyCombi() const noexcept { return 255; }

	public:
		UIInputMappings();
		~UIInputMappings();

	//	void AddKeyInputMapping(BYTE keyCode, const std::vector<EInputModifiers> mods = std::vector<EInputModifiers>());

		// For fixing a rare bug where mouse click stuck (after using FileOpen dialog)
		void Clear();

		EInputMappingAction GetActionFromCurrentState();

		void AssignDefaultMappings();

		EInputMappingAction KeyEventHappened(EKeyEvent keyEvent, BYTE keyCode);

		EInputMappingAction MouseEventHappened(EMouseEvent mouseEvent, const MouseData& mouseData);

		EInputMappingAction MouseMoveHappened(const MouseData& mouseData);

	};

}

