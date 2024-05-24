#ifndef BBP_STD_ERRNOSTRINGS_H
#define BBP_STD_ERRNOSTRINGS_H

#include "errno.h"
#include "Strings.h"

namespace BBP
{
	namespace std
	{

		// Convert error to string
		void strerror(std::errno_t, std::string &);

		// 

	}
}

#endif