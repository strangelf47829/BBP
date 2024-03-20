#include "../include/Signal.h"
#include "../include/stdio.h"
#include "../include/Resources.h"
#include "../include/FileSys.h"
#include "../include/Executable.h"
#include "../include/CPPApplications.h"
#include "../include/Time.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/Kernel.h"
#include "../include/UEFI.h"
#include "../include/Daemon.h"

bool BBP::system::BBPEntryStub(BBP::system::UEFI *uefi)
{
	return BBP::system::getKernelInstance().BBPEntry(uefi);
}

// It's kinda funny putting an entire operating system in one 'OS.cpp' hahahahahahhahahahahhhaa

bool BBP::system::Kernel::BBPEntry(BBP::system::UEFI *uefi)
{
	// Set active context to kernel context
	setKernelContext();

	// Set active context memory
	setSystemProgmem();

	// Set system volume
	setSystemVolume(uefi->system.volumeLabel, uefi->system.volumePath);

	// Now switch to hypervisors
	setHypervisorContext(0);

	// Load hypervisors
	for (std::index_t index = 0; index < subsystems.hypervisors.dataSize; index++)
	{
		// Switch to hypervisor
		setHypervisorContext(index);

		// Set context progmem
		setSystemProgmem();

		// Allocate string
		std::static_string<32> pathName;
		
		// print into buffer
		std::sprintf(pathName.static_data, "/hyper/%d", index);

		// Set volume
		setSystemVolume('h', pathName.data);

	}

	// And set volume
	setSystemVolume('h', "/hyper/0/");

	// Configure root user
	root = &systemUsers[0];
	root->username = std::String("root");
	root->usernameHash = std::strhsh(root->username);
	root->password = std::String(uefi->system.rootPassword);
	root->passwordHash = std::strhsh(root->password);
	root->connectionMethod = std::String(uefi->system.deviceName);

	// Set active user to root
	activeUser = root;

	// Set kernel context
	setKernelContext();

	// Allocate a new proc frame, spawned from itself.
	system::procFrame *frame = allocateDaemonFrame(&system::DaemonRecord::records[0], std::String("shell"));

	// set working directory to frame stuff
	subsystems.activeContext->workingDirectory = &frame->processContext.workingDirectory;

	// Execute frame.
	std::errno_t result = 0;
	if (executeFrame(frame))
		result = getFrameResult(frame);

	return 0;

	/*

	//BBP::std::STDERR.writeTo = BBP::std::print_to_terminal;
	BBP::system::kernelSS()->activeContext->activemem = &BBP::system::kernelSS()->activeContext->progmem;

	for (BBP::std::index_t node = 0; node < BBP::std::max_open_files; node++)
		BBP::system::kernelSS()->activeContext->fileTable.data[node] = BBP::std::FileNode();

	BBP::std::VOLUME prim('v', "/mnt/v/");
	BBP::system::kernelSS()->activeContext->primaryVolume = &prim;

	BBP::std::PATH p("/home/");
	BBP::system::kernelSS()->activeContext->workingDirectory = &p;

	BBP::std::FILE f(BBP::system::kernelSS()->activeContext->STDOUT, "/proc/stdout");
	BBP::std::FILE f2(BBP::system::kernelSS()->activeContext->STDIN, "/proc/stdin");

	BBP::std::Driver::initializeGraphics(&BBP::system::kernelSS()->activeContext->display, 1000, 800);

	// Initialize system clock
	BBP::std::initClock();

	int magicCount = 4;

	BBP::std::PAGE<BBP::std::string_element> badapplePage(magicCount + sizeof(BBP::std::executable_main) / sizeof(BBP::std::string_element),
		(BBP::std::string_element *)BBP::system::kernelSS()->activeContext->activemem->malloc(magicCount * sizeof(BBP::std::string_element) + sizeof(BBP::std::executable_main)));
	BBP::std::Stack<BBP::std::string_element> badAppleStack(&badapplePage, badapplePage.dataSize);
	((BBP::std::executable_main *)badapplePage.raw)[0] = BBP::smile_main;

	badapplePage.raw[BBP_EXEC_MAGIC0IDX] = BBP_EXEC_MAGIC0;
	badapplePage.raw[BBP_EXEC_MAGIC1IDX] = BBP_EXEC_MAGIC1;
	badapplePage.raw[BBP_EXEC_MAGIC2IDX] = BBP_EXEC_MAGIC2;
	badapplePage.raw[BBP_EXEC_MAGIC3IDX] = BBP_EXEC_MAGIC3;

	BBP::std::FILE badappleExec(badAppleStack, "/proc/exec/elsa");

	try {
		BBP::std::execute(BBP::std::executables[0].entry, 0, nullptr);
	}
	catch (...)
	{

	}

	// Print error message if errno is not 0
	if (BBP::system::kernelSS()->activeContext->__errno && BBP::system::kernelSS()->activeContext->STDERR.atElement)
		BBP::system::kernelSS()->activeContext->STDOUT <<= BBP::system::kernelSS()->activeContext->STDERR.page->data;

	// Clean up
	f.close();
	f2.close();

	BBP::system::kernelSS()->activeContext->progmem.free(badapplePage.data);

	BBP::system::kernelSS()->activeContext->progmem.deleteAll();

	BBP::std::Driver::destructGraphics();
	BBP::std::vmem.dealloc();

	return BBP::system::kernelSS()->activeContext->__errno;
	*/
}