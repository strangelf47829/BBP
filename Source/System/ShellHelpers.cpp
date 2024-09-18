#include "../include/DaemonRegistry.h"
#include "../include/stdio.h"
#include "../include/Shell.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"

// just for show
#include "../include/Executable.h"

#include "../include/Shell.h"

// Shell information
BBP::system::appInfo shellInformation;

// get Shell information
BBP::system::appInfo BBP::system::getShellInformation()
{
	return shellInformation;
}

void BBP::system::printShellDirectory()
{
	// Also get some paths
	std::PATH fullpath = system::Shell::getWorkingDirectory();
	fullpath = fullpath.resolveAbsolutes();

	// fancy flag bc why not
	bool fancy = true;

	if (fancy == false)
	{

		// Print username
		std::printf("\e[0;92m%s@", "kangel");
		std::printf("%s\e[0;37m:", "dd");


		std::printf("\e[1;34m%s\e[0;37m$ ", fullpath.relName());

	}
	else
	{
		std::printf("\n+---(\e[0;92m%s@", "kangel");
		std::printf("%s\e[0;37m)-[", "dd");
		std::printf("\e[1;94m%s\e[0;37m]\n+--$ ", fullpath.relName());

	}




}
void BBP::system::printShellInformation()
{
	// Get license information
	appInfo info = getShellInformation();

	// Print build information
	std::printf("BBP shell, version %u.%u.%u(%u)", info.build.majorVersion, info.build.minorVersion, info.build.patchVersion, info.build.buildVersion);

	if (info.build.extraInfo)
		std::printf(" %s\n", info.build.extraInfo);
	else
		std::printf("\n");

	// Print copyright information
	std::printf("Copyright (C) %u %s\n", info.copyright.copyrightYear, info.copyright.copyrightHolder);

	// Print license information
	std::printf("License %s: %s", info.license.licenseName, info.license.licenseInfo);

	if (info.license.licenseURL)
		std::printf(" <%s>\n\n", info.license.licenseURL);
	else
		std::printf("\n\n");
}