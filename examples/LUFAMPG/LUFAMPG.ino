/*
 * Example MPG implementation using LUFA for ATmega32U4 boards.
 */

#include <MPG.h>
#include <LUFA.h>
#include "LUFADriver.h"

#define GAMEPAD_DEBOUNCE_MILLIS 5

// Define time function for gamepad debouncer
uint32_t getMillis() { return millis(); }

MPG mpg; // The gamepad instance

void setup()
{
	mpg.setup(); // Runs your custom setup logic
	mpg.load();  // Get saved options if enabled
	mpg.read();  // Perform an initial button read so we can set input mode

	// Use the inlined `pressed` convenience methods
	if (mpg.pressedR3())
		mpg.inputMode = INPUT_MODE_HID;
	else if (mpg.pressedS1())
		mpg.inputMode = INPUT_MODE_SWITCH;
	else if (mpg.pressedS2())
		mpg.inputMode = INPUT_MODE_XINPUT;

	// Initialize USB device driver
	setupHardware(mpg.inputMode);
}

void loop()
{
	static const uint8_t reportSize = mpg.getReportSize();  // Get report size from Gamepad instance
	static void *report;                                    // Pointer to our HID report
	static GamepadHotkey hotkey;                            // The last hotkey pressed
	static uint8_t lastReport[64] = { };                    // Use for report comparisons, better latency when only send on change

	mpg.read();                                             // Read raw inputs
	mpg.debounce();                                         // Run debouncing if enabled
	hotkey = mpg.hotkey();                                  // Check hotkey presses (dpad mode, socd mode, etc.), hotkey enum returned
	mpg.process();                                          // Perform final input processing before report conversion
	report = mpg.getReport();                               // Convert to USB report
	sendReport(report, reportSize);                         // Send it!
}
