#ifndef BBP_STD_FILESYSINFO_H
#define BBP_STD_FILESYSINFO_H

#include "FileSys.h"
#include "Time.h"

namespace BBP
{
	namespace std
	{

		// Structure used to get volume information
		struct VolumeInfo
		{
			// Is mounted or not
			bool mounted;

			// Full name
			std::string name;

			// Capacity
			std::dword capacity;

		};

		// Populate volume info
		bool populateVolumeInfo(VolumeInfo &, PATH &);

		// Class used to get directory information
		class DirectoryInfo
		{
			// Is symbolic link?
			bool symbolic;

			// Exists?
			bool exists;

			// Is this actually a directory?
			bool isDirectory;

			// Amount of entities in directory
			std::size_t entityCount;

			// Of which, ... are files
			std::size_t fileCount;

			// Of which, ... are directories
			std::size_t dirCount;

			// string table for entries
			std::string stringTable;

			// Precomputed lengths
			std::size_t fileLength;
			std::size_t dirLength;

			// indicies for files and directories within the stringTable
			std::PAGE<index_t> fileIndicies;
			std::PAGE<index_t> dirIndicies;

			// Index of next entity to fetch
			std::index_t fetchEntity;

			// Creation time
			std::time_t creation;

			// Modification time
			std::time_t modification;

			// Directory hash
			std::hash_t hash;

			// Allocator used to populate this
			std::ResourceManager *allocator;

			// Has already been populated?
			bool populated;

		public:

			// Scarcely populate the directory information (Only count files, directories, and their sizes.)
			bool scarce_populate(PATH &);

			// Get entity count
			std::size_t entity_count();

			// Get file count
			std::size_t file_count();

			// Get directory count
			std::size_t dir_count();

			// Does this exist at all?
			bool path_exists();

			// Is this a directory?
			bool is_directory();

			// Populate the directory information.
			bool populate(PATH &, std::ResourceManager *allocator);

			// Release directory information
			void release();

			// Load a file into a string
			bool loadFile(std::index_t idx, std::string &);

			// Load a dir into a string
			bool loadDirectory(std::index_t idx, std::string &);

			// Load entity (first file, then directory)
			bool loadEntity(std::index_t idx, std::string &);

		};

		// Structure used to get file information
		struct FileInfo
		{

		};

		// Populate volume info
		bool populateFile(FileInfo &, PATH &);

		// Class used to keep track of all of this
		class FileSysInfo
		{
		public:
			// Enum used to track type of file system entry
			enum FileSysEntryType : word
			{
				None,
				File,
				Directory,
				Volume,
				Unkown = 0xFF
			};

		private:

			// The type of thing
			FileSysEntryType type;

			// Information
			union
			{
				VolumeInfo volumeInfo;
				DirectoryInfo directoryInfo;
				FileInfo fileInfo;
			};

			// 

		public:

			// Constructors
			FileSysInfo(PATH &);

			// Get entity information
			bool isFile();
			bool isDirectory();
			bool isVolume();

			// Check if it even exists or not.
			bool doesExist();

		};

	}
}

#endif
