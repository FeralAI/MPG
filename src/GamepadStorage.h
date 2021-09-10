/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_STORAGE_H_
#define GAMEPAD_STORAGE_H_

#include "GamepadEnums.h"

class GamepadStorage
{
	public:
		GamepadStorage();

		void save();

		DpadMode getDpadMode();
		void setDpadMode(DpadMode mode);

		InputMode getInputMode();
		void setInputMode(InputMode mode);

		SOCDMode getSOCDMode();
		void setSOCDMode(SOCDMode mode);
};

static GamepadStorage GamepadStore;

#endif
