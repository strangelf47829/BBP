#include "../../../include/FileSystemDriver.h"
#include "../../../../include/drvcmd.h"
#include "../../../../include/Shell.h"
#include "Filedrvhdr.h"

// Reduces amount of characters needed to type
namespace FS = Host::Drivers::FileSystem;

// Amount of filesystem commands
constexpr BBP::std::size_t fileCMDsize = 14;

// List of filesystem commands and actions
BBP::system::HardwareCmd fileCMD[fileCMDsize] = { FS::initializeFilesystem, FS::deinitializeFilesystem, FS::setModeLoadPath, FS::queryFileMetadata, FS::getFileType, FS::openFile, FS::closeFile, FS::setReadMode, FS::setWriteMode, FS::pathInspect, FS::stepInspect, FS::emitNameInspect, FS::emitTypeInspect, FS::canStepInspect };
BBP::system::HardwareAction fileActions[3] = { FS::sendDataToFileSystem, FS::receiveDataFromFileSystem, FS::receiveFileMetadata };

void Host::Drivers::FileSystem::loadFileSystem(BBP::system::DeviceDriver &driver)
{
	// Set commands
	driver.hardwareDriver.setHandleData(fileActions, fileCMDsize, fileCMD);

	// Also load primary volume
	BBP::system::Shell::getPrimaryVolume().volumePath = "V:/";
}