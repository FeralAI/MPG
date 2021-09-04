/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_DESCRIPTORS_H_
#define GAMEPAD_DESCRIPTORS_H_

#include "GamepadEnums.h"
#include "descriptors/HIDDescriptors.h"
#include "descriptors/SwitchDescriptors.h"
#include "descriptors/XInputDescriptors.h"

static uint16_t getStringDescriptor(uint16_t *buffer, InputMode mode, uint8_t index)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			buffer = (uint16_t *)xinput_string_descriptors[index];
			return sizeof(xinput_string_descriptors[index]);

		case INPUT_MODE_SWITCH:
			buffer = (uint16_t *)switch_string_descriptors[index];
			return sizeof(switch_string_descriptors[index]);

		default:
			buffer = (uint16_t *)hid_string_descriptors[index];
			return sizeof(hid_string_descriptors[index]);
	}
}

#endif
