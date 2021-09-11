/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef MPGS_H_
#define MPGS_H_

#include "MPG.h"
#include "GamepadStorage.h"

class MPGS : public MPG
{
	public:
		MPGS(int debounceMS = 5) : MPG(debounceMS)
		{
			if (debounceMS > 0)
			{
				for (int i = 0; i < GAMEPAD_DIGITAL_INPUT_COUNT; i++)
					debouncers[i].setGamepadState(state);
			}
		}

		/**
		 * @brief Checks and executes any hotkey being pressed...with automatic save!
		 *
		 * @return GamepadHotkey - The selected hotkey action
		 */
		GamepadHotkey hotkey();

		/**
		 * @brief Load the saved configuration from persitent storage.
		 */
		virtual void load();

		/**
		 * @brief Save the current configuration to persitent storage if changed.
		 */
		virtual void save();
};

#endif
