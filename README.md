# MPG - Multi-Platform Gamepad Library

* [What is MPG?](#what-is-mpg)
  * [Features](#features)
* [Installation](#installation)
  * [Arduino IDE](#arduino-ide)
  * [PlatformIO](#platformio)
* [Usage](#usage)
  * [MPG Class](#mpg-class)
  * [MPGS Class](#mpgs-class)
  * [Buttons](#buttons)
    * [Function Buttons](#function-buttons)
  * [Hotkeys](#hotkeys)
    * [Home Button](#home-button)
    * [D-pad Modes](#d-pad-modes)
    * [SOCD Modes](#socd-modes)
  * [USB Descriptors](#usb-descriptors)
* [Contributing](#contributing)

## What is MPG?

MPG is a C++ library for processing and converting gamepad inputs, with support for XInput, DirectInput and Nintendo Switch. MPG has a fast and flexible API, but also makes no assumptions about your implementation details. This makes it a great option to use across different architectures, and facilitates easy integration into existing projects. Just implement a few methods and BYO USB implementation and you're all set!

### Features

* An abstract API for managing gamepad input state for:
  * XInput (PC, Android, Raspberry Pi, MiSTer, etc.)
  * DirectInput (PC, Mac, PS3)
  * Nintendo Switch
* A standard set of USB descriptors, report data structures and conversion methods for supported input types
* Per-button debouncing with a configurable interval
* Use D-pad to emulate Left or Right analog stick movement
* Supports common SOCD cleaning methods to prevent invalid directional inputs (👉😎👉 [/r/fightsticks](https://www.reddit.com/r/fightsticks/))
* Overridable hotkeys for on-the-fly configuration

## Installation

MPG is available in the Arduino and PlatformIO library feeds. Just open the library manager for your platform and search for `MPG`.

A zip package is available in the [Releases](https://github.com/FeralAI/MPG/releases) section for manual installation.

### Arduino IDE

If you're manually installing from a release zip, place the extracted folder (e.g. `MPG-0.1.1`) in your Arduino Libraries folder. On Windows this will usually be in `C:\Users\%USERNAME%\Documents\Arduino\libraries`.

MPG comes with a few example sketches: a full ATmega32U4 gamepad and a naive benchmarking application. The examples can be loaded from `File > Examples > MPG`.

### PlatformIO

An alterative installation option for PlatformIO is to edit `platformio.ini` and add MPG to the `lib_deps` property:

```ini
[env]
platform = wizio-pico
board = raspberry-pi-pico
framework = baremetal
build_type = release
build_flags =
  -D PICO_USB
lib_deps =
  feralai/MPG@^0.1.1
```

PlatformIO will download the dependency once the `platformio.ini` file is saved.

There are no example projects included, but the following projects are example implementations:

* [GP2040](https://github.com/FeralAI/GP2040/) - Multi-platform Gamepad Firmware for RP2040 microcontrollers
* [vsFIGHTER-Firmware](https://github.com/FeralAI/vsFIGHTER-Firmware/) - Firmware for vsFIGHTER controllers by Leaf Cutter Labs.

## Usage

There are two gamepad classes available: `MPG` and `MPGS`. The `MPG` class is the base class with all of the input handling and report conversion methods, while `MPGS` extends the base class with some additional methods for persisting gamepad options. Just create a derived class from one of these base classes, and use like this:

```c++
/*
 * MyAwesomeGamepad.ino
 */

#define GAMEPAD_DEBOUNCE_MILLIS 5

#include "Gamepad.h" // This will pull in our MPG implementation

Gamepad mpg(GAMEPAD_DEBOUNCE_MILLIS);

void setup() {
  mpg.setup(); // Runs your custom setup logic
  mpg.load();  // Load saved input mode, D-pad and SOCD options (MPGS class only)
  mpg.read();  // Perform an initial button read so we can set input mode

  // Use the inlined `pressed` convenience methods
  InputMode inputMode = mpg.options.inputMode;
  if (mpg.pressedR3())
    inputMode = INPUT_MODE_HID;
  else if (mpg.pressedS1())
    inputMode = INPUT_MODE_SWITCH;
  else if (mpg.pressedS2())
    inputMode = INPUT_MODE_XINPUT;

  if (inputMode != mpg.options.inputMode)
  {
    mpg.options.inputMode = inputMode;
    mpg.save(); // Input mode changed...better save it! (MPGS class only)
  }

  // TODO: Add your USB initialization logic here, something like:
  // setupHardware(mpg.options.inputMode);
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

* `MPG::setup()` - Use to configure pins, calibrate analog, etc.
* `MPG::read()` - Use to fill the `MPG.state` class member, which is then used in other class methods
* `getMillis()` - Global timing function for checking debounce state (can be no-op if `debounceMS` set to `0`)

An optimized Arduino `MPG` class implementation for a Leonardo might look like this:

```c++
#include <MPG.h>

class Gamepad : public MPG
{
  public:
    Gamepad(int debounceMS = 5) : MPG(debounceMS) { }
    void setup() override;
    void read() override;
}
```

```c++
/*
 * Gamepad.cpp
 *
 * Example uses direct register reads for faster performance.
 * digitalRead() can still work, but not recommended because SLOW.
 */

#include "Gamepad.h"

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
uint32_t getMillis() { return millis(); }

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
  // Get port states, invert since INPUT_PULLUP
  uint8_t ports[] = { ~PINB, ~PIND, ~PINF };

  // Read dpad inptus
  state.dpad = 0
    | ((ports[PORTF_INDEX] >> PORT_PIN_UP    & 1)  ? GAMEPAD_MASK_UP    : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_DOWN  & 1)  ? GAMEPAD_MASK_DOWN  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_LEFT  & 1)  ? GAMEPAD_MASK_LEFT  : 0)
    | ((ports[PORTF_INDEX] >> PORT_PIN_RIGHT & 1)  ? GAMEPAD_MASK_RIGHT : 0)
  ;

  // Read button inputs
  state.buttons = 0
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

  // No analog, but could read them here with analogRead() or fill outside of this method
  state.lt = 0;
  state.rt = 0;
  state.lx = GAMEPAD_JOYSTICK_MID;
  state.ly = GAMEPAD_JOYSTICK_MID;
  state.rx = GAMEPAD_JOYSTICK_MID;
  state.ry = GAMEPAD_JOYSTICK_MID;
}
```

Most of the code are the pin definitions and fancy formatting of bitwise button reads. You can also extend and override methods in the `MPG` class if you want to do something like change hotkeys, customize input processing steps, etc.

### MPGS Class

If your platform supports some form of persistent storage like EEPROM, you can use the `MPGS` abstract class instead. The differences between the `MPG` and `MPGS` classes are:

* `MPGS` class has two additional methods available for use:
  * `save()`
  * `load()`
* The `hotkey()` method is overridden to automatically save all options on change.
* `MPGS` requires two methods from `GamepadStorage.h` to be defined:
  * `GamepadOptions GamepadStorage::getGamepadOptions();`
  * `void GamepadStorage::setGamepadOptions(GamepadOptions options);`

Implement the `MPG` class as previously described, then implement the `GamepadStorage` class methods:

```c++
/*
 * storage.cpp
 *
 * Example storage for ATmega32U4.
 */

#include <GamepadStorage.h>
#include <EEPROM.h>

GamepadOptions GamepadStorage::getGamepadOptions() {
  GamepadOptions options =
  {
    .inputMode = InputMode::INPUT_MODE_XINPUT,
    .dpadMode = DpadMode::DPAD_MODE_DIGITAL,
    .socdMode = SOCDMode::SOCD_MODE_NEUTRAL,
  };

  EEPROM.get(0, options);
  return options;
}

void GamepadStorage::setGamepadOptions(GamepadOptions options) {
  EEPROM.put(0, options);
}

// 32u4 doesn't need these, but others might!
void GamepadStorage::start() { }
void GamepadStorage::save() { }

```

### Buttons

MPG uses a generic button labeling for gamepad state, which is then converted to the appropriate input type before sending. Here are the mappings of generic buttons to each supported platform/layout:

| MPG    | XInput | Switch  | PS3          | DirectInput  | Arcade |
| ------ | ------ | ------- | ------------ | ------------ | ------ |
| **B1** | A      | B       | Cross        | 2            | K1     |
| **B2** | B      | A       | Circle       | 3            | K2     |
| **B3** | X      | Y       | Square       | 1            | P1     |
| **B4** | Y      | X       | Triangle     | 4            | P2     |
| **L1** | LB     | L       | L1           | 5            | P4     |
| **R1** | RB     | R       | R1           | 6            | P3     |
| **L2** | LT     | ZL      | L2           | 7            | K4     |
| **R2** | RT     | ZR      | R2           | 8            | K3     |
| **S1** | Back   | Minus   | Select       | 9            | Coin   |
| **S2** | Start  | Plus    | Start        | 10           | Start  |
| **L3** | LS     | LS      | L3           | 11           | LS     |
| **R3** | RS     | RS      | R3           | 12           | RS     |
| **A1** | Guide  | Home    | -            | 13           | -      |
| **A2** | -      | Capture | -            | 14           | -      |

The MPG class contains helper methods for checking the state of each button, for instance `MPG::pressedB1()`, `MPG::pressedR3`, etc.

#### Function Buttons

There are two virtual function buttons for handling hotkey actions - `F1` and `F2`. By default they are mapped to the following two-button combinations:

* `F1` = `S1 + S2`
* `F2` = `L3 + R3`

The function button mapping can be overridden by setting the `MPG::f1Mask` and/or `MPG::f2Mask` class members:

### Hotkeys

MPG provides a predefined set of hotkeys for managing gamepad options. All options can be changed while running, and are persisted if gamepad storage is implemented.

```c++
Gamepad gamepad(DEBOUNCE_TIME_MS);

void setup()
{
  gamepad.f1Mask = (GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3);
  gamepad.f2Mask = GAMEPAD_MASK_A2;
}
```

The `GamepadState.h` file contains definitions for all gamepad input masks. Only button input (no D-pad) masks can be used for function mappings.

#### Home Button

If you do not have a dedicated Home button, you can activate it via the **`F1 + DPAD UP`** button combination.

> NOTE: The PS button in DirectInput/PS3 mode is currently not supported.

#### D-pad Modes

The D-pad can Choose from 3 operating modes for the D-pad:

* **`F1 + DPAD DOWN`** - D-pad
* **`F1 + DPAD LEFT`** - Emulate Left Analog stick
* **`F1 + DPAD RIGHT`** - Emulate Right Analog stick

#### SOCD Modes

Simultaneous Opposite Cardinal Direction (SOCD) cleaning will ensure the controller doesn't send invalid directional inputs to the computer/console, like Left + Right at the same time. There are 3 modes to choose from:

* **`F2 + DPAD UP`** - **Up Priority mode**: Up + Down = Up, Left + Right = Neutral (Hitbox behavior)
* **`F2 + DPAD DOWN`** - **Neutral mode**: Up + Down = Neutral, Left + Right = Neutral
* **`F2 + DPAD LEFT`** - **Last Input Priority (Last Win)**: Hold Up then hold Down = Down, then release and re-press Up = Up. Applies to both axes.

## USB Descriptors

MPG includes a set of USB descriptors and report data structures for the supported input types. There are 5 `get` methods available to make descriptor integration easier:

* `uint16_t *getStringDescriptor(uint16_t *size, InputMode mode, uint8_t index)`
* `uint8_t *getConfigurationDescriptor(uint16_t *size, InputMode mode)`
* `uint8_t *getDeviceDescriptor(uint16_t *size, InputMode mode)`
* `uint8_t *getHIDDescriptor(uint16_t *size, InputMode mode)` *(not used for XInput)*
* `uint8_t *getHIDReport(uint16_t *size, InputMode mode)` *(not used for XInput)*

All functions take in a pointer to a size variable and the `InputMode` (`getStringDescriptor` also requires the string index), and return a pointer to the selected descriptor. An example of usage:

```c++
// LUFA library descriptor callback function
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void **const address)
{
  const uint8_t descriptorType = (wValue >> 8);
  const uint8_t descriptorIndex = (wValue & 0xFF);
  uint16_t size = 0;

  switch (descriptorType)
  {
    case DTYPE_Device:
      *address = getDeviceDescriptor(&size, inputMode);
      break;

    case DTYPE_Configuration:
      *address = getConfigurationDescriptor(&size, inputMode);
      break;

    case DTYPE_String:
      *address = getStringDescriptor(&size, inputMode, descriptorIndex);
      break;

    case HID_DTYPE_HID:
      *address = getHIDDescriptor(&size, inputMode);
      break;

    case HID_DTYPE_Report:
      *address = getHIDReport(&size, inputMode);
      break;
  }

  return size;
}
```

## Support

If you would like to discuss features, issues, or anything else related to the MPG library please join the [MPG Discord channel](https://discord.gg/fxWDYxxg).

## Contributing

The library is under active development, with any and all ideas and contributions welcome.

If you'd like to submit a pull request for a bugfix/feature, please respect the `.editorconfig` file and match the established coding style.

If you'd like to report a bug or feature request, please [create an issue](https://github.com/FeralAI/MPG/issues/new).
