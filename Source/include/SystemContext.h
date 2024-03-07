#pragma once

#include "FileSys.h"
#include "Graphics.h"


namespace BBP
{
	namespace system
	{

		// A process context is a structure that stores all the stuff each process might need (such as the resource manager)
		struct ProcessContext
		{



		};

		// A system context is a structure that stores all the stuff each hypervisor might need 
		struct SystemContext
		{

			// Stores the active process context 
			ProcessContext activeProcessContext;

			// Standard IO
			std::static_string<509> str_STDOUT;
			std::static_string<509> str_STDIN;
			std::static_string<509> str_STDERR;

			std::Stack<std::string_element> STDOUT;
			std::Stack<std::string_element> STDIN;
			std::Stack<std::string_element> STDERR;
			
			// The window pertaining to this hypervisor
			std::window display;
			
			// Volume information
			std::VOLUME *primaryVolume;
			std::PATH *workingDirectory;

			// Resource Allocation
			std::ResourceManager progmem;
			std::ResourceManager *activemem;

			// File table data
			std::STATIC_PAGE<std::FileNode, std::max_open_files> fileTable;

			// Error messages
			volatile std::errno_t __errno;

			SystemContext();

		};

		// The kernel context structure holds all 'globals' in a single structure.
		// This is really the only global
		struct KernelContext
		{

			// The processor frames
			// TODO: Implement

			// 


		};

	}
}
