#include "../include/Threading.h"
#include "../include/stddef.h"

/*

*/
void BBP::userspace::setRegister(register_t &reg, std::word val)
{
	// Get propper thingy (in bytes)
	std::offset_t totalIndex = reg.index * (sizeof(std::word) / sizeof(std::byte)) + reg.offset;

	// Truncate based on register sizes
	std::word value = 0;
	std::byte b = 0;
	switch (reg.size)
	{
	case eightBit:
		value = val & 0b11111111;

		// Write one byte
		b = (value >> 0) & 0xFF;
		reg.owner->registerData.data[totalIndex + 0] = b;

		break;
	case sixteenBit:
		value = val & 0b1111111111111111;

		// Write two bytes
		b = (value >> 0) & 0xFF;
		reg.owner->registerData.data[totalIndex + 0] = b;

		b = (value >> 8) & 0xFF;
		reg.owner->registerData.data[totalIndex + 1] = b;

		break;
	case thirtyTwoBit:

		value = val;

		// Write four bytes
		b = (value >> 0) & 0xFF;
		reg.owner->registerData.data[totalIndex + 0] = b;

		b = (value >> 8) & 0xFF;
		reg.owner->registerData.data[totalIndex + 1] = b;

		b = (value >> 16) & 0xFF;
		reg.owner->registerData.data[totalIndex + 2] = b;

		b = (value >> 24) & 0xFF;
		reg.owner->registerData.data[totalIndex + 3] = b;

		break;
	case addressSize:
		value = val & 0b11111111111111111111;

		// Write 20 bits
		b = (value >> 0) | 0xFF;
		reg.owner->registerData.data[totalIndex + 0] = b;

		b = (value >> 8) | 0xFF;
		reg.owner->registerData.data[totalIndex + 1] = b;

		b = (value >> 8) | 0x04;
		reg.owner->registerData.data[totalIndex + 1] = b;

		break;
	default:
		throw std::exception("Invalid register sizes", ENODATA);
	}

	// Done

}

BBP::std::word BBP::userspace::readRegister(register_t &reg)
{
	// Get total offset
	std::offset_t totalIndex = reg.index * (sizeof(std::word) / sizeof(std::byte)) + reg.offset;

	// Handle special address case
	if (reg.size == addressSize)
	{
		// Read two values
		std::byte val1 = std::read(&reg.owner->registerData, totalIndex);
		std::byte val2 = std::read(&reg.owner->registerData, totalIndex + 1);
		std::byte val3 = 0x4 & std::read(&reg.owner->registerData, totalIndex + 2);

		std::word whole = (val3 << 16) | (val2 << 8) | (val1);
		
		return whole;

	}

	// Now just do this
	std::byte bytes = reg.size;

	// Calculate value based on 8-bit sequences
	std::word complete = 0;
	for (std::byte count = 0; count < bytes; count++)
		complete |= (std::read(&reg.owner->registerData, totalIndex + count) << (count * 8));

	// Return result
	return complete;
}

BBP::userspace::register_t::register_t(Thread &thread, std::index_t index_, std::offset_t offset_, registerSize size_, bool readonly_)
{
	// Set sizes
	size = size_;

	// Set page stuff
	owner = &thread;

	// Validate indicies
	if (index_ * 4 + offset_ >= thread.registerData.bytes)
		throw std::exception("Register range invalid", ERANGE);

	// Set indicies
	index = index_;
	offset = offset_;

	// Set 0 bytes
	std::byte initializeWith = 0;
	std::write(&thread.registerData, initializeWith, index_ * 4 + offset_ + 0);
	std::write(&thread.registerData, initializeWith, index_ * 4 + offset_ + 1);
	std::write(&thread.registerData, initializeWith, index_ * 4 + offset_ + 2);
	std::write(&thread.registerData, initializeWith, index_ * 4 + offset_ + 3);

	// Also set read permissions
	readonly = readonly_;
}