#include "MPGS.h"

GamepadHotkey MPGS::hotkey()
{
	GamepadHotkey hotkey = MPG::hotkey();
	if (hotkey != GamepadHotkey::HOTKEY_NONE)
		save();

	return hotkey;
}

void MPGS::load()
{
	mpgStorage->get(STORAGE_INPUT_MODE_INDEX, &inputMode, sizeof(InputMode));
	if (inputMode > INPUT_MODE_HID)
		inputMode = DEFAULT_INPUT_MODE;

	mpgStorage->get(STORAGE_DPAD_MODE_INDEX, &dpadMode, sizeof(DpadMode));
	if (dpadMode > DPAD_MODE_RIGHT_ANALOG)
		dpadMode = DEFAULT_DPAD_MODE;

	mpgStorage->get(STORAGE_SOCD_MODE_INDEX, &socdMode, sizeof(SOCDMode));
	if (socdMode > SOCD_MODE_SECOND_INPUT_PRIORITY)
		socdMode = DEFAULT_SOCD_MODE;
}

void MPGS::save()
{
	bool needsSave = false;

	InputMode savedInputMode;
	mpgStorage->get(STORAGE_INPUT_MODE_INDEX, &savedInputMode, sizeof(InputMode));
	if (savedInputMode != inputMode)
	{
		mpgStorage->set(STORAGE_INPUT_MODE_INDEX, &inputMode, sizeof(InputMode));
		needsSave = true;
	}

	DpadMode savedDpadMode;
	mpgStorage->get(STORAGE_DPAD_MODE_INDEX, &savedDpadMode, sizeof(DpadMode));
	if (savedDpadMode != dpadMode)
	{
		mpgStorage->set(STORAGE_DPAD_MODE_INDEX, &dpadMode, sizeof(DpadMode));
		needsSave = true;
	}

	SOCDMode savedSOCDMode;
	mpgStorage->get(STORAGE_SOCD_MODE_INDEX, &savedSOCDMode, sizeof(SOCDMode));
	if (savedSOCDMode != socdMode)
	{
		mpgStorage->set(STORAGE_SOCD_MODE_INDEX, &socdMode, sizeof(SOCDMode));
		needsSave = true;
	}

	if (needsSave)
		mpgStorage->save();
}
