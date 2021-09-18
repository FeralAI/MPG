/*
 * MPG benchmark/analysis test sketch
 */

#define DEBOUNCE_MILLIS 5

// Define time function for gamepad debouncer
#include <GamepadDebouncer.h>
uint32_t getMillis() { return millis(); }

#include <MPGS.h>
MPG mpg(DEBOUNCE_MILLIS); // The gamepad instance

void setup()
{
	Serial.begin(115200);
	while (!Serial) { }

	mpg.setup(); // Runs your custom setup logic
	mpg.read();  // Perform an initial button read so we can set input mode

	// Use the inlined `pressed` convenience methods
	if (mpg.pressedR3())
		mpg.inputMode = INPUT_MODE_HID;
	else if (mpg.pressedS1())
		mpg.inputMode = INPUT_MODE_SWITCH;
	else if (mpg.pressedS2())
		mpg.inputMode = INPUT_MODE_XINPUT;
}

void loop()
{
	static const uint8_t reportSize = mpg.getReportSize();  // Get report size from Gamepad instance
	static GamepadHotkey hotkey;                            // The last hotkey pressed

	static uint32_t startTime = 0;
	static uint32_t endTime = 0;

	uint32_t totalTime = 0;

	// Read raw inputs
	startTime = micros();
	mpg.read();
	endTime = micros() - startTime;
	Serial.print("R: ");
	Serial.print(endTime);
	totalTime += endTime;

	// Run debouncing if enabled
	startTime = micros();
	mpg.debounce();
	endTime = micros() - startTime;
	Serial.print(", D: ");
	Serial.print(endTime);
	totalTime += endTime;

	// Check hotkey presses (D-pad mode, SOCD mode, etc.), hotkey enum returned
	startTime = micros();
	hotkey = mpg.hotkey();
	endTime = micros() - startTime;
	Serial.print(", H: ");
	Serial.print(endTime);
	totalTime += endTime;

	// Perform final input processing (SOCD cleaning, LS/RS emulation, etc.)
	startTime = micros();
	mpg.process();
	endTime = micros() - startTime;
	Serial.print(", P: ");
	Serial.print(endTime);
	totalTime += endTime;

	// Convert to report
	startTime = micros();
	mpg.getReport();
	endTime = micros() - startTime;
	Serial.print(", U: ");
	Serial.print(endTime);
	totalTime += endTime;

	Serial.print(", T: ");
	Serial.println(totalTime);
}
