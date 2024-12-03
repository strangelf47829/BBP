#include "../include/FAT.h"
#include "../include/FileSys.h"

BBP::std::FILE f;

BBP::std::STATIC_PAGE<BBP::std::byte, 512> sec;

// Set sector
void BBP::fs::FATfs::setSector(std::index_t sect)
{
	// ** This is just for debugging **
	if (f.is_open() == false)
		f = std::FILE("/disk.img");

	// ** Copy over data **
	for (std::index_t idx = 0; idx < 512; idx++)
		sec[idx] = f.b()[idx + sect * 512];
	
	// ** Then set data **
	sector = sec;
}

// Calculate sector from cluster
void BBP::fs::FATfs::setCluster(std::index_t N)
{
	// Calculate first sector
	std::index_t sct = ((N - 2) * header.common.sectorsPerCluster) + header.dataSector;

	// Then set sector
	setSector(sct);
}

