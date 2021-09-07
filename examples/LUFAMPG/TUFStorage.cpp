#include <GamepadStorage.h>
#include <GamepadConfig.h>
#include <EEPROM.h>

// No-op implmenetations

GamepadStorage::GamepadStorage() { }

void GamepadStorage::save() { }

// "Real-op" implementations

DpadMode GamepadStorage::getDpadMode() {
	DpadMode mode = DEFAULT_DPAD_MODE;
	EEPROM.get(STORAGE_DPAD_MODE_INDEX, mode);
	return mode;
}

void GamepadStorage::setDpadMode(DpadMode mode) {
	EEPROM.put(STORAGE_DPAD_MODE_INDEX, mode);
}

InputMode GamepadStorage::getInputMode() {
	InputMode mode = DEFAULT_INPUT_MODE;
	EEPROM.get(STORAGE_INPUT_MODE_INDEX, mode);
	return mode;
}

void GamepadStorage::setInputMode(InputMode mode) {
	EEPROM.put(STORAGE_INPUT_MODE_INDEX, mode);
}

SOCDMode GamepadStorage::getSOCDMode() {
	SOCDMode mode = DEFAULT_SOCD_MODE;
	EEPROM.get(STORAGE_SOCD_MODE_INDEX, mode);
	return mode;
}

void GamepadStorage::setSOCDMode(SOCDMode mode) {
	EEPROM.put(STORAGE_SOCD_MODE_INDEX, mode);
}
