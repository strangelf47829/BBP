#include "../include/StackTrace.h"
#include "../include/FileSys.h"

BBP::std::stack_trace_db::stack_trace_db()
{
	hdr = std::FILE("/mnt/v/lib/memmaphdr");
}

BBP::std::stack_trace_db::~stack_trace_db()
{
	hdr.close();
}

void BBP::std::stack_trace_db::printName()
{
	// If any invalid flags are set, set to ??
	if (binIndex == -1 || functionStart == -1 || functionOffset == -1 || line == -1)
		std::strcpy(name.data, "??");

	std::printf("%s<+0x%04x>", name.data, functionOffset);
}

void BBP::std::stack_trace_db::lineLookup()
{
	// If any invalid flags are set, set to ??
	if (binIndex == -1 || functionStart == -1 || functionOffset == -1 || line == -1)
	{
		std::strcpy(name.data, "??");
		return;
	}

	// Create buffer
	std::static_string<32> buffer;

	// write path to header info
	std::sprintf(buffer.data, "/mnt/v/lib/memmap.%d", binIndex);

	// Open file
	std::FILE lines(buffer.data);

	// Count amount of lines
	std::index_t atLine = 0;

	// Count where in the file we are
	std::index_t fileIdx = 0;

	// While atLine is not line
	while (atLine < line)
	{
		// Get a character
		std::string_element c = lines.b()[fileIdx++];

		// If newline, increase atLine
		if (c == '\n')
			atLine++;
	}

	// Where we are in the name
	std::index_t nameIdx = 0;

	// Now we are at line. Keep copying data until newline hit again
	while (nameIdx < name.dataSize)
	{
		// Get a character
		std::string_element c = lines.b()[fileIdx++];

		// If newline, write null terminator and break
		if (c == '\n')
		{
			name[nameIdx] = 0;
			break;
		}

		// Copy
		name[nameIdx++] = c;
	}

	// Close file
	lines.close();
}

void BBP::std::stack_trace_db::binLookup(std::address_t address)
{
	// Create two buffers, one for addresses and the other for size
	std::static_string<7> addressBuffer;
	std::static_string<5> sizeBuffer;

	// Keep track of where in the file we are
	std::index_t fileIdx = 0;

	// Keep track of the line number
	std::index_t lineNum = 0;

	// File size
	std::size_t fileSize = hdr.b().max_elements;

	// Everytime read a line
	while (fileIdx < fileSize)
	{
		// Read 6 characters into addressBuffer
		for (std::index_t idx = 0; idx < 6; idx++)
			addressBuffer[idx] = hdr.b()[fileIdx++];

		// Skip comma
		fileIdx++;

		// Read 4 characters into sizeBuffer
		for (std::index_t idx = 0; idx < 4; idx++)
			sizeBuffer[idx] = hdr.b()[fileIdx++];

		// Set terminating nulls
		addressBuffer[6] = 0;
		sizeBuffer[4] = 0;

		// Convert those into actual addresses and sizes
		std::address_t funcAddress = 0;
		std::size_t funcSize = 0;

		// Set radix
		std::word radix = 16 * 16 * 16 * 16 * 16;

		// Loop 6 times
		for (std::index_t idx = 0; idx < 6; idx++)
		{
			// Get nth character from address (and size, if possible)
			std::string_element addressnth = addressBuffer[idx];
			std::string_element sizenth = (idx > 1 ? sizeBuffer[idx - 2] : 0);

			// Now parse that into a number
			std::byte addressSemiNum = addressnth - '0';
			std::byte sizeSemiNum = sizenth - '0';

			// Check address for hexadecimal
			if (addressnth >= 'a' && addressnth <= 'f')
				addressSemiNum = addressnth - 'a' + 10;

			// Check size for hexadecimal
			if (sizenth >= 'a' && sizenth <= 'f')
				sizeSemiNum = sizenth - 'a' + 10;
			
			// Add up numbers
			funcAddress += addressSemiNum * radix;
			funcSize += sizeSemiNum * radix;

			// Divide
			radix /= 16;
		}

		// Make sure funcSize is restricted to 2 bytes
		funcSize &= 0xFFFF;
		
		// Skip question mark
		fileIdx++;

		// Get upper and lower bound
		std::address_t lowerBound = funcAddress;
		std::address_t upperBound = funcAddress + funcSize;

		// If not within these bounds, continue
		if (address >= upperBound || address < lowerBound)
		{
			// Go until new line
			while (hdr.b()[fileIdx++] != '\n' && fileIdx < fileSize);

			// Increase line number
			lineNum++;

			// Then continue
			continue;
		}

		// Match. Get bin number
		std::string_element binNC0 = hdr.b()[fileIdx++];
		std::string_element binNC1 = hdr.b()[fileIdx++];

		// Declare
		std::halfword binIndex = 0;

		// If binNC1 is a newline, this is trivial. Otherwise less so, but still easy
		if (binNC1 == '\n')
			binIndex = binNC0 - '0';
		else
			binIndex = 10 * (binNC0 - '0') + (binNC1 - '0');

		// Now calculate the index in the file itself
		std::index_t nameIndex = lineNum - (binIndex * 75) + !!binIndex;

		// Set information
		this->binIndex = binIndex;
		this->functionStart = lowerBound;
		this->functionOffset = address - lowerBound;
		this->line = nameIndex;

		return;

	}

	// Nothing found. Set information
	this->binIndex = -1;
	this->functionStart = -1;
	this->functionOffset = -1;
	this->line = -1;

}