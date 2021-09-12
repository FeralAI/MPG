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
		 * @brief Perform pin setup and any other initialization the board requires
		 */
		virtual void setup();

		/**
		 * @brief Retrieve the inputs and save to the current state
		 */
		virtual void read();

		/**
		 * @brief Load the saved configuration from persitent storage.
		 */
		virtual void load();

		/**
		 * @brief Save the current configuration to persitent storage if changed.
		 */
		virtual void save();

		/**
		 * @brief Checks and executes any hotkey being pressed...with automatic save!
		 *
		 * @return GamepadHotkey - The selected hotkey action
		 */
		virtual GamepadHotkey hotkey();

		/**
		 * @brief Check for F1 button press. Can override in derived board class.
		 */
		virtual bool pressedF1();

		/**
		 * @brief Check for F1 button press. Can override in derived board class.
		 */
		virtual bool pressedF2();

	protected:
		GamepadStorage *mpgStorage;
};

#endif
