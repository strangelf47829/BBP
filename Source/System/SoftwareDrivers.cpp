#include "../include/Kernel.h"


// Software
BBP::system::SoftwareHandle::SoftwareHandle(std::Stack<std::string_element> *input, std::Stack<std::string_element> *output, std::conststring name)
{
	// Check for valid input pointer
	InputBuffer = input;
	OutputBuffer = output;

	// Check if name is nullptr
	if (name == nullptr)
		return;

	// Get string size
	std::size_t nameLength = std::strlen(name);

	// If length is over permitted length, override name
	static_assert(std::static_length("<invalid name>") < maximumDeviceNameLength, "'maximumDeviceNameLength' is not big enough to store '<invalid name>'.");
	if (nameLength + 1 >= deviceName.dataSize)
		name = "<invalid name>";

	// Copy over name
	for (std::index_t nameIndex = 0; name[nameIndex]; nameIndex++)
		deviceName.data[nameIndex] = name[nameIndex];

}

void BBP::system::SoftwareHandle::setInputPage(BBP::std::Stack<std::string_element> *stk)
{
	InputBuffer = stk;
}

void BBP::system::SoftwareHandle::setOutputPage(BBP::std::Stack<std::string_element> *stk)
{
	OutputBuffer = stk;
}

BBP::std::size_t BBP::system::SoftwareHandle::available()
{
	// Check if valid input buffer
	if (InputBuffer == nullptr)
		return 0; // Nothing availale.

	return InputBuffer->atElement;
}

BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator<<(std::string_element b)
{
	// Check for valid output Buffer
	if (OutputBuffer == nullptr)
		return *this;	// Nothing output.

	// Just write that into output buffer.
	*OutputBuffer <<= b;

	return *this;
}

BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator>>(std::string_element &b)
{
	// Check for valid input Buffer
	if (InputBuffer == nullptr)
		return *this;	// Nothing output.

	// Just read that from input
	*InputBuffer >> b;

	return *this;
}
BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator<<(std::c_string b)
{
	// Check for valid output Buffer
	if (OutputBuffer == nullptr)
		return *this;	// Nothing output.

	// Get current position
	std::size_t prePosition = OutputBuffer->atElement;

	// Just write that into output buffer.
	*OutputBuffer <<= b;

	// Get position after write
	std::size_t postPosition = OutputBuffer->atElement;

	// Send that amount of stuff to hardware
	std::size_t bytesSent = BBP::system::getKernelInstance().getScreenDriver().hardwareDriver.send(postPosition - prePosition);

	// Move output buffer back that amount of spaces, plus one
	OutputBuffer->atElement -= (bytesSent + 1);

	return *this;
}

BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator>>(std::c_string b)
{
	// Check for valid input Buffer
	if (InputBuffer == nullptr || b == nullptr)
		return *this;	// Nothing output.

	// Just read that from input
	*InputBuffer >>= &b;

	return *this;
}



BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator<(std::string_element byte)
{
	// Check for valid input Buffer
	if (InputBuffer == nullptr)
		return *this;	// Nothing output.

	// Just write that into output buffer.
	*InputBuffer << byte;
	return *this;
}

BBP::system::SoftwareHandle &BBP::system::SoftwareHandle::operator>(std::string_element &byte)
{
	// Check for valid output Buffer
	if (OutputBuffer == nullptr)
		return *this;	// Nothing output.

	// Just write that into output buffer.
	*OutputBuffer >> byte;
	return *this;
}


