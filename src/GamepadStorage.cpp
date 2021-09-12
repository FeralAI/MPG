#include "GamepadStorage.h"

DpadMode GamepadStorage::getDpadMode()
{
	DpadMode mode = DPAD_MODE_DIGITAL;
	get(STORAGE_DPAD_MODE_INDEX, &mode, sizeof(DpadMode));
	return mode;
}

void GamepadStorage::setDpadMode(DpadMode mode)
{
	set(STORAGE_DPAD_MODE_INDEX, &mode, sizeof(DpadMode));
}

InputMode GamepadStorage::getInputMode()
{
	InputMode mode = INPUT_MODE_XINPUT;
	get(STORAGE_INPUT_MODE_INDEX, &mode, sizeof(InputMode));
	return mode;
}

void GamepadStorage::setInputMode(InputMode mode)
{
	set(STORAGE_INPUT_MODE_INDEX, &mode, sizeof(InputMode));
}

SOCDMode GamepadStorage::getSOCDMode()
{
	SOCDMode mode = SOCD_MODE_NEUTRAL;
	get(STORAGE_SOCD_MODE_INDEX, &mode, sizeof(SOCDMode));
	return mode;
}

void GamepadStorage::setSOCDMode(SOCDMode mode)
{
	set(STORAGE_SOCD_MODE_INDEX, &mode, sizeof(SOCDMode));
}
