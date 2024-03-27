#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::MoveBack(offset_t count)
{
	// If count is more than current position, set count
	if (count >= state.activePresentationPosition.horizontal)
		state.activePresentationPosition.horizontal = 0;
	else
		state.activePresentationPosition.horizontal -= count;

	// Calculate horizontal page stuff
	if (state.implicitScroll)
		calculateViewportHorizontal();
}

void BBP::std::Terminal::TerminalApplication::MoveForward(offset_t count)
{
	// Get line length
	std::size_t lineLength = getCurrentLineLength();

	// If count + current position exceeds line length, set position to line length
	if (count + state.activePresentationPosition.horizontal >= lineLength)
		state.activePresentationPosition.horizontal = lineLength - 1;
	else
		state.activePresentationPosition.horizontal += count;

	// Calculate horizontal page stuff
	if (state.implicitScroll)
		calculateViewportHorizontal();
}

void BBP::std::Terminal::TerminalApplication::MoveUp(offset_t count)
{
	// If count is more than current position, set count
	if (count >= state.activePresentationPosition.vertical)
		state.activePresentationPosition.vertical = 0;
	else
		state.activePresentationPosition.vertical -= count;

	// If vertical is now above page, set viewport to vertical
	if (state.activePresentationPosition.vertical < state.viewportPosition.vertical && state.implicitScroll)
		ScrollUp(1);
}

void BBP::std::Terminal::TerminalApplication::MoveDown(offset_t count)
{
	// If count is more than current position, set count
	if (count >= state.lineCount - state.activePresentationPosition.vertical)
		state.activePresentationPosition.vertical = state.lineCount - 1;
	else
		state.activePresentationPosition.vertical += count;

	// If vertical is now below page, increment viewport by one
	if (state.activePresentationPosition.vertical >= state.viewportPosition.vertical + state.viewportSize.vertical && state.implicitScroll)
		ScrollDown(1);
}