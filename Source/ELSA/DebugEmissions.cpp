#include "../include/ELSA/DebugUtils.h"
#include "../include/stdio.h"

void BBP::elsa::Debug_db::EmitDiagnostic()
{
	std::printf("Something went wrong.\n");
}

void BBP::elsa::Debug_db::emitHeader(DebugEmissionSource_t &source)
{
	// Check if file exists or not
	if (std::strlen(source.inputName) == 0)
	{
		// Emit internal diagnostic
		EmitDiagnostic();
		return;
	}
	
	// Create static buffer for header.
	std::static_string<256> buffer;

	// Now 
	std::sprintf(buffer.data, "%s:", source.inputName.data);

	// Print stuff
	std::printf("");
}