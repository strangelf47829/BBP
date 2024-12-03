#ifndef BBP_FAT_H
#define BBP_FAT_H

#include "Memory.h"
#include "errno.h"
#include "FileSys.h"

namespace BBP
{

	namespace fs
	{

		// This class defines a *virtual* base class for drivers that store and retrieve FAT images.
		// The point of making this an abstract class is that this allows for multiple drivers
		// to access FAT records, as using a simple PAGE<std::byte> or something similar would
		// mean loading the entire disk to memory, defeating the entire purpose of storing something on disk.
		class FATfsdrv
		{
			// Check if available
			virtual bool available() = 0;

			// Load sector into memory
			virtual std::errno_t loadSector(std::index_t) = 0;

			// Write data to sector from memory
			virtual std::errno_t writeSector(std::index_t) = 0;

			// Get byte from sector
			virtual std::byte &operator [](std::index_t) = 0;

			// Sector size count
			virtual std::size_t sectorSize() = 0;
		};

		// This structure describes how the universal part of the first sector of a FAT file system
		// describes the rest of the file system. This is because FAT32 headers are different from FAT12/16 headers,
		// However some parts are the same.
		struct FATfsheader_universal
		{
			// The first 3 bytes are executable code
			std::byte exec[3];

			// The name of the system that formatted the volume
			std::byte OEMname[9];

			// The amount of bytes per sector, almost always 512.
			std::halfword bytesPerSector;

			// Amount of sectors per cluster. Must be a power of 2
			std::byte sectorsPerCluster;

			// Reserved sector count, usually 32 on FAT32 systems and .
			std::halfword reservedSectorCount;

			// Amount of FAT entries in the described volume. Almost always 2
			std::byte FATcount;

			// Amount of 32-bit entries in the root directory. Must be set to 0 apparently on FAT32 systems.
			std::halfword rootEntryCount;

			// Low half-word of the total amount of sectors in the volume
			std::halfword sectorCount_low;

			// Type of media volume is on. 0xF8 is usually used for fixed media, whereas 0xF0 is usually used for removable media.
			std::byte media;

			// The amount of sectors used by a FAT entry on FAT12/16 systems, otherwise set to 0
			std::halfword FATsize16;

			// CHS information -- used for interrupt 0x13 (so pretty much useless today)
			std::halfword sectorsPerTrack;
			std::halfword headCount;

			// Amount of hidden sectors preceding the first FAT entry.
			// The data on those sectors have no standard meaning
			std::word hiddenSectorCount;

			// This field contains the high sectorCount on FAT12/16 systems, if and only if the low count is set to 0.
			// On FAT32 systems, this value must be 0.
			std::word sectorCount_high;
		};

		// After this section, the defined meanings of the header differ from FAT12/16 and FAT32.
		// This header describes the meaning of fields on FAT12/16 file systems.
		// This header is also present on FAT32, however the offsets are different.
		struct FATfsheader_volident
		{
			// Drive number; used by interrupt 0x13 to determine the ID of the drive
			std::byte driveNumber;

			// Reserved. Set to 0
			std::byte reserved;

			// Boot signature; ??
			std::byte bootSignature;

			// Contains the 'serial number' of the drive. Usually some pseudo-random 32 bit when formatting drive.
			std::word volumeSerialNumber;

			// An 11-byte long string that contains the label of the drive.
			std::byte volumeLabel[12];

			// An 8-byte long string that *might* contain the type of file system.
			std::byte volumeTypeString[9];
		};

		// This contains the FAT32 header definitions that appends to the FATfsheader_universal structure.
		struct FATfsheader_32
		{
			// This contains the 32-bit sector count of the FAT entries.
			std::word FATsize32;

			// Bit mask for determining *which* FAT entry is currently in use.
			// 	bits 0-3 ; Number of active FAT entry (starting from 0)
			// 	bits 4-6 ; Reserved
			// 	bit  7   ; [0 if FAT is mirrored] -- [1 if bits 0-3 indicate active FAT]
			// 	bits 8-15; Reserved
			std::halfword activeFat;

			// Version of FAT32 file system; High byte is major version, low byte is minor version
			std::halfword fatVersion;

			// The first cluster number of the root directory. Most commonly '2'
			std::word rootClusterIndex;

			// Values of sector index of FSINFO structure. Only the primary structu is kept up to date. Typically 1 and 6 respectively.
			std::halfword FSInfoSectorIndex;
			std::halfword FSInfoBackupSectorIndex;
			
			// Reserved 12 bits. Should be 0
			std::byte reserved[12];	
		};

		// This structure contains the full definition for a FAT file system header.
		struct FATfsheader
		{
			// Common header fields
			FATfsheader_universal common;

			// Volume identification data
			FATfsheader_volident vol;

			// FAT32 data 
			FATfsheader_32 fat32;

			// bytes 511 and 512
			std::byte magic[2];

			// Either 12, 16, or 32, though could theoretically be any number
			std::byte fatType;

			// Size of the FAT
			std::size_t fatEntrySize;

			// index of sector where data actually starts
			std::index_t dataSector;
		};

		// This is the FATfs_info field, and is also referred to as a FAT entry.
		struct FATfsinfo
		{
			// 4 byte magic number 0x41615252
			std::byte magic1[4];

			// ** 480 bytes of reserved bytes **

			// Another 4 magic bytes 0x61417272
			std::byte magic2[4];

			// The amount of free sectors
			std::word freeSectors;

			// The suspected last free sector of the volume
			std::word nextFreeSector;

			// ** Another 12 reserved bytes **

			// Another 4 byte magic number 0xAA550000
			std::byte magic3[4]; 	
		};

		// This structure holds the information of a file entry
		struct FATfs_fileEntry
		{
			// 11-byte string
			std::byte shortname[12];

			// Attributes
			std::byte attributes;
			/* 0x01; read only
			 * 0x02; hidden
			 * 0x04; system
			 * 0x08; volumeId
			 * 0x10; directory
			 * 0x20; archive
			 */

			// Reserved by microsoft. Probably give it some use tho
			std::byte NTRes;

			// Time of creation in centiseconds
			std::byte centiseconds_creation;

			// Time and date of creation
			std::halfword creationTime;
			std::halfword creationDate;

			// Date of last access
			std::halfword lastAccess;

			// High word of this entry's first cluster number
			std::halfword cluster_high;

			// Last write time and date
			std::halfword lastWriteTime;
			std::halfword lastWriteDate;

			// Low word of this entry's first cluster number
			std::halfword cluster_low;

			// Size of this file (in bytes)
			std::word fileSize;
		};

		// Date and time encoding/decoding
		std::halfword encodeFATDate(std::byte day, std::byte month, std::halfword year);
		std::halfword encodeFATTime(std::byte hour, std::byte minute, std::byte second);

		void decodeFATDate(std::halfword data, std::byte &, std::byte &, std::halfword &);
		void decodeFATTime(std::halfword data, std::byte &, std::byte &, std::byte &);


		// This class actually controls a FAT volume
		class FATfs
		{
			
			// Read common section of boot header
			std::offset_t readCommonBootSector();

			// Read FAT32 information
			std::offset_t readFAT32Information(std::offset_t);

			// Read volume information
			std::offset_t readVolumeInformation(std::offset_t);

			// Determine cluster count (needed to determine FAT12/FAT16/FAT32
			std::size_t determineClusterCount();

			// Set sector index (From BPB)
			void setSector(std::index_t);

			// Set cluster index (Relative to BPB)
			void setCluster(std::index_t);

			// Get start sector number for given cluster
			std::index_t startSectorForCluster(std::index_t);

		public:
	
			// Current FATfs header
			FATfsheader header;

			// Current file node
			FATfs_fileEntry node;

			// Active sector
			std::PAGE<std::byte> sector;

			// Read active sector as first sector
			void readBootSector();
		
			// Go to root
			void goToRoot();

			// Go to FAT (sector after reserved sectors)
			void goToFAT();

			// Read FAT entry
			void readFATEntry(std::index_t);

			// Travel to entry sector
			void TravelToFirstEntityCluster();

			// Data cluster idk
			void goToDataCluster(std::index_t iid)
			{ setCluster(iid); }

			// Look for entity
			void loadEntity(std::PATH);

			// Print data
			void pp();

		};


	}

}


#endif
