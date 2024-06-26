#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::insertLine()
{

}

void BBP::std::Terminal::TerminalApplication::deleteLine()
{

}

void BBP::std::Terminal::TerminalApplication::extendLine()
{
	// Get currently active line
	std::w_string *activeLine = state.terminalLines[state.activePresentationPosition.vertical];

	// If null, insert line instead.
	if (activeLine == nullptr)
	{
		insertLine();
		return;
	}

	// Get last in sequence
	std::w_string *lastPage = &last(*activeLine);

	// Allocate new line
	std::w_string *newLine = state.allocator->add_object(new std::w_string);

	// Allocate string page
	state.allocator->page_calloc(*newLine, state.viewportSize.horizontal);

	// link newline
	newLine->prevPage = lastPage;
	lastPage->nextPage = newLine;
}

BBP::std::size_t BBP::std::Terminal::TerminalApplication::getCurrentLineLength()
{
	return seqlen(*state.terminalLines[state.activePresentationPosition.vertical]);
}