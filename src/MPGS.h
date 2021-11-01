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
		MPGS(int debounceMS = 5, GamepadStorage *storage = &GamepadStore)
			: MPG(debounceMS), mpgStorage(storage)
		{

		}

		/**
		 * @brief Load the saved configuration from persitent storage.
		 */
		void load();

		/**
		 * @brief Save the current configuration to persitent storage if changed.
		 */
		void save();

		/**
		 * @brief Checks and executes any hotkey being pressed...with automatic save!
		 *
		 * @return GamepadHotkey - The selected hotkey action
		 */
		GamepadHotkey hotkey() override;

	protected:
		GamepadStorage *mpgStorage;
};

#endif
