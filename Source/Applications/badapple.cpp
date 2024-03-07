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
#include "../include/Graphics.h"

#include <stdlib.h>
#include <cstdio>

#define FRAMECOUNT 6572
#define FRAMERATE 30

/*
int BBP::badapple_main(int argc, char **argv)
{

	

	std::STDOUT << "Cannot play bad apple: refactoring in progres..." <<= std::endl;
	return 1;
}
*/


BBP::std::ResourceManager framePage;

int BBP::badapple_main(int argc, char **argv)
{

	/*
	char *fileName = (char *)BBP::std::activemem->calloc(34, sizeof(char));


	BBP::std::ResourceManager *oldMem = BBP::std::activemem;
	BBP::std::activemem = &framePage;

	for (unsigned int i = 1; i < FRAMECOUNT + 1; i += 1)
	{
		BBP::std::sprintf(fileName, "/home/bapple/ascii/%04u.txt", i);
		

		std::FILE frame(fileName);

		::std::printf("%s", frame.b().page->data);
		

		frame.close();

		std::delay(1000 / FRAMERATE);
		std::printf("\033[2J\033[1;1H");
		::system("clear");
		
		if (i % 50 == 0)
			BBP::std::activemem->deleteAll();



	}

	BBP::std::activemem->deleteAll();

	BBP::std::activemem = oldMem;
	std::activemem->free(fileName);
	*/
	return 0;
}
