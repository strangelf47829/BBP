#include "../include/ELSA/BinaryApplication.h"

// ELFIdent constructor
BBP::ELF::ELFIdent::ELFIdent(std::PAGE<std::byte> *headerData)
	: data(0, nullptr)
{
	// Set next page
	data.nextPage = headerData;
}

// Read data from 'data'. Return false if invalid data
bool BBP::ELF::ELFIdent::readData()
{
	// Fifth byte identifies 32bit or 64bit. 0: invalid, 1: 32bit, 2: 64bit
	switch (data[4])
	{
	default: // Invalid
		return false;

	case 1: // 32 Bit
		thirtyTwoBit = true;
		break;

	case 2: // 64 bit
		thirtyTwoBit = false;
		break;
	}

	// Sixth bit identifies endiannes. 0: invalid, 1: little endian, 2: big endian
	switch (data[5])
	{
	default: // Invalid
		return false;

	case 1: // little endian
		littleEndian = true;
		break;

	case 2: // big endian
		littleEndian = false;
		break;
	}

	// Seventh bit identifies version. Must equal 1
	if (data[6] != 1)
		return false;
	
	// Read ABI
	ABI = (ELFABI) data[7];

	// Check rest of the bytes are zero
	for (std::byte idx = 8; idx < 16; idx++)
		if (data[idx])
			return false;

	// Return true (successfully read data)
	return true;
}

// Write data from fields into array
void BBP::ELF::ELFIdent::writeData()
{
	// Write magic data
	data[0] = 0x7f;
	data[1] = 'E';
	data[2] = 'L';
	data[3] = 'F';

	// Then save class information
	data[4] = (thirtyTwoBit ? 1 : 2);

	// Then save endianness
	data[5] = (littleEndian ? 1 : 2);

	// Then save version info (1)
	data[6] = 1;

	// Then save ABI type
	data[7] = ABI;

	// Then save ABI version
	data[8] = ABIVersion;

	// Then pad remaining 8 bytes
	for (std::index_t idx = 9; idx < 16; idx++)
		data[idx] = 0;
}