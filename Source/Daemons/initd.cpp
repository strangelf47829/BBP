#include "initd.h"
#include "../include/stdio.h"


BBP::std::errno_t BBP::system::initd::initialize()
{
	// Set shell service
	shellService = { std::static_hash("shell"), true, 0, "help", "version" };

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
	
	// Switch statement
	switch (cmdHash)
	{
	case std::static_hash("shell"):
		return &shellService;
	}

	return nullptr;
}

BBP::std::errno_t BBP::system::initd::functorFunctor(std::hash_t hash, std::size_t argc, std::c_string *argv)
{
	// Switch case
	switch (hash)
	{
	case std::static_hash("shell"):
		return shell(argc, argv);
	}

}

