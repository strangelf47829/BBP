#include "Strings.h"

#ifndef BBP_SYSTEM_DAEMON_H
#define BBP_SYSTEM_DAEMON_H

namespace BBP
{

	namespace system
	{

		// These function definitions allow for a daemon to have multiple stuff
		typedef std::word (*daemonService)(std::PAGE<std::c_string>);
		typedef daemonService (*daemonResolutionService)(std::hash_t, std::string);

		// A daemon record stores information about a daemon. 
		class DaemonRecord
		{

			// This variable stores the maximum amount of services a daemon may have
			constexpr static std::size_t maxDaemonServices = 3;

			// Stores the various services
			std::word serviceCount;
			std::STATIC_PAGE<daemonService, maxDaemonServices> services; 
			std::STATIC_PAGE<std::hash_t, maxDaemonServices> serviceHashes;

			virtual void daemonInit() = 0;

		public:

			// Used to interface with the daemon
			std::word daemonAction(std::string);


		};



	}


}

#endif