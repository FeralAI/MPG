/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include <stdio.h>
#include <stdint.h>

#include "GamepadEnums.h"
#include "GamepadOptions.h"
#include "GamepadConfig.h"
#include "GamepadDescriptors.h"
#include "GamepadState.h"
#include "GamepadDebouncer.h"

#define GAMEPAD_DIGITAL_INPUT_COUNT 18 // Total number of buttons, including D-pad

class MPG
{
	public:
		MPG(int debounceMS = 5)
			: debounceMS(debounceMS)
			, f1Mask((GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2))
			, f2Mask((GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3))
			, debouncer(debounceMS)
		{
		}

		/**
		 * @brief The button debounce time in milliseconds. A value of 0 disables debouncing.
		 */
		const uint8_t debounceMS;

		/**
		 * @brief The input mask for the F1 button
		 */
		uint16_t f1Mask;

		/**
		 * @brief The input mask for the F2 button
		 */
		uint16_t f2Mask;

		/**
		 * @brief The current D-pad mode.
		 */
		GamepadOptions options;

		/**
		 * @brief The current gamepad state object.
		 */
		GamepadState state;

		/**
		 * @brief Flag to indicate analog trigger support.
		 */
		bool hasAnalogTriggers {false};

		/**
		 * @brief Flag to indicate Left analog stick support.
		 */
		bool hasLeftAnalogStick {false};

		/**
		 * @brief Flag to indicate Right analog stick support.
		 */
		bool hasRightAnalogStick {false};

		/**
		 * @brief Perform pin setup and any other initialization the board requires. Derived classes must overide this member.
		 */
		virtual void setup() = 0;

		/**
		 * @brief Retrieve the inputs and save to the current state. Derived classes must overide this member.
		 */
		virtual void read() = 0;

		/**
		 * @brief Checks and executes any hotkey being pressed.
		 *
		 * @return GamepadHotkey The selected hotkey action
		 */
		virtual GamepadHotkey hotkey();

		/**
		 * @brief Run debouncing algorithm against current state inputs
		 */
		inline void __attribute__((always_inline)) debounce() { debouncer.debounce(&state); }

		/**
		 * @brief Process the inputs before sending state to host
		 */
		virtual void process();

		/**
		 * @brief Generate USB report for the current input mode.
		 *
		 * @return uint8_t* Report data pointer
		 */
		void *getReport();

		/**
		 * @brief Get the size of the USB report for the current input mode.
		 *
		 * @return uint8_t Report data size
		 */
		uint16_t getReportSize();

		/**
		 * @brief Generate USB report for HID mode.
		 *
		 * @return HIDReport* HID report pointer
		 */
		HIDReport *getHIDReport();

		/**
		 * @brief Generate USB report for Switch mode.
		 *
		 * @return SwitchReport* Switch report pointer
		 */
		SwitchReport *getSwitchReport();

		/**
		 * @brief Generate USB report for XInput mode.
		 *
		 * @return XInputReport XInput report pointer.
		 */
		XInputReport *getXInputReport();

		/**
		 * @brief Check for a button press. Used by `pressed[Button]` helper methods.
		 */
		inline bool __attribute__((always_inline)) pressedButton(const uint16_t mask) { return (state.buttons & mask) == mask; }

		/**
		 * @brief Check for a dpad press. Used by `pressed[Dpad]` helper methods.
		 */
		inline bool __attribute__((always_inline)) pressedDpad(const uint8_t mask) { return (state.dpad & mask) == mask; }

		inline bool __attribute__((always_inline)) pressedUp()    { return pressedDpad(GAMEPAD_MASK_UP); }
		inline bool __attribute__((always_inline)) pressedDown()  { return pressedDpad(GAMEPAD_MASK_DOWN); }
		inline bool __attribute__((always_inline)) pressedLeft()  { return pressedDpad(GAMEPAD_MASK_LEFT); }
		inline bool __attribute__((always_inline)) pressedRight() { return pressedDpad(GAMEPAD_MASK_RIGHT); }
		inline bool __attribute__((always_inline)) pressedB1()    { return pressedButton(GAMEPAD_MASK_B1); }
		inline bool __attribute__((always_inline)) pressedB2()    { return pressedButton(GAMEPAD_MASK_B2); }
		inline bool __attribute__((always_inline)) pressedB3()    { return pressedButton(GAMEPAD_MASK_B3); }
		inline bool __attribute__((always_inline)) pressedB4()    { return pressedButton(GAMEPAD_MASK_B4); }
		inline bool __attribute__((always_inline)) pressedL1()    { return pressedButton(GAMEPAD_MASK_L1); }
		inline bool __attribute__((always_inline)) pressedR1()    { return pressedButton(GAMEPAD_MASK_R1); }
		inline bool __attribute__((always_inline)) pressedL2()    { return pressedButton(GAMEPAD_MASK_L2); }
		inline bool __attribute__((always_inline)) pressedR2()    { return pressedButton(GAMEPAD_MASK_R2); }
		inline bool __attribute__((always_inline)) pressedS1()    { return pressedButton(GAMEPAD_MASK_S1); }
		inline bool __attribute__((always_inline)) pressedS2()    { return pressedButton(GAMEPAD_MASK_S2); }
		inline bool __attribute__((always_inline)) pressedL3()    { return pressedButton(GAMEPAD_MASK_L3); }
		inline bool __attribute__((always_inline)) pressedR3()    { return pressedButton(GAMEPAD_MASK_R3); }
		inline bool __attribute__((always_inline)) pressedA1()    { return pressedButton(GAMEPAD_MASK_A1); }
		inline bool __attribute__((always_inline)) pressedA2()    { return pressedButton(GAMEPAD_MASK_A2); }
		inline bool __attribute__((always_inline)) pressedF1()    { return pressedButton(f1Mask); }
		inline bool __attribute__((always_inline)) pressedF2()    { return pressedButton(f2Mask); }

	protected:
		/**
		 * @brief Button debouncer instance.
		 */
		GamepadDebouncer debouncer;
};
