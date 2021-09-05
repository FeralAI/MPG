#include "GamepadDescriptors.h"

static uint16_t getConfigurationDescriptor(const uint8_t *buffer, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			buffer = xinput_configuration_descriptor;
			return sizeof(xinput_configuration_descriptor);

		case INPUT_MODE_SWITCH:
			buffer = switch_configuration_descriptor;
			return sizeof(switch_configuration_descriptor);

		default:
			buffer = hid_configuration_descriptor;
			return sizeof(hid_configuration_descriptor);
	}
}

static uint16_t getDeviceDescriptor(const uint8_t *buffer, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			buffer = xinput_device_descriptor;
			return sizeof(xinput_device_descriptor);

		case INPUT_MODE_SWITCH:
			buffer = switch_device_descriptor;
			return sizeof(switch_device_descriptor);

		default:
			buffer = hid_device_descriptor;
			return sizeof(hid_device_descriptor);
	}
}

static uint16_t getHIDDescriptor(const uint8_t *buffer, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_SWITCH:
			buffer = switch_hid_descriptor;
			return sizeof(switch_hid_descriptor);

		default:
			buffer = hid_hid_descriptor;
			return sizeof(hid_hid_descriptor);
	}
}

static uint16_t getHIDReport(const uint8_t *buffer, InputMode mode)
{
	switch (mode)
	{
		case INPUT_MODE_SWITCH:
			buffer = switch_report_descriptor;
			return sizeof(switch_report_descriptor);

		default:
			buffer = hid_report_descriptor;
			return sizeof(hid_report_descriptor);
	}
}

static uint16_t getStringDescriptor(const uint8_t *buffer, InputMode mode, uint8_t index)
{
	switch (mode)
	{
		case INPUT_MODE_XINPUT:
			buffer = xinput_string_descriptors[index];
			return sizeof(xinput_string_descriptors[index]);

		case INPUT_MODE_SWITCH:
			buffer = switch_string_descriptors[index];
			return sizeof(switch_string_descriptors[index]);

		default:
			buffer = hid_string_descriptors[index];
			return sizeof(hid_string_descriptors[index]);
	}
}
