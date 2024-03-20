#ifndef BBP_DAEMONS_INITD_H
#define BBP_DAEMONS_INITD_H

#include "../include/Daemon.h"

namespace BBP
{
	namespace system
	{

		// Initial daemon
		class initd : public Daemon
		{
		private:
			// Services
			DaemonService shellService;

			// Functors
			std::errno_t shell(std::size_t, std::c_string *);

			// Get a line for d
			static constexpr std::size_t shellLineLength = 32;
			std::static_string<shellLineLength> shellLine;

			std::size_t getLine(BBP::std::static_string<shellLineLength> &str);
			std::size_t getShellLine();

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
