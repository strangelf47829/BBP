#include "Kernel.h"
#include "ELF32.h"

using namespace BBP::IO::Buffers;

stringTable *BBP::IO::Buffers::createStringTable()
{
	// Allocate memory to string table
	stringTable *table = new stringTable();

	// Initialize data
	table->strings = NULL;
	table->lengths = NULL;
	table->stringCount = 0;

	return table;
}

void BBP::IO::Buffers::setStringTableSize(stringTable *table, unsigned int count)
{
	if (table == NULL)
		return; // Invalid ELF, do nothing

	if (count == table->stringCount)
		return; // no change in count, so do nothing

	if (count == 0)
	{
		// Free all chars, If it exists
		for (WORD i = 0; i < table->stringCount; i++)
			if (table->strings != nullptr)
				BBP::Services::Free(table->strings[i]);

		// We can now free the array itself too, to indicate that the data is invalid, if it exists
		if (table->strings != nullptr)
			BBP::Services::Free(table->strings);
		if (table->lengths != nullptr)
			BBP::Services::Free(table->lengths);

		// Update symbolCount
		table->stringCount = 0;

		// Set the sections variable to NULL, then return.
		table->strings = nullptr;
		table->lengths = nullptr;

		return;
	}

	if (table->strings == NULL && table->stringCount)
		return; // Invalid symbols array.

	if (count > table->stringCount)
	{
		// Create new array
		char **newArr = (char **)BBP::Services::c_alloc(count, sizeof(char *));
		unsigned int *lengths = (unsigned int *)BBP::Services::c_alloc(count, sizeof(unsigned int));

		// Copy over data from old array
		for (unsigned int i = 0; i < table->stringCount; i++)
		{
			newArr[i] = table->strings[i];
			lengths[i] = table->lengths[i];
		}

		// Alloc new data.
		for (unsigned int i = table->stringCount; i < count; i++)
		{
			newArr[i] = NULL; // Has to be assigned later
			lengths[i] = 0;
		}

		// Update string count
		table->stringCount = count;

		// Free old array, if it exists.
		if (table->strings != nullptr)
			BBP::Services::Free(table->strings);
		if (table->lengths != nullptr)
			BBP::Services::Free(table->lengths);

		// Set old array to new array and return
		table->strings = newArr;
		table->lengths = lengths;

		return;
	}

	// Create new array
	char **newArr = (char **)BBP::Services::c_alloc(count, sizeof(char *));
	unsigned int *lengths = (unsigned int *)BBP::Services::c_alloc(count, sizeof(unsigned int *));

	// Copy only relevant data from old array
	for (unsigned int i = 0; i < count; i++)
	{
		newArr[i] = table->strings[i];
		lengths[i] = table->lengths[i];
	}

	// Free old data from array
	for (unsigned int i = count; i < table->stringCount; i++)
		BBP::Services::Free(table->strings[i]); // Internally alloc'ed data must be freed manually.

	// Update string count
	table->stringCount = count;

	// Free the old array
	BBP::Services::Free(table->strings);
	BBP::Services::Free(table->lengths);

	// Replace the array, and return
	table->strings = newArr;
	table->lengths = lengths;
}

unsigned int BBP::IO::Buffers::addStringToTable(stringTable *table, char *string)
{
	if (table == NULL)
		return 0; // Invalid table

	// Increase table size by one
	BBP::IO::Buffers::setStringTableSize(table, table->stringCount + 1);

	// Calculate string length
	unsigned int length = 0;
	for (; string[length++];);



	// Create buffer with length 'length'
	char *buffer = (char *)BBP::Services::c_alloc(length + 1, sizeof(char));

	// Copy data
	for (unsigned int i = 0; i < length; i++)
		buffer[i] = string[i];
	buffer[length] = 0;


	if (string[0] == 0)
		length++;

	// Update values accordingly.
	table->strings[table->stringCount - 1] = buffer;
	table->lengths[table->stringCount - 1] = length;

	return table->stringCount - 1;
}

unsigned int BBP::IO::Buffers::addStringToTable(stringTable *table, const char *string, char ignore)
{
	if (table == NULL)
		return 0; // Invalid table

	// Increase table size by one
	setStringTableSize(table, table->stringCount + 1);

	// Store the amount of ignored characters
	unsigned int ignoredChars = 0;

	// Calculate string length, and get ignored char amount
	unsigned int length = 0;
	while (string[length++])
	{
		if (string[length] == ignore)
			ignoredChars++;
	}

	// Create buffer with length 'length - ignoredChars'
	char *buffer = (char *)BBP::Services::c_alloc(length - ignoredChars + 1, sizeof(char));

	// Copy data
	unsigned int mod = 0;
	for (unsigned int i = 0; i < length; i++)
	{
		char c = string[i];
		if (c == ignore)
			mod++;
		else
			buffer[i - mod] = c;
	}

	buffer[length - ignoredChars] = 0;

	if (string[0] == 0)
		length++;

	// Update values accordingly.
	table->strings[table->stringCount - 1] = buffer;
	table->lengths[table->stringCount - 1] = length - ignoredChars;

	return table->stringCount - 1;
}

unsigned int BBP::IO::Buffers::getStringOffset(stringTable *table, unsigned int index)
{
	if (table == NULL)
		return 0; // Invalid table

	if (index >= table->stringCount)
		return 0; // Invalid argument

	if (index == 0)
		return 0; // Return 0 if index is 0

	unsigned int cummul = 0;
	for (unsigned int i = 0; i < index; i++)
		cummul += table->lengths[i];

	return cummul;
}
