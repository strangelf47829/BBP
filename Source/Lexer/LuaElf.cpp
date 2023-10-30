#include "LuaElf.h"

using namespace BBP::Services::Lexer;

LuaProcessor *BBP::Services::Lexer::P_Create(bool link, bool strip, HALFWORD a, HALFWORD b)
{
	LuaProcessor *proc = new LuaProcessor({ nullptr, nullptr, link, strip, true });

	proc->extVarTable = new BBP::Services::Binaries::ELF::stringTable();
	proc->intVarTable = new BBP::Services::Binaries::ELF::stringTable();
	proc->varTypeTable = new BBP::Services::Binaries::ELF::stringTable();
	proc->funcSigTables = new BBP::Services::Binaries::ELF::stringTable();
	proc->funcBodyTables = new BBP::Services::Binaries::ELF::stringTable();
	proc->globalsTable = new BBP::Services::Binaries::ELF::stringTable();
	proc->fileTable = new BBP::Services::Binaries::ELF::stringTable();

	proc->context = new lex_context();

	P_fetchKeywords(proc);

	proc->elf = BBP::Services::Binaries::ELF::createELF(a, b);

	return proc;
}

void BBP::Services::Lexer::P_Readfile(LuaProcessor *proc, const char *at)
{
	proc->activeFile = (char *)at;

	// DEBUG
	proc->context->activeFile = (char *)at;
	proc->context->atCol = 0;
	proc->context->atLine = 0;

	P_doFile(proc, at, proc->doLinking);	 
	L_findAllKeywords(proc->context);

	// Ignore the one's
	L_ignore(proc->context, 1);

	// Save global stuff, so we can store it elsewhere
	addStringToTable(proc->globalsTable, proc->context->data);

	// Save file name
	addStringToTable(proc->fileTable, (char *)at);
}

void BBP::Services::Lexer::P_Link(LuaProcessor *proc, char *_ext, char *_int, char *type)
{

	addStringToTable(proc->extVarTable, _ext);
	addStringToTable(proc->intVarTable, _int);
	addStringToTable(proc->varTypeTable, type);

	if (proc->doLinking || type[0] == 'M')
	{
		L_expandKeywords(proc->context, 1);
		L_setKeywordTo(proc->context, _int, proc->context->keywordCount - 1, replace_Symbol);
	}
}

void BBP::Services::Lexer::P_BuildELF(LuaProcessor *proc)
{
	proc->elf->emittedAuxLibs = 1;
	unsigned int globalSize = 0;
	unsigned char *global = nullptr;

	globalSize = BBP::Services::Binaries::ELF::calculateStringTableLength(proc->globalsTable);
	global = BBP::Services::Binaries::ELF::emitStringTable(proc->globalsTable);
	
	// Create section with all globals in it
	unsigned int globalSectionIndx = proc->elf->header.e_shnum;
	BBP::Services::Binaries::ELF::sectionBuilder *globalSection = BBP::Services::Binaries::ELF::createSection(".text.globals", global, SHT_PROGBITS, globalSize, true);
	globalSection->header.sh_flags = proc->strip ? 6 : 0x102;
	globalSection->header.sh_flags |= (proc->funcBodyTables->stringCount || (proc->globalsTable->stringCount > 1)) ? 0x106 : 0b110;
	
	// add global section
	BBP::Services::Binaries::ELF::addELFSection(proc->elf, globalSection);

	L_destroyLexer(proc->context);
	proc->context = nullptr;

	unsigned int funcSection = proc->elf->header.e_shnum;

	P_EmitELFFunctions(proc);
	P_EmitELFLinkInfo(proc);

	// This segment holds the actual (executable) program
	BBP::Services::Binaries::ELF::segmentBuilder *actualProgram = BBP::Services::Binaries::ELF::createSegment(".text", nullptr, 0, 0, (proc->funcSigTables->stringCount + proc->globalsTable->stringCount == 1) ? (0x1 | 0x4) : (0x1 | 0x2), false);
	actualProgram->header.type = PT_LOAD;
	BBP::Services::Binaries::ELF::addELFSegment(proc->elf, actualProgram);

	// Request stb lib if enabled
	if (proc->emitCallsToSTDLib)
	{
		// If interpreter is not set, set interpreter.
		if (proc->interp == nullptr)
		{
			proc->interp = (char *)calloc(24, sizeof(char));
			const char* defaultInterp = "/liblua/ld-bbp32-lua.so";

			for (unsigned int idx = 0; idx < 24; idx++)
				proc->interp[idx] = defaultInterp[idx];
			
		}

		// Calculate intepreter name lsength
		unsigned int interpSize = 0;
		for (; proc->interp[interpSize++];);
		proc->interp[interpSize - 1] = 0;

		// Create interpreter segment
		BBP::Services::Binaries::ELF::segmentBuilder *stdlib = BBP::Services::Binaries::ELF::createSegment("interp", (unsigned char *)proc->interp, interpSize, interpSize, 0, false);
		
		
		
		stdlib->header.type = PT_INTERP;
		BBP::Services::Binaries::ELF::addELFSegment(proc->elf, stdlib);
	}

	// Emit file names
	for (unsigned int file = 0; file < proc->fileTable->stringCount; file++)
	{
		SymbolEntry *symb = new SymbolEntry();
		symb->st_info = 0x14;
		BBP::Services::Binaries::ELF::addELFSymbol(proc->elf, symb, proc->fileTable->strings[file]);
	}

	P_deleteTables(proc);

	proc->elf->emittedAuxLibs = 0;
	normalizeELF(proc->elf);

	// Set executable data right
	proc->elf->segments[0]->header.offset = proc->elf->sections[globalSectionIndx]->header.sh_offset;
	proc->elf->segments[0]->header.filesz = (proc->elf->sections[funcSection]->header.sh_offset + proc->elf->sections[funcSection]->header.sh_size) - proc->elf->segments[0]->header.offset;
	proc->elf->segments[0]->header.memsz = proc->elf->segments[0]->header.filesz;

	
}

void BBP::Services::Lexer::P_deleteTables(LuaProcessor *proc)
{
	BBP::Services::Binaries::ELF::setStringTableSize(proc->extVarTable, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->intVarTable, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->varTypeTable, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->funcSigTables, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->funcBodyTables, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->globalsTable, 0);
	BBP::Services::Binaries::ELF::setStringTableSize(proc->fileTable, 0);

	delete(proc->extVarTable);
	delete(proc->intVarTable);
	delete(proc->varTypeTable);
	delete(proc->funcSigTables);
	delete(proc->funcBodyTables);
	delete(proc->globalsTable);
	delete(proc->fileTable);
}

