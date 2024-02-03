#ifndef BBP_STDLIB_EXECUTABLE_H
#define BBP_STDLIB_EXECUTABLE_H

#include "../include/Strings.h"

#define BBP_EXEC_MAGIC0 0xBB
#define BBP_EXEC_MAGIC1 0x66
#define BBP_EXEC_MAGIC2 0x00
#define BBP_EXEC_MAGIC3 0x77

#define BBP_EXEC_MAGIC0IDX 8
#define BBP_EXEC_MAGIC1IDX 9
#define BBP_EXEC_MAGIC2IDX 10
#define BBP_EXEC_MAGIC3IDX 11

namespace BBP
{
	namespace std
	{
		
		// Define the signature of a main function
		typedef int(*executable_main)(int argc, char **argv);

		// Define the type that PID_t is
		typedef uint8_t PID_t;

		// This structure defines the structure of an executable
		struct Executable
		{
			executable_main entry;
			std::conststring name;
		};

		struct executableFrame
		{
			// The PID of the executable.
			PID_t PID;

			// Program memory
			std::ResourceManager *pmem;

			// Check if current execution frame is the active frame
			bool isActiveFrame;

			// Check if current frame is running anything
			bool isExecuting;

			// Arguments
			int argc;

		};

		// This page stores each executableFrame
		extern STATIC_PAGE<executableFrame, 12> procFrames;
		extern Stack<executableFrame> Processes;

		// Stores which PID is the active PID
		extern PID_t activeExecutable;

		// Helper functions
		PID_t getNextAvailablePID();

		
		// ...
		PID_t spawnExecutable(executable_main, std::string &);




		// This is a constant array that stores the executables. In the future, this could be done dynamically, but i am too lazy for that :)
		extern const Executable executables[];

		// Executes an executable. Pretty simple.
		void execute(executable_main, int argc, char **argv);
		void exit();


	}
}



#endif
