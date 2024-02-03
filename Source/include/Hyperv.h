#ifndef BBP_LIB_HYPERV
#define BBP_LIB_HYPERV

#include "RAE.h"
#include "Threading.h"
#include "ELF.h"


// This header defines structs and classes to provide an environment for RAE to do her work
namespace BBP
{
	namespace userspace
	{
		class HyperVisor
		{
		public:
			// Thread info
			static constexpr std::size_t threadCount = 16;
			std::size_t activeThreadCount = 0;
			std::index_t activeThread = 0;

			// Thread page
			std::STATIC_PAGE<Thread, threadCount> threads;

			// This ELF Object serves as the 'shared library' of this hyper visor
			BinaryExecutable sharedlib;

			/*
			INSERT FUNCTIONS TO ACCESS SHARED LIB
			*/

			/*
			INSERT THREAD MANIPULATION STUFF
			*/
			pid_t allocateThread();
			userspace::Thread *spawnThread(std::ELF::ELFBuilder &binary, std::ResourceManager *allocator);
			void destroyThread(userspace::pid_t pid);
			void FetchActiveThreadInstruction();


		};


	}
}

#endif