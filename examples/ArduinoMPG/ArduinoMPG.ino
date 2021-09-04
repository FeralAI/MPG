/*
 * MPG example for Arduino Leonardo and other 32u4 compatible boards using the HID-Project library.
 */

#include "MPG.h"
#include "HID-Project.h"

#define GAMEPAD_DEBOUNCE_MILLIS 5

// Define time function for gamepad debouncer
uint32_t getMillis() { return millis(); }

MPG mpg;

void setup()
{
	// Set up controller
	mpg.setup();

	// Set to HID mode
	mpg.inputMode = INPUT_MODE_HID;

	// Start up USB device
	Gamepad.begin();
}

void loop()
{
	static const uint8_t reportSize = mpg.getReportSize();  // Get report size from Gamepad instance
	static HIDReport *report;                               // Pointer to our HID report
	static GamepadHotkey hotkey;                            // The last hotkey pressed
	static uint8_t previousReport[sizeof(HIDReport)] = { }; // Use for report comparisons, better latency when only send on change

	mpg.read();                                             // Read raw inputs
	mpg.debounce();                                         // Run debouncing if enabled
	hotkey = mpg.hotkey();                                  // Check hotkey presses (dpad mode, socd mode, etc.), hotkey enum returned
	mpg.process();                                          // Perform final input processing before report conversion
	report = mpg.getHIDReport();                            // Convert to USB report

	if (memcmp(&previousReport, &report, reportSize) != 0)  // Did the report change?
	{
		Gamepad.releaseAll();

		// HID-Project HAT values put neutral at 0 instead of 8, need to compensate
		if (report->hat < HID_HAT_NOTHING)
			Gamepad.dPad1((report->hat + 1));
		else
			Gamepad.dPad1(GAMEPAD_DPAD_CENTERED);

		Gamepad.buttons(report->buttons);

		// MPG uses 8-bit values for analog values in HID reports, scale the value correctly
		Gamepad.xAxis((report->lx * 255) - 32768);
		Gamepad.yAxis((report->ly * 255) - 32768);
		Gamepad.rxAxis((report->rx * 255) - 32768);
		Gamepad.ryAxis((report->ry * 255) - 32768);

		Gamepad.write();
		memcpy(&previousReport, report, reportSize);         // Save as previous report
	}
}
