#ifndef BBP_DEVICE_H
#define BBP_DEVICE_H

#include "FileSys.h"

namespace BBP
{
	
	namespace system
	{

		// This structure stores the necessary information to describe a device.
		struct deviceDescriptor
		{
			// The physical ID of the device
			std::word physicalID;

			// The amount of subdevices
			std::size_t subdeviceCount;

			// The amount of metadata fields
			std::size_t metadataFieldCount;

			// The name/key to the metadata stored in the metadata field
			std::static_string<32> metadatafield;

			// The actual metadata for the key stored in 'metadatafield'
			std::string metadata;
		};

		// This constant expression stores the "root" device, providing this physical ID provides a guaranteed hit.
		constexpr std::word rootDeviceID = 0x0001;

		// Load a device using a physical ID
		std::errno_t loadDevice(deviceDescriptor &, std::word);

		// Load a device using a path
		std::errno_t loadDevice(deviceDescriptor &, std::PATH);

		// Load a subdevice
		std::errno_t loadSubdevice(deviceDescriptor &, std::index_t);

		// Load a metadata field
		std::errno_t loadDeviceMetadata(deviceDescriptor &, std::index_t);

	}

}

#endif
