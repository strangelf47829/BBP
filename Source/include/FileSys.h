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
		extern std::STATIC_PAGE<FileNode, max_open_files> fileTable;

		// Variables
		extern PATH *workingDirectory;
		extern VOLUME *primaryVolume;

		// Paths
		class PATH
		{
		public:

			std::c_string pathName();
			std::c_string fileName();

			std::c_string relName();

			bool isRelativeToRoot();
			bool isDefinedFromRoot();

			PATH &makeAbsolutePath(PATH *currentPath);
			PATH &resolveAbsolutes();

			void makeRelative(PATH &reference, PATH &dir);

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

			~VOLUME();

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

			FileNode()
				: allocator(nullptr),
				fileData((std::PAGE<std::string_element> *)nullptr, 0),
				filePath(),
				filePathHash(0)
			{}

			FileNode(std::ResourceManager *res, std::conststring path)
				: allocator(res),
				fileData((std::PAGE<std::string_element> *)nullptr),
				filePath(path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

			FileNode(std::Stack<std::string_element> &data, std::conststring path)
				: allocator(nullptr),
				fileData(data),
				filePath(path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

			FileNode(std::ResourceManager *res, std::VOLUME *v, std::conststring path)
				: allocator(res),
				fileData((std::PAGE<std::string_element> *)nullptr, 0),
				filePath(v, path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

			FileNode(std::ResourceManager *res, std::size_t size, std::VOLUME *v, std::conststring path)
				: allocator(res),
				fileData(
					(std::PAGE<std::string_element> *)res->add_object(new std::PAGE<std::string_element>(size, (std::string_element *)res->calloc(size, sizeof(std::string_element)))),
					size),
				filePath(v, path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

			FileNode(std::ResourceManager *res, std::size_t size, std::PATH &path)
				: allocator(res),
				fileData(
					(std::PAGE<std::string_element> *)res->add_object(new std::PAGE<std::string_element>(size, (std::string_element *)res->calloc(size, sizeof(std::string_element)))),
					size),
				filePath(path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

			

			FileNode(std::Stack<std::string_element> &data, std::VOLUME *v, std::conststring path)
				: allocator(nullptr),
				fileData(data),
				filePath(v, path),
				filePathHash(0)
			{
				filePath.makeAbsolutePath(std::workingDirectory);
				filePathHash = std::strhsh(filePath.relName());
			}

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
		inline void fprintf(BBP::std::FILE &file, std::conststring format, Args... args)
		{
			// Check if can write to file
			if (!file.is_open())
				throw std::exception("ERR_FILE_NOT_OPEN", EBADF);

			// Allocate a buffer
			std::string buffer(BBP::std::max_page_elements, (BBP::std::string_element *)BBP::std::activemem->calloc(BBP::std::max_page_elements, sizeof(BBP::std::string_element)));

			// Format the string
			std::sprintf(buffer.data, format, args...);

			// Then print
			file.data()->fileData <<= buffer;

			// Then free the buffer
			BBP::std::activemem->free(buffer.data);

		}

	}
}


#endif
