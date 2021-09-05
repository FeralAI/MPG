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

static uint16_t getConfigurationDescriptor(const uint8_t *buffer, InputMode mode);
static uint16_t getDeviceDescriptor(const uint8_t *buffer, InputMode mode);
static uint16_t getHIDDescriptor(const uint8_t *buffer, InputMode mode);
static uint16_t getHIDReport(const uint8_t *buffer, InputMode mode);
static uint16_t getStringDescriptor(const uint16_t *buffer, InputMode mode, uint8_t index);

#endif
