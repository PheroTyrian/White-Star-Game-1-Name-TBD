#pragma once
/*
	HAPI_InputCodes.h 
	HAPI key code and controller defines. 
	All HAPI supported platforms map to these codes
	Each specifies the index of the boolean state in the structure
*/

namespace HAPISPACE {

	// Key mappings are similar to Windows VK_ codes (for historical reasons)
	// Note: numbers are the same as ASCII '0' - '9' as are the letters 'A' - 'Z' so do not need a define
	#define HK_UNKNOWN		  0x00
	#define HK_LBUTTON        0x01
	#define HK_RBUTTON        0x02
	#define HK_CANCEL         0x03
	#define HK_MBUTTON        0x04 
	#define HK_XBUTTON1       0x05 
	#define HK_XBUTTON2       0x06
	#define HK_BACK           0x08
	#define HK_TAB            0x09
	#define HK_CLEAR          0x0C
	#define HK_RETURN         0x0D
	#define HK_SHIFT          0x10
	#define HK_CONTROL        0x11
	#define HK_ALT			  0x12
	#define HK_PAUSE          0x13
	#define HK_CAPITAL        0x14
	#define HK_ESCAPE         0x1B
	#define HK_SPACE          0x20

	// These are now into ascii space so beware
	#define HK_PRIOR          0x21
	#define HK_NEXT           0x22
	#define HK_END            0x23
	#define HK_HOME           0x24
	#define HK_LEFT           0x25
	#define HK_UP             0x26
	#define HK_RIGHT          0x27
	#define HK_DOWN           0x28
	#define HK_SELECT         0x29
	#define HK_PRINT          0x2A
	#define HK_EXECUTE        0x2B
	#define HK_SNAPSHOT       0x2C
	#define HK_INSERT         0x2D
	#define HK_DELETE         0x2E
	#define HK_HELP           0x2F

	#define HK_LWIN           0x5B
	#define HK_RWIN           0x5C
	#define HK_APPS           0x5D
	#define HK_SLEEP          0x5F
	#define HK_NUMPAD0        0x60
	#define HK_NUMPAD1        0x61
	#define HK_NUMPAD2        0x62
	#define HK_NUMPAD3        0x63
	#define HK_NUMPAD4        0x64
	#define HK_NUMPAD5        0x65
	#define HK_NUMPAD6        0x66
	#define HK_NUMPAD7        0x67
	#define HK_NUMPAD8        0x68
	#define HK_NUMPAD9        0x69
	#define HK_MULTIPLY       0x6A
	#define HK_ADD            0x6B
	#define HK_SEPARATOR      0x6C
	#define HK_SUBTRACT       0x6D
	#define HK_DECIMAL        0x6E
	#define HK_DIVIDE         0x6F
	#define HK_F1             0x70
	#define HK_F2             0x71
	#define HK_F3             0x72
	#define HK_F4             0x73
	#define HK_F5             0x74
	#define HK_F6             0x75
	#define HK_F7             0x76
	#define HK_F8             0x77
	#define HK_F9             0x78
	#define HK_F10            0x79
	#define HK_F11            0x7A
	#define HK_F12            0x7B
	#define HK_F13            0x7C
	#define HK_F14            0x7D
	#define HK_F15            0x7E
	#define HK_F16            0x7F
	#define HK_F17            0x80
	#define HK_F18            0x81
	#define HK_F19            0x82
	#define HK_F20            0x83
	#define HK_F21            0x84
	#define HK_F22            0x85
	#define HK_F23            0x86
	#define HK_F24            0x87
	#define HK_NUMLOCK        0x90
	#define HK_SCROLL         0x91
	#define HK_LSHIFT         0xA0
	#define HK_RSHIFT         0xA1
	#define HK_LCONTROL       0xA2
	#define HK_RCONTROL       0xA3
	#define HK_LALT			  0xA4
	#define HK_RALT			  0xA5		// Note: this also triggers a control key ALT-GR is Control - Alt
	#define HK_BROWSER_BACK        0xA6
	#define HK_BROWSER_FORWARD     0xA7
	#define HK_BROWSER_REFRESH     0xA8
	#define HK_BROWSER_STOP        0xA9
	#define HK_BROWSER_SEARCH      0xAA
	#define HK_BROWSER_FAVORITES   0xAB
	#define HK_BROWSER_HOME        0xAC
	#define HK_VOLUME_MUTE         0xAD
	#define HK_VOLUME_DOWN         0xAE
	#define HK_VOLUME_UP           0xAF
	#define HK_MEDIA_NEXT_TRACK    0xB0
	#define HK_MEDIA_PREV_TRACK    0xB1
	#define HK_MEDIA_STOP          0xB2
	#define HK_MEDIA_PLAY_PAUSE    0xB3
	#define HK_LAUNCH_MAIL         0xB4
	#define HK_LAUNCH_MEDIA_SELECT 0xB5
	#define HK_LAUNCH_APP1         0xB6
	#define HK_LAUNCH_APP2         0xB7
	#define HK_MENU				   0xB8

	// Controller codes for digital buttons
	// Depending on the controller not all may be available and the mappings may differ (they are fine for XBox controller)
	#define HK_DIGITAL_INVALID				-1			// added in 1.5
	#define HK_DIGITAL_DPAD_UP				0
	#define HK_DIGITAL_DPAD_DOWN			1
	#define HK_DIGITAL_DPAD_LEFT			2
	#define HK_DIGITAL_DPAD_RIGHT			3
	#define HK_DIGITAL_START				4
	#define HK_DIGITAL_BACK					5	
	#define HK_DIGITAL_SELECT				5	
	#define HK_DIGITAL_LEFT_THUMB			6	// Left thumbstick pushed
	#define HK_DIGITAL_RIGHT_THUMB			7   // Right thumbstick pushed (Xb360 only)
	#define HK_DIGITAL_LEFT_SHOULDER		8
	#define HK_DIGITAL_RIGHT_SHOULDER		9
	#define HK_DIGITAL_A					10
	#define HK_DIGITAL_B					11
	#define HK_DIGITAL_X					12
	#define HK_DIGITAL_Y					13
	#define HK_DIGITAL_VOL_UP				14	// Not on XBox Controller
	#define HK_DIGITAL_VOL_DOWN				15	// Not on XBox Controller

	// Analogue buttons
	#define HK_ANALOGUE_INVALID				-1			// added in 1.5
	#define HK_ANALOGUE_LEFT_TRIGGER		0
	#define HK_ANALOGUE_RIGHT_TRIGGER		1
	#define HK_ANALOGUE_LEFT_THUMB_X		2
	#define HK_ANALOGUE_LEFT_THUMB_Y		3
	#define HK_ANALOGUE_RIGHT_THUMB_X		4
	#define HK_ANALOGUE_RIGHT_THUMB_Y		5
	// These two are for trackballs:
	#define HK_ANALOGUE_REL_MOTIONX			6		// Not on XBox Controller
	#define HK_ANALOGUE_REL_MOTIONY			7		// Not on XBox Controller
	// This is for when a user moves a hat on a joystick, there are 9 possible values (see hat motion values below)
	#define HK_ANALOGUE_HAT_MOTION			8		// Not on XBox Controller

	// Recommended gamepad deadzones (these are for XBox 360 controller - other controllers may differ)
	#define HK_GAMEPAD_LEFT_THUMB_DEADZONE  7849
	#define HK_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
	#define HK_GAMEPAD_TRIGGER_THRESHOLD    30

	// HAT MOTION VALUES
	#define HK_HAT_UP		   0x01
	#define HK_HAT_RIGHT       0x02
	#define HK_HAT_DOWN        0x04
	#define HK_HAT_LEFT        0x08
	#define HK_HAT_RIGHTUP     (HK_HAT_RIGHT | HK_HAT_UP)
	#define HK_HAT_RIGHTDOWN   (HK_HAT_RIGHT | HK_HAT_DOWN)
	#define HK_HAT_LEFTUP      (HK_HAT_LEFT | HK_HAT_UP)
	#define HK_HAT_LEFTDOWN    (HK_HAT_LEFT | HK_HAT_DOWN)
}