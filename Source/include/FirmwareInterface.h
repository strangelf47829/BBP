#ifndef BBP_STD_EFI_H
#define BBP_STD_EFI_H

#include "EFI.h"
#include "FileSys.h"
#include "DeviceDriver.h"

// This header defines objects that the bootloader loads from memory.
// The bootloader then passes this on to the Kernel

namespace BBP
{

	// Each environment derives this pure abstract class, which is then called by the bootloader.
	// This class should allow for very-very simple input and output (Therefore BIOS!)
	class BIOS
	{
		// Basic input: open BIOS only file
		virtual std::errno_t fopen_b(std::conststring) = 0;

	public:
		// Basic output: print a character on screen
		virtual void printCharacter(std::string_element) = 0;

		// Basic output: print a string
		virtual void print(std::conststring) = 0;

		// Basic output: set character colours
		virtual void setOutputFG(std::byte) = 0;
		virtual void setOutputBG(std::byte) = 0;

		// Basic output: clear screen
		virtual void clearScreen() = 0;

		// Basic input: get keyboard input
		virtual bool getKeyboardInput(std::string_element &) = 0;

		// Basic input: get file 
		virtual std::errno_t fopen(std::c_string) = 0;
		virtual std::errno_t fclose() = 0;
		
		// Basic input: get file size
		virtual std::size_t fsize() = 0;

		// Basic input: read file element
		virtual std::errno_t fread(std::string_element &) = 0;

		// Basic output: write entire file
		virtual std::errno_t fwrite(std::c_string, std::Stack<std::string_element> &) = 0;

		// Basic input: milliseconds and microseconds
		virtual std::word milliseconds() = 0;
		virtual std::word microseconds() = 0;

		// Entry point for bios -- called from bootloader
		virtual void boot(BBP::system::EFI &) = 0;

		// Initialize and deinitialize BIOS
		virtual std::errno_t init() = 0;
		virtual std::errno_t fini() = 0;
	};



	// This is the class that is used by the kernel to interface with the firmware.
	class FirmwareInterface
	{

		// This is what interfaces with the BIOS
		BIOS *bios;

		// Hardware drivers
		system::HardwareHandle HardwareKeyboard;
		system::HardwareHandle HardwareScreen;
		system::HardwareHandle HardwareFile;
		system::HardwareHandle HardwareSystem;

		// EFI configuration
		system::EFI configuration;

	public:

		// Constructor using BIOS
		FirmwareInterface(BIOS *);

		// 

	};
}

#endif