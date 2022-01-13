/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_OPTIONS_H_
#define GAMEPAD_OPTIONS_H_

#include <stdint.h>
#include "GamepadEnums.h"

struct __attribute((__packed__)) GamepadOptions
{
	InputMode inputMode;
	DpadMode dpadMode;
	SOCDMode socdMode;
	uint32_t checksum;
	bool invertXAxis;
	bool invertYAxis;
};

#endif
