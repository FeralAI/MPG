/*
 * MPG benchmark/analysis test sketch
 *
 * Not a perfect mechanism for benchmarking, but will give a good indicator if performance takes a hit somewhere. Measures the
 * following MPG methods in microseconds (μs):
 *
 *     read()
 *     debounce()
 *     hotkey()
 *     process()
 *     getReport() [xinput]
 *
 * 2021-09-18
 * ---------------------------------------------
 * Min: R: 16, D: 36, H: 4, P:  8, U: 12, T:  76
 * Typ: R: 16, D: 40, H: 8, P:  8, U: 16, T:  88
 * Max: R: 24, D: 44, H: 8, P: 12, U: 16, T: 104
 *
 */

#define DEBOUNCE_MILLIS 5

#include <GamepadDebouncer.h>
uint32_t getMillis() { return millis(); }

#include "Gamepad.h"
Gamepad gamepad(DEBOUNCE_MILLIS);

void setup()
{
	Serial.begin(115200);
	while (!Serial) { }

	gamepad.setup();
	gamepad.read();

	if (gamepad.pressedR3())
		gamepad.options.inputMode = INPUT_MODE_HID;
	else if (gamepad.pressedS1())
		gamepad.options.inputMode = INPUT_MODE_SWITCH;
	else if (gamepad.pressedS2())
		gamepad.options.inputMode = INPUT_MODE_XINPUT;

  Serial.println("read,debounce,hotkeys,process,report,total,mintotal,maxtotal");
}

void loop()
{
	static const uint8_t reportSize = gamepad.getReportSize();
	static GamepadHotkey hotkey;

	static uint32_t startTime = 0;
	static uint32_t endTime = 0;
	static uint32_t minTime = UINT32_MAX;
	static uint32_t maxTime = 0;

	uint32_t totalTime = 0;

	// Read raw inputs
	startTime = micros();
	gamepad.read();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Run debouncing if enabled
	startTime = micros();
	gamepad.debounce();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Check hotkey presses (D-pad mode, SOCD mode, etc.), hotkey enum returned
	startTime = micros();
	hotkey = gamepad.hotkey();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Perform final input processing (SOCD cleaning, LS/RS emulation, etc.)
	startTime = micros();
	gamepad.process();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Convert to USB report
	startTime = micros();
	gamepad.getReport();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Total method time
	Serial.print(totalTime);
	Serial.print(",");

	// Calc min/max times
	minTime = (totalTime < minTime) ? totalTime : minTime;
	maxTime = (totalTime > maxTime) ? totalTime : maxTime;

	Serial.print(minTime);
	Serial.print(",");
	Serial.println(maxTime);
}
