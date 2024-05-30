#ifndef BBP_DRIVERS_TYPES_H
#define BBP_DRIVERS_TYPES_H

#include "stddef.h"

namespace BBP
{
	namespace system
	{
		// This type defines the integral type used for a driver 'hardware type'.
		// A 'hardware' type is the type of hardware used to connect to a piece of hardware.
		// Examples of a type of hardware includes: Builtin, SDMMC, I2C, Serial, Parallel, VGA, TTY, HDMI, etc...
		using HWID_t = std::word;

		// This type defines the integral type used for a driver 'device type'.
		// A 'device type' is what describes the type of thing on the other side of the hardware connection.
		// Examples of a type of device includes: Keyboard, Mouse, File system, Monitor (TTY), Monitor (GUI), Generic device (Serial), Generic device (I2C), Network item, ...

		// Used to store the device type
		using DVID_t = std::word;

		// This namespace is used to keep 'system' tidy, and to hold several enums in itself
		namespace HardwareClassification
		{
			// The 'hardware' type must remain as hardware agnostic as possible, which is a bit oxymoronic.
			// It is still possible to achieve this, however, by instead describing what the device is and isn't.
			// Therefore, the first 16 bits are reserved to provide a minimal amount of functionality while also maintaining
			// enough room for extensibility.

			// The first 2 bits are reserved to dedicated 'flags', such as specifying whether the
			// device type even has a type.

			// This is what the flags field signifies.
			// (THIS IS NOT A BITFIELD)
			
			// 0b00: Unkown.		The hardware used to connect to this device is unkown.
			// 0b01: Unspecified:	The hardware used to connect to this device is known to be undetermined.
			// 0b10: Builtin:		The hardware used to connect to this device has been abstracted
			// 0b11: Other:			The hardware used to connect to this device is abstract, but cannot function on its own. (Such as a TTY terminal over serial)

			constexpr std::byte FlagsOffset = 0;
			enum Flags : HWID_t
			{
				Unkown = 0b00 << FlagsOffset,
				Unspecified = 0b01 << FlagsOffset,
				Builtin = 0b10 << FlagsOffset,
				Other = 0b11 << FlagsOffset
			};

			// The next 4 bits are reserved to a bitfield specifying how the device is connected to this driver specifically

			// 0bxx01: Async			This hardware performs operations asynchronously (such as a serial or I2C connection)
			// 0bxx10: Sync				This hardware does not perform operations asynchronously (such as a TTY connection)
			// 0bxx00: (invalid)
			
			// 0b01xx: External			The hardware abstraction is external (Such as a serial connection)
			// 0b10xx: Internal			The hardware abstraction is internal (Such as specially developed radar drivers)
			// 0b00xx: Abstract			The hardware abstraction is provided as is (such as a builtin gui driver)

			constexpr std::byte ModeOffset = 2;
			enum Mode : HWID_t
			{
				Async = 0b0001 << ModeOffset,
				Sync = 0b0010 << ModeOffset,
				
				External = 0b0100 << ModeOffset,
				Internal = 0b1000 << ModeOffset,
				Abstract = 0b000000 << ModeOffset
			};

			// The next 4 bits are reserved to describe the data flow control

			// 0bxx0... Simplex
			// 0b010... Simplex-Host
			// 0b100... Simplex-Receiver

			// 0bxx1... Duplex
			// 0bx01... Half Duplex
			// 0bx11... Full Duplex
			
			// 0b0x1... Reserved
			// 0b1x1... Reserved

			constexpr std::byte FlowOffset = 6;
			enum Flow : HWID_t
			{
				Simplex = 0b0 << FlowOffset,
				Duplex = 0b1 << FlowOffset,

				SimplexHost = 0b010 << FlowOffset,
				SimplexReceiver = 0b100 << FlowOffset,

				HalfDuplex = Duplex,
				FullDuplex = 0b011 << FlowOffset
			};

			// The next 4 bits describe who is responsible for this driver (I.E., Where did this come from?)
			constexpr std::byte PermsOffset = 10;
			enum Perms : HWID_t
			{
				// This is a special driver reserved by the system (keyboard, file, screen)
				System = 0b0001 << PermsOffset,	

				// This driver has been created by the BIOS/EFI system.
				BIOS = 0b0010 << PermsOffset,

				// This driver has been created by the kernel
				Kernel = 0b0100 << PermsOffset,

				// This driverhas been created by an application/daemon
				Userspace = 0b1000 << PermsOffset
			};

			// The last 2 bytes are reserved

			// 16 bits to 32 bits is reserved for use by the OS.
			constexpr HWID_t low_proc = 0x00010000;
			constexpr HWID_t high_proc = 0xFFFFFFFF;
		}

		// Same as hardware
		namespace DeviceClassification
		{
			// Does the device do I/O?
			enum IO : DVID_t
			{
				input = 0b01,
				output = 0b10
			};

			// What does this device transmit?
			enum Data : DVID_t
			{
				// User input ?
				// User output ?
				// System input ?
				// System output ?

				// Graphics
				// Files
				// User input

				// Raw bytes
				// Unspecified

			};

		}
	}
}

#endif