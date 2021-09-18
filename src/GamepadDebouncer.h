/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_DEBOUNCER_H_
#define GAMEPAD_DEBOUNCER_H_

#include <string.h>
#include <stdint.h>
#include "GamepadState.h"

// Implement this wrapper function for your platform
uint32_t getMillis();

class GamepadDebouncer
{
	public:
		GamepadDebouncer(const uint8_t debounceMS = 5) : debounceMS(debounceMS) { }

		void debounce(GamepadState *state);

		const uint8_t debounceMS;
		GamepadState debounceState;
		uint32_t dpadTime[4];
		uint32_t buttonTime[GAMEPAD_BUTTON_COUNT];
};

#endif
