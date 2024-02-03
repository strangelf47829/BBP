#include "Lpp.h"
#include "LuaFlags.h"

char *getNumberAsString(unsigned int num)
{
	int Log = 1; // Calculate Log of name
	for (unsigned int len = num; len > 9; len /= 10)
		Log++;

	// Allocate memory to length
	char *len = (char *)BBP::Services::c_alloc(Log + 1, sizeof(char));

	// Create new variable to store variable
	unsigned int inPlaceLength = num;
	for (unsigned int l = 0; l < Log; l++)
	{
		// Get current digit
		int digit = inPlaceLength % 10;

		// Store digit as character
		len[Log - l - 1] = '0' + digit;

		// Then divide inPlaceLength
		inPlaceLength /= 10;
	}

	// 0 terminate
	len[Log] = 0;

	return len;
}

char *getSizeAsString(char *name)
{
	unsigned int nameLength = 0; // Get length of name
	for (; name[nameLength]; nameLength++);

	return getNumberAsString(nameLength);
}


void BBP::Services::Binaries::LPP::exportName(BBP::Services::Lexer::LuaProcessor *proc)
{
	// Get type of variable
	char type = (proc->varTypeTable->strings[proc->varTypeTable->stringCount - 1])[0];
	bool isFunction = (type == 'f' || type == 'F');

	// Set prepend
	char prepend = (isFunction ? 'F' : 'V');

	// Get the size of the symbol as 'char'
	char *nameSize = getSizeAsString(proc->intVarTable->strings[proc->intVarTable->stringCount - 1]);

	char *suffix = nullptr;
	char *PS = nullptr;

	if (proc->context->flags[FLAG_SYMBOLFILEMANGLE])
	{
		PS = proc->activeFile;
	}
	else
	{
		PS = (char *)BBP::Services::c_alloc(2, sizeof(char));
		PS[0] = 'A';
		PS[1] = 0;
	}

	// If is function, look at function signature
	if (isFunction)
	{
		if (proc->context->flags[FLAG_FUNCPARAMSAVAIL])
		{
			// Get the name after the '(' or end of string
			char *paramStr = proc->funcSigTables->strings[proc->funcSigTables->stringCount - 1];
			unsigned int paramStrLen = 0;
			for (; paramStrLen++ < proc->funcSigTables->lengths[proc->funcSigTables->stringCount - 1] && paramStr[0] != '('; paramStr++);
			if (paramStr[0] == '(')
			{
				paramStr++;
				paramStrLen--;
			}


			// Calculate the amount of parameters
			unsigned int params = 0;
			if (paramStr[0])
			{
				// Add one
				params++;

				// For every comma, increment
				for (unsigned int k = 0; k < paramStrLen; k++)
					if (paramStr[k] == ',')
						params++;
			}
			// Append as the suffix
			suffix = getNumberAsString(params);
		}
		else
		{
			// Append U
			suffix = (char *)BBP::Services::c_alloc(2, sizeof(char));
			suffix[0] = 'U';
			suffix[1] = 0;
		}
	}
	else
	{
		// Append V
		suffix = (char *)BBP::Services::c_alloc(2, sizeof(char));
		suffix[0] = 'V';
		suffix[1] = 0;
	}

	// Calculate the total length
	unsigned mangledName = 2;
	for (unsigned int strLen = 0; nameSize[strLen]; strLen++)
		mangledName++;
	for (unsigned int strLen = 0; (proc->intVarTable->strings[proc->intVarTable->stringCount - 1])[strLen]; strLen++)
		mangledName++;
	for (unsigned int strLen = 0; suffix[strLen]; strLen++)
		mangledName++;
	for (unsigned int strLen = 0; PS[strLen]; strLen++)
		mangledName++;

	// Create buffer
	unsigned int strCopy = 2;
	char *extName = (char *)BBP::Services::c_alloc(mangledName + 1, sizeof(char));
	extName[0] = prepend;
	extName[1] = '_';

	// Copy all the data
	for (unsigned int strLen = 0; nameSize[strLen]; strLen++)
		extName[strCopy++] = nameSize[strLen];
	for (unsigned int strLen = 0; (proc->intVarTable->strings[proc->intVarTable->stringCount - 1])[strLen]; strLen++)
		extName[strCopy++] = (proc->intVarTable->strings[proc->intVarTable->stringCount - 1])[strLen];
	for (unsigned int strLen = 0; suffix[strLen]; strLen++)
		extName[strCopy++] = suffix[strLen];
	for (unsigned int strLen = 0; PS[strLen]; strLen++)
		if (PS[strLen] != '.')
			extName[strCopy++] = PS[strLen];
		else
			extName[strCopy++] = 'D';

	// Free buffers
	BBP::Services::Free(nameSize);
	BBP::Services::Free(suffix);
	if (!proc->context->flags[FLAG_SYMBOLFILEMANGLE])
		BBP::Services::Free(PS);

	// null terminate
	extName[mangledName] = 0;

	// Export the name, then free buffer
	BBP::IO::Buffers::addStringToTable(proc->extVarTable, extName);
	BBP::Services::Free(extName);
}




char *BBP::Services::Binaries::LPP::mangleName(bool *flags, char *name)
{
	// If name does not exist, return nothing
	if (name == nullptr)
		return nullptr;

	// Get name length
	int nameLength = 0;
	for (; name[nameLength]; nameLength++);

	// scheme: V_os_


}