#ifndef KTY_KEYBOARD
#define KTY_KEYBOARD

#include <conio.h>
#include <stdio.h>
#include <wtypes.h>
#include "Logger.h"
#include <wchar.h>
#include "global.h"

#ifndef KTY_KEYCODE
#define KEYCODE

#define key_backspace 0x08
#define key_tab 0x09
#define key_enter 0x0D
#define key_left_shift 0xA0
#define key_right_shift 0xA1
#define key_left_ctrl 0xA2
#define key_right_ctrl 0xA3
#define key_left_alt 0xA4
#define key_right_alt 0xA5
#define key_pause 0x13
#define key_caps_lock 0x14
#define key_space 0x20
#define key_page_up 0x21
#define key_page_down 0x22
#define key_end 0x23
#define key_home 0x24
#define key_left 0x25
#define key_up 0x26
#define key_right 0x27
#define key_down 0x28
#define key_select 0x29
#define key_print 0x2A
#define key_print_screen 0x2C
#define key_insert 0x2D
#define key_delete 0x2E
#define key_0 0x30
#define key_1 0x31
#define key_2 0x32
#define key_3 0x33
#define key_4 0x34
#define key_5 0x35
#define key_6 0x36
#define key_7 0x37
#define key_8 0x38
#define key_9 0x39
#define key_A 0x41
#define key_B 0x42
#define key_C 0x43
#define key_D 0x44
#define key_E 0x45
#define key_F 0x46
#define key_G 0x47
#define key_H 0x48
#define key_I 0x49
#define key_J 0x4A
#define key_K 0x4B
#define key_L 0x4C
#define key_M 0x4D
#define key_N 0x4E
#define key_O 0x4F
#define key_P 0x50
#define key_Q 0x51
#define key_R 0x52
#define key_S 0x53
#define key_T 0x54
#define key_U 0x55
#define key_V 0x56
#define key_W 0x57
#define key_X 0x58
#define key_Y 0x59
#define key_Z 0x5A
#define key_left_windows 0x5B
#define key_right_windows 0x5C
#define key_numpad_0 0x60
#define key_numpad_1 0x61
#define key_numpad_2 0x62
#define key_numpad_3 0x63
#define key_numpad_4 0x64
#define key_numpad_5 0x65
#define key_numpad_6 0x66
#define key_numpad_7 0x67
#define key_numpad_8 0x68
#define key_numpad_9 0x69
#define key_multiply 0x6A
#define key_add 0x6B
#define key_separator 0x6C
#define key_subtract 0x6D
#define key_decimal 0x6E
#define key_divide 0x6F
#define key_f1 0x70
#define key_f2 0x71
#define key_f3 0x72
#define key_f4 0x73
#define key_f5 0x74
#define key_f6 0x75
#define key_f7 0x76
#define key_f8 0x77
#define key_f9 0x78
#define key_f10 0x79
#define key_f11 0x7A
#define key_f12 0x7B
#define key_f13 0x7C
#define key_f14 0x7D
#define key_f15 0x7E
#define key_f16 0x7F
#define key_f17 0x80
#define key_f18 0x81
#define key_f19 0x82
#define key_f20 0x83
#define key_f21 0x84
#define key_f22 0x85
#define key_f23 0x86
#define key_f24 0x87
#define key_num_lock 0x90
#define key_scroll_lock 0x91
#define key_media_volume_mute 0xAD
#define key_media_volume_down 0xAE
#define key_media_volume_up 0xAF
#define key_media_next_track 0xB0
#define key_media_prev_track 0xB1
#define key_media_play_pause 0xB2
#endif

#define pressed 0
#define depressed 1
#define pressed_this_frame 2
#define depressed_this_frame 3


enum KeyState {
	Pressed = 0,
	Depressed = 1,
	PressedThisFrame = 2,
	DepressedThisFrame = 3
};
namespace Input
{
	class Keyboard {
	private:
		static bool key_press(int KeyCode) {
			int KeyState = GetKeyState(KeyCode);
			if (KeyState == -127 || KeyState == -128)
				return true;
			return false;
		}
		static inline int keys[256]{ 0 };
		static inline int old_keys[256]{ 0 };
	public:
		static bool enabled;
		static bool Key(int state, int keyCode, bool IgnoreDefocus = false) {

			if (!enabled)
				return false;

			if (!KTYGlobal::WindowFocused() && !IgnoreDefocus)
				return false;

			keys[keyCode] = GetKeyState(keyCode);
			switch (state) {
			case pressed:
				if (key_press(keyCode))
					return true;
				break;
			case depressed:
				if (!key_press(keyCode))
					return true;
				break;
			case pressed_this_frame:
				if (key_press(keyCode) && keys[keyCode] != old_keys[keyCode])
					return true;
				break;
			case depressed_this_frame:
				if (!key_press(keyCode) && keys[keyCode] != old_keys[keyCode])
					return true;
				break;
			}
			return false;
		}
		static void SwapKeyBuffer(GLFWwindow* window) {
			for (int i = 0; i < std::size(keys); i++) {
				old_keys[i] = keys[i];
			}
		}
	};
}
#endif