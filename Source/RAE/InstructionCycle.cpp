#include "../include/Threading.h"
#include "../include/stddef.h"
#include "../include/Hyperv.h"
#include "../include/stdio.h"

void BBP::userspace::HyperVisor::FetchActiveThreadInstruction()
{
	// Check if active thread is within bounds
	if (activeThread >= threadCount)
	{
		// Signal Processor fault
		__SIGNAL__(SIGINV);
	}

	// Get active thread
	Thread *currentThread = &threads.static_data[activeThread];

	// If thread is inactive, raise signal
	if (currentThread->active == false)
	{
		__SIGNAL__(SIGTHREXP);
	}

	// Read program counter (eip) and convert from virtual memory to physical memory
	std::word programCounter = currentThread->executable.virtualToPhysical(readRegister(currentThread->eip));

	// Clear instruction
	currentThread->instruction = userspace::Instruction();
	
	try
	{
		// Decode word
		currentThread->instruction.header.prefix = std::read(&currentThread->executable.BinaryData, programCounter + 0);
		currentThread->instruction.header.opcode = std::read(&currentThread->executable.BinaryData, programCounter + 1);
		currentThread->instruction.header.suffix = std::read(&currentThread->executable.BinaryData, programCounter + 2);

		// Decode last eight bits
		std::byte flagsAndArgs = std::read(&currentThread->executable.BinaryData, programCounter + 3);

		currentThread->instruction.header.args = flagsAndArgs & 0x0F;
		currentThread->instruction.header.flags = (flagsAndArgs & 0xF0) >> 4;

		// Now for arguments.
		std::word executableIndex = programCounter + 4;
		for (std::index_t argumentIndex = 0; argumentIndex < currentThread->instruction.header.args; argumentIndex++)
		{
			// Decide if argument is a literal
			bool isLiteral = !(currentThread->instruction.header.prefix & (1 << argumentIndex));

			// Decode word
			std::word decoded = 0;
			decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 0) << 24;
			decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 1) << 16;
			decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 2) <<  8;
			decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 3) <<  0;
			executableIndex += 4;

			// If literal, set argument and stop
			if (isLiteral)
			{
				currentThread->instruction.args[argumentIndex].data = decoded;
				currentThread->instruction.args[argumentIndex].isLiteralArg = true;

				continue;
			}

			// Get decoded or referenced 
			currentThread->instruction.args[argumentIndex].referenced   = !!((decoded >> 24) & 0b10000000);
			currentThread->instruction.args[argumentIndex].dereferenced = !!((decoded >> 24) & 0b01000000);

			// Next, get offset and trailer count
			currentThread->instruction.args[argumentIndex].offset = ((decoded >> 24) & 0b00111100) >> 2;
			currentThread->instruction.args[argumentIndex].trailerCount = ((decoded >> 24) & 0b0000011);
			
			// Save 20 bits of decoded into args
			currentThread->instruction.args[argumentIndex].data = decoded & 0xfffff;

			// Now decode trailers
			for (std::nibble trailerIndex = 0; trailerIndex < currentThread->instruction.args[argumentIndex].trailerCount && trailerIndex < 4; trailerIndex++)
			{
				// Decode next word
				decoded = 0;
				decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 0) << 24;
				decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 1) << 16;
				decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 2) << 8;
				decoded |= std::read(&currentThread->executable.BinaryData, executableIndex + 3) << 0;
				executableIndex += 4;
				
				// Save
				currentThread->instruction.args[argumentIndex].trailers[trailerIndex] = decoded;
			}

		}

		// Now that the instruction has been decoded, set eip to executableIndex
		setRegister(currentThread->eip, currentThread->executable.physicalToVirtual(executableIndex));

	}
	catch (const std::except &e)
	{
		// Doesn't really matter what happens, just signal fault
		__SIGNAL__(SIGTHREXP);
	}


}


void BBP::userspace::Thread::LogInstruction()
{
	// Print header
	std::printf("Instruction: 0x%02x (0x%02x), %u args.\n", instruction.header.opcode, instruction.header.suffix, instruction.header.args);

	for (std::byte i = 0; i < instruction.header.args; i++)
	{
		// Print argument number
		std::printf("\tArg %u:\t", i);

		// Print argument type
		if (!((instruction.header.prefix >> i) & 1))
		{
			std::printf("(Literal):\t0x%08x\n", instruction.args[i].data);
			continue;
		}
		else
			std::printf("(Reference)\t");

		// Check for referenced or dereferenced
		if (instruction.args[i].dereferenced)
			std::printf("*");

		if (instruction.args[i].referenced)
			std::printf("&");

		if (instruction.args[i].dereferenced && instruction.args[i].referenced)
			std::printf("\e[0;31mILLEGAL\e[0;37m");

		std::printf("\t");

		switch (instruction.args[i].offset)
		{
		case 0b00: // Starts at beginning of this instance
			std::printf("Rel: Instance\t");
			break;

		case 0b01: // Starts at beginning of whole chain
			std::printf("Rel: Chain\t");
			break;

		case 0b10: // Starts at beginning of shared user memory
			std::printf("Rel: User\t");
			break;

		case 0b11: // Starts at beginning of shared system memory
			std::printf("Rel: System\t");
			break;
		}

		std::printf("Data: 0x%05x\t", instruction.args[i].data);
		std::printf("Page: 0x%01x\t", instruction.args[i].data >> 16);
		std::printf("Address: 0x%05x\n", instruction.args[i].data & 0xFFFF);

		std::printf("\t\tTrailers: %u\n", instruction.args[i].trailerCount);

		for (std::byte k = 0; k < instruction.args[i].trailerCount; k++)
			std::printf("\t\tTrailer %u: 0x%08x\n", k, instruction.args[i].trailers[k]);

	}

	std::printf("\n");

}