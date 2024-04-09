
#include "../include/Kernel.h"

BBP::std::PATH::PATH()
	: Volume(nullptr), allocator(nullptr), fileNameStartIndex(0)
{
}
BBP::std::PATH::PATH(std::string &str)
	: PATH(BBP::system::kernelSS()->activeContext->primaryVolume, str)
{}

BBP::std::PATH::PATH(std::conststring str)
	: PATH(BBP::system::kernelSS()->activeContext->primaryVolume, str)
{}

BBP::std::PATH::PATH(std::VOLUME *v, std::string &str)
	: PATH(v, str.data) {}

BBP::std::PATH::PATH(std::VOLUME *v, std::conststring str)
	: fileNameStartIndex(0), allocator(BBP::system::kernelSS()->activeContext->activemem), Volume(v)
{

	// Get string sizes
	std::size_t pathlength = std::strlen(str);

	// Zero out buffer
	for (std::index_t idx = 0; idx < rawPath.dataSize; idx++)
		rawPath.static_data[idx] = 0;

	// Copy string into rawPath
	std::strcpy(&rawPath, str);

	// Then prepare _path
	this->getFileAndPaths();

}

BBP::std::PATH::PATH(std::PATH &path)
	: PATH((std::conststring)path.relName())
{

}

BBP::std::PATH::~PATH()
{
	
}

BBP::std::c_string BBP::std::PATH::fileName()
{
	// Move to somewhere else?
	_path.data = _path.static_data;

	return std::read_a(&_path, fileNameStartIndex + 2);
}

BBP::std::c_string BBP::std::PATH::pathName()
{
	return std::read_a(&_path, 0);
}

BBP::std::c_string BBP::std::PATH::relName()
{
	// Move to somewhere else?
	rawPath.data = rawPath.static_data;

	return rawPath.data;
}

// Copy over from something
void BBP::std::PATH::copyFrom(PATH &path)
{
	// Copy strings
	std::strcpy(_path.static_data, path._path.static_data);
	std::strcpy(rawPath.static_data, path.rawPath.static_data);

	// Copy indicies and volume
	this->fileNameStartIndex = path.fileNameStartIndex;
	this->Volume = path.Volume;
}

bool BBP::std::PATH::isRelativeToRoot()
{
	// Get first character
	char first = __UNSAFE__(std::read)(&rawPath, 0);

	return first == '/';
}

bool BBP::std::PATH::isDefinedFromRoot()
{
	// Check if char is '/'
	if (this->Volume == nullptr)
		return false;

	// Check if volume is not the same as ..
	std::size_t VolPathLength = std::strlen(this->Volume->volumePath.relName());
	for (std::index_t idx = 0; idx < VolPathLength; idx++)
	{
		char c1 = this->Volume->volumePath.relName()[idx];
		char c2 = __UNSAFE__(std::read)(&rawPath, idx);

		if (c1 != c2)
			return false;
	}
	return true;
}

BBP::std::PATH &BBP::std::PATH::getFileAndPaths()
{
	// Get length of rawPath, and create copy
	std::size_t rawPathLength = std::strlen(rawPath);
	char nul = '\0';

	// Copy over string
	for (index_t index = 0; index < rawPathLength; index++)
	{
		char c = __UNSAFE__(read)(&rawPath, index);

		if (c == '/')
			this->fileNameStartIndex = index;
		__UNSAFE__(write)(&_path, c, index);
	}

	// Used to store index for copying over name
	std::index_t copyIndex = fileNameStartIndex;

	if (fileNameStartIndex)
	{
		// Then seperate file from directory.
		for (; copyIndex < rawPathLength; copyIndex++)
			__UNSAFE__(write)(&_path, __UNSAFE__(read)(&rawPath, copyIndex), copyIndex + 1);

		// Null terminate directory.
		__UNSAFE__(write)(&_path, nul, fileNameStartIndex + 1);

		// Null terminate file
		__UNSAFE__(write)(&_path, nul, copyIndex + 1);
	}
	else
	{
		
		for (; copyIndex < rawPathLength; copyIndex++)
			__UNSAFE__(write)(&_path, __UNSAFE__(read)(&rawPath, copyIndex), copyIndex + 2);

		// Null terminate directory
		__UNSAFE__(write)(&_path, nul, 0);

		// Null terminate file
		__UNSAFE__(write)(&_path, nul, copyIndex + 1);
	}

	return *this;
}

BBP::std::PATH &BBP::std::PATH::volumePath()
{
	// Check for empty volume
	if (Volume == nullptr)
		std::raise(SIGSEGV);

	// Return volume pathname
	return Volume->volumePath;
}

BBP::std::PATH &BBP::std::PATH::makeAbsolutePath()
{
	return makeAbsolutePath(this);
}

BBP::std::PATH &BBP::std::PATH::makeAbsolutePath(std::PATH *currentDir)
{
	if (!isDefinedFromRoot())
	{
		// Check if dir is relative to root
		bool isRelToRootDir = isRelativeToRoot();

		// Declare strings used to calculate absolute path
		std::c_string relativeTo;
		std::c_string activeDirectory = rawPath.data;

		// Check if this path is relative or absolute
		if (isRelToRootDir)
		{
			if (Volume == nullptr)
				throw std::exception("ERR_UNDEF_VOL", EFAULT);
			relativeTo = Volume->volumePath.rawPath.data;
		}
		else
		{
			if (currentDir == nullptr)
				throw std::exception("ERR_UNDEF_DIR", ENOENT);
			relativeTo = currentDir->rawPath.data;
		}

		// Create buffers
		std::static_string<2 * std::max_path_length> result;

		// Calculate path lengths
		std::size_t relativeToLength = std::strlen(relativeTo);
		std::size_t activeDirectoryLength = std::strlen(activeDirectory);

		// If paths are larger than ...
		if (relativeToLength + activeDirectoryLength + 1 > std::max_page_elements)
		{
			allocator->free(result.data);
			throw std::exception("REL_DIR_TOOLONG", ENAMETOOLONG);
		}

		// Buffer whatever
		if (isRelToRootDir)
			std::sprintf(result.data, "%s%s", relativeTo, activeDirectory + 1);
		else
			std::sprintf(result.data, "%s%s", relativeTo, activeDirectory);

		// Copy result stuff into rawData
		std::strcpy(&rawPath, result.data);
	}

	// Resolve paths
	return resolveAbsolutes();
}

BBP::std::PATH &BBP::std::PATH::resolveAbsolutes()
{
	// Get rawPath length
	std::size_t rawPathLength = std::strlen(rawPath);

	// Stack to 
	std::STATIC_PAGE<index_t, 64> separatorPage;
	std::Stack<index_t> separatorStack(&separatorPage, 64);

	for (index_t index = 0; index < rawPathLength - 1; index++)
	{
		// Get current and next character
		char c = __UNSAFE__(read)(&rawPath, index);
		char nextC = __UNSAFE__(read)(&rawPath, index + 1);
		char nextnextC = __UNSAFE__(read)(&rawPath, index + 2);

		// If next character is null, break out of loop
		if (nextC == '\0')
			break;

		// If this character is a '/', do some things based on next character
		if (c == '/')
		{
			// If next character is '/', just move everything left by one
			if (nextC == '/')
			{
				for (index_t i = index; i < rawPathLength; i++)
					__UNSAFE__(write)(&rawPath, __UNSAFE__(read)(&rawPath, i + 1), i);
				index--;
			}
			else // Otherwise 
			{
				separatorStack << index;
			}
			continue;
		}

		// If this character is a '.', do some things based on next character
		if (c == '.')
		{
			// If next character is a '/', just move everything left by two
			if (nextC == '/')
			{
				for (index_t i = index; i < rawPathLength; i++)
					__UNSAFE__(write)(&rawPath, __UNSAFE__(read)(&rawPath, i + 2), i);
				index--;
			}
			else if (nextC == '.' && nextnextC == '/')
			{
				// Read last two '/''s from stack, if there exists at least 2.
				index_t from = 0, to = 0;

				if (separatorStack.atElement >= 2)
					separatorStack >> to >> from;

				// Looping
				for (index_t idx = 0; idx + to + 2 < rawPathLength; idx++)
					__UNSAFE__(write)(&rawPath, __UNSAFE__(read)(&rawPath, idx + to + 4), idx + from + 1);

				// Move back to 'from'
				index = from - 1;
			}

			continue;
		}
		
	}

	// Mirror edited stuff into ...
	return getFileAndPaths();
}

void BBP::std::PATH::makeRelative(PATH &reference, PATH &dir)
{
	if (!dir.isRelativeToRoot())
	{
		dir.makeAbsolutePath(&reference);

		// Do not do this!
		//if (reference.isRelativeToRoot())
		//	dir.makeAbsolutePath(nullptr);
	}

}

bool BBP::std::PATH::isDirectory()
{
	// Check if file actually has something.
	return (rawPath[fileNameStartIndex + 2] == 0);
}

bool BBP::std::PATH::isFile()
{
	// Self explanatory lol
	return !isDirectory();
}

BBP::std::PATH &BBP::std::PATH::makeFile()
{
	// If already file, do nothing
	if (isFile())
		return *this;

	// Otherwise, remove '/' suffix
	std::printf("raw: %s\n", rawPath.static_data);

	return *this;
}

BBP::std::PATH &BBP::std::PATH::makeDirectory()
{
	// If already directory, do nothing.
	if (isDirectory())
		return *this;

	// Keep track of index, for nullptr.
	std::index_t totalLength = fileNameStartIndex;

	// Otherwise, move file back and append '/', then get whatever.
	for (std::index_t idx = fileNameStartIndex + 2; _path[idx] && idx < max_path_length - 2; idx++)
	{
		// Move back data
		_path.static_data[idx - 1] = _path[idx];

		// Increase totalLength
		totalLength++;
	}

	_path.static_data[totalLength + 1] = '/';
	_path.static_data[totalLength + 2] = 0;

	rawPath.static_data[totalLength + 1] = '/';
	rawPath.static_data[totalLength + 2] = 0;
	
	// I could probably get rid of this? just leave this here for now.
	getFileAndPaths();

	return *this;
}