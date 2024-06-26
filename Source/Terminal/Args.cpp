#include "../include/TerminalController.h"
#include "../include/stdint.h"

void BBP::std::Terminal::TerminalApplication::pushArgument(word argument)
{
	// If at max arguments, do nothing
	if (atArgument >= maxArguments)
		return;

	// Otherwise write reversed argument
	args[atArgument++] = reverse(argument);
}

BBP::std::size_t BBP::std::Terminal::TerminalApplication::getArgumentCount()
{
	return atArgument;
}

BBP::std::word BBP::std::Terminal::TerminalApplication::getArgument(index_t idx)
{
	// If out of bounds, return 0
	if (idx >= maxArguments)
		return 0;

	// Otherwise return arg
	return args[idx];
}

void BBP::std::Terminal::TerminalApplication::pushIntermediateByte(word)
{

}

void BBP::std::Terminal::TerminalApplication::getIntermediateBytes(string &)
{

}
