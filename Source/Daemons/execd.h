#ifndef BBP_DAEMONS_EXECD_H
#define BBP_DAEMONS_EXECD_H

#include "../include/Daemon.h"

namespace BBP
{
	namespace system
	{

		// Application manager daemon
		class execd : public Daemon
		{

			// Loading applications
			DaemonService startBuiltin; // Start a built-in application and run until completion
			DaemonService startBinary;  // Allocate resources to binary application. Do not start execution
			
			// 



		public:

			// Initialization 
			std::errno_t initialize() override;
			std::errno_t deinitialize() override;

			// Lookup stuff
			DaemonService *lookupFunctor(std::string &) override;
			std::errno_t functorFunctor(std::hash_t, std::size_t, std::c_string *) override;
		};

	}
}

#endif
