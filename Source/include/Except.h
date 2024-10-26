

#include "errno.h"
#include "Memory.h"

namespace BBP
{
	namespace std
	{

		// Forward declare exception
		struct except;
		except exception(const char *msg, errno_t _errno);

		// Log level for printing error information at exceptions
		void setExceptionLogLevel(std::byte);
		std::byte getExceptionLogLevel();


#ifndef BBP_STDLIB_EXCEPT_H
#define BBP_STDLIB_EXCEPT_H

		struct except
		{
			errno_t code;
		};

#endif
		

	}
}
