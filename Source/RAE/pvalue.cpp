#include "../include/ValueCategories.h"
#include "../include/stdio.h"
#include "../include/Opcodes.h"
#include "../include/Threading.h"
#include "../include/StateMachine.h"


bool BBP::userspace::pvalue::isArgumentLValue(userspace::Instruction::Arg &arg)
{
	return !(arg.isLiteralArg || arg.referenced);
}

bool BBP::userspace::pvalue::isArgumentRValue(userspace::Instruction::Arg &arg)
{
	return (arg.isLiteralArg || arg.referenced);
}

bool BBP::userspace::pvalue::isAddressRegisterAddress(std::address_t address)
{
	return (address >> 16 == userspace::REGISTERPAGE >> 16);
}

BBP::userspace::register_t *BBP::userspace::pvalue::getAddressForRegister(userspace::StateMachine &state, std::address_t lvalueFor)
{
	// A pointer to the register
	register_t *_register = nullptr;

	constexpr std::word LOBYHIHW = userspace::LOBY | userspace::HIHW | REGISTERPAGE;
	constexpr std::word HIBYHIHW = userspace::HIBY | userspace::HIHW | REGISTERPAGE;
	constexpr std::word LOBYLOHW = userspace::LOBY | userspace::LOHW | REGISTERPAGE;
	constexpr std::word HIBYLOHW = userspace::HIBY | userspace::LOHW | REGISTERPAGE;
	constexpr std::word HIHW = userspace::HIHW | REGISTERPAGE;
	constexpr std::word LOHW = userspace::LOHW | REGISTERPAGE;
	constexpr std::word TT = REGISTERPAGE;

	switch (lvalueFor & 0b1111)
	{
	case 1: // eax
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// AL
			_register = &state.getActiveThread().al;
			break;
		case HIBYLOHW:	// AH
			_register = &state.getActiveThread().ah;
			break;
		case LOHW:		// AX
			_register = &state.getActiveThread().ax;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// EAX
			_register = &state.getActiveThread().eax;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 2: // ecx
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// CL
			_register = &state.getActiveThread().cl;
			break;
		case HIBYLOHW:	// CH
			_register = &state.getActiveThread().ch;
			break;
		case LOHW:		// CX
			_register = &state.getActiveThread().cx;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// ECX
			_register = &state.getActiveThread().ecx;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 3: // edx
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// DL
			_register = &state.getActiveThread().dl;
			break;
		case HIBYLOHW:	// DH
			_register = &state.getActiveThread().dh;
			break;
		case LOHW:		// DX
			_register = &state.getActiveThread().dx;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// EDX
			_register = &state.getActiveThread().edx;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 4: // ebx
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// BL
			_register = &state.getActiveThread().bl;
			break;
		case HIBYLOHW:	// BH
			_register = &state.getActiveThread().bh;
			break;
		case LOHW:		// BX
			_register = &state.getActiveThread().bx;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// EBX
			_register = &state.getActiveThread().ebx;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 5: // esi
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// sil
			_register = &state.getActiveThread().sil;
			break;
		case HIBYLOHW:	// sih
			_register = &state.getActiveThread().sih;
			break;
		case LOHW:		// six
			_register = &state.getActiveThread().six;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// esi
			_register = &state.getActiveThread().esi;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 6: // edi
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// dil
			_register = &state.getActiveThread().dil;
			break;
		case HIBYLOHW:	// dih
			_register = &state.getActiveThread().dih;
			break;
		case LOHW:		// dix
			_register = &state.getActiveThread().dix;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// edi
			_register = &state.getActiveThread().edi;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 7: // tix
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// tb
			_register = &state.getActiveThread().tb;
			break;
		case HIBYHIHW:	// ob
			_register = &state.getActiveThread().ob;
			break;
		case LOBYLOHW:	// tp
			_register = &state.getActiveThread().tp;
			break;
		case HIBYLOHW:  // pp
			_register = &state.getActiveThread().pp;
			break;
		case LOHW:		// ct
			_register = &state.getActiveThread().ct;
			break;
		case HIHW:		// st
			_register = &state.getActiveThread().st;
			break;
		case TT:		// tix
			_register = &state.getActiveThread().tix;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 8: // ssx
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// hv
			_register = &state.getActiveThread().hv;
			break;
		case HIBYHIHW:	// ho
			_register = &state.getActiveThread().ho;
			break;
		case LOBYLOHW:	// sf
			_register = &state.getActiveThread().sf;
			break;
		case HIBYLOHW:  // cs
			_register = &state.getActiveThread().cs;
			break;
		case LOHW:		// ss
			_register = &state.getActiveThread().ss;
			break;
		case HIHW:		// hv
			_register = &state.getActiveThread().hv;
			break;
		case TT:		// ssx
			_register = &state.getActiveThread().ssx;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 9: // sui
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYLOHW:  // Doesn't exist
			__SIGNAL__(SIGILL);
		case LOHW:		// gsi
			_register = &state.getActiveThread().gsi;
			break;
		case HIHW:		// asi
			_register = &state.getActiveThread().asi;
			break;
		case TT:		// sui
			_register = &state.getActiveThread().sui;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 10: // eip
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// il
			_register = &state.getActiveThread().il;
			break;
		case HIBYLOHW:	// ih
			_register = &state.getActiveThread().ih;
			break;
		case LOHW:		// ip
			_register = &state.getActiveThread().ip;
			break;
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// eip
			_register = &state.getActiveThread().eip;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	case 11: // sip
		switch (lvalueFor & ~0b1111)
		{
		case LOBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYHIHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case LOBYLOHW:	// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIBYLOHW:  // Doesn't exist
			__SIGNAL__(SIGILL);
		case LOHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case HIHW:		// Doesn't exist
			__SIGNAL__(SIGILL);
		case TT:		// sip
			_register = &state.getActiveThread().sip;
			break;
		default: // Doesn't exist
			__SIGNAL__(SIGILL);
		}
		break;
	default: // Doesn't exist
		__SIGNAL__(SIGILL);
	}

	// Return address
	return _register;
}