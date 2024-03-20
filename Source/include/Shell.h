#ifndef BBP_SYSTEM_SHELL_H
#define BBP_SYSTEM_SHELL_H

#include "stddef.h"
#include "Strings.h"
#include "License.h"

namespace BBP
{
	namespace system
	{

		// License information
		appInfo getShellInformation();
		void printShellInformation();
		void printShellDirectory();

		

	}
}

#endif