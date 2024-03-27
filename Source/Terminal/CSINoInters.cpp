#include "../include/TerminalController.h"

void BBP::std::Terminal::TerminalApplication::ICH_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::CUU_CSINo()	// (1 arg) (default: 1) move up
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Move up n times
	MoveUp(arg);
}

void BBP::std::Terminal::TerminalApplication::CUD_CSINo()	// (1 arg) (default: 1) move down
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Move down n times
	MoveDown(arg);
}

void BBP::std::Terminal::TerminalApplication::CUF_CSINo()	// (1 arg) (default: 1) move forward
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Move forward n times
	MoveForward(arg);
}

void BBP::std::Terminal::TerminalApplication::CUB_CSINo()	// (1 arg) (default: 1) move back
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Move back n times
	MoveBack(arg);
}

void BBP::std::Terminal::TerminalApplication::CNL_CSINo()	// (1 arg) (default: 1) move n lines down, then home
{
	// Scroll down
	CUD_CSINo();

	// Set horizontal home
	setHorizontalHome();
}

void BBP::std::Terminal::TerminalApplication::CPL_CSINo()	// (1 arg) (default: 1) move n lines up, then home
{
	// Scroll up
	CUU_CSINo();

	// Set horizontal home
	setHorizontalHome();
}

void BBP::std::Terminal::TerminalApplication::CHA_CSINo()	// (1 arg) (default: 1) move to nth position on line
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Check if line has that position
	size_t lineLength = getCurrentLineLength();

	// If requested position is more than line length, do nothing
	if (arg > lineLength)
		return;

	// Set cursor position
	state.activePresentationPosition.horizontal = arg - 1;
}

void BBP::std::Terminal::TerminalApplication::CUP_CSINo()	// (2 arg) (default: 1,1) move cursor to n,m. 
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word argHorizontal = 1;
	word argVertical = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		argHorizontal = getArgument(0);

	// If this is equal to or greater than 2, get second arg
	if (argCount >= 2)
		argVertical = getArgument(1);

	// Check if line has that position
	size_t lineLength = getCurrentLineLength();

	// If requested position is more than line length, do nothing
	if (argHorizontal <= lineLength)
		state.activePresentationPosition.horizontal = argHorizontal - 1;

	// If has lines, set lines
	if (argVertical <= state.lineCount)
		state.activePresentationPosition.horizontal = argVertical - 1;
}

void BBP::std::Terminal::TerminalApplication::CHT_CSINo()	// (1 arg) (default: 1) move n tabs forward
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Tab that many times
	for (std::index_t count = 0; count < arg; arg++)
		HT_C0();
}

void BBP::std::Terminal::TerminalApplication::ED_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::EL_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::IL_CSINo()	// (1 arg) (default: 1) insert n lines
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		insertLine();
}

void BBP::std::Terminal::TerminalApplication::DL_CSINo()	// (1 arg) (default: 1) delete n lines
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		deleteLine();
}

void BBP::std::Terminal::TerminalApplication::EF_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::EA_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::DCH_CSINo()	// (1 arg) (default: 1) Delete a character
{

}

void BBP::std::Terminal::TerminalApplication::SSE_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::CPR_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SU_CSINo()	// (1 arg) (default: 1) Scroll up n lines
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Scroll up n times
	ScrollUp(arg);
}

void BBP::std::Terminal::TerminalApplication::SD_CSINo()	// (1 arg) (default: 1) Scroll down n lines
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Scroll up n times
	ScrollDown(arg);
}

void BBP::std::Terminal::TerminalApplication::NP_CSINo()	// (1 arg) (default: 1) Scroll n pages down
{

}

void BBP::std::Terminal::TerminalApplication::PP_CSINo()	// (1 arg) (default: 1) Scroll n pages up
{

}

void BBP::std::Terminal::TerminalApplication::CTC_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::ECH_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::CVT_CSINo()	// (1 arg) (default: 1) Vertical tab n times
{

}

void BBP::std::Terminal::TerminalApplication::CBT_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SRS_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::PTX_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SDS_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SIMD_CSINo()	// (1 arg) (default: 1) set SIMD
{
	// Get arguments
	size_t argCount = getArgumentCount();
	word arg = 1;

	// If this is equal to or greater than 1, get first arg
	if (argCount >= 1)
		arg = getArgument(0);

	// Set SIMD
	state.SIMD = arg;
}

void BBP::std::Terminal::TerminalApplication::HPA_CSINo()	// (1 arg) (default: 1) set absolute position to n
{

}

void BBP::std::Terminal::TerminalApplication::HPR_CSINo()	// (1 arg) (default: 1) move n positions forward
{

}

void BBP::std::Terminal::TerminalApplication::REP_CSINo()	// (1 arg) (default: 1) repeat past character n times
{

}

void BBP::std::Terminal::TerminalApplication::DA_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::VPA_CSINo()	// (1 arg) (default: 1) Set absolute line to n
{

}

void BBP::std::Terminal::TerminalApplication::VPR_CSINo()	// (1 arg) (default: 1) move n lines forward
{

}

void BBP::std::Terminal::TerminalApplication::HVP_CSINo()	// (2args) (default:1,1) move n chars forward, m lines forward
{

}

void BBP::std::Terminal::TerminalApplication::TBC_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SM_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::MC_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::HPB_CSINo()	// (1 arg) (default: 1) move back n positions
{

}

void BBP::std::Terminal::TerminalApplication::VPB_CSINo()	// (1 arg) (default: 1) move forward n positions
{

}

void BBP::std::Terminal::TerminalApplication::RM_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::SGR_CSINo()	// Set graphics rendition
{
	// Do action based on arguments
	size_t argCount = getArgumentCount();

	// For arguments
	for (std::index_t argIndex = 0; argIndex < argCount; argIndex++)
	{
		// Get argument
		word argument = getArgument(argIndex);

		// Do command
		SGR_CMD(argument);
	}
}

void BBP::std::Terminal::TerminalApplication::DSR_CSINo()	// No-op
{

}

void BBP::std::Terminal::TerminalApplication::DAQ_CSINo()	// No-op
{

}


void BBP::std::Terminal::TerminalApplication::CSIActionNoInters(CC finalByte)
{
	// convert finalByte to enum
	CSI_F_NOINTER action = (CSI_F_NOINTER)finalByte;

	// switch based on action
	switch (action)
	{
	case ICH:
		ICH_CSINo();
		break;
	case CUU:
		CUU_CSINo();
		break;
	case CUD:
		CUD_CSINo();
		break;
	case CUF:
		CUF_CSINo();
		break;
	case CUB:
		CUB_CSINo();
		break;
	case CNL:
		CNL_CSINo();
		break;
	case CPL:
		CPL_CSINo();
		break;
	case CHA:
		CHA_CSINo();
		break;
	case CUP:
		CUP_CSINo();
		break;
	case CHT:
		CHT_CSINo();
		break;
	case ED: 
		ED_CSINo();
		break;
	case EL: 
		EL_CSINo();
		break;
	case IL: 
		IL_CSINo();
		break;
	case DL: 
		DL_CSINo();
		break;
	case EF: 
		EF_CSINo();
		break;
	case EA: 
		EA_CSINo();
		break;
	case DCH:
		DCH_CSINo();
		break;
	case SSE:
		SSE_CSINo();
		break;
	case CPR:
		CPR_CSINo();
		break;
	case SU: 
		SU_CSINo();
		break;
	case SD: 
		SD_CSINo();
		break;
	case NP: 
		NP_CSINo();
		break;
	case PP: 
		PP_CSINo();
		break;
	case CTC:
		CTC_CSINo();
		break;
	case ECH:
		ECH_CSINo();
		break;
	case CVT:
		CVT_CSINo();
		break;
	case CBT:
		CBT_CSINo();
		break;
	case SRS:
		SRS_CSINo();
		break;
	case PTX:
		PTX_CSINo();
		break;
	case SDS:
		SDS_CSINo();
		break;
	case SIMD:
		SIMD_CSINo();
		break;

	case HPA:
		HPA_CSINo();
		break;
	case HPR:
		HPR_CSINo();
		break;
	case REP:
		REP_CSINo();
		break;
	case DA: 
		DA_CSINo();
		break;
	case VPA:
		VPA_CSINo();
		break;
	case VPR:
		VPR_CSINo();
		break;
	case HVP:
		HVP_CSINo();
		break;
	case TBC:
		TBC_CSINo();
		break;
	case SM: 
		SM_CSINo();
		break;
	case MC: 
		MC_CSINo();
		break;
	case HPB:
		HPB_CSINo();
		break;
	case VPB:
		VPB_CSINo();
		break;
	case RM: 
		RM_CSINo();
		break;
	case SGR:
		SGR_CSINo();
		break;
	case DSR:
		DSR_CSINo();
		break;
	case DAQ:
		DAQ_CSINo();
		break;
	}

	// Check if currentState is still CSI. If it is, reset mode
	if (currentState == ControlSequence)
		currentState = Normal;

	// Reset arguments and so
	// ...

}