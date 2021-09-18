/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "MPG.h"

static HIDReport hidReport;
static SwitchReport switchReport;
static XInputReport xinputReport;

void MPG::debounce()
{
	for (int i = 0; i < GAMEPAD_DIGITAL_INPUT_COUNT; i++)
	{
		if (debouncers[i].update())
		{
			if (debouncers[i].rose())
				state.buttons |= debouncers[i].inputMask;
			else
				state.buttons &= ~(debouncers[i].inputMask);
		}
	}
}

void *MPG::getReport()
{
	switch (inputMode)
	{
		case INPUT_MODE_XINPUT:
			return getXInputReport();

		case INPUT_MODE_SWITCH:
			return getSwitchReport();

		default:
			return getHIDReport();
	}
}

uint16_t MPG::getReportSize()
{
	switch (inputMode)
	{
		case INPUT_MODE_XINPUT:
			return sizeof(XInputReport);

		case INPUT_MODE_SWITCH:
			return sizeof(SwitchReport);

		default:
			return sizeof(HIDReport);
	}
}

HIDReport *MPG::getHIDReport()
{
	static HIDReport report =
	{
		.buttons = 0,
		.hat = HID_HAT_NOTHING,
		.lx = HID_JOYSTICK_MID,
		.ly = HID_JOYSTICK_MID,
		.rx = HID_JOYSTICK_MID,
		.ry = HID_JOYSTICK_MID,
	};

	report.lx = state.lx >> 8;
	report.ly = state.ly >> 8;
	report.rx = state.rx >> 8;
	report.ry = state.ry >> 8;

	switch (state.buttons & GAMEPAD_MASK_DPAD)
	{
		case GAMEPAD_MASK_UP:                        report.hat = HID_HAT_UP;        break;
		case GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT:   report.hat = HID_HAT_UPRIGHT;   break;
		case GAMEPAD_MASK_RIGHT:                     report.hat = HID_HAT_RIGHT;     break;
		case GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT: report.hat = HID_HAT_DOWNRIGHT; break;
		case GAMEPAD_MASK_DOWN:                      report.hat = HID_HAT_DOWN;      break;
		case GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT:  report.hat = HID_HAT_DOWNLEFT;  break;
		case GAMEPAD_MASK_LEFT:                      report.hat = HID_HAT_LEFT;      break;
		case GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT:    report.hat = HID_HAT_UPLEFT;    break;
		default:                                     report.hat = HID_HAT_NOTHING;   break;
	}

	report.buttons = 0
		| (pressedB1() ? HID_MASK_CROSS    : 0)
		| (pressedB2() ? HID_MASK_CIRCLE   : 0)
		| (pressedB3() ? HID_MASK_SQUARE   : 0)
		| (pressedB4() ? HID_MASK_TRIANGLE : 0)
		| (pressedL1() ? HID_MASK_L1       : 0)
		| (pressedR1() ? HID_MASK_R1       : 0)
		| (pressedL2() ? HID_MASK_L2       : 0)
		| (pressedR2() ? HID_MASK_R2       : 0)
		| (pressedS1() ? HID_MASK_SELECT   : 0)
		| (pressedS2() ? HID_MASK_START    : 0)
		| (pressedL3() ? HID_MASK_L3       : 0)
		| (pressedR3() ? HID_MASK_R3       : 0)
		| (pressedA1() ? HID_MASK_PS       : 0)
		| (pressedA2() ? HID_MASK_TP       : 0)
	;

	return &report;
}

SwitchReport *MPG::getSwitchReport()
{
	static SwitchReport report =
	{
		.buttons = 0,
		.hat = 0,
		.lx = SWITCH_JOYSTICK_MID,
		.ly = SWITCH_JOYSTICK_MID,
		.rx = SWITCH_JOYSTICK_MID,
		.ry = SWITCH_JOYSTICK_MID,
		.vendor = 0,
	};

	report.lx = state.lx >> 8;
	report.ly = state.ly >> 8;
	report.rx = state.rx >> 8;
	report.ry = state.ry >> 8;

	switch (state.buttons & GAMEPAD_MASK_DPAD)
	{
		case GAMEPAD_MASK_UP:                        report.hat = SWITCH_HAT_UP;        break;
		case GAMEPAD_MASK_UP | GAMEPAD_MASK_RIGHT:   report.hat = SWITCH_HAT_UPRIGHT;   break;
		case GAMEPAD_MASK_RIGHT:                     report.hat = SWITCH_HAT_RIGHT;     break;
		case GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT: report.hat = SWITCH_HAT_DOWNRIGHT; break;
		case GAMEPAD_MASK_DOWN:                      report.hat = SWITCH_HAT_DOWN;      break;
		case GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT:  report.hat = SWITCH_HAT_DOWNLEFT;  break;
		case GAMEPAD_MASK_LEFT:                      report.hat = SWITCH_HAT_LEFT;      break;
		case GAMEPAD_MASK_UP | GAMEPAD_MASK_LEFT:    report.hat = SWITCH_HAT_UPLEFT;    break;
		default:                                     report.hat = SWITCH_HAT_NOTHING;   break;
	}

	report.buttons = 0
		| (pressedB1() ? SWITCH_MASK_B       : 0)
		| (pressedB2() ? SWITCH_MASK_A       : 0)
		| (pressedB3() ? SWITCH_MASK_Y       : 0)
		| (pressedB4() ? SWITCH_MASK_X       : 0)
		| (pressedL1() ? SWITCH_MASK_L       : 0)
		| (pressedR1() ? SWITCH_MASK_R       : 0)
		| (pressedL2() ? SWITCH_MASK_ZL      : 0)
		| (pressedR2() ? SWITCH_MASK_ZR      : 0)
		| (pressedS1() ? SWITCH_MASK_MINUS   : 0)
		| (pressedS2() ? SWITCH_MASK_PLUS    : 0)
		| (pressedL3() ? SWITCH_MASK_L3      : 0)
		| (pressedR3() ? SWITCH_MASK_R3      : 0)
		| (pressedA1() ? SWITCH_MASK_HOME    : 0)
		| (pressedA2() ? SWITCH_MASK_CAPTURE : 0)
	;

	return &report;
}

