#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

BBP::std::constexprstring helpMsg = "\n\
These shell commands are defined internally.Type `help' to see this list.\n\
Type `help name' to find out more about the function `name'.\n\n";

void printLicense(BBP::std::c_string funcName, BBP::system::appInfo &info)
{
	// Print build information
	BBP::std::printf("%s, version %u.%u.%u(%u)", funcName, info.build.majorVersion, info.build.minorVersion, info.build.patchVersion, info.build.buildVersion);

	if (info.build.extraInfo)
		BBP::std::printf(" %s\n", info.build.extraInfo);
	else
		BBP::std::printf("\n");

	// Print copyright information
	BBP::std::printf("Copyright (C) %u %s\n", info.copyright.copyrightYear, info.copyright.copyrightHolder);

	// Print license information
	BBP::std::printf("License %s: %s", info.license.licenseName, info.license.licenseInfo);

	if (info.license.licenseURL)
		BBP::std::printf(" <%s>\n\n", info.license.licenseURL);
	else
		BBP::std::printf("\n\n");
}

BBP::std::errno_t BBP::system::help_builtin(std::size_t argc, std::c_string *argv)
{
	// Print shell info
	printShellInformation();

	// Print help information
	std::printf("%s", helpMsg);

	// Loop over each command
	for (std::index_t idx = 0; idx < builtinCommands.dataSize; idx++)
	{
		// Get pointer to data
		BuiltinCommand *cmd = builtinCommands[idx];

		// Print data
		std::printf("\e[0;32m%s\e[0;38m\t\t\t%s\n", cmd->name.data, cmd->helpInformation);
	}

	return 0;
}