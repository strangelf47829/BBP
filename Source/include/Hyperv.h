#ifndef BBP_LIB_HYPERV
#define BBP_LIB_HYPERV

#include "RAE.h"
#include "Threading.h"
#include "ELF.h"
#include "SystemContext.h"


// This header defines structs and classes to provide an environment for RAE to do her work
namespace BBP
{
	namespace userspace
	{

		// Forward declaration for syscall.
		class StateMachine;

		class HyperVisor
		{
		public:

			// Used to define system calls. System calls belong to the hypervisor, not the state machine.
			using syscall_t = std::word (*)(userspace::StateMachine *, userspace::HyperVisor *, userspace::Thread *, userspace::Instruction &);

			// Thread info
			static constexpr std::size_t threadCount = 8;
			std::size_t activeThreadCount = 0;
			std::index_t activeThread = 0;

			// Thread page
			std::STATIC_PAGE<Thread, threadCount> threads;

			// System call information
			static constexpr std::size_t systemCallCount = 128;
			std::STATIC_PAGE<syscall_t, systemCallCount> systemcalls;

			// This ELF Object serves as the 'shared library' of this hyper visor
			BinaryExecutable sharedlib;

			// Count of PID counts
			pid_t currentPIDCount;

			// IDX: The index of the thread, PID: PID.
			std::index_t allocateThreadIDX();
			pid_t allocateThreadPID();

			userspace::Thread *spawnThread(std::ELF::ELFBuilder &binary, std::ResourceManager *allocator);
			void destroyThread(userspace::pid_t pid);
			void FetchActiveThreadInstruction();

			// Functions called to advance the state of a program
			bool advanceThread(userspace::StateMachine &, std::word millis);
			bool advanceThread(userspace::StateMachine &);


		};


	}
}

#endif