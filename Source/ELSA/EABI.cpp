#include "../include/ELSA.h"
#include "../include/ELSABackend.h"
#include "Actions.h"

void BBP::esa::BinaryApplication::mangle(std::conststring str, std::Lexer::lex_thing &thing, dataEntry &entry)
{
	// Just a test
	mangled.page = &mangledPage;

	// Clear mangled string
	mangled.atElement = 0;

	// Then write '_Z' if not shared library, otherwise '_S'
	if (thing.subtype & SYMTMOD_SHARED)
	{
		mangled << "_S";
		entry.type = (entry.type & 0x0F) | std::ELF::SYM_WEAK;
	}
	else
		mangled << "_Z";

	// Calculate the length of the string
	std::size_t length = std::strlen(str);

	// Prepend length
	mangled < length;

	// Then write name
	mangled << str;

	if ((entry.type & std::ELF::SYM_OBJECT || thing.subtype & SYMTACC_VIRTUAL) && !(entry.type & std::ELF::SYM_FUNC))
	{
		// If variable, do the following:

		// If the thing is virtual, do the following
		if (thing.subtype & SYMTACC_VIRTUAL)
		{

			// Append the size of the object
			switch (thing.subtype & 0b111111100000)
			{
			case SYMTSZ_DN:
			case SYMTSZ_DB:
				mangled << "Z1";
				break;
			case SYMTSZ_DH:
				mangled << "Z2";
				break;
			case SYMTSZ_DA:
			case SYMTSZ_DW:
			case SYMTSZ_DI:
			case SYMTSZ_NIL:
			default:
				mangled << "Z4";
				break;
			}

			// Then append NData if this is also the data that goes into .bss
			if (entry.type != std::ELF::SYM_LOCAL)
				mangled << "NData";
		}
		else
		{
			// Otherwise, just append the size of the thing (in bytes)
			switch (thing.subtype & 0b111111100000)
			{
			case SYMTSZ_DN:
			case SYMTSZ_DB:
				mangled << "Z1";
				break;
			case SYMTSZ_DH:
				mangled << "Z2";
				break;
			case SYMTSZ_DA:
			case SYMTSZ_DW:
			case SYMTSZ_DI:
			case SYMTSZ_NIL:
			default:
				mangled << "Z4";
				break;
			}
		}
		
	}
	else if (entry.type & std::ELF::SYM_FUNC)
	{
		// If procedure, add 'P'
		mangled << "P";

		// Then write the amount of arguments it takes.
		mangled < (thing.subtype & 0x0F);
	}

	// Then 0 terminate
	mangled << '\0';

}

void BBP::esa::esaProcessor::reserveRegisters()
{
	addReservedIdentifier("eax", { userspace::REGISTERPAGE | 1, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ax",  { userspace::REGISTERPAGE | 1 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ah",  { userspace::REGISTERPAGE | 1 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("al",  { userspace::REGISTERPAGE | 1 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("ecx", { userspace::REGISTERPAGE | 2, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("cx",  { userspace::REGISTERPAGE | 2 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ch",  { userspace::REGISTERPAGE | 2 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("cl",  { userspace::REGISTERPAGE | 2 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("edx", { userspace::REGISTERPAGE | 3, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dx",  { userspace::REGISTERPAGE | 3 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dh",  { userspace::REGISTERPAGE | 3 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dl",  { userspace::REGISTERPAGE | 3 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("ebx", { userspace::REGISTERPAGE | 4, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("bx",  { userspace::REGISTERPAGE | 4 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("bh",  { userspace::REGISTERPAGE | 4 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("bl",  { userspace::REGISTERPAGE | 4 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("esi", { userspace::REGISTERPAGE | 5, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("six", { userspace::REGISTERPAGE | 5 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("sih", { userspace::REGISTERPAGE | 5 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("sil", { userspace::REGISTERPAGE | 5 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("edi", { userspace::REGISTERPAGE | 6, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dix", { userspace::REGISTERPAGE | 6 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dih", { userspace::REGISTERPAGE | 6 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("dil", { userspace::REGISTERPAGE | 6 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("tix", { userspace::REGISTERPAGE | 7, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("st",  { userspace::REGISTERPAGE | 7 | userspace::HIHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ob",  { userspace::REGISTERPAGE | 7 | userspace::HIHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("tb",  { userspace::REGISTERPAGE | 7 | userspace::HIHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ct",  { userspace::REGISTERPAGE | 7 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("pp",  { userspace::REGISTERPAGE | 7 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("tp",  { userspace::REGISTERPAGE | 7 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("ssx", { userspace::REGISTERPAGE | 8, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("hv",  { userspace::REGISTERPAGE | 8 | userspace::HIHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ho",  { userspace::REGISTERPAGE | 8 | userspace::HIHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ht",  { userspace::REGISTERPAGE | 8 | userspace::HIHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ss",  { userspace::REGISTERPAGE | 8 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("cs",  { userspace::REGISTERPAGE | 8 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("sf",  { userspace::REGISTERPAGE | 8 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
								 
	addReservedIdentifier("sui", { userspace::REGISTERPAGE | 9, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("asi", { userspace::REGISTERPAGE | 9 | userspace::HIHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("gsi", { userspace::REGISTERPAGE | 9 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("eip", { userspace::REGISTERPAGE | 10, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ip",  { userspace::REGISTERPAGE | 10 | userspace::LOHW, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("ih",  { userspace::REGISTERPAGE | 10 | userspace::LOHW | userspace::HIBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });
	addReservedIdentifier("il",  { userspace::REGISTERPAGE | 10 | userspace::LOHW | userspace::LOBY, false, true, 0, 1, {1, 0, 0, 0}, false, false });

	addReservedIdentifier("sip", { userspace::REGISTERPAGE | 11, false, true, 0, 1, {1, 0, 0, 0}, false, false });
}