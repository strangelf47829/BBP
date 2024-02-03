#include "Lpp.h"

void BBP::Services::Binaries::LPP::setOutputFile(BBP::Services::Binaries::LPP::CompilationContext* compiler, const char* name)
{
	// Get file name size
	for (; name[compiler->outFileLength++];);

	// Alloc data to outfile
	compiler->outFile = (char*)BBP::Services::c_alloc(compiler->outFileLength + 1, sizeof(char));

	// Copy data
	for (unsigned int idx = 0; idx < compiler->outFileLength; idx++)
		compiler->outFile[idx] = name[idx];

	// Set terminating zero
	compiler->outFile[compiler->outFileLength] = 0;
}

void BBP::Services::Binaries::LPP::setOutputFile(BBP::Services::Binaries::LPP::CompilationContext *compiler, BBP::IO::Buffers::stringTable *args, unsigned int arg)
{
	// Check that the file isnt a switch either
	if ((args->strings[arg])[0] == '-')
	{
		BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "EExpected file name after -o.");
		return;
	}

	// Get file name size
	for (; (args->strings[arg])[compiler->outFileLength++];);

	// Alloc data to outfile
	compiler->outFile = (char *)BBP::Services::c_alloc(compiler->outFileLength + 1, sizeof(char));

	// Copy data
	for (unsigned int idx = 0; idx < compiler->outFileLength; idx++)
		compiler->outFile[idx] = (args->strings[arg])[idx];

	// Set terminating zero
	compiler->outFile[compiler->outFileLength] = 0;
}

void BBP::Services::Binaries::LPP::setInterp(BBP::Services::Binaries::LPP::CompilationContext *compiler, BBP::IO::Buffers::stringTable *args, unsigned int arg)
{
	// Check that the file isnt a switch either
	if ((args->strings[arg])[0] == '-')
	{
		BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "EExpected path after -p.");
		return;
	}

	compiler->interpLength = 0;

	// Get file name size
	for (; (args->strings[arg])[compiler->interpLength++];);

	// Alloc data to outfile
	compiler->interp = (char *)BBP::Services::c_alloc(compiler->interpLength + 1, sizeof(char));

	// Copy data
	for (unsigned int idx = 0; idx < compiler->interpLength; idx++)
		compiler->interp[idx] = (args->strings[arg])[idx];

	// Set terminating zero
	compiler->interp[compiler->interpLength] = 0;

	// Set processor interpreter to interp
	compiler->proc->interp = compiler->interp;
}

int BBP::Services::Binaries::LPP::ParseArguments(CompilationContext *compiler, BBP::IO::Buffers::stringTable *args, unsigned int argc)
{
	for (unsigned int arg = 0; arg < argc - 1; arg++)
	{
		// Get the next word
		char *option = args->strings[arg];

		char c = option[0];

		if (c == '-')
		{
			switch (option[1])
			{
			case 0:
				compiler->proc->context->showLines = false;
				BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "EInvalid argument 'nil'");
				break;

			case 'V':
				compiler->verbose = true;
				break;
			case 'v':
				showVersion();
				return 1;
			case 'h':
			case 'H':
				showHelp();
				return 2;
			case 'C': // Same as -r -e -c
				compiler->emitExecutable = true;
				compiler->proc->doLinking = false;
				compiler->proc->strip = false;
				break;
			case 'o': // Set output file if it doesn't exist, set if it doesn't.
				if (compiler->outFile != nullptr)
				{
					BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "EInvalid switch '-o': output file already specified");
					return 3;
				}

				// Check if this is last switch
				if (arg + 1 >= args->stringCount)
				{
					BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "E-o Cannot be last switch.");
					return 3;
				}

				setOutputFile(compiler, args, arg + 1);
				arg++;
				break;
			case 'p': // program (the interpreter section thing)
				if (compiler->interp != nullptr)
				{
					BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "EInvalid switch '-p': Program interpreter already set.");
					return 5;
				}

				// Check if this is last switch
				if (arg + 1 >= args->stringCount)
				{
					BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "E-p Cannot be last switch.");
					return 5;
				}

				setInterp(compiler, args, arg + 1);
				arg++;

				break;

			case 'r': // Raw
				compiler->proc->strip = false;
				break;
			case 'c': // Do not link
				compiler->proc->doLinking = false;
				break;
			case 'f': // freestanding
				compiler->proc->emitCallsToSTDLib = false;
				break;
			case 'e': // lua executable
				compiler->emitExecutable = true;
				break;
			default:
				BBP::Services::Lexer::L_Log(compiler->proc->context, arg, "L++", "Unrecognized option.");
				break;
			}
			continue;
		}
		BBP::IO::Buffers::addStringToTable(compiler->inputFiles, option);
	}

	return 0;
}

