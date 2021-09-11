#include "MPGS.h"

GamepadHotkey MPGS::hotkey()
{
	GamepadHotkey hotkey = MPG::hotkey();
	save();
	return hotkey;
}

void MPGS::load()
{
	mpgStorage->get(STORAGE_INPUT_MODE_INDEX, &inputMode, sizeof(inputMode));
	if (inputMode > INPUT_MODE_HID)
		inputMode = DEFAULT_INPUT_MODE;

	mpgStorage->get(STORAGE_DPAD_MODE_INDEX, &dpadMode, sizeof(dpadMode));
	if (dpadMode > DPAD_MODE_RIGHT_ANALOG)
		dpadMode = DEFAULT_DPAD_MODE;

	mpgStorage->get(STORAGE_SOCD_MODE_INDEX, &socdMode, sizeof(socdMode));
	if (socdMode > SOCD_MODE_SECOND_INPUT_PRIORITY)
		socdMode = DEFAULT_SOCD_MODE;
}

void MPGS::save()
{
	bool needsSave = false;

	InputMode savedInputMode;
	mpgStorage->get(STORAGE_INPUT_MODE_INDEX, &savedInputMode, sizeof(savedInputMode));
	if (savedInputMode != inputMode)
	{
		mpgStorage->set(STORAGE_INPUT_MODE_INDEX, &inputMode, sizeof(inputMode));
		needsSave = true;
	}

	DpadMode savedDpadMode;
	mpgStorage->get(STORAGE_DPAD_MODE_INDEX, &savedDpadMode, sizeof(savedDpadMode));
	if (savedDpadMode != dpadMode)
	{
		mpgStorage->set(STORAGE_DPAD_MODE_INDEX, &dpadMode, sizeof(dpadMode));
		needsSave = true;
	}

	SOCDMode savedSOCDMode;
	mpgStorage->get(STORAGE_SOCD_MODE_INDEX, &savedSOCDMode, sizeof(savedSOCDMode));
	if (savedSOCDMode != socdMode)
	{
		mpgStorage->set(STORAGE_SOCD_MODE_INDEX, &socdMode, sizeof(socdMode));
		needsSave = true;
	}

	if (needsSave)
		mpgStorage->save();
}