XInputReport *MPG::getXInputReport()
{
	static XInputReport report =
	{
		.report_id = 0,
		.report_size = XINPUT_ENDPOINT_SIZE,
		.buttons1 = 0,
		.buttons2 = 0,
		.lt = 0,
		.rt = 0,
		.lx = GAMEPAD_JOYSTICK_MID,
		.ly = GAMEPAD_JOYSTICK_MID,
		.rx = GAMEPAD_JOYSTICK_MID,
		.ry = GAMEPAD_JOYSTICK_MID,
		._reserved = { },
	};

	report.lx = state.lx + -32768;
	report.ly = ~(state.ly) + -32768;
	report.rx = state.rx + -32768;
	report.ry = ~(state.ry) + -32768;

	// Convert button states
	report.buttons1 = 0
		| (pressedUp()    ? XBOX_MASK_UP    : 0)
		| (pressedDown()  ? XBOX_MASK_DOWN  : 0)
		| (pressedLeft()  ? XBOX_MASK_LEFT  : 0)
		| (pressedRight() ? XBOX_MASK_RIGHT : 0)
		| (pressedS2()    ? XBOX_MASK_START : 0)
		| (pressedS1()    ? XBOX_MASK_BACK  : 0)
		| (pressedL3()    ? XBOX_MASK_LS    : 0)
		| (pressedR3()    ? XBOX_MASK_RS    : 0)
	;

	report.buttons2 = 0
		| (pressedL1() ? XBOX_MASK_LB   : 0)
		| (pressedR1() ? XBOX_MASK_RB   : 0)
		| (pressedA1() ? XBOX_MASK_HOME : 0)
		| (pressedB1() ? XBOX_MASK_A    : 0)
		| (pressedB2() ? XBOX_MASK_B    : 0)
		| (pressedB3() ? XBOX_MASK_X    : 0)
		| (pressedB4() ? XBOX_MASK_Y    : 0)
	;

	// Handle trigger values
	if (hasAnalogTriggers)
	{
		report.lt = state.lt;
		report.rt = state.rt;
	}
	else
	{
		report.lt = state.buttons & GAMEPAD_MASK_L2 ? 0xFF : 0;
		report.rt = state.buttons & GAMEPAD_MASK_R2 ? 0xFF : 0;
	}

	return &report;
}

GamepadHotkey MPG::hotkey()
{
	GamepadHotkey action = HOTKEY_NONE;
	if (pressedF1())
	{
		switch (state.buttons & GAMEPAD_MASK_DPAD)
		{
			case GAMEPAD_MASK_LEFT:
				action = HOTKEY_DPAD_LEFT_ANALOG;
				dpadMode = DPAD_MODE_LEFT_ANALOG;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
				break;

			case GAMEPAD_MASK_RIGHT:
				action = HOTKEY_DPAD_RIGHT_ANALOG;
				dpadMode = DPAD_MODE_RIGHT_ANALOG;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
				break;

			case GAMEPAD_MASK_DOWN:
				action = HOTKEY_DPAD_DIGITAL;
				dpadMode = DPAD_MODE_DIGITAL;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
				break;

			case GAMEPAD_MASK_UP:
				action = HOTKEY_HOME_BUTTON;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
				state.buttons |= GAMEPAD_MASK_A1; // Press the Home button
				break;
		}
	}
	else if (pressedF2())
	{
		switch (state.buttons & GAMEPAD_MASK_DPAD)
		{
			case GAMEPAD_MASK_DOWN:
				action = HOTKEY_SOCD_NEUTRAL;
				socdMode = SOCD_MODE_NEUTRAL;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3);
				break;

			case GAMEPAD_MASK_UP:
				action = HOTKEY_SOCD_UP_PRIORITY;
				socdMode = SOCD_MODE_UP_PRIORITY;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3);
				break;

			case GAMEPAD_MASK_LEFT:
				action = HOTKEY_SOCD_LAST_INPUT;
				socdMode = SOCD_MODE_SECOND_INPUT_PRIORITY;
				state.buttons &= ~(GAMEPAD_MASK_DPAD | GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3);
				break;
		}
	}

	return action;
}

void MPG::process()
{
	uint8_t dpadValue = runSOCDCleaner(socdMode, (state.buttons & GAMEPAD_MASK_DPAD));

	switch (dpadMode)
	{
		case DpadMode::DPAD_MODE_LEFT_ANALOG:
			state.lx = dpadToAnalogX(dpadValue);
			state.ly = dpadToAnalogY(dpadValue);
			dpadValue = 0;
			break;

		case DpadMode::DPAD_MODE_RIGHT_ANALOG:
			state.rx = dpadToAnalogX(dpadValue);
			state.ry = dpadToAnalogY(dpadValue);
			dpadValue = 0;
			break;
	}

	if (!hasLeftAnalogStick) {
		state.lx = GAMEPAD_JOYSTICK_MID;
		state.ly = GAMEPAD_JOYSTICK_MID;
	}

	if (!hasRightAnalogStick) {
		state.rx = GAMEPAD_JOYSTICK_MID;
		state.ry = GAMEPAD_JOYSTICK_MID;
	}
}