void BBP::Services::Binaries::LPP::showHelp()
{
	printf("Usage: l++ [options] file...\n");
	printf("Options:\n");

	printf("-V\t\tDisplay verbose logging during compilation.\n");
	printf("-v\t\tDisplay version information.\n");
	printf("-h\t\tDisplay this page.\n");
	printf("-H\t\tSame as -h\n");
	printf("-o <file>\tPlace the output into <file>.\n");
	printf("-p <path>\tUse <path> as the requested interpreter in ELF files.\n");
	printf("-r\t\tDo not strip files of l++ features.\n");
	printf("-c\t\tCompile, but do not link.\n");
	printf("-C\t\tSame as -r -e -c\n");
	printf("-f\t\tDo not emit calls to any external libraries.\n");
	printf("-e\t\tCreate an executable lua file instead of an ELF file.\n");
}
void BBP::Services::Binaries::LPP::showVersion()
{
	printf("BBP L++ 1.2.1 " BBP_ARCH " Experimental branch.\n");
	printf("Copyright (C) 2023 PCiD.\n");
	printf("Author: Rafael de Bie\n");
}

BBP::Services::Binaries::LPP::CompilationContext *BBP::Services::Binaries::LPP::createCompiler()
{
	LPP::CompilationContext *compiler = new LPP::CompilationContext();
	compiler->proc = BBP::Services::Lexer::P_Create(true, true, 0, 0);
	compiler->proc->context->showWhere = false;

	return compiler;
}

void BBP::Services::Binaries::LPP::destroyCompiler(BBP::Services::Binaries::LPP::CompilationContext *compiler)
{
	if (compiler->outFile)
		free(compiler->outFile);

	BBP::IO::Buffers::setStringTableSize(compiler->inputFiles, 0);
	delete(compiler->inputFiles);

	BBP::Services::Lexer::P_destroyLua(compiler->proc);
	delete(compiler->proc);
	delete(compiler);
}

void BBP::Services::Binaries::LPP::DoCompiling(BBP::Services::Binaries::LPP::CompilationContext *compiler, BBP::IO::Buffers::stringTable *args)
{
	for (unsigned int k = 0; k < compiler->inputFiles->stringCount; k++)
		BBP::Services::Lexer::P_Readfile(compiler->proc, compiler->inputFiles->strings[k]);
}

void BBP::Services::Binaries::LPP::DoLinking(BBP::Services::Binaries::LPP::CompilationContext *compiler, BBP::IO::Buffers::stringTable *args)
{
	compiler->proc->doLinking = false;
	bool strip = compiler->proc->strip;

	for (unsigned int k = 0; k < compiler->inputFiles->stringCount; k++)
	{
		// Free outfile if exists
		if (compiler->outFile)
			free(compiler->outFile);

		// Copy input file into outfile
		compiler->outFile = (char *)BBP::Services::c_alloc(compiler->inputFiles->lengths[k], sizeof(char));

		// Find the '.' index
		for (unsigned int dotIndex = 0; dotIndex < compiler->inputFiles->lengths[k]; dotIndex++)
		{
			compiler->outFile[dotIndex] = (compiler->inputFiles->strings[k])[dotIndex];

			// Skip if character is not dot
			if ((compiler->inputFiles->strings[k])[dotIndex] != '.')
				continue;

			// Overwrite TODO: Check invalid read
			if (compiler->emitExecutable)
			{
				compiler->outFile[dotIndex + 1] = 'l';
				compiler->outFile[dotIndex + 2] = '+';
				compiler->outFile[dotIndex + 3] = '+';
				compiler->outFile[dotIndex + 4] = 0;
			}
			else
			{
				compiler->outFile[dotIndex + 1] = 'o';
				compiler->outFile[dotIndex + 2] = 0;
			}
			break;
		}

		// Process Lua
		BBP::Services::Lexer::P_Readfile(compiler->proc, compiler->inputFiles->strings[k]);

		// Build ELF
		

		// Do thing based on -e flag
		if (!compiler->emitExecutable)
		{
			BBP::Services::Lexer::P_BuildELF(compiler->proc);
			BBP::Services::Binaries::ELF::saveELFFile(compiler->proc->elf, compiler->outFile);
		}
		else
			LPP::EmitExecutable(compiler);

		// Destroy processor
		BBP::Services::Lexer::P_destroyLua(compiler->proc);

		// Create new processor
		compiler->proc = BBP::Services::Lexer::P_Create(false, strip, 0, 0);
	}

}

