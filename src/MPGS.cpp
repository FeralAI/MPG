#include "MPGS.h"

GamepadHotkey MPGS::hotkey()
{
	GamepadHotkey hotkey = MPG::hotkey();
	save();
	return hotkey;
}

void MPGS::load()
{
	inputMode = GamepadStore.getInputMode();
	if (inputMode > INPUT_MODE_HID)
		inputMode = DEFAULT_INPUT_MODE;

	dpadMode = GamepadStore.getDpadMode();
	if (dpadMode > DPAD_MODE_RIGHT_ANALOG)
		dpadMode = DEFAULT_DPAD_MODE;

	socdMode = GamepadStore.getSOCDMode();
	if (socdMode > SOCD_MODE_SECOND_INPUT_PRIORITY)
		socdMode = DEFAULT_SOCD_MODE;
}

void MPGS::save()
{
	bool needsSave = false;

	InputMode savedInputMode = GamepadStore.getInputMode();
	if (savedInputMode != inputMode)
	{
		GamepadStore.setInputMode(inputMode);
		needsSave = true;
	}

	DpadMode savedDpadMode = GamepadStore.getDpadMode();
	if (savedDpadMode != dpadMode)
	{
		GamepadStore.setDpadMode(dpadMode);
		needsSave = true;
	}

	SOCDMode savedSOCDMode = GamepadStore.getSOCDMode();
	if (savedSOCDMode != socdMode)
	{
		needsSave = true;
		GamepadStore.setSOCDMode(socdMode);
	}

	if (needsSave)
		GamepadStore.save();
}
