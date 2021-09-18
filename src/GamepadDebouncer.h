/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 * SPDX-FileCopyrightText: Copyright (c) 2013 thomasfredericks
 *
 * Debouncer class ported from the Arduino Bounce2 library (MIT license).
 */

#ifndef GAMEPAD_DEBOUNCER_H_
#define GAMEPAD_DEBOUNCER_H_

#include <stdint.h>
#include "GamepadState.h"

// Implement this wrapper function for your platform
uint32_t getMillis();

// Bounce2 base Debouncer class containing only the BOUNCE_WITH_PROMPT_DETECTION logic.
class GamepadDebouncer
{
	private:
		static const uint8_t DEBOUNCED_STATE = 0b00000001; // Final returned calculated debounced state
		static const uint8_t UNSTABLE_STATE  = 0b00000010; // Actual last state value behind the scene
		static const uint8_t CHANGED_STATE   = 0b00000100; // The DEBOUNCED_STATE has changed since last update()

		inline void __attribute__((always_inline)) changeState();
		inline void __attribute__((always_inline)) setStateFlag(const uint8_t flag) { state |= flag; }
		inline void __attribute__((always_inline)) unsetStateFlag(const uint8_t flag) { state &= ~flag; }
		inline void __attribute__((always_inline)) toggleStateFlag(const uint8_t flag) { state ^= flag; }
		inline bool __attribute__((always_inline)) getStateFlag(const uint8_t flag) const { return ((state & flag) != 0); }

	public:
		GamepadDebouncer();
		GamepadDebouncer(uint32_t m, uint16_t i = 0)
				: inputMask(m), intervalMillis(i) {}

		uint32_t inputMask;

		__attribute__((always_inline)) inline void setGamepadState(GamepadState &gamepadState)
		{
			this->gamepadState = &gamepadState;
		}

		void setInterval(uint16_t intervalMillis);
		bool update();
		bool read() const;
		bool fell() const;
		bool rose() const;

		bool changed() const { return getStateFlag(CHANGED_STATE); }

		unsigned long currentDuration() const;
		unsigned long previousDuration() const;

	protected:
		void begin();
		unsigned long previousMillis;
		uint16_t intervalMillis;
		uint8_t state;
		unsigned long stateChangeLastTime;
		unsigned long durationOfPreviousState;
		GamepadState *gamepadState;
};

#endif
