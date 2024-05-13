#ifndef BBP_STDLIB_FILESYS_H
#define BBP_STDLIB_FILESYS_H

#include "Strings.h"
#include "Resources.h"
#include "stdio.h"

#ifndef BBP_STDLIB_FILESYS_NOOVERWRITE
#define BBP_STDLIB_FILESYS_OVERWRITEEXISTINGFILES
#endif

namespace BBP
{

	namespace std
	{

		// Nodes that stores the actual data. (See INode)
		struct FileNode;
		struct DirectoryNode;

		// What the "user" uses
		class VOLUME;
		class DIRECTORY;
		class FILE;
		class PATH;

		// Constants
		static constexpr std::size_t max_open_files = 32;
		static constexpr std::size_t max_path_length = 128;

		// This is ...
		typedef std::index_t noderef_t;
		//extern std::STATIC_PAGE<FileNode, max_open_files> fileTable;

		// Variables
		//extern PATH *workingDirectory;
		//extern VOLUME *primaryVolume;

		// Paths
		class PATH
		{
		public:

			// Get dir component of path
			std::c_string pathName();

			// Get file component of path
			std::c_string fileName();

			// Get complete path name
			std::c_string relName();

			// Copy path from other path
			void copyFrom(PATH &);

			// Does this path start from root? (/)
			bool isRelativeToRoot();

			// Is this path defined to be referenced using root? (Volume path)
			bool isDefinedFromRoot();

			// If defined from root, reference path from root.
			PATH &makeAbsolutePath();

			// If defined from root, reference path from root. Otherwise, make this path relative to another path.
			PATH &makeAbsolutePath(PATH *currentPath);

			// Resolve any absolute file references.
			PATH &resolveAbsolutes();

			// Make dir relative to reference
			static void makeRelative(PATH &reference, PATH &dir);

			// Is this path a directory?
			bool isDirectory();

			// Is this path a file?
			bool isFile();

			// Make this path a file
			std::PATH &makeFile();

			// Make this path a directory
			std::PATH& makeDirectory();

			// Get pointer to volume root
			std::PATH &volumePath();

			PATH(VOLUME *, std::string &path);
			PATH(VOLUME *, std::conststring path);
			PATH(std::string &path);
			PATH(std::conststring path);
			PATH(std::PATH &);
			PATH();

			~PATH();

		private:

			// The string where the path + filename lives
			std::static_string<max_path_length> _path;

			// Pretty much a copy of the input path string
			std::static_string<max_path_length> rawPath;

			// This index stores where the file name begins
			std::index_t fileNameStartIndex;

			// This is where _path is allocated
			std::ResourceManager *allocator;

			// The volume which this path is relative to
			VOLUME *Volume;

			PATH &getFileAndPaths();

		};

		class VOLUME
		{
		public:
			VOLUME(char l, std::conststring);
			VOLUME(char l) : VOLUME(l, "/") {}

			VOLUME() : volumePath(), label(0) {}

			~VOLUME();

			// Mount empty volume
			void mount(char label, std::conststring str);

			PATH volumePath;
			char label;

		};

		class FILE
		{
		public:

			FILE();
			FILE(std::PATH);
			FILE(std::conststring);

			FILE(std::Stack<std::string_element> &data, std::conststring path);

			FileNode *data();

			std::Stack<std::string_element> &b();

			bool is_open();
			inline bool on_disk() { return _is_on_disk; }

			void close();

			FILE &write(std::conststring str);
			FILE &write(std::c_string &str);
			FILE &write(std::string &str);
			FILE &finish(std::conststring str);
			FILE &finish(std::c_string &str);
			FILE &finish(std::string &str);
			FILE &finish();

			bool doesFileExistOnDisk(std::PATH &);
			std::size_t getFileSizeFromDisk(std::PATH &);
			std::size_t readFileFromDisk(std::FileNode *, std::PATH &);
			void writeFileToDisk(std::PATH &);
			void writeFileToDisk();


		private:
			
			std::index_t checkIfIsOpen(std::conststring path);
			std::index_t findClosedFile();

			void getINodeIndex(std::PATH &);
			bool _is_on_disk = false;
			bool _dealloc_page = false;
			bool _loaded_intomem = false;
			bool _unload_inode = false;

			noderef_t node;
		};

		class DIRECTORY
		{
		public:



		private:
			noderef_t node;
			DirectoryNode *getNode();
		};

		struct FileNode
		{
			// The internal data of the file
			std::ResourceManager *allocator;
			std::Stack<std::string_element> fileData;

			// Path of the file
			std::PATH filePath;
			std::hash_t filePathHash;

			FileNode();
			FileNode(std::ResourceManager *res, std::conststring path);
			FileNode(std::Stack<std::string_element> &data, std::conststring path);
			FileNode(std::ResourceManager *res, std::VOLUME *v, std::conststring path);
			FileNode(std::ResourceManager *res, std::size_t size, std::VOLUME *v, std::conststring path);
			FileNode(std::ResourceManager *res, std::size_t size, std::PATH &path);
			FileNode(std::Stack<std::string_element> &data, std::VOLUME *v, std::conststring path);

		};

		struct DirectoryNode
		{
			// The volume this directory is in
			VOLUME *volume;

			// The path of this directory
			PATH path;

			// Relationships between this and other directories
			DIRECTORY *parent;
			std::Stack<DIRECTORY> subDirectories;

			// Files stored in this directory
			std::Stack<FILE> files;
		};

		
		template<typename... Args>
		inline void fprintf(BBP::std::FILE &file, std::conststring format, Args... args, ResourceManager *memory)
		{
			// Check if can write to file
			if (!file.is_open())
				throw std::exception("ERR_FILE_NOT_OPEN", EBADF);

			// Allocate a buffer
			std::string buffer(BBP::std::max_page_elements, (BBP::std::string_element *)memory->calloc(BBP::std::max_page_elements, sizeof(BBP::std::string_element)));

			// Format the string
			std::sprintf(buffer.data, format, args...);

			// Then print
			file.data()->fileData <<= buffer;

			// Then free the buffer
			memory->free(buffer.data);

		}

	}
}


#endif
