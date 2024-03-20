#ifndef OS_REGISTRY_H
#define OS_REGISTRY_H

// Provide a list of boot records for ...
#include "../include/BootRecord.h"

namespace Environment
{
	namespace BIOS
	{
		extern BBP::system::BootRecord BBPRecord;
		extern BBP::system::BootRecord Windowse20Record;
	}
}

#endif
