/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "Kernel.h"
#undef printf
#include "ELFUtils.h"
#include "Lexer.h"
#include "LuaLexer.h"
#include "LuaELF.h"

#include <cstdio>
#include <cstdlib>

LuaProcessor *proc;
bool emitExecutable = false;
bool verbose = false;

char *outFile;
unsigned int outFileNameSize = 0;

char *pathName;
unsigned int pathSize = 0;

stringTable *inputFiles;

void printStringTable(stringTable *table, const char *header)
{
	printf("+----------------------+\n");
	printf("|  %-9s\n", header);
	printf("+----------------------+\n");
	
	if (table->stringCount == 0)
	{
		printf("|     <table empty>    |\n");
		printf("+----------------------+\n");
		return;
	}
	
	for (unsigned int k = 0; k < table->stringCount; k++)
	{
		printf("+----------------------+\n");
		printf("| Entry: %u\n", k);

		printf("%s\n", table->strings[k]);

		printf("| Size: 0x%06x\n", table->lengths[k]);
		printf("+----------------------+\n");
	}

}

void showVersionInfo()
{
	printf("BBP L++ 1.0.0\n");
	printf("Copyright (C) 2023 PCiD.\n");
	printf("Author: Rafael de Bie\n");
}

void showHelp()
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
	printf("-f\t\tDo not emit calls to any external libraries.\n");
	printf("-e\t\tCreate an executable lua file instead of an ELF file.\n");

}

void setOutputFile(lex_context *context, stringTable *args, int argc)
{

	// Check that the file isnt a switch either
	if ((args->strings[argc])[0] == '-')
	{
		L_Log(context, argc, "L++", "EExpected file name after -o.");
		return;
	}

	// Get file name size
	for (; (args->strings[argc])[outFileNameSize++];);

	// Alloc data to outfile
	outFile = (char *)calloc(outFileNameSize + 1, sizeof(char));

	// Copy data
	for (unsigned int idx = 0; idx < outFileNameSize; idx++)
		outFile[idx] = (args->strings[argc])[idx];

	// Set terminating zero
	outFile[outFileNameSize] = 0;
}



void setInterp(lex_context *context, stringTable *args, int argc)
{
	// Check that the file isnt a switch either
	if ((args->strings[argc])[0] == '-')
	{
		L_Log(context, argc, "L++", "EExpected path after -p.");
		return;
	}

	pathSize = 0;

	// Get file name size
	for (; (args->strings[argc])[pathSize++];);

	// Alloc data to outfile
	pathName = (char *)calloc(pathSize + 1, sizeof(char));

	// Copy data
	for (unsigned int idx = 0; idx < pathSize; idx++)
		pathName[idx] = (args->strings[argc])[idx];

	// Set terminating zero
	pathName[pathSize] = 0;
}

int parseArguments(lex_context *context, stringTable *args, int argc)
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
				context->showLines = false;
				L_Log(context, arg, "L++", "EInvalid argument 'nil'");
				break;

			case 'V':
				verbose = true;
				break;
			case 'v':
				showVersionInfo();
				return 1;
			case 'h':
			case 'H':
				showHelp();
				return 2;
			case 'o': // Set output file if it doesn't exist, set if it doesn't.
				if (outFile != nullptr)
				{
					L_Log(context, arg, "L++", "EInvalid switch '-o': output file already specified");
					return 3;
				}

				// Check if this is last switch
				if (arg + 1 >= args->stringCount)
				{
					L_Log(context, arg, "L++", "E-o Cannot be last switch.");
					return 3;
				}

				setOutputFile(context, args, arg + 1);
				arg++;
				break;
			case 'p': // program (the interpreter section thing)
				if (pathName != nullptr)
				{
					L_Log(context, arg, "L++", "EInvalid switch '-p': Program interpreter already set.");
					return 5;
				}

				// Check if this is last switch
				if (arg + 1 >= args->stringCount)
				{
					L_Log(context, arg, "L++", "E-p Cannot be last switch.");
					return 5;
				}

				setInterp(context, args, arg + 1);
				arg++;

				break;

			case 'r': // Raw
				proc->strip = false;
				break;
			case 'c': // Do not link
				proc->doLinking = false;
				break;
			case 'f': // freestanding
				proc->emitCallsToSTDLib = false;
				break;
			case 'e': // lua executable
				emitExecutable = true;
				break;
			default:
				L_Log(context, arg, "L++", "Unrecognized option.");
				break;
			}
			continue;
		}
		addStringToTable(inputFiles, option);
	}

	return 0;
}

