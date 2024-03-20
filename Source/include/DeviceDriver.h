#ifndef BBP_SYSTEM_DEVDRIVER_H
#define BBP_SYSTEM_DEVDRIVER_H

#include "Strings.h"

// This header defines device-agnostic drivers that are used to provide an interface between hardware and software.
// This is done by providing an API 

namespace BBP
{

	namespace system
	{

		// Used to store device data
		constexpr std::size_t deviceBufferSize = 0x1000;
		constexpr std::size_t deviceMetadataBufferSize = 32;
		constexpr std::size_t deviceMetadataFieldCount = 32;
		constexpr std::size_t deviceCommandSize = 32;

		// Function definitions used to interface with hardware
		using HardwareAction = std::size_t(*)(std::size_t, std::PAGE<std::string_element> &);
		using HardwareCmd = bool (*)(std::size_t argc, std::word *argv);


		// The instance for this
		class HardwareHandle
		{
			// Buffers to store input and output
			std::STATIC_PAGE<std::string_element, deviceBufferSize> inputBuffer;
			std::STATIC_PAGE<std::string_element, deviceBufferSize> outputBuffer;

			// Handles to pointers to retrieve and send data
			HardwareAction sendData;
			HardwareAction receiveData;
			HardwareAction readMetadata;

			// Page used to store device metadata.
			std::static_string<deviceMetadataBufferSize> metadata;
			std::STATIC_PAGE<std::index_t, deviceMetadataFieldCount> metadataFieldIndicies;

			// Handles to device command pointers
			std::STATIC_PAGE<HardwareCmd, deviceCommandSize> commands;

		public:

			// Constructors
			HardwareHandle() = delete;
			HardwareHandle(HardwareAction[3], std::size_t, const HardwareCmd *);

			// Used to define common commands
			static constexpr std::index_t connectCmd = 0;		// Used to connect to a device
			static constexpr std::index_t disconnectCmd = 1;	// Used to disconnect from a device
			static constexpr std::index_t switchPort = 2;		// Used to switch interface type (Such as from I2C0 to I2C1, or from SD0 to SD1)

			// Used to go through commands
			bool executeCommand(std::index_t, std::size_t, std::word *);

			// Used to update stuff
			std::size_t send(std::size_t);
			std::size_t receive(std::size_t);

			// Get pointers to pages
			std::PAGE<std::string_element> &getInput();
			std::PAGE<std::string_element> &getOutput();

			// Used to check/set connection status
			bool isConnected();
			bool Connect();
			bool Disconnect();

			// Used to retrieve device metadata
			std::c_string getMetadata(std::index_t);

		};


		// This class provides an interface to interact with software
		class SoftwareHandle
		{
			static constexpr std::size_t maximumDeviceNameLength = 32;
			static constexpr std::size_t maximumDeviceMountNameLength = 8;

			// Name of the device, such as 'SD CARD'
			std::static_string<maximumDeviceNameLength> deviceName;

			// Where to mount the device
			std::static_string<maximumDeviceMountNameLength> mountName;

			// Data IO
			std::Stack<std::string_element> *InputBuffer; // Mounts to /dev/<mountName>/stdin
			std::Stack<std::string_element> *OutputBuffer; // Mounts to /dev/<mountName>/stdout

		public:

			// Constructors
			SoftwareHandle() = delete;
			SoftwareHandle(std::Stack<std::string_element> *input, std::Stack<std::string_element> *output, std::conststring name);

			// Used to mount and dismount drivers from the file system
			void Mount(std::string);
			void Dismount();

			// Writing data into
			SoftwareHandle &operator <<(std::string_element);	// From kernel to driver (So to output)
			SoftwareHandle &operator >>(std::string_element&);	// From driver to kernel (So from input)

			SoftwareHandle &operator <<(std::c_string);			// From kernel to driver (So to output)
			SoftwareHandle &operator >>(std::c_string);			// From driver to kernel (So from input)

			SoftwareHandle &operator <(std::string_element);	// From hardware to driver (So to input)
			SoftwareHandle &operator >(std::string_element&);	// From driver to hardware (So from output)

			// Returns the amount of available bytes.
			std::size_t available();

			// Sets the input page
			void setInputPage(std::Stack<std::string_element> *);
			void setOutputPage(std::Stack<std::string_element> *);
		};

		// The actual device driver structure to hold stuff
		class DeviceDriver
		{
		public:
			SoftwareHandle softwareDriver;
			HardwareHandle hardwareDriver;

			DeviceDriver(std::Stack<std::string_element> *input, std::Stack<std::string_element> *output, std::conststring name, HardwareAction[3], std::size_t, const HardwareCmd *);
		
			// Used to send data
			std::word writeData(std::c_string str);
			std::Stack<std::string_element> &receiveData(std::word);

		};




	}

}


#endif