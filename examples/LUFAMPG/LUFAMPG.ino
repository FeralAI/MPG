/*
 * Example MPG implementation using LUFA for ATmega32U4 boards.
 */

#define DEBOUNCE_MILLIS 5

#include <LUFA.h>
#include "LUFADriver.h"

// Define time function for gamepad debouncer
#include <GamepadDebouncer.h>
uint32_t getMillis() { return millis(); }

#include "TUFGamepad.h"
TUFGamepad gamepad(DEBOUNCE_MILLIS); // The gamepad instance

char USB_STRING_MANUFACTURER[] = "FeralAI";
char USB_STRING_PRODUCT[] = "MPG Sample Gamepad";
char USB_STRING_VERSION[] = "1.0";

void setup()
{
	gamepad.setup(); // Runs your custom setup logic
	gamepad.load();  // Get saved options if enabled
	gamepad.read();  // Perform an initial button read so we can set input mode

	// Use the inlined `pressed` convenience methods
	InputMode inputMode = gamepad.options.inputMode;
	if (gamepad.pressedR3())
		inputMode = INPUT_MODE_HID;
	else if (gamepad.pressedS1())
		inputMode = INPUT_MODE_SWITCH;
	else if (gamepad.pressedS2())
		inputMode = INPUT_MODE_XINPUT;

	if (inputMode != gamepad.options.inputMode)
	{
		gamepad.options.inputMode = inputMode;
		gamepad.save();
	}

	// Initialize USB device driver
	setupHardware(gamepad.options.inputMode);
}

void loop()
{
	static const uint8_t reportSize = gamepad.getReportSize();  // Get report size from Gamepad instance
	static GamepadHotkey hotkey;                            // The last hotkey pressed

	gamepad.read();                                             // Read raw inputs
	gamepad.debounce();                                         // Run debouncing if enabled
	hotkey = gamepad.hotkey();                                  // Check hotkey presses (D-pad mode, SOCD mode, etc.), hotkey enum returned
	gamepad.process();                                          // Perform final input processing (SOCD cleaning, LS/RS emulation, etc.)
	sendReport(gamepad.getReport(), reportSize);                // Convert and send it!
}
