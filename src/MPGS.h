/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include "MPG.h"
#include "GamepadStorage.h"

class MPGS : public MPG
{
	public:
		MPGS(int debounceMS = 5, GamepadStorage *storage = &GamepadStore)
			: MPG(debounceMS), mpgStorage(storage)
		{
		}

		/**
		 * @brief Load the saved configuration from persistent storage.
		 */
		void load();

		/**
		 * @brief Save the current configuration to persistent storage if changed.
		 */
		void save();

		/**
		 * @brief Checks and executes any hotkey being pressed...with automatic save!
		 *
		 * @return GamepadHotkey - The selected hotkey action
		 */
		GamepadHotkey hotkey() override;

	protected:
		// TODO: bare pointers should be avoided when possible. Consider using shared_ptr or similar.
		GamepadStorage *mpgStorage;
};