void BBP::Services::Lexer::P_EmitELFLinkInfo(LuaProcessor *proc)
{
	
	if (proc->extVarTable->stringCount != proc->intVarTable->stringCount)
	{
		printf("Lua linker error: Export table does not have the same amount of entires as the internal variable table.\n");
		return;
	}

	SymbolEntry **externalSymbols = (SymbolEntry **)calloc(proc->extVarTable->stringCount, sizeof(SymbolEntry *));
	BBP::Services::Binaries::ELF::stringTable *externalSymbolTable = BBP::Services::Binaries::ELF::createStringTable();

	unsigned int runningLength = 0;
	unsigned int funcs = 0;
	for (unsigned int sym = 0; sym < proc->intVarTable->stringCount; sym++)
	{
		char type = (proc->varTypeTable->strings[sym])[0];
		externalSymbols[sym] = nullptr;

		if (type == 'F' || type == 'M')
		{
			funcs++;
			continue;
		}

		SymbolEntry *internalSymbol = new SymbolEntry();
		SymbolEntry *externalSymbol = new SymbolEntry();

		internalSymbol->st_value = sym;
		internalSymbol->st_size = proc->extVarTable->lengths[sym];
		internalSymbol->st_shndx = 4;
		internalSymbol->st_info = 0x10;

		externalSymbol->st_info = (type == 'f') ? 0x12 : 0x10;
		externalSymbol->st_value = (type == 'f') ? funcs : 0;
		externalSymbol->st_name = runningLength;
		runningLength += proc->extVarTable->lengths[sym];

		if (type != 'f')
			addELFSymbol(proc->elf, internalSymbol, proc->intVarTable->strings[sym]);
		else
		{
			proc->elf->symbols[funcs]->st_info = 0x22;
			delete(internalSymbol);
		}
		

		externalSymbols[sym] = externalSymbol;
		BBP::Services::Binaries::ELF::addStringToTable(externalSymbolTable, proc->extVarTable->strings[sym]);

		if (type == 'f')
			funcs++;
	}

	unsigned char *relocationData = (unsigned char *)calloc(proc->extVarTable->stringCount * 16, sizeof(unsigned char));

	unsigned int skipped = 0;
	for (unsigned int sym = 0; sym < proc->intVarTable->stringCount; sym++)
	{
		if (externalSymbols[sym] == nullptr)
		{
			skipped++;
			continue;
		}

		unsigned char *symData = BBP::Services::Binaries::ELF::emitSymbol(externalSymbols[sym]);

		for (unsigned int i = 0; i < 16; i++)
			relocationData[i + (sym - skipped) * 16] = symData[i];

		free(symData);
	}

	BBP::Services::Binaries::ELF::sectionBuilder *relocation = BBP::Services::Binaries::ELF::createSection(".rel.exp", relocationData, SHT_SYMTAB, (proc->extVarTable->stringCount - skipped) * 16, true);
	BBP::Services::Binaries::ELF::sectionBuilder *relocationStrings = BBP::Services::Binaries::ELF::createSection(".exp.str", emitStringTable(externalSymbolTable), SHT_STRTAB, calculateStringTableLength(externalSymbolTable), true);

	relocation->header.sh_link = proc->elf->header.e_shnum + 1;
	relocation->header.sh_entsize = 16;
	relocation->header.sh_flags = proc->doLinking ? 0x40 : 0x140;

	BBP::Services::Binaries::ELF::addELFSection(proc->elf, relocation);
	BBP::Services::Binaries::ELF::addELFSection(proc->elf, relocationStrings);

	BBP::Services::Binaries::ELF::setStringTableSize(externalSymbolTable, 0);
	delete(externalSymbolTable);

	for (unsigned int sym = 0; sym < proc->extVarTable->stringCount; sym++)
		delete(externalSymbols[sym]);

	free(externalSymbols);
}


unsigned int functionSection = 0;

void BBP::Services::Lexer::P_EmitELFFunctions(LuaProcessor *proc)
{

	if (proc->funcSigTables->stringCount != proc->funcSigTables->stringCount)
	{
		printf("Lua linker error: Function body count does not match function signature count.\n");
		return;
	}

	// Create function body section with data stored in proc->funcBodyTables
	unsigned char *funcBodyData = emitStringTable(proc->funcBodyTables);
	BBP::Services::Binaries::ELF::sectionBuilder *funcBodySection = BBP::Services::Binaries::ELF::createSection(".text.func", funcBodyData, SHT_PROGBITS, calculateStringTableLength(proc->funcBodyTables), true);
	
	funcBodySection->header.sh_flags = 0;
	if (proc->funcBodyTables->stringCount)
	{
		funcBodySection->header.sh_flags = proc->strip ? 0b110 : 0x102;
		funcBodySection->header.sh_flags |= (proc->funcSigTables->stringCount > 1) ? 0x100 : 0;
	}


	BBP::Services::Binaries::ELF::addELFSection(proc->elf, funcBodySection);

	// Remove any terminating null strings, so that this becomes one executable string

	functionSection = proc->elf->header.e_shnum - 1;

	unsigned int runningLength = 0;
	for (unsigned int func = 0; func < proc->funcSigTables->stringCount; func++)
	{
		SymbolEntry *symb = new SymbolEntry();
		symb->st_shndx = functionSection;

		unsigned int size = proc->funcBodyTables->lengths[func];
		symb->st_value = runningLength;
		symb->st_size = size;
		symb->st_info = 0x12;

		addELFSymbol(proc->elf, symb, proc->funcSigTables->strings[func]);
		runningLength += size;
	}


}