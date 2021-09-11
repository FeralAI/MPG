/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef MPG_H_
#define MPG_H_

#include <stdio.h>
#include <stdint.h>

#include "GamepadEnums.h"
#include "GamepadConfig.h"
#include "GamepadDescriptors.h"
#include "GamepadState.h"
#include "GamepadDebouncer.h"
#include "GamepadStorage.h"

#define GAMEPAD_DIGITAL_INPUT_COUNT 18 // Total number of buttons, including D-pad

class MPG
{
	public:
		MPG(int debounceMS = 5, bool hasStorage = true)
			: debounceMS(debounceMS), hasStorage(hasStorage)
		{
			if (debounceMS > 0)
			{
				for (int i = 0; i < GAMEPAD_DIGITAL_INPUT_COUNT; i++)
					debouncers[i].setGamepadState(state);
			}
		}

		/**
		 * @brief The button debounce time in milliseconds. A value of 0 disables debouncing.
		 */
		const uint8_t debounceMS;

		/**
		 * @brief Flag to indicate the gamepad has storage support.
		 */
		const bool hasStorage;

		/**
		 * @brief The current D-pad mode.
		 */
		DpadMode dpadMode = DPAD_MODE_DIGITAL;

		/**
		 * @brief The current input mode.
		 */
		InputMode inputMode = INPUT_MODE_XINPUT;

		/**
		 * @brief The current SOCD cleaning mode.
		 */
		SOCDMode socdMode = SOCD_MODE_UP_PRIORITY;

		/**
		 * @brief The current gamepad state object.
		 */
		GamepadState state =
		{
			.buttons = 0,
			.lx = GAMEPAD_JOYSTICK_MID,
			.ly = GAMEPAD_JOYSTICK_MID,
			.rx = GAMEPAD_JOYSTICK_MID,
			.ry = GAMEPAD_JOYSTICK_MID,
			.lt = 0,
			.rt = 0,
		};

		/**
		 * @brief Flag to indicate analog trigger support.
		 */
		bool hasAnalogTriggers = false;

		/**
		 * @brief Flag to indicate Left analog stick support.
		 */
		bool hasLeftAnalogStick = false;

		/**
		 * @brief Flag to indicate Right analog stick support.
		 */
		bool hasRightAnalogStick = false;

		/**
		 * @brief Load the saved configuration from persitent storage
		 */
		virtual void load();

		/**
		 * @brief Save the current configuration to persitent storage
		 */
		virtual void save();

		/**
		 * @brief Perform pin setup and any other initialization the board requires
		 */
		virtual void setup();

		/**
		 * @brief Retrieve the inputs and save to the current state
		 */
		virtual void read();

		/**
		 * @brief Checks and executes any hotkey being pressed.
		 *
		 * @return GamepadHotkey The selected hotkey action
		 */
		virtual GamepadHotkey hotkey();

		/**
		 * @brief Run debouncing algorithm against current state inputs
		 */
		void debounce();

		/**
		 * @brief Process the inputs before sending state to host
		 */
		void process();

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
		HIDReport getHIDReport();

		/**
		 * @brief Generate USB report for Switch mode.
		 *
		 * @return SwitchReport* Switch report pointer
		 */
		SwitchReport getSwitchReport();

		/**
		 * @brief Generate USB report for XInput mode.
		 *
		 * @return XInputReport XInput report pointer.
		 */
		XInputReport getXInputReport();

		/**
		 * @brief Check for F1 button press. Can override in derived board class.
		 */
		virtual bool pressedF1();

		/**
		 * @brief Check for F1 button press. Can override in derived board class.
		 */
		virtual bool pressedF2();

		/**
		 * @brief Check for a button press.
		 */
		inline bool __attribute__((always_inline)) pressedButton(const uint32_t mask) { return (state.buttons & mask) == mask; }

		inline bool __attribute__((always_inline)) pressedUp()    { return pressedButton(GAMEPAD_MASK_UP); }
		inline bool __attribute__((always_inline)) pressedDown()  { return pressedButton(GAMEPAD_MASK_DOWN); }
		inline bool __attribute__((always_inline)) pressedLeft()  { return pressedButton(GAMEPAD_MASK_LEFT); }
		inline bool __attribute__((always_inline)) pressedRight() { return pressedButton(GAMEPAD_MASK_RIGHT); }
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

	protected:
		/**
		 * @brief Button debouncer instances.
		 */
		GamepadDebouncer debouncers[GAMEPAD_DIGITAL_INPUT_COUNT] =
		{
			GamepadDebouncer(GAMEPAD_MASK_UP, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_DOWN, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_LEFT, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_RIGHT, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_B1, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_B2, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_B3, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_B4, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_L1, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_R1, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_L2, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_R2, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_S1, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_S2, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_L3, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_R3, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_A1, debounceMS),
			GamepadDebouncer(GAMEPAD_MASK_A2, debounceMS),
		};
};

#endif
