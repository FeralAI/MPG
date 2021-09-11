/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_STORAGE_H_
#define GAMEPAD_STORAGE_H_

#include <stdint.h>

#include "GamepadEnums.h"

#ifndef STORAGE_INPUT_MODE_INDEX
#define STORAGE_INPUT_MODE_INDEX 0
#endif

#ifndef STORAGE_DPAD_MODE_INDEX
#define STORAGE_DPAD_MODE_INDEX 4
#endif

#ifndef STORAGE_SOCD_MODE_INDEX
#define STORAGE_SOCD_MODE_INDEX 8
#endif

#define STORAGE_FIRST_AVAILBLE_INDEX 12

class GamepadStorage
{
	public:
		virtual void get(int index, void *data, uint16_t size);
		virtual void set(int index, void *data, uint16_t size);

		virtual void start() { }
		virtual void stop() { }
		virtual void save() { }
};

static GamepadStorage GamepadStore;

#endif
