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
	options = mpgStorage->getGamepadOptions();
}
void MPGS::save()
{
	bool dirty = false;
	GamepadOptions savedOptions = mpgStorage->getGamepadOptions();
	if (memcmp(&savedOptions, &options, sizeof(GamepadOptions)))
	{
		mpgStorage->setGamepadOptions(options);
		dirty = true;
	}

	if (dirty)
		mpgStorage->save();
}