void BBP::Services::Binaries::LPP::EmitExecutable(LPP::CompilationContext *compiler)
{
	// Build and normalize ELF
	Lexer::P_BuildELF(compiler->proc);
	ELF::normalizeELF(compiler->proc->elf);

	// Take ELF executable section and remove the null terminators
	// Find executable segment
	unsigned int execStart = compiler->proc->elf->segments[0]->header.offset;
	unsigned int execLen = compiler->proc->elf->segments[0]->header.filesz;

	// emit ELF
	unsigned char *binaryData = ELF::emitELF(compiler->proc->elf);

	// Remove terminating zeros
	for (unsigned int k = 0; k < execLen; k++)
		binaryData[execStart + k] = (binaryData[execStart + k] ? binaryData[execStart + k] : '\n');

	// Add last one back in
	binaryData[execLen] = 0;

	// Write data from elf into file
	BBP::IO::SDMMC::writeFile(compiler->outFile, binaryData + execStart, execLen);

	// Free ELF binary
	free(binaryData);
}

int BBP::Services::Binaries::LPP::Run(BBP::IO::Buffers::stringTable *args)
{
	BBP::Services::Binaries::LPP::CompilationContext *compiler = BBP::Services::Binaries::LPP::createCompiler();

	// These variables hold a "string" of the arguments
	unsigned int argDataSize = BBP::Services::Binaries::ELF::calculateStringTableLength(args);
	unsigned char *argData = BBP::Services::Binaries::ELF::emitStringTable(args);

	// Flip all terminating nulls, except for the last one (which is out of bounds anyway).
	for (unsigned int idx = 0; idx < argDataSize; idx++)
	{
		if (argData[idx])
			continue;
		argData[idx] = '\t';
	}

	// In case terminating null was lost: add it back
	argData[argDataSize - 1] = 0;

	// Set context data to emitted data.
	compiler->proc->context->data = (char *)argData;
	compiler->proc->context->dataSize = argDataSize;

	// Create string table where the input files are stored.
	compiler->inputFiles = BBP::IO::Buffers::createStringTable();

	// Parse arguments
	int code = BBP::Services::Binaries::LPP::ParseArguments(compiler, args, args->stringCount + 1);

	// Do error code handling
	if (code)
	{
		if (code != 1 && code != 2)
			printf("Cannot compile: L++ error code is %d.\n", code);


		BBP::Services::Lexer::P_deleteTables(compiler->proc);
		BBP::Services::Binaries::LPP::destroyCompiler(compiler);
		return code;
	}

	compiler->proc->context->showWhere = true;
	// Do things based on -c flag
	if (compiler->proc->doLinking)
		LPP::DoCompiling(compiler, args);
	else
		LPP::DoLinking(compiler, args);

	// If -c flag was set, destroy compiler then return since nothing more is to be done.
	if (!compiler->proc->doLinking)
	{
		LPP::destroyCompiler(compiler);
		return 0;
	}

	// If -e flag was not set, emit ELF
	if (!compiler->emitExecutable)
	{
		// Build ELF
		Lexer::P_BuildELF(compiler->proc);

		// If output file was not set, set one now
		if (!compiler->outFile)
			LPP::setOutputFile(compiler, "a.out");

		// Then emit
		ELF::saveELFFile(compiler->proc->elf, compiler->outFile);
	}
	else
	{
		// If output file was not set, set one now
		if (!compiler->outFile)
		{
			// If more than one input file, just set to a.l++
			if (compiler->inputFiles->stringCount > 1)
				LPP::setOutputFile(compiler, "a.l++");
			else
			{
				// Otherwise, copy over input file name and switch the .lua for .l++
				LPP::setOutputFile(compiler, compiler->inputFiles->strings[0]);

				// Then switch over last two chars
				compiler->outFile[compiler->outFileLength - 2] = '+';
				compiler->outFile[compiler->outFileLength - 3] = '+';
			}
		}
		LPP::EmitExecutable(compiler);

	}

	BBP::Services::Binaries::LPP::destroyCompiler(compiler);
	return 0;
}