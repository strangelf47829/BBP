#ifndef BBP_DAEMONS_UTILD_H
#define BBP_DAEMONS_UTILD_H

#include "../include/Daemon.h"

namespace BBP
{
	namespace system
	{

		// Utilities daemon.
		class utild : public Daemon
		{

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
