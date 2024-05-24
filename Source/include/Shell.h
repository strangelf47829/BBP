#ifndef BBP_SYSTEM_SHELL_H
#define BBP_SYSTEM_SHELL_H

#include "stddef.h"
#include "Strings.h"
#include "License.h"
#include "FileSys.h"

namespace BBP
{
	namespace system
	{

		// Shell stuff
		class Shell
		{

			// Static stuff
			static Shell singleton;

			// Shell information
			appInfo shellInfo;

			// Page with memories
			std::STATIC_PAGE<std::ResourceManager *, 32> memories;

			// Stack
			std::Stack<std::ResourceManager *> memoryStack;

			// Own memory
			std::ResourceManager sysMemory;

			// Primary volume
			std::VOLUME primaryVolume;

			// Working directory
			std::PATH workingDirectory;

			// Private constructor
			Shell();

		public:

			// Get active memory
			static std::ResourceManager &getActiveMemory() { return singleton.sysMemory; }
			static std::ResourceManager &getSysMemory() { return singleton.sysMemory; }

			// Get primary volume
			static std::VOLUME &getPrimaryVolume() { return singleton.primaryVolume;  }

			// Get working directory
			static std::PATH &getWorkingDirectory() { return singleton.workingDirectory; }


		};




		// License information
		appInfo getShellInformation();
		void printShellInformation();
		void printShellDirectory();

		// Do a single shell command
		void shell(std::string &);

	}
}

#endif