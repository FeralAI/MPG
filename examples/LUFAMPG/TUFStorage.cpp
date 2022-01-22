#include <GamepadStorage.h>
#include <EEPROM.h>

GamepadOptions GamepadStorage::getGamepadOptions()
{
  GamepadOptions options;
	// = {
  //   .inputMode = INPUT_MODE_XINPUT,
  //   .dpadMode = DPAD_MODE_DIGITAL,
  //   .socdMode = SOCD_MODE_NEUTRAL,
  // };

  EEPROM.get(0, options);
  return options;
}

void GamepadStorage::setGamepadOptions(GamepadOptions options)
{
  EEPROM.put(0, options);
}

void GamepadStorage::start() { }
void GamepadStorage::save() { }
