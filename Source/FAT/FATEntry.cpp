#include "../include/FAT.h"
#include "../include/stdio.h"

void BBP::fs::FATfs::goToRoot()
{
	// If fat isn't 32, just go to header.dataSector since that is the start of 'data' (Root dir is immediately after FAT entries
	if (header.fatType != 32)
	{
		setSector(header.dataSector);
		return;
	}

	// This is a FAT32 image. Find root:
	setCluster(header.fat32.rootClusterIndex);
}

void BBP::fs::FATfs::readFATEntry(std::index_t entry)
{
	// Calculate offset
	std::offset_t offset = 32 * entry;

	// Then copy over data -- first 11 bytes of name
	for (std::index_t idx = 0; idx < 11; idx++)
		node.shortname[idx] = sector[offset + idx];
	node.shortname[11] = 0; offset += 11;

	// Carry on with rest
	node.attributes = sector[offset];
	offset++;
	
	node.NTRes = sector[offset];
	offset++;

	node.centiseconds_creation = sector[offset];
	offset++;

	// Retrieve dates
	node.creationTime = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.creationDate = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.lastAccess = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.cluster_high = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.lastWriteTime = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.lastWriteDate = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.cluster_low = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	node.fileSize = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	// Print information
	std::printf("File '%s' (0x%02x): %d bytes; at 0x%02x %02x \n", node.shortname, node.attributes, node.fileSize, node.cluster_high, node.cluster_low); 
}

void BBP::fs::FATfs::goToFAT()
{
	// Sector index is:
	std::index_t sectorIndex = header.common.reservedSectorCount;

	// Go To data sector
	setSector(sectorIndex);
}

// Travel to first cluster of loaded entity
void BBP::fs::FATfs::TravelToFirstEntityCluster()
{
	// Go to FAT
	goToFAT();

	// Get cluster upper bits
	std::word clust = std::stitch(node.cluster_high, node.cluster_low, true); 

	// Cluster
	std::printf("data is at cluster %u\n", clust); 

	// Then move to cluster
	setCluster(clust);
}

void BBP::fs::FATfs::pp()
{
	for (std::index_t idx = 0; idx < node.fileSize; idx++)
		std::printf("%c", sector[idx]);

	std::printf("\n");
}
