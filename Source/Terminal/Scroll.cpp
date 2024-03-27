#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::ScrollBack(offset_t count)
{

}

void BBP::std::Terminal::TerminalApplication::ScrollForward(offset_t count)
{

}

void BBP::std::Terminal::TerminalApplication::ScrollUp(offset_t count)
{
	if (count >= state.viewportPosition.vertical)
		state.viewportPosition.vertical = 0;
	else
		state.viewportPosition.vertical -= count;
}

void BBP::std::Terminal::TerminalApplication::ScrollDown(offset_t count)
{
	// If count is more than current position, set count
	if (count >= state.lineCount - state.viewportPosition.vertical)
		state.viewportPosition.vertical = state.lineCount - 1;
	else
		state.viewportPosition.vertical += count;
}