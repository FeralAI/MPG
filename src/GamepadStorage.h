/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_STORAGE_H_
#define GAMEPAD_STORAGE_H_

#include <stdint.h>

#include "GamepadOptions.h"

#define STORAGE_FIRST_AVAILBLE_INDEX 2048

class GamepadStorage
{
	public:
		virtual void start();
		virtual void save();

		GamepadOptions getGamepadOptions();
		void setGamepadOptions(GamepadOptions options);
};

static GamepadStorage GamepadStore;

#endif
