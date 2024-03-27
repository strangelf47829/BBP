#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::calculateViewportHorizontal()
{
	// Get horizontal count
	size_t coloumnCount = state.viewportSize.horizontal;

	// If this is 0, return
	if (coloumnCount == 0)
		return;

	// Set page index to active presentation position divided by coloumnCount
	state.viewportPosition.horizontal = state.activePresentationPosition.horizontal / coloumnCount;

}

void BBP::std::Terminal::TerminalApplication::calculateViewportVertical()
{
	// get row count
	size_t rowCount = state.viewportSize.vertical;

	// If it is 0, return.
	if (rowCount == 0)
		return;

	// Get amount of lines in current page
	size_t excessRowCount = state.activePresentationPosition.vertical % rowCount;

	// Subtract excess row count from vertical positioning
	state.viewportPosition.vertical = state.activePresentationPosition.vertical - excessRowCount;
}

void BBP::std::Terminal::TerminalApplication::calculateViewport()
{
	// Calculate both vertical and horizontal positions
	calculateViewportHorizontal();
	calculateViewportVertical();
}