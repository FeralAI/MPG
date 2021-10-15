#ifndef LUFA_DRIVER_
#define LUFA_DRIVER_

#include <avr/power.h>
#include <avr/wdt.h>
#include "LUFAConfig.h"
#include <LUFA/LUFA/Drivers/USB/USB.h>
#include <GamepadDescriptors.h>

#define EPADDR_IN  (ENDPOINT_DIR_IN  | 1)
#define EPADDR_OUT (ENDPOINT_DIR_OUT | 2)

extern char USB_STRING_MANUFACTURER[];
extern char USB_STRING_PRODUCT[];
extern char USB_STRING_VERSION[];

// LUFA USB descriptor callback

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** const DescriptorAddress)
	ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#ifdef __cplusplus
extern "C" {
#endif

void setupHardware(InputMode mode);
void sendReport(void *data, uint8_t size);

// LUFA USB device event handlers

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#ifdef __cplusplus
}
#endif

#endif
