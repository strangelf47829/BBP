#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Resources.h"
#include "../include/ELF.h"
#include "../include/FileSys.h"
#include "../include/Lex.h"
#include "../include/CPPApplications.h"
#include "../include/Executable.h"
#include "../include/System.h"
#include "../include/Time.h"


#define RED "\e[0;31m"
#define NORM "\e[0;37m"


int BBP::neofetch_main(int argc, char **argv)
{

	//std::printf(RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n");
	//std::printf(RED "B::::::::::::::::B  B::::::::::::::::B" NORM "\n");
	//std::printf(RED "B::::::BBBBBB:::::B B::::::BBBBBB:::::B" NORM "\n");
	//std::printf(RED "BB:::::B     B:::::BBB:::::B     B:::::B" NORM "\n");
	//std::printf(RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n");
	//std::printf(RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n");
	//std::printf(RED "  B::::BBBBBB:::::B   B::::BBBBBB:::::B" NORM "\n");
	//std::printf(RED "  B:::::::::::::BB    B:::::::::::::BB" NORM "\n");
	//std::printf(RED "  B::::BBBBBB:::::B   B::::BBBBBB:::::B" NORM "\n");
	//std::printf(RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n");
	//std::printf(RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n");
	//std::printf(RED "  B::::B     B:::::B  B::::B     B:::::B" NORM "\n");
	//std::printf(RED "BB:::::BBBBBB::::::BBB:::::BBBBBB::::::B" NORM "\n");
	//std::printf(RED "B:::::::::::::::::B B:::::::::::::::::B" NORM "\n");
	//std::printf(RED "B::::::::::::::::B  B::::::::::::::::B" NORM "\n");
	//std::printf(RED "BBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBB" NORM "\n");

	std::printf(RED "        66666666    " NORM "         000000000          77777777777777777777" NORM "\n");
	std::printf(RED "       6::::::6     " NORM "       00:::::::::00        7::::::::::::::::::7" NORM "     " RED HOSTNAME NORM "\n");
	std::printf(RED "      6::::::6      " NORM "     00:::::::::::::00      7::::::::::::::::::7" NORM "     ---------- " "\n");
	std::printf(RED "     6::::::6       " NORM "    0:::::::000:::::::0     777777777777:::::::7" NORM "     " RED "OS: " NORM OS "\n");
	std::printf(RED "    6::::::6        " NORM "    0::::::0   0::::::0                7::::::7 " NORM "     " RED "Kernel: " NORM KERNEL "\n");
	std::printf(RED "   6::::::6         " NORM "    0:::::0     0:::::0               7::::::7  " NORM "     " RED "Shell: " NORM SHELL "\n");
	std::printf(RED "  6::::::6          " NORM "    0:::::0     0:::::0              7::::::7   " NORM "     " RED "Terminal: " NORM TERMINAL "\n");
	std::printf(RED " 6::::::::66666     " NORM "    0:::::0 000 0:::::0             7::::::7    " NORM "     " RED "CPU: " NORM CPU "\n");
	std::printf(RED "6::::::::::::::66   " NORM "    0:::::0 000 0:::::0            7::::::7     " NORM "     " RED "Awesome: " NORM "Yes" "\n");
	std::printf(RED "6::::::66666:::::6  " NORM "    0:::::0     0:::::0           7::::::7      " NORM "\n");
	std::printf(RED "6:::::6     6:::::6 " NORM "    0:::::0     0:::::0          7::::::7       " NORM "\n");
	std::printf(RED "6:::::6     6:::::6 " NORM "    0::::::0   0::::::0         7::::::7        " NORM "\n");
	std::printf(RED "6::::::66666::::::6 " NORM "    0:::::::000:::::::0        7::::::7         " NORM "\n");
	std::printf(RED " 66:::::::::::::66  " NORM "     00:::::::::::::00        7::::::7          " NORM "\n");
	std::printf(RED "   66:::::::::66    " NORM "       00:::::::::00         7::::::7           " NORM "\n");
	std::printf(RED "     666666666      " NORM "         000000000          77777777            " NORM "\n");


	return 0;
}