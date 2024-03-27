#ifndef BBP_PLUGINS_ECMA35_H
#define BBP_PLUGINS_ECMA35_H

#include "../../../include/Graphics.h"

namespace Environment
{
	namespace Plugins
	{
		struct ECMAContext
		{
			
			// Window that this context pertains to.
			BBP::std::window *window;
		};

		// Pass a character
		bool passChar();

	}
}

#endif