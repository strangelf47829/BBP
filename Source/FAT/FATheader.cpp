#include "../include/FAT.h"
#include "../include/stdio.h"

void BBP::fs::FATfs::readBootSector()
{
	// Seek 0th sector
	setSector(0);

	// Read common portion of boot sector
	std::offset_t offset = readCommonBootSector();

	// Then read FAT32 and Volume information to determine FAT type
	readVolumeInformation(offset);
	std::offset_t fat32offset = readFAT32Information(offset);

	// Then determine cluster count
	std::size_t clusterCount = determineClusterCount();

	// Then determine FAT type
	if (clusterCount < 4085)
		header.fatType = 12;
	else if (clusterCount < 65525)
		header.fatType = 16;
	else
		header.fatType = 32;

	// If fat type is 32, read volume info from new offset. Otherwise read from original offset
	if (header.fatType == 32)
		readVolumeInformation(fat32offset);

	// Print info
	std::printf("Printing information for FAT%d drive '%s' (serial no. 0x%08x)\n", header.fatType, header.vol.volumeLabel, header.vol.volumeSerialNumber);
	std::printf("%d clusters on FAT%d system.\n\n", clusterCount, header.fatType);

	std::printf("Displaying files on '/':\n");

}

BBP::std::size_t BBP::fs::FATfs::determineClusterCount()
{
	// If 0 bytes per sector, calculation cannot proceed since that would mean division by 0. If such is the case, return '0'
	if (header.common.bytesPerSector == 0)
		return 0;

	// Get Amount of root directory sectors
	std::size_t rootDirSectors = ((header.common.rootEntryCount * 32) + (header.common.bytesPerSector - 1)) / header.common.bytesPerSector;

	// Then determine the amount of sectors in the data region of the volume
	header.fatEntrySize = header.common.FATsize16;

	// If this is set to 0, read the FAT32 version instead
	if (header.fatEntrySize == 0)
		header.fatEntrySize = header.fat32.FATsize32;

	// Then get total sector count
	std::size_t sectorCount = header.common.sectorCount_low;

	// If this is set to 0, read the FAT32 version instead
	if (sectorCount == 0)
		sectorCount = header.common.sectorCount_high;

	// Then get amount of data sectors
	std::size_t dataSectors = sectorCount - (header.common.reservedSectorCount + (header.common.FATcount * header.fatEntrySize) + rootDirSectors);

	// Check if 'sectorsPerCluster' isn't 0, as this would cause division by 0. If that is the case, return 0
	if (header.common.sectorsPerCluster == 0)
		return 0;

	// Then reduce this down to cluster count
	std::size_t clusterCount = dataSectors / header.common.sectorsPerCluster;

	// Also calculate the data start sector
	header.dataSector = header.common.reservedSectorCount + (header.common.FATcount * header.fatEntrySize);

	return clusterCount;
}

// Return the start sector for any given cluster 'N'
BBP::std::index_t BBP::fs::FATfs::startSectorForCluster(std::index_t N)
{ return ((N - 2) * header.common.sectorsPerCluster) + header.dataSector; }

BBP::std::offset_t BBP::fs::FATfs::readFAT32Information(std::offset_t offset)
{
	// See "FAT.h" to see what each field is.
	
	header.fat32.FATsize32 = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	header.fat32.activeFat = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.fat32.fatVersion = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.fat32.rootClusterIndex = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	header.fat32.FSInfoSectorIndex = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.fat32.FSInfoBackupSectorIndex = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	// 12-bit reserved field
	for (std::index_t idx = 0; idx < 12; idx++)
		header.fat32.reserved[idx] = sector[idx + offset];
	offset += 12;

	return offset;
}

BBP::std::offset_t BBP::fs::FATfs::readVolumeInformation(std::offset_t offset)
{
	// See "FAT.h" to see what each field is.
	
	header.vol.driveNumber = sector[offset];
	offset++;

	header.vol.reserved = sector[offset];
	offset++;

	header.vol.bootSignature = sector[offset];
	offset++;

	header.vol.volumeSerialNumber = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	// 11-byte long string
	for (std::index_t idx = 0; idx < 11; idx++)
		header.vol.volumeLabel[idx] = sector[offset + idx];
	header.vol.volumeLabel[11] = 0; offset += 11;

	// 8-byte long string
	for (std::index_t idx = 0; idx < 8; idx++)
		header.vol.volumeTypeString[idx] = sector[offset + idx];
	header.vol.volumeTypeString[8] = 0; offset += 8;

	return offset;
}

BBP::std::offset_t BBP::fs::FATfs::readCommonBootSector()
{
	// Running offset
	std::offset_t offset = 0;

	// Assume first sector is that of a boot sector. Set first three bytes to executable
	for (std::index_t idx = 0; idx < 3; idx++)
		header.common.exec[idx] = sector[idx];
	offset += 3;

	// Now copy over OEM name (offset is '3')
	for (std::index_t idx = 0; idx < 8; idx++)
		header.common.OEMname[idx] = sector[idx + offset];
	header.common.OEMname[8] = 0; offset += 8;

	// Two bytes for bytes per sector. Offset is now += 8
	header.common.bytesPerSector = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	// Then sectors per cluster
	header.common.sectorsPerCluster = sector[offset];
	offset++;

	// And so forth.. See "FAT.h" for information on how this is read
	header.common.reservedSectorCount = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.FATcount = sector[offset];
	offset++;

	header.common.rootEntryCount = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.sectorCount_low = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.media = sector[offset];
	offset++;

	header.common.FATsize16 = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.sectorsPerTrack = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.headCount = std::stitch(sector[offset + 1], sector[offset], true);
	offset += 2;

	header.common.hiddenSectorCount = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	header.common.sectorCount_high = std::stitch(sector[offset + 3], sector[offset + 2], sector[offset + 1], sector[offset], true);
	offset += 4;

	// Then return offset
	return offset;	
}
