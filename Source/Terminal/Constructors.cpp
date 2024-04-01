#include "../include/TerminalController.h"

BBP::std::Terminal::TerminalState::TerminalState(size_t lc, size_t vp_w, size_t vp_h, ResourceManager *man)
{
	// Set line count
	lineCount = lc;

	// Set allocator
	allocator = man;

	// Allocate data to first line
	allocator->page_calloc(firstLine, vp_w);

	// Allocate amount of lines to page
	allocator->page_calloc(terminalLines, lineCount);

	// Create pointer to a null thingy
	std::string *activeLine = &firstLine;

	// If line count, write activeLine to index 0
	if (lineCount)
		write(&terminalLines, activeLine, 0);

	// Set viewport information
	viewportSize = { vp_h, vp_w };
	tabularSettings = {4, 4};

	// Allocate more lines if needed
	for (std::index_t lineIndex = 1; lineIndex < lineCount; lineIndex++)
	{
		// Allocate new object for active line
		std::string *newLine = allocator->add_object(new std::string());

		// Allocate bytes to activeLine
		allocator->page_calloc(*newLine, vp_w);

		// Add line to terminalLines
		write(&terminalLines, newLine, lineIndex);

		// Set new active line
		activeLine = newLine;
	}

}

BBP::std::Terminal::TerminalApplication::TerminalApplication(TerminalState::TerminalRect vp_size, size_t lc, ResourceManager *manager, TerminalDevice *dev)
	: state(lc, vp_size.horizontal, vp_size.vertical, manager),
	stringStack(&activeString, 255)
{
	device = dev;

	state.implicitCR = true;
}