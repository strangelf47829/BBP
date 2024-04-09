#include "initd.h"
#include "../include/stdio.h"


BBP::std::errno_t BBP::system::initd::initialize()
{
	// Set shell service
	shellService = { std::static_hash("shell-invoke"), true, 0, "help", "version" };
	shellApplicationService = { std::static_hash("shell-app"), true, 1, "help", "version" };

	return 0;
}

BBP::std::errno_t BBP::system::initd::deinitialize()
{
	return 0;
}

BBP::system::DaemonService *BBP::system::initd::lookupFunctor(std::string &str)
{
	// Get request hash
	std::hash_t cmdHash = std::strhsh(str);

	// Look through service list


	// Switch statement
	switch (cmdHash)
	{
		// Invoke a single command
	case std::static_hash("shell-invoke"):
		return &shellService;

		// Spawn the shell application
	case std::static_hash("shell-app"):
		return &shellApplicationService;

	case std::static_hash("shell-builtin"):
		return &shellBuiltinService;
	}

	return nullptr;
}

BBP::std::errno_t BBP::system::initd::functorFunctor(std::hash_t hash, std::size_t argc, std::c_string *argv)
{
	// Switch case
	switch (hash)
	{
	case std::static_hash("shell-invoke"):
		return shell(argc, argv);

	case std::static_hash("shell-app"):
		return shellApplication(argc, argv);

	case std::static_hash("shell-builtin"):
		return shellBuiltin(argc, argv);
	}

}

