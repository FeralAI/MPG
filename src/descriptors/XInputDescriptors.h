/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef XINPUT_DESCRIPTORS_H_
#define XINPUT_DESCRIPTORS_H_

#include <stdint.h>

#define XINPUT_ENDPOINT_SIZE 20

// Buttons 1 (8 bits)
#define XBOX_MASK_UP    (1U << 0)
#define XBOX_MASK_DOWN  (1U << 1)
#define XBOX_MASK_LEFT  (1U << 2)
#define XBOX_MASK_RIGHT (1U << 3)
#define XBOX_MASK_START (1U << 4)
#define XBOX_MASK_BACK  (1U << 5)
#define XBOX_MASK_LS    (1U << 6)
#define XBOX_MASK_RS    (1U << 7)

// Buttons 2 (8 bits)
#define XBOX_MASK_LB    (1U << 0)
#define XBOX_MASK_RB    (1U << 1)
#define XBOX_MASK_HOME  (1U << 2)
//#define UNUSED        (1U << 3)
#define XBOX_MASK_A     (1U << 4)
#define XBOX_MASK_B     (1U << 5)
#define XBOX_MASK_X     (1U << 6)
#define XBOX_MASK_Y     (1U << 7)

typedef struct __attribute((packed, aligned(1)))
{
	uint8_t report_id;
	uint8_t report_size;
	uint8_t buttons1;
	uint8_t buttons2;
	uint8_t lt;
	uint8_t rt;
	int16_t lx;
	int16_t ly;
	int16_t rx;
	int16_t ry;
	uint8_t _reserved[6];
} XInputReport;

static const uint8_t xinput_string_language[]    = { 0x09, 0x04 };
static const uint8_t xinput_string_manfacturer[] = "Microsoft";
static const uint8_t xinput_string_product[]     = "XInput STANDARD GAMEPAD";
static const uint8_t xinput_string_version[]     = "1.0";

static const uint8_t *xinput_string_descriptors[] =
{
	xinput_string_language,
	xinput_string_manfacturer,
	xinput_string_product,
	xinput_string_version
};

static const uint8_t xinput_device_descriptor[] =
{
	0x12,       // bLength
	0x01,       // bDescriptorType (Device)
	0x00, 0x02, // bcdUSB 2.00
	0xFF,	      // bDeviceClass
	0xFF,	      // bDeviceSubClass
	0xFF,	      // bDeviceProtocol
	0x40,	      // bMaxPacketSize0 64
	0x5E, 0x04, // idVendor 0x045E
	0x8E, 0x02, // idProduct 0x028E
	0x14, 0x01, // bcdDevice 2.14
	0x01,       // iManufacturer (String Index)
	0x02,       // iProduct (String Index)
	0x03,       // iSerialNumber (String Index)
	0x01,       // bNumConfigurations 1
};

static const uint8_t xinput_configuration_descriptor[] =
{
	0x09,        // bLength
	0x02,        // bDescriptorType (Configuration)
	0x99, 0x00,  // wTotalLength 153
	0x04,        // bNumInterfaces 4
	0x01,        // bConfigurationValue
	0x00,        // iConfiguration (String Index)
	0xA0,        // bmAttributes Remote Wakeup
	0xFA,        // bMaxPower 500mA

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x00,        // bInterfaceNumber 0
	0x00,        // bAlternateSetting
	0x02,        // bNumEndpoints 2
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x01,        // bInterfaceProtocol
	0x00,        // iInterface (String Index)

	0x11,        // bLength
	0x21,        // bDescriptorType (HID)
	0x00, 0x01,  // bcdHID 1.00
	0x01,        // bCountryCode
	0x25,        // bNumDescriptors
	0x81,        // bDescriptorType[0] (Unknown 0x81)
	0x14, 0x00,  // wDescriptorLength[0] 20
	0x00,        // bDescriptorType[1] (Unknown 0x00)
	0x00, 0x00,  // wDescriptorLength[1] 0
	0x13,        // bDescriptorType[2] (Unknown 0x13)
	0x01, 0x08,  // wDescriptorLength[2] 2049
	0x00,        // bDescriptorType[3] (Unknown 0x00)
	0x00,
	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x81,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	// 0x04,        // bInterval 4 (unit depends on device speed)
	0x01,        // bInterval (override to 1)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x01,        // bEndpointAddress (OUT/H2D)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x08,        // bInterval 8 (unit depends on device speed)

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x01,        // bInterfaceNumber 1
	0x00,        // bAlternateSetting
	0x04,        // bNumEndpoints 4
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x03,        // bInterfaceProtocol
	0x00,        // iInterface (String Index)

	0x1B,        // bLength
	0x21,        // bDescriptorType (HID)
	0x00, 0x01,  // bcdHID 1.00
	0x01,        // bCountryCode
	0x01,        // bNumDescriptors
	0x82,        // bDescriptorType[0] (Unknown 0x82)
	0x40, 0x01,  // wDescriptorLength[0] 320
	0x02, 0x20, 0x16, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x82,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x02,        // bInterval 2 (unit depends on device speed)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x02,        // bEndpointAddress (OUT/H2D)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x04,        // bInterval 4 (unit depends on device speed)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x83,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x40,        // bInterval 64 (unit depends on device speed)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x03,        // bEndpointAddress (OUT/H2D)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x10,        // bInterval 16 (unit depends on device speed)

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x02,        // bInterfaceNumber 2
	0x00,        // bAlternateSetting
	0x01,        // bNumEndpoints 1
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x02,        // bInterfaceProtocol
	0x00,        // iInterface (String Index)

	0x09,        // bLength
	0x21,        // bDescriptorType (HID)
	0x00, 0x01,  // bcdHID 1.00
	0x01,        // bCountryCode
	0x22,        // bNumDescriptors
	0x84,        // bDescriptorType[0] (Unknown 0x84)
	0x07, 0x00,  // wDescriptorLength[0] 7

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x84,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x10,        // bInterval 16 (unit depends on device speed)

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x03,        // bInterfaceNumber 3
	0x00,        // bAlternateSetting
	0x00,        // bNumEndpoints 0
	0xFF,        // bInterfaceClass
	0xFD,        // bInterfaceSubClass
	0x13,        // bInterfaceProtocol
	0x04,        // iInterface (String Index)

	0x06,        // bLength
	0x41,        // bDescriptorType (Unknown)
	0x00, 0x01, 0x01, 0x03
};

#endif
