#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::SGR_CMD(word arg)
{

	// If arg is '0', reset colours
	if (arg == 0)
	{
		state.simple_bg = 0;
		state.simple_fg = 7;
	}

	// Check if arg is for foreground
	if ((arg >= 30 && arg <= 37) || (arg >= 90 && arg <= 97))
	{
		// If below 37,
		if (arg <= 37)
			state.simple_fg = arg - 30;
		else
			state.simple_fg = (arg - 90) + 8;
	}

	// Check if arg is for background
	if ((arg >= 40 && arg <= 47) || (arg >= 100 && arg <= 107))
	{
		// If below 47,
		if (arg <= 47)
			state.simple_bg = arg - 40;
		else
			state.simple_bg = (arg - 100) + 8;
	}

}