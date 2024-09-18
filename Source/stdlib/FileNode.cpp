#include "../include/FileSys.h"
#include "../include/Kernel.h"
#include "../include/Shell.h"

BBP::std::FileNode::FileNode()
	: allocator(nullptr),
	fileData((std::PAGE<std::string_element> *)nullptr, 0),
	filePath(),
	filePathHash(0)
{}

BBP::std::FileNode::FileNode(std::ResourceManager *res, std::conststring path)
	: allocator(res),
	fileData((std::PAGE<std::string_element> *)nullptr),
	filePath(path),
	filePathHash(0)
{
	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}

BBP::std::FileNode::FileNode(std::Stack<std::string_element> &data, std::conststring path)
	: allocator(nullptr),
	fileData(data),
	filePath(path),
	filePathHash(0)
{
	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}

BBP::std::FileNode::FileNode(std::ResourceManager *res, std::VOLUME *v, std::conststring path)
	: allocator(res),
	fileData((std::PAGE<std::string_element> *)nullptr, 0),
	filePath(v, path),
	filePathHash(0)
{
	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}

BBP::std::FileNode::FileNode(std::ResourceManager *res, std::size_t size, std::VOLUME *v, std::conststring path)
	: allocator(res),
	fileData(
		(std::PAGE<std::string_element> *)res->add_object(new std::PAGE<std::string_element>(size, (std::string_element *)res->calloc(size, sizeof(std::string_element)))),
		size),
	filePath(v, path),
	filePathHash(0)
{
	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}

BBP::std::FileNode::FileNode(std::ResourceManager *res, std::size_t size, std::PATH &path)
	: allocator(res),
	fileData(
		(std::PAGE<std::string_element> *)res->add_object(new std::PAGE<std::string_element>(size, (std::string_element *)res->calloc(size, sizeof(std::string_element)))),
		size),
	filePath(path),
	filePathHash(0)
{
	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}



BBP::std::FileNode::FileNode(std::Stack<std::string_element> &data, std::VOLUME *v, std::PATH &path)
	: allocator(nullptr),
	fileData(data),
	filePath(v, path.relName()),
	filePathHash(0)
{
	// Copy data
	fileData = data;

	filePath.makeAbsolutePath(&BBP::system::Shell::getWorkingDirectory());
	filePathHash = std::strhsh(filePath.relName());
}