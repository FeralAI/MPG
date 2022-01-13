/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "MPG.h"

static HIDReport hidReport;
static SwitchReport switchReport;
static XInputReport xinputReport;

void *MPG::getReport()
{
	switch (options.inputMode)
	{
		case INPUT_MODE_XINPUT:
			xinputReport = getXInputReport();
			return &xinputReport;

		case INPUT_MODE_SWITCH:
			switchReport = getSwitchReport();
			return &switchReport;

		default:
			hidReport = getHIDReport();
			return &hidReport;
	}
}

uint16_t MPG::getReportSize()
{
	switch (options.inputMode)
	{
		case INPUT_MODE_XINPUT:
			return sizeof(XInputReport);

		case INPUT_MODE_SWITCH:
			return sizeof(SwitchReport);

		default:
			return sizeof(HIDReport);
	}
}

HIDReport MPG::getHIDReport()
{
	HIDReport report =
	{
		.buttons = 0,
		.hat = HID_HAT_NOTHING,
		.lx = (uint8_t)(state.lx >> 8),
		.ly = (uint8_t)(state.ly >> 8),
		.rx = (uint8_t)(state.rx >> 8),
		.ry = (uint8_t)(state.ry >> 8),
	};

	switch (state.dpad & GAMEPAD_MASK_DPAD)
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

	return report;
}

SwitchReport MPG::getSwitchReport()
{
	SwitchReport report =
	{
		.buttons = 0,
		.hat = SWITCH_HAT_NOTHING,
		.lx = (uint8_t)(state.lx >> 8),
		.ly = (uint8_t)(state.ly >> 8),
		.rx = (uint8_t)(state.rx >> 8),
		.ry = (uint8_t)(state.ry >> 8),
		.vendor = 0,
	};

	switch (state.dpad & GAMEPAD_MASK_DPAD)
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

	return report;
}

XInputReport MPG::getXInputReport()
{
	XInputReport report =
	{
		.report_id = 0,
		.report_size = XINPUT_ENDPOINT_SIZE,
		.buttons1 = 0,
		.buttons2 = 0,
		.lt = state.lt,
		.rt = state.rt,
		.lx = state.lx + -32768,
		.ly = ~(state.ly) + -32768,
		.rx = state.rx + -32768,
		.ry = ~(state.ry) + -32768,
		._reserved = { },
	};

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

	if (hasAnalogTriggers)
	{
		report.lt = state.lt;
		report.rt = state.rt;
	}
	else
	{
		report.lt = pressedL2() ? 0xFF : 0;
		report.rt = pressedR2() ? 0xFF : 0;
	}

	return report;
}

GamepadHotkey MPG::hotkey()
{
	static GamepadHotkey lastAction = HOTKEY_NONE;

	GamepadHotkey action = HOTKEY_NONE;
	if (pressedF1())
	{
		switch (state.dpad & GAMEPAD_MASK_DPAD)
		{
			case GAMEPAD_MASK_LEFT:
				action = HOTKEY_DPAD_LEFT_ANALOG;
				options.dpadMode = DPAD_MODE_LEFT_ANALOG;
				state.dpad = 0;
				state.buttons &= ~(f1Mask);
				break;

			case GAMEPAD_MASK_RIGHT:
				action = HOTKEY_DPAD_RIGHT_ANALOG;
				options.dpadMode = DPAD_MODE_RIGHT_ANALOG;
				state.dpad = 0;
				state.buttons &= ~(f1Mask);
				break;

			case GAMEPAD_MASK_DOWN:
				action = HOTKEY_DPAD_DIGITAL;
				options.dpadMode = DPAD_MODE_DIGITAL;
				state.dpad = 0;
				state.buttons &= ~(f1Mask);
				break;

			case GAMEPAD_MASK_UP:
				action = HOTKEY_HOME_BUTTON;
				state.dpad = 0;
				state.buttons &= ~(f1Mask);
				state.buttons |= GAMEPAD_MASK_A1; // Press the Home button
				break;
		}
	}
	else if (pressedF2())
	{
		switch (state.dpad & GAMEPAD_MASK_DPAD)
		{
			case GAMEPAD_MASK_DOWN:
				action = HOTKEY_SOCD_NEUTRAL;
				options.socdMode = SOCD_MODE_NEUTRAL;
				state.dpad = 0;
				state.buttons &= ~(f2Mask);
				break;

			case GAMEPAD_MASK_UP:
				action = HOTKEY_SOCD_UP_PRIORITY;
				options.socdMode = SOCD_MODE_UP_PRIORITY;
				state.dpad = 0;
				state.buttons &= ~(f2Mask);
				break;

			case GAMEPAD_MASK_LEFT:
				action = HOTKEY_SOCD_LAST_INPUT;
				options.socdMode = SOCD_MODE_SECOND_INPUT_PRIORITY;
				state.dpad = 0;
				state.buttons &= ~(f2Mask);
				break;

			case GAMEPAD_MASK_RIGHT:
				if (lastAction != HOTKEY_INVERT_Y_AXIS)
					options.invertYAxis = !options.invertYAxis;
				action = HOTKEY_INVERT_Y_AXIS;
				state.dpad = 0;
				state.buttons &= ~(f2Mask);
				break;
		}
	}

	lastAction = action;
	return action;
}

void MPG::process()
{
	state.dpad = runSOCDCleaner(options.socdMode, state.dpad);

	switch (options.dpadMode)
	{
		case DpadMode::DPAD_MODE_LEFT_ANALOG:
			if (!hasRightAnalogStick) {
				state.rx = GAMEPAD_JOYSTICK_MID;
				state.ry = GAMEPAD_JOYSTICK_MID;
			}
			state.lx = dpadToAnalogX(state.dpad);
			state.ly = dpadToAnalogY(state.dpad);
			state.dpad = 0;
			break;

		case DpadMode::DPAD_MODE_RIGHT_ANALOG:
			if (!hasLeftAnalogStick) {
				state.lx = GAMEPAD_JOYSTICK_MID;
				state.ly = GAMEPAD_JOYSTICK_MID;
			}
			state.rx = dpadToAnalogX(state.dpad);
			state.ry = dpadToAnalogY(state.dpad);
			state.dpad = 0;
			break;

		default:
			if (!hasLeftAnalogStick) {
				state.lx = GAMEPAD_JOYSTICK_MID;
				state.ly = GAMEPAD_JOYSTICK_MID;
			}
			if (!hasRightAnalogStick) {
				state.rx = GAMEPAD_JOYSTICK_MID;
				state.ry = GAMEPAD_JOYSTICK_MID;
			}
			break;
	}

}
