#ifndef BBP_STD_EFI_H
#define BBP_STD_EFI_H

#include "EFI.h"
#include "FileSys.h"
#include "DeviceDriver.h"
#include "cstdio.h"
#include "FileSysInfo.h"
#include "SyscallArgs.h"

// This header defines objects that the bootloader loads from memory.
// The bootloader then passes this on to the Kernel

namespace BBP
{

	// Forward declaration
	class FirmwareInterface;

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
		virtual void boot(BBP::system::EFI &, BBP::FirmwareInterface &) = 0;

		// Initialize and deinitialize BIOS
		virtual std::errno_t init() = 0;
		virtual std::errno_t fini() = 0;
	};



	// This is the class that is used by the kernel to interface with the firmware.
	class FirmwareInterface
	{

		// This is what interfaces with the BIOS
		BIOS *bios;

		// If in BIOS mode, calls to the firmware interface go through the BIOS. Otherwise they go through drivers
		bool biosMode;

		// EFI configuration
		system::EFI *configuration;

		// File stuff
		void setPath(std::PATH &);
		void loadFileMetadata(std::PATH &, std::size_t, std::word *);
		std::FileSysInfo::FileSysEntryType queryFileType(std::PATH &);

		// Built-in system commands
		void initClock();

		// Built-in keyboard commands
		std::index_t getKeyCount();
		std::string_element getKeyboardKey();

	public:

		// Standard device drivers
		system::DeviceDriver HardwareKeyboard;
		system::DeviceDriver HardwareScreen;
		system::DeviceDriver HardwareFile;
		system::DeviceDriver HardwareSystem;

		// Constructor using BIOS
		FirmwareInterface(BIOS *, system::EFI *);

		// Switch from BIOS mode to Driver mode
		void SwitchToDrivers();

		// Query if file is on disk or not
		bool isFileOnDisk(std::PATH &);

		// Query if path is of type file
		bool isPathOfTypeFile(std::PATH &);

		// Query if path is of type directory
		bool isPathOfTypeDirectory(std::PATH &);

		// Does anything even exist at path location
		bool isPathRealObject(std::PATH &);

		// Query file size for file on disk
		std::size_t getFilesizeFromDisk(std::PATH &);

		// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
		std::size_t readFileFromDisk(std::stream_t, std::PATH &);

		// Write file to disk (Takes all the data from the file and dumps it into a file driver)
		void writeFileToDisk(std::PATH &, std::stream_t);

		// Inspect a given path. Returns 0 if success, 
		std::errno_t Inspect(std::PATH &);

		// Step iterator
		bool stepInspectionIterator();

		// Get name of path
		void getInspectorPath(std::string &);

		// Get inspector file type
		std::FileSysInfo::FileSysEntryType getInspectorFileType();

		// Check if can step
		bool canStepInspector();

		// Time related stuff
		std::time_t millis();
		std::time_t micros();

		// Keyboard related stuff
		std::string_element getKey();
		std::string_element getKeyAsync();

		// Screen related stuff
		void printString(std::string &);
		void clearScreen();

		// Syscalls
		std::errno_t syscall(system::syscall_t call, system::syscall_args_t &args);

		// Input
		void startCapture();
		void stopCapture();

	};
}

#endif