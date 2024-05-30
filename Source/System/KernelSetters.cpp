#include "../include/Kernel.h"
#include "../include/Daemon.h"

BBP::system::KernelSubSystems &BBP::system::Kernel::SubSystems()
{
	return subsystems;
}

void BBP::system::Kernel::setKernelContext()
{
	subsystems.activeContext = subsystems.getKernelSystemContext();
}

void BBP::system::Kernel::setSystemVolume(std::VOLUME *volume)
{
	subsystems.activeContext->primaryVolume = volume;
}

void BBP::system::Kernel::setSystemVolume(std::string_element label, std::conststring path)
{
	subsystems.activeContext->contextVolume.mount(label, path);
	subsystems.activeContext->primaryVolume = &subsystems.activeContext->contextVolume;
}

void BBP::system::Kernel::configureRoot(EFI *EFI)
{
	if (rootConfigured)
		return;

	rootConfigured = true;
	root = &systemUsers[0];
	root->username = std::String("kangel");
	root->usernameHash = std::strhsh(root->username);
	root->password = std::String(EFI->system.rootPassword);
	root->passwordHash = std::strhsh(root->password);
	root->connectionMethod = std::String(EFI->system.deviceName);

	// Set active user to root
	activeUser = root;
}

BBP::system::pid_t BBP::system::Kernel::getNextDaemonPID()
{
	return pidCount++;
}

BBP::system::pid_t BBP::system::Kernel::getNextBinPID()
{
	return pidCount++;
}

BBP::system::pid_t BBP::system::Kernel::getNextShellPID()
{
	return pidCount++;
}

BBP::std::errno_t BBP::system::Kernel::getFrameResult(procFrame *)
{
	return 0;
}

BBP::system::UserInformation &BBP::system::Kernel::getActiveUser()
{
	return *activeUser;
}

void BBP::system::Kernel::setHypervisorContext(std::index_t idx)
{
	// Check for valid context
	if (idx >= subsystems.hypervisors.dataSize)
		std::raise(std::SIGSEGV);

	// Now set active context
	subsystems.activeContext = &subsystems.hypervisors[idx].context;

}


BBP::system::BootRecord::bootRecordEntryPoint BBP::system::Kernel::getBBPEntryPoint()
{
	return BBPEntryStub;
}

BBP::std::errno_t BBP::system::Kernel::Entry(EFI &configuration, BootRecord::bootRecordEntryPoint entry)
{
	// Yolo barebone that shiiiiit
	return entry(&configuration);
}

void BBP::system::Kernel::allocateExternalDrivers(std::size_t)
{

}

void BBP::system::Kernel::setSystemProgmem()
{
	subsystems.activeContext->activemem = &subsystems.activeContext->progmem;
}

BBP::system::DeviceDriver &BBP::system::Kernel::getNextAvailableDriver()
{

}

BBP::system::DeviceDriver &BBP::system::Kernel::getFileDriver()
{
	return fileDriver;
}

BBP::system::DeviceDriver &BBP::system::Kernel::getKeyboardDriver()
{
	return keyboardDriver;
}

BBP::system::DeviceDriver &BBP::system::Kernel::getScreenDriver()
{
	return screenDriver;
}

BBP::system::DeviceDriver &BBP::system::Kernel::getSystemDriver()
{
	return systemDriver;
}