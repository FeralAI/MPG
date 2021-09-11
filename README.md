# MPG - Multi-Platform Gamepad Library

MPG is a C++ library for managing, manipulating and converting inputs into a usable state for your gamepad projects. With MPG you can:

* Track input state for XInput, DirectInput or Nintendo Switch
* Use a standard set of USB descriptors, report data structures and conversion methods for supported platforms
* Run per-button debouncing with a configurable interval
* Detect and react to hotkey actions
* Emulate Left and Right analog stick movement with digital (D-pad) inputs
* Support a variety of SOCD cleaning methods to prevent invalid directional inputs (finger guns at you /r/fightsticks)

## Usage

There are two gamepad classes available: `MPG` and `MPGS`. The `MPG` class is the base class with all of the input handling and report conversion methods, while `MPGS` extends the base class with some additional methods for persisting gamepad options. The main file of an MPG application will look something like this:

```c++
/*
 * MyAwesomeGamepad.ino
 */

#define GAMEPAD_DEBOUNCE_MILLIS 5

#include <MPGS.h>

MPGS mpg(GAMEPAD_DEBOUNCE_MILLIS);

void setup() {
  mpg.setup(); // Runs your custom setup logic
  mpg.load();  // Load saved input mode, D-pad and SOCD options (MPGS class only)
  mpg.read();  // Perform an initial button read so we can set input mode

  // Use the inlined `pressed` convenience methods
  InputMode inputMode = mpg.inputMode;
  if (mpg.pressedR3())
    inputMode = INPUT_MODE_HID;
  else if (mpg.pressedS1())
    inputMode = INPUT_MODE_SWITCH;
  else if (mpg.pressedS2())
    inputMode = INPUT_MODE_XINPUT;

  if (inputMode != mpg.inputMode)
  {
    mpg.inputMode = inputMode;
    mpg.save(); // Input mode changed...better save it! (MPGS class only)
  }

  // TODO: Add your USB initialization logic here, something like:
  // setupHardware(mpg.inputMode);
}

void loop() {
  // Cache report pointer and size value
  static uint8_t *report;
  static const uint8_t reportSize = mpg.getReportSize();

  mpg.read();               // Read inputs
  mpg.debounce();           // Run debouncing if required
  mpg.hotkey();             // Check for hotkey changes, can react to returned hotkey action
  mpg.process();            // Process the raw inputs into a usable state
  report = mpg.getReport(); // Convert state to USB report for the selected input mode
  
  // TODO: Add your USB report sending logic here, something like:
  // sendReport(report, reportSize);
}

```

### MPG Class

MPG provides some declarations and virtual methods that require implementation in order for the library to function correctly. A basic `MPG` class implementation requires just three methods to be defined:

* **MPG::setup()** - Use to configure pins, calibrate analog, etc.
* **MPG::read()** - Use to fill the `MPG.state` class member, which is then used in other class methods
* **GamepadDebouncer::getMillis()** - Used to get timing for checking debounce state (can be no-op if `debounceMS` set to `0`)

An optimized Arduino `MPG` class implementation for a Leonardo might look like this:

