#ifndef BBP_STD_CSTDIO_H
#define BBP_STD_CSTDIO_H

#include "Memory.h"
#include "Stack.h"
#include "Strings.h"
#include "FileSys.h"

// Read stuff from STDIN, STDOUT, and files.
// This is the file that defines keyboard, file, and screen interactions.
// These standard functions then interact with functions defined in stddrv to actually perform these functions.

namespace BBP
{
	namespace std
	{
		// Type used to denote a file stream
		using fstream_t = FILE *;

		// Open a file







		// Type used to denote stream
		using stream_t = std::Stack<std::string_element> &;

		// Read and write data to and from a stream
		void fread(stream_t);
		void fwrite(stream_t);

		// 

	}
}

#endif
