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
 * Max: R: 24, D: 48, H: 8, P: 12, U: 16, T: 108
 *
 */

#define DEBOUNCE_MILLIS 5

#include <GamepadDebouncer.h>
uint32_t getMillis() { return millis(); }

#include <MPGS.h>
MPG mpg(DEBOUNCE_MILLIS);

void setup()
{
	Serial.begin(115200);
	while (!Serial) { }

	mpg.setup();
	mpg.read();

	if (mpg.pressedR3())
		mpg.options.inputMode = INPUT_MODE_HID;
	else if (mpg.pressedS1())
		mpg.options.inputMode = INPUT_MODE_SWITCH;
	else if (mpg.pressedS2())
		mpg.options.inputMode = INPUT_MODE_XINPUT;

  Serial.println("read,debounce,hotkeys,process,report,total");
}

void loop()
{
	static const uint8_t reportSize = mpg.getReportSize();
	static GamepadHotkey hotkey;

	static uint32_t startTime = 0;
	static uint32_t endTime = 0;

	uint32_t totalTime = 0;

	// Read raw inputs
	startTime = micros();
	mpg.read();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Run debouncing if enabled
	startTime = micros();
	mpg.debounce();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Check hotkey presses (D-pad mode, SOCD mode, etc.), hotkey enum returned
	startTime = micros();
	hotkey = mpg.hotkey();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Perform final input processing (SOCD cleaning, LS/RS emulation, etc.)
	startTime = micros();
	mpg.process();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Convert to USB report
	startTime = micros();
	mpg.getReport();
	endTime = micros() - startTime;
	Serial.print(endTime);
	Serial.print(",");
	totalTime += endTime;

	// Total method time
	Serial.println(totalTime);
}
