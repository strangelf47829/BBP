#include "../include/Kernel.h"

BBP::system::BootRecord::BootRecord(bool (*check)(), bootRecordEntryPoint entry, std::conststring name)
{
	// Initialize values
	this->isBootRecordAvailable = check;
	this->entryPoint = entry;

	// Check for name
	if (name == nullptr)
		return;

	// Get name length
	std::size_t nameLength = std::strlen(name);
	
	// Clamp name length if needed
	if (nameLength >= recordName.dataSize)
		nameLength = recordName.dataSize;

	// Copy name
	std::strcpy(recordName.data, name);
}