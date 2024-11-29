#include "../include/BBP.h"
#include "../include/FAT.h"

// FAT class
BBP::fs::FATfs fatfs;

BBP::std::errno_t BBP::system::rm_builtin(std::size_t argc, std::c_string *argv)
{
	// Now read fatfile data
	fatfs.readBootSector();

	// Go to root
	fatfs.goToRoot();
	
	// Next, read 1st and 2nd FATEntry
	fatfs.readFATEntry(1);

	// Go to Cluster
	fatfs.TravelToFirstEntityCluster();

	// Three entries:
	fatfs.readFATEntry(2);

	// Then travel there
	fatfs.TravelToFirstEntityCluster();

	// Then print
	fatfs.pp();

	return 0;
}