int main(int argc, char **argv)
{
	// Create proc.
	proc = new LuaProcessor({ nullptr, nullptr, true, true, true });

	lex_context *context = new lex_context();

	// If there are no arguments, throw error
	if (argc == 1)
	{
		L_Log(context, 0, "L++", "ENo Input files");
		delete(context);
		return 1;
	}

	// Aux table to store the arg data in.
	stringTable *argTable = createStringTable();

	// Copy over arguments into string table
	for (unsigned int arg = 1; arg < argc; arg++)
		addStringToTable(argTable, argv[arg]);

	// These variables hold a "string" of the arguments
	unsigned int argDataSize = calculateStringTableLength(argTable);
	unsigned char *argData = emitStringTable(argTable);
	
	// Flip all terminating nulls, except for the last one (which is out of bounds anyway).
	for (unsigned int idx = 0; idx < argDataSize; idx++)
	{
		if (argData[idx])
			continue;
		argData[idx] = '\t';
	}
	
	// In case terminating null was lost: add it back
	argData[argDataSize] = 0;

	// Set context data to emitted data.
	context->data = (char *)argData;
	context->dataSize = argDataSize;

	// Create string table where the input files are stored.
	inputFiles = createStringTable();

	// Parse arguments
	int code = parseArguments(context, argTable, argc);

	if (code)
	{
		if (code != 1 && code != 2)
			printf("Cannot compile: L++ error code is %d.\n", code);


		if (outFile)
			free(outFile);

		if (pathSize)
			free(proc->interp);

		delete(proc);

		
	}

	// Delete proc and set a different proc
	bool link = proc->doLinking;
	bool strip = proc->strip;
	bool emitCalls = proc->emitCallsToSTDLib;

	delete(proc);

	if (!link && inputFiles->stringCount != 1)
	{

		L_Log(context, 0, "L++", "ECannot compile more than one file when -c is set.");

		free(argData);
		setStringTableSize(argTable, 0);
		setStringTableSize(inputFiles, 0);
		delete(argTable);
		delete(inputFiles);
		return 10;
	}

	proc = P_Create(link, strip, link ? 2 : 1, 0);

	bool mallocedOutFile = (outFile != nullptr);

	if (link)
	{
		if (!mallocedOutFile && !emitExecutable)
			outFile = "a.out";
		else if (!mallocedOutFile)
			outFile = "a.lua";
	}
	else
	{
		if (mallocedOutFile)
			free(outFile);

		outFile = (char *)calloc(inputFiles->lengths[0] + 2, sizeof(char));
		
		for (unsigned int k = 0; k < inputFiles->lengths[0]; k++)
			outFile[k] = (inputFiles->strings[0])[k];

		outFile[inputFiles->lengths[0] - 1] = '.';
		outFile[inputFiles->lengths[0]] = 'o';
		outFile[inputFiles->lengths[0] + 1] = 0;
	}
	

	proc->interp = pathName;

	if (verbose)
	{
		printf("Verbose L++ Output\n");
		printf(" > List of arguments: ");
		for (unsigned int k = 0; k < argTable->stringCount; k++)
			printf("\t%s\n", argTable->strings[k]);
		printf(" > Output file: %s\n", outFile);
		printf("\t(default? %d)\n", !mallocedOutFile);
		printf("\tLink? %d\n", proc->doLinking);
		printf("\tStrip? %d\n", proc->strip);
		printf("\tstdlib? %d\n", proc->emitCallsToSTDLib);
		printf("\texecutable? %d\n", emitExecutable);
		for (unsigned int k = 0; k < inputFiles->stringCount; k++)
			printf(" > Input file: %s\n", inputFiles->strings[k]);
	}

	if (inputFiles->stringCount == 0)
		L_Log(context, 0, "L++", "ENo Input files.");

	for (unsigned int k = 0; k < inputFiles->stringCount; k++)
	{
		P_Readfile(proc, inputFiles->strings[k]);
	}

	if (verbose)
	{
		printf("Done reading files.\n");
		
		printStringTable(proc->extVarTable, "External Symbols");
		printStringTable(proc->intVarTable, "Internal Symbols");
		printStringTable(proc->varTypeTable, "Symbol Types");
		printStringTable(proc->funcSigTables, "Function Signatures");
		printStringTable(proc->funcBodyTables, "Function Bodies");
		printStringTable(proc->globalsTable, "Globals");
		printStringTable(proc->fileTable, "Files");
	}

	P_BuildELF(proc);
	normalizeELF(proc->elf);

	if (emitExecutable)
	{
		// Take ELF executable section and remove the null terminators
		// Find executable segment
		unsigned int execStart = proc->elf->segments[0]->header.offset;
		unsigned int execLen = proc->elf->segments[0]->header.filesz;

		// emit ELF
		unsigned char *binaryData = emitELF(proc->elf);

		// Remove terminating zeros
		for (unsigned int k = 0; k < execLen; k++)
			binaryData[execStart + k] = (binaryData[execStart + k] ? binaryData[execStart + k] : '\n');

		// Add last one back in
		binaryData[execLen] = 0;

		// Write data from elf into file
		BBP::IO::SDMMC::writeFile(outFile, binaryData + execStart, execLen);

		// Free ELF binary
		free(binaryData);
	}
	else
		saveELFFile(proc->elf, outFile);

	
	destroyELF(proc->elf);


	if (mallocedOutFile)
		free(outFile);

	if (pathSize)
		free(proc->interp);

	delete(proc);

	free(argData);
	setStringTableSize(argTable, 0);
	setStringTableSize(inputFiles, 0);
	delete(argTable);
	delete(inputFiles);

	return 0;
}




void BBP::Services::Free(void *ptr)
{
	free(ptr);
}

void *BBP::Services::c_alloc(unsigned long long int count, unsigned long long int size)
{
	return calloc(count, size);
}

void *BBP::Services::m_alloc(unsigned long long int size)
{
	return malloc(size);
}
