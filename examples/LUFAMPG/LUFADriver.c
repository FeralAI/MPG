#include "LUFADriver.h"

static HIDReport hidReportData;
static SwitchReport switchReportData;
static XInputReport xinputReportData;

static InputMode inputMode;
static void *reportData;
static uint8_t reportSize;
static uint8_t lastReportBytes[64] = {0};

// Configures hardware and peripherals, such as the USB peripherals.
void setupHardware(InputMode mode)
{
	inputMode = mode;

	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);

	// We can then initialize our hardware and peripherals, including the USB stack.
	USB_Init();

	// Enable interrupts after USB init
	GlobalInterruptEnable();
}

void sendReport(void *data, uint8_t size)
{
	reportData = data;
	reportSize = size;
	if (
		memcmp(lastReportBytes, reportData, reportSize) != 0 && // Did the report change?
		USB_DeviceState == DEVICE_STATE_Configured              // Is USB ready?
	)
	{
		Endpoint_SelectEndpoint(EPADDR_OUT);
		if (Endpoint_IsOUTReceived())
		{
			if (Endpoint_IsReadWriteAllowed())
			{
				SwitchOutReport JoystickOutputData;
				Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL);
				// At this point, we can react to this data.
			}

			Endpoint_ClearOUT();
		}

		Endpoint_SelectEndpoint(EPADDR_IN);

		if (Endpoint_IsINReady())
		{
			Endpoint_Write_Stream_LE(reportData, reportSize, NULL);
			Endpoint_ClearIN();
			memcpy(lastReportBytes, reportData, reportSize);
			memset(reportData, 0, reportSize);
		}
	}

	USB_USBTask();
}

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void **const address)
{
	const uint8_t descriptorType = (wValue >> 8);
	const uint8_t descriptorIndex = (wValue & 0xFF);
	uint16_t size = NO_DESCRIPTOR;

	switch (descriptorType)
	{
		case DTYPE_Device:
			*address = getDeviceDescriptor(&size, inputMode);
			break;

		case DTYPE_Configuration:
			*address = getConfigurationDescriptor(&size, inputMode);
			break;

		case DTYPE_String:
			switch (descriptorIndex)
			{
				case 1:
					*address = convertStringDescriptor(&size, USB_STRING_MANUFACTURER, strlen(USB_STRING_MANUFACTURER));
					break;
				case 2:
					*address = convertStringDescriptor(&size, USB_STRING_PRODUCT, strlen(USB_STRING_PRODUCT));
					break;
				case 3:
					*address = convertStringDescriptor(&size, USB_STRING_VERSION, strlen(USB_STRING_VERSION));
					break;
				default:
					*address = getStringDescriptor(&size, inputMode, descriptorIndex);
					break;
			}
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

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void)
{
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void)
{
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void)
{
	switch (inputMode)
	{
		case INPUT_MODE_XINPUT:
			Endpoint_ConfigureEndpoint((ENDPOINT_DIR_IN | 3), EP_TYPE_INTERRUPT, 32, 1);
			Endpoint_ConfigureEndpoint(EPADDR_IN, EP_TYPE_INTERRUPT, XINPUT_ENDPOINT_SIZE, 1);
			break;

		default:
			Endpoint_ConfigureEndpoint(EPADDR_OUT, EP_TYPE_INTERRUPT, HID_ENDPOINT_SIZE, 1);
			Endpoint_ConfigureEndpoint(EPADDR_IN, EP_TYPE_INTERRUPT, HID_ENDPOINT_SIZE, 1);
			break;
	}
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void)
{
	switch (USB_ControlRequest.bRequest)
	{
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				switch (inputMode)
				{
					case INPUT_MODE_XINPUT:
						Endpoint_Write_Control_Stream_LE(&xinputReportData, XINPUT_ENDPOINT_SIZE);
						break;

					case INPUT_MODE_SWITCH:
						Endpoint_Write_Control_Stream_LE(&switchReportData, HID_ENDPOINT_SIZE);
						break;

					case INPUT_MODE_HID:
						Endpoint_Write_Control_Stream_LE(&hidReportData, HID_ENDPOINT_SIZE);
						break;
				}

				Endpoint_ClearOUT();
			}
			break;
	}
}
