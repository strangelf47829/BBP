#ifndef BBP_SYSTEM_USERINFO_H
#define BBP_SYSTEM_USERINFO_H

#include "Strings.h"

namespace BBP
{
	namespace system
	{

		struct UserPermissions
		{
			// If user is or needs to be root
			bool isRoot;
		};

		struct UserInformation
		{
			// Store the user name
			std::static_string<16> username;

			// Store user password
			std::static_string<16> password;

			// Keep permissions
			UserPermissions perms;
		};

	}
}

#endif