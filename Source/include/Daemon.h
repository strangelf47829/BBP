#ifndef BBP_SYSTEM_DAEMON_H
#define BBP_SYSTEM_DAEMON_H

#include "Strings.h"

// A Daemon is a module that can be loaded by the bootloader and passed onto the kernel.

// Daemons are interfaced with using a hashtable, where each element is a hash of a string corresponding with the command line thing.

namespace BBP
{

	namespace system
	{

		class Daemon;

		// A daemon service structure describes the relationship between a hash, it's help page, it's version page, and it's index within
		// The derived classes functions page
		struct DaemonService
		{
			// The hash for this service
			std::hash_t hash;

			// The index where the service is located.
			bool hasAssociatedService;
			std::index_t serviceIndex;

			// It's help and version page. nullptr if not present
			std::conststring helpMSG;
			std::conststring versMSG;
		};

		// Hash lookup functor looks up a given hash, then sets that ...
		using DaemonHashLookupFunctor = DaemonService *(Daemon::*)(std::string &);
		using DaemonHashLookupFunctorDecl = DaemonService * (std::string &);

		// This function actually executes the 
		using DaemonFunctor = std::errno_t (Daemon::*)(std::hash_t, std::size_t, std::c_string *);
		using DaemonFunctorDecl = std::errno_t(std::hash_t, std::size_t, std::c_string *);

		constexpr std::size_t maxDaemonNameLength = 32;

		// A daemon record is the structure that actually stores the relationship between a service and daemon
		struct DaemonRecord
		{
			// Stores a pointer to the daemon who owns this record.
			Daemon *owner;

			// Points to the Lookup functor
			std::static_string<maxDaemonNameLength> daemonName;

			// This member 
			static std::PAGE<DaemonRecord> records;

			// Constructor automatically adds to records
			DaemonRecord(Daemon *, std::conststring);
			DaemonRecord();
		};

		// The actual daemon class
		class Daemon
		{
		protected:

			// Holds the list of services served by this daemon
			std::PAGE<DaemonService> services;

		public:

			// Initialization 
			virtual std::errno_t initialize() = 0;
			virtual std::errno_t deinitialize() = 0;

			// Virtual lookup stuff
			virtual DaemonHashLookupFunctorDecl lookupFunctor = 0;
			virtual DaemonFunctorDecl functorFunctor = 0;

		};
	}


}

#endif