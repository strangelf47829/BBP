#ifndef HOST_BIOS_H
#define HOST_BIOS_H

#include "../../include/Bootloader.h"

namespace Host
{

	class BIOS : public virtual BBP::BIOS
	{

		// Volatile keyboard stuff
		volatile bool listenForInput;

		// Start capturing key input
		void listenForKeyboard();
		void stopListeningForKeyboard();

		// Start clocks
		void initClocks();

		// Print memory
		void printMem(BBP::std::word);

		// Print splash
		void splash(BBP::system::EFI &);

		// File
		BBP::std::errno_t fopen_b(BBP::std::conststring);

	public:

		// Basic output: print a character on screen
		void printCharacter(BBP::std::string_element) override;

		// Print a string
		void print(BBP::std::conststring str) override;

		// Print number
		void print(BBP::std::word);

		// Basic output: set character colours
		void setOutputFG(BBP::std::byte);
		void setOutputBG(BBP::std::byte);

		// Basic output: clear screen
		void clearScreen();

		// Basic input: get keyboard input
		bool getKeyboardInput(BBP::std::string_element &);

		// Basic input: get file 
		BBP::std::errno_t fopen(BBP::std::c_string);
		BBP::std::errno_t fclose();

		// Basic input: get file size
		BBP::std::size_t fsize();

		// Basic input: read file element
		BBP::std::errno_t fread(BBP::std::string_element &);

		// Basic output: write entire file
		BBP::std::errno_t fwrite(BBP::std::c_string, BBP::std::Stack<BBP::std::string_element> &);

		// Basic input: milliseconds and microseconds
		BBP::std::word milliseconds();
		BBP::std::word microseconds();

		// Actually boot something
		void boot(BBP::system::EFI &, BBP::FirmwareInterface &);

		// Initialize and deinitialize BIOS
		BBP::std::errno_t init();
		BBP::std::errno_t fini();

	};

}

#endif