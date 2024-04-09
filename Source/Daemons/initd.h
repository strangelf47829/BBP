#ifndef BBP_DAEMONS_INITD_H
#define BBP_DAEMONS_INITD_H

#include "../include/Daemon.h"

namespace BBP
{
	namespace system
	{

		// Structure to hold shell actions
		struct shell
		{

		};

		// Initial daemon
		class initd : public Daemon
		{
		private:
			// Services
			static constexpr std::size_t serviceCount = 3;
			BBP::std::STATIC_PAGE<DaemonService, serviceCount> initdServices;

			// Maximum length for a shell command
			static constexpr std::size_t shellLineLength = 32;

			// Static page to hold command
			std::static_string<shellLineLength> shellLine;

			// Argument count and so
			std::size_t argumentCount;
			std::STATIC_PAGE<std::c_string, shellLineLength / 2> argumentVectors;

			// A shell service gives you a chance to ...
			DaemonService shellService;
			std::errno_t shell(std::size_t, std::c_string *);

			// This service spawns an application that provides an interactive shell
			DaemonService shellApplicationService;
			std::errno_t shellApplication(std::size_t, std::c_string *);

			// This service invokes a builtin shell action
			DaemonService shellBuiltinService;
			std::errno_t shellBuiltin(std::size_t, std::c_string *);

			// Load line from user
			std::size_t loadUserInput();

			// Delimit input
			void delimitInput(std::size_t);

			// Go through one cycle of a shell
			void shellApplicationGetLine();

			// Is line builtin?
			bool isBuiltin();

			// Call shell with shellLine
			std::errno_t shell();
		public:

			// Initialization 
			std::errno_t initialize() override;
			std::errno_t deinitialize() override;

			// Lookup stuff
			DaemonService *lookupFunctor(std::string &) override;
			std::errno_t functorFunctor(std::hash_t, std::size_t, std::c_string *) override;

			// Call a shell thing
			std::errno_t shell(std::string &line);

		};

	}
}

#endif