```c++
/*
 * Gamepad.cpp
 *
 * Example uses direct register reads for faster performance.
 * digitalRead() can still work, but not recommended because SLOW.
 */

#include <MPG.h>

/* Define port/pins for easy readability */

#define PORT_PIN_UP     PF7 // A0
#define PORT_PIN_DOWN   PF6 // A1
#define PORT_PIN_LEFT   PF5 // A2
#define PORT_PIN_RIGHT  PF4 // A3
#define PORT_PIN_P1     PD2 // 1
#define PORT_PIN_P2     PD3 // 0
#define PORT_PIN_P3     PB1 // 15
#define PORT_PIN_P4     PD4 // 4
#define PORT_PIN_K1     PD0 // 3/SCL
#define PORT_PIN_K2     PD1 // 2/SDA
#define PORT_PIN_K3     PB6 // 10
#define PORT_PIN_K4     PD7 // 6
#define PORT_PIN_SELECT PB3 // 14
#define PORT_PIN_START  PB2 // 16
#define PORT_PIN_LS     PB4 // 8
#define PORT_PIN_RS     PB5 // 9

/* Input masks and indexes for setup and read logic */

#define PORTB_INPUT_MASK 0b01111110
#define PORTD_INPUT_MASK 0b10011111
#define PORTF_INPUT_MASK 0b11110000

#define PORTB_INDEX 0
#define PORTD_INDEX 1
#define PORTF_INDEX 2


/* Real implementation starts here... */

// Define time function for gamepad debouncer
uint32_t GamepadDebouncer::getMillis() { return millis(); }

void MPG::setup() {
  // Set to input (invert mask to set to 0)
  DDRB = DDRB & ~PORTB_INPUT_MASK;
  DDRD = DDRD & ~PORTD_INPUT_MASK;
  DDRF = DDRF & ~PORTF_INPUT_MASK;

  // Set to high/pullup
  PORTB = PORTB | PORTB_INPUT_MASK;
  PORTD = PORTD | PORTD_INPUT_MASK;
  PORTF = PORTF | PORTF_INPUT_MASK;
}


void MPG::read() {
  // Get port states, invert since INPUT_PULLUP
  uint8_t ports[] = { ~PINB, ~PIND, ~PINF };

  // No analog, but could read them here with analogRead()
  state.lt = 0;
  state.rt = 0;
  state.lx = GAMEPAD_JOYSTICK_MID;
  state.ly = GAMEPAD_JOYSTICK_MID;
  state.rx = GAMEPAD_JOYSTICK_MID;
  state.ry = GAMEPAD_JOYSTICK_MID;

  // Read digital inputs
  state.buttons = 0
    | ((ports[PORTF_INDEX] >> PORT_PIN_UP    & 1)  ? GAMEPAD_MASK_UP    : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_DOWN  & 1)  ? GAMEPAD_MASK_DOWN  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_LEFT  & 1)  ? GAMEPAD_MASK_LEFT  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_RIGHT & 1)  ? GAMEPAD_MASK_RIGHT : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_K1 & 1)     ? GAMEPAD_MASK_B1    : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_K2 & 1)     ? GAMEPAD_MASK_B2    : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_P1 & 1)     ? GAMEPAD_MASK_B3    : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_P2 & 1)     ? GAMEPAD_MASK_B4    : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_P4 & 1)     ? GAMEPAD_MASK_L1    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_P3 & 1)     ? GAMEPAD_MASK_R1    : 0)
    | ((ports[PORTD_INDEX] >> PORT_PIN_K4 & 1)     ? GAMEPAD_MASK_L2    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_K3 & 1)     ? GAMEPAD_MASK_R2    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_SELECT & 1) ? GAMEPAD_MASK_S1    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_START & 1)  ? GAMEPAD_MASK_S2    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_LS & 1)     ? GAMEPAD_MASK_L3    : 0)
    | ((ports[PORTB_INDEX] >> PORT_PIN_RS & 1)     ? GAMEPAD_MASK_R3    : 0)
  ;
}
```

Most of the code are the pin definitions and fancy formatting of bitwise button reads. You can also extend and override methods in the `MPG` class if you want to do something like change hotkeys, customize input processing steps, etc.

### MPGS Class

If your platform supports some form of persistent storage, like EEPROM, you can use the `MPGS` class instead. The differences between the `MPG` and `MPGS` classes are:

* `MPGS` class has two additional methods available for use:
  * `save()`
  * `load()`
* The `hotkey()` method is overridden to automatically save all options on change.
* `MPGS` requires two methods in `GamepadStorage` to be defined:
  * `GamepadStorage::get(int index, void *data, uint16_t size)`
  * `GamepadStorage::set(int index, void *data, uint16_t size)`

Then implement the `GamepadStorage` class methods, something like this ATmega32U4 example:

```c++
/*
 * MyAwesomeStorage.cpp
 */

#include <string.h>
#include <GamepadStorage.h>
#include <EEPROM.h>

void GamepadStorage::get(int index, void *data, uint16_t size)
{
  uint8_t buffer[size] = { };
  for (int i = 0; i < size; i++)
    EEPROM.get(index + i, buffer[i]);

  memcpy(data, buffer, size);
}

void GamepadStorage::set(int index, void *data, uint16_t size)
{
  uint8_t buffer[size] = { };
  memcpy(buffer, data, size);
  for (int i = 0; i < size; i++)
    EEPROM.put(index + i, buffer[i]);
}

```
