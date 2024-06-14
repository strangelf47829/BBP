#include "../include/EFI.h"
#include "../include/FirmwareInterface.h"
#include "../env/include/Bootloader.h"
#include "../env/include/EFIConfig.h"

int main(int argc, char **argv)
{

	// Get reference to BIOS
	BBP::BIOS *bios = Host::getBIOS();

	// Initialize BIOS
	bios->init();

	// DEBUG - create EFI record
	BBP::system::EFI record;

	// Configure
	Host::configure(bios, record);

	// Set active efi
	Host::setActiveInterface(record);

	// Create firmware interface
	BBP::FirmwareInterface Hardware(bios, &record);

	// Jump into bios space (with configured record);
	bios->boot(record, Hardware);

	// Deinitialize BIOS
	bios->fini();
}