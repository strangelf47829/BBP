

#include "errno.h"
#include "Memory.h"


namespace BBP
{
	namespace std
	{

		struct except;
		except exception(const char *msg, errno_t _errno);


#ifndef BBP_STDLIB_EXCEPT_H
#define BBP_STDLIB_EXCEPT_H

		struct except
		{
			errno_t code;
		};

#endif
		

	}
}
