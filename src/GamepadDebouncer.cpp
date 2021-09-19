/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "GamepadDebouncer.h"

void GamepadDebouncer::debounce(GamepadState *state)
{
	uint32_t now = getMillis();

	for (int i = 0; i < 4; i++)
	{
		if ((debounceState.dpad & dpadMasks[i]) != (state->dpad & dpadMasks[i]) && (now - dpadTime[i]) > debounceMS)
		{
			debounceState.dpad ^= dpadMasks[i];
			dpadTime[i] = now;
		}
	}

	for (int i = 0; i < GAMEPAD_BUTTON_COUNT; i++)
	{
		if ((debounceState.buttons & buttonMasks[i]) != (state->buttons & buttonMasks[i]) && (now - buttonTime[i]) > debounceMS)
		{
			debounceState.buttons ^= buttonMasks[i];
			buttonTime[i] = now;
		}
	}

	memcpy(&state->dpad, &debounceState.dpad, sizeof(uint8_t));
	memcpy(&state->buttons, &debounceState.buttons, sizeof(uint16_t));
}
