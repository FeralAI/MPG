# MPG - Multi-Platform Gamepad Library

C++ library for USB gamepad handling with support for XInput, DirectInput and Nintendo Switch. MPG makes no assumptions about your USB implementation, but provides the platform-specific USB descriptors and HID/XID reports to make your implementation easier. Should be compatible with any type of C++ project (Arduino, PlatformIO, CMake, etc.).

## Gamepad Class

The heart of the library is the `Gamepad` class, and more specifically the virtual `Gamepad::setup()` and `Gamepad::read()` class methods that require implementation in your project. Use the `setup()` method for things like initializing microcontroller pins, performing analog calibration, etc., and the `read()` method to implement your platform-specific logic to read inputs and fill the `Gamepad::state` member variable. That `state` variable is then used in other class methods to generate USB report data for the selected input type.

A basic `Gamepad` class implementation in Arduino-land for a Leonardo might look like this:

```c++
/*
 * LeoPad.cpp
 *
 * Example uses direct register reads for faster performance.
 * digitalRead() can still work, but not recommended because SLOW.
 */

#include <Gamepad.h>

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

#define PORTB_INPUT_MASK 0b01111110
#define PORTD_INPUT_MASK 0b10011111
#define PORTF_INPUT_MASK 0b11110000

#define PORTB_INDEX 0
#define PORTD_INDEX 1
#define PORTF_INDEX 2

void Gamepad::setup() {
  // Set to input (invert mask to set to 0)
  DDRB = DDRB & ~PORTB_INPUT_MASK;
  DDRD = DDRD & ~PORTD_INPUT_MASK;
  DDRF = DDRF & ~PORTF_INPUT_MASK;

  // Set to high/pullup
  PORTB = PORTB | PORTB_INPUT_MASK;
  PORTD = PORTD | PORTD_INPUT_MASK;
  PORTF = PORTF | PORTF_INPUT_MASK;
}

void Gamepad::read() {
  // Get port states
  uint8_t ports[] = { ~PINB, ~PIND, ~PINF };

  // No analog triggers, but could read them here
  state.lt = 0;
  state.rt = 0;

  // Read digital inputs
  state.dpad = 0
    | ((ports[PORTF_INDEX] >> PORT_PIN_UP    & 1)  ? GAMEPAD_MASK_UP    : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_DOWN  & 1)  ? GAMEPAD_MASK_DOWN  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_LEFT  & 1)  ? GAMEPAD_MASK_LEFT  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_RIGHT & 1)  ? GAMEPAD_MASK_RIGHT : 0);

  state.buttons = 0
    | ((ports[PORTD_INDEX] >> PORT_PIN_K1 & 1)     ? GAMEPAD_MASK_B1    : 0) // Switch: B, XInput: A
    | ((ports[PORTD_INDEX] >> PORT_PIN_K2 & 1)     ? GAMEPAD_MASK_B2    : 0) // Switch: A, XInput: B
    | ((ports[PORTD_INDEX] >> PORT_PIN_P1 & 1)     ? GAMEPAD_MASK_B3    : 0) // Switch: Y, XInput: X
    | ((ports[PORTD_INDEX] >> PORT_PIN_P2 & 1)     ? GAMEPAD_MASK_B4    : 0) // Switch: X, XInput: Y
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

From there just call the `Gamepad` class methods:

```c++

#include <Gamepad.h>
#include <GamepadDescriptors.h>

Gamepad gamepad;

void setup() {
  gamepad.setup(); // Runs your custom setup logic
  gamepad.load();  // Get saved options if enabled
  gamepad.read();  // Perform an initial button read so we can set input mode

  // Check for input mode override
  if (gamepad.pressedR3())
    gamepad.inputMode = INPUT_MODE_HID;
  else if (gamepad.pressedS1())
    gamepad.inputMode = INPUT_MODE_SWITCH;
  else if (gamepad.pressedS2())
    gamepad.inputMode = INPUT_MODE_XINPUT;

  // TODO: Add your USB initialization logic here, something like:
  // setupHardware(gamepad.inputMode);
}

void loop() {
  // Create local static variables for loop
  static uint8_t *report;
  static const uint8_t reportSize = gamepad.getReportSize();

  gamepad.read();               // Read inputs
  gamepad.debounce();           // Run debouncing if required
  gamepad.hotkey();             // Check for hotkey changes, can react to returned hotkey action
  gamepad.process();            // Process the raw inputs into a usable state
  report = gamepad.getReport(); // Convert state to USB report for the selected input mode
  
  // TODO: Add your USB report sending logic here, something like:
  // sendReport(report, reportSize);
}

```
