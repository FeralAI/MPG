/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef GAMEPAD_DESCRIPTORS_H_
#define GAMEPAD_DESCRIPTORS_H_

#include <string.h>
#include "GamepadEnums.h"
#include "descriptors/HIDDescriptors.h"
#include "descriptors/SwitchDescriptors.h"
#include "descriptors/XInputDescriptors.h"

static const uint8_t *getConfigurationDescriptor(uint16_t *size, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			*size = sizeof(xinput_configuration_descriptor);
			return xinput_configuration_descriptor;

		case INPUT_MODE_SWITCH:
			*size = sizeof(switch_configuration_descriptor);
			return switch_configuration_descriptor;

		default:
			*size = sizeof(hid_configuration_descriptor);
			return hid_configuration_descriptor;
	}
}

static const uint8_t *getDeviceDescriptor(uint16_t *size, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			*size = sizeof(xinput_device_descriptor);
			return xinput_device_descriptor;

		case INPUT_MODE_SWITCH:
			*size = sizeof(switch_device_descriptor);
			return switch_device_descriptor;

		default:
			*size = sizeof(hid_device_descriptor);
			return hid_device_descriptor;
	}
}

static const uint8_t *getHIDDescriptor(uint16_t *size, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_SWITCH:
			*size = sizeof(switch_hid_descriptor);
			return switch_hid_descriptor;

		default:
			*size = sizeof(hid_hid_descriptor);
			return hid_hid_descriptor;
	}
}

static const uint8_t *getHIDReport(uint16_t *size, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_SWITCH:
			*size = sizeof(switch_report_descriptor);
			return switch_report_descriptor;

		default:
			*size = sizeof(hid_report_descriptor);
			return hid_report_descriptor;
	}
}

static const uint16_t *getStringDescriptor(uint16_t *size, InputMode mode, uint8_t index)
{
	static uint16_t utf16Descriptor[32];

	uint8_t charCount;
	const char *str;

	if (index == 0)
	{
		memcpy(&utf16Descriptor[1], xinput_string_descriptors[0], 2);
		charCount = 1;
	}
	else
	{
		// Convert ASCII string into UTF-16
		switch (mode)
		{
			case INPUT_MODE_XINPUT:
				str = (const char *)xinput_string_descriptors[index];
				break;

			case INPUT_MODE_SWITCH:
				str = (const char *)switch_string_descriptors[index];
				break;

			default:
				str = (const char *)hid_string_descriptors[index];
				break;
		}

		// Cap at max char
		charCount = strlen(str);
		if (charCount > 31)
			charCount = 31;

		for (uint8_t i = 0; i < charCount; i++)
		{
			utf16Descriptor[1 + i] = str[i];
		}
	}

	// first byte is length (including header), second byte is string type
	uint8_t payloadSize = (2 * charCount + 2);
	utf16Descriptor[0] = (0x03 << 8) | payloadSize;
	*size = payloadSize;
	return utf16Descriptor;
}

#endif
