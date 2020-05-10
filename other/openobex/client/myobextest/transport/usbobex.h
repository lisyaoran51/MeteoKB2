/**
	\file usbobex.h
	USB OBEX, USB transport for OBEX.
	OpenOBEX library - Free implementation of the Object Exchange protocol.

	Copyright (c) 2005 Alex Kanavin, All Rights Reserved.

	OpenOBEX is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as
	published by the Free Software Foundation; either version 2.1 of
	the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with OpenOBEX. If not, see <http://www.gnu.org/>.
 */

#ifndef USBOBEX_H
#define USBOBEX_H

#include <openobex/obex_const.h>

#ifdef HAVE_USB1
#include <libusb.h>
#else
#include <usb.h>
#endif

/* Information about a USB OBEX interface present on the system */
struct obex_usb_intf_transport_t {
	struct obex_usb_intf_transport_t *next;	/* Next interface in the list */
	int configuration;			/* Device configuration */
	int configuration_description;		/* Configuration string descriptor number */
	int control_interface;			/* OBEX master interface */
	int control_setting;			/* OBEX master interface setting */
	int control_interface_description;	/* OBEX master interface string descriptor number 
						 * If non-zero, use usb_get_string_simple() from 
						 * libusb to retrieve human-readable description
						 */
	char *extra_descriptors;		/* Extra master interface descriptors */
	int extra_descriptors_len;		/* Length of extra descriptors */
	int data_interface;			/* OBEX data/slave interface */
	int data_idle_setting;			/* OBEX data/slave idle setting */
	int data_interface_idle_description;	/* OBEX data/slave interface string descriptor number
						 * in idle setting */
	int data_active_setting;		/* OBEX data/slave active setting */
	int data_interface_active_description;	/* OBEX data/slave interface string descriptor number
						 * in active setting */
	int data_endpoint_read;			/* OBEX data/slave interface read endpoint */
	int data_endpoint_write;		/* OBEX data/slave interface write endpoint */
#ifdef HAVE_USB1
	struct libusb_device *device;		/* libusb 1.x device */
	struct libusb_device_handle *dev;	/* libusb 1.x device handle */
#else
	struct usb_device *device;		/* libusb 0.x device */
	usb_dev_handle *dev;			/* libusb 0.x device handle */
#endif
};

/* "Union Functional Descriptor" from CDC spec 5.2.3.X
 * used to find data/slave OBEX interface */
#pragma pack(1)
struct cdc_union_desc {
	uint8_t      bLength;
	uint8_t      bDescriptorType;
	uint8_t      bDescriptorSubType;

	uint8_t      bMasterInterface0;
	uint8_t      bSlaveInterface0;
};
#pragma pack()

/* CDC class and subclass types */
#define USB_CDC_CLASS			0x02
#define USB_CDC_OBEX_SUBCLASS		0x0b

/* class and subclass specific descriptor types */
#define CDC_HEADER_TYPE			0x00
#define CDC_CALL_MANAGEMENT_TYPE	0x01
#define CDC_AC_MANAGEMENT_TYPE		0x02
#define CDC_UNION_TYPE			0x06
#define CDC_COUNTRY_TYPE		0x07
#define CDC_OBEX_TYPE			0x15
#define CDC_OBEX_SERVICE_ID_TYPE	0x19

/* Interface descriptor */
#define USB_DT_CS_INTERFACE		0x24
#define CDC_DATA_INTERFACE_TYPE		0x0a

#define WMC_DEFAULT_OBEX_SERVER_UUID \
{ 0x02, 0xae, 0xb3, 0x20, \
0xf6, 0x49, 0x11, 0xda, \
0x97, 0x4d, 0x08, 0x00, \
0x20, 0x0c, 0x9a, 0x66 }

#define USB_MAX_STRING_SIZE		256

struct obex_transport * usbobex_transport_create(void);

struct usbobex_data {
#ifdef HAVE_USB1
	struct libusb_context *ctx;
	int fd;
#endif
	struct obex_usb_intf_transport_t self;
};
#endif
