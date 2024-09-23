#include "../include/ELSA/ELF.h"

// Constructor
BBP::ELF::ELF::ELF(BBP::std::PAGE<elsa::Section *> *sects)
	: sections(sects)
{
	// Use 32 bit little endian
	header.ident.littleEndian = true;
	header.ident.thirtyTwoBit = true;

	// Tell the ELF we have one entry
	header.e_shentsize = 40;
	header.e_shoff = 52;
	header.e_shnum = 1;

	// Set segment size
	header.e_phentsize = 32;

	// Create empty null-section
	std::PAGE<std::byte> nullPage;
	sectionTable.Allocate(nullPage, 40);
	for (std::index_t idx = 0; idx < 40; idx++) nullPage[idx] = 0;

	// Then allocate a nullstring for the null-section
	sectionStringTable.Allocate(nullPage, 1);
	nullPage[0] = 0;
}

BBP::std::index_t BBP::ELF::ELF::registerSection(elsa::Section &section, BBP::ELF::Section &sectionData, std::string name)
{
	// Check if sections is available, otherwise do nothing
	if (sections == nullptr)
		return 0;

	// Get index
	std::index_t idx = header.e_shnum - 1;

	// Then increase shnum
	header.e_shnum++;

	// Set section thingy
	(*sections)[idx] = &section;

	// Now create a new section in sectionTable
	sectionTable.Allocate(sectionData.data, 40);

	// Also get offset in string table
	std::offset_t nameOffset = sectionStringTable.size();

	// Then get name length
	std::size_t nameLength = std::strlen(name) + 1;

	// Create string to copy name into
	std::PAGE<std::byte> dest;

	// Then Allocate that length + 1
	sectionStringTable.Allocate(dest, nameLength);

	// Then copy string
	for (std::index_t idx_name = 0; idx_name < nameLength; idx_name++)
		dest[idx_name] = name[idx_name];

	// And then set relevant data
	sectionData.name = nameOffset;

	// Then save
	sectionData.writeData(header.ident.littleEndian);

	// Then return index
	return idx;
}

BBP::std::offset_t BBP::ELF::ELF::pack()
{
	// Packing means taking each section and putting them in place, sequentially
	// This allows for the ELF object to be saved

	// If sections page does not exist do nothing
	if (sections == nullptr)
		return 0;

	// Address indicates current position, which advances with each section
	std::address_t atPosition = header.e_ehsize;

	// If ELF has any sections,
	if (header.e_shnum != 0)
	{
		// Then set shoff at position
		header.e_shoff = atPosition;

		// Then advance position with amount of headers and its size
		atPosition += (header.e_shnum * header.e_shentsize);
	}
	else
	{
		header.e_shoff = 0;
		header.e_shentsize = 0;
	}

	// Else, if ELF has any segments
	if (header.e_phnum != 0)
	{
		// Then set phoff at position
		header.e_phoff = atPosition;

		// Then advance position with amount of segments and its size
		atPosition += (header.e_phnum * header.e_phentsize);
	}
	else
	{
		header.e_phoff = 0;
		header.e_phentsize = 0;
	}

	// Write data
	header.writeHeaderFields();

	// Create sectionTable
	BBP::ELF::Section sectionData;

	// Now loop over each section (skip over first since first is always null)
	for (std::index_t idx = 1; idx < header.e_shnum; idx++)
	{
		// If sections contains no information, do nothing
		if ((*sections)[idx - 1] == nullptr)
			continue;

		// Create section data
		sectionData.data = std::PAGE<std::byte>(40, &sectionTable[idx * 40]);

		// Now read values
		sectionData.readData(header.ident.littleEndian);

		// Then update size information
		sectionData.size = (*sections)[idx - 1]->size();

		// If align in set to 0, set position data here
		if (sectionData.addralign == 0)
		{
			sectionData.offset = atPosition;
			atPosition += sectionData.size;
		}

		// Otherwise, find next suitable place
		else
		{
			// Which is: 
			sectionData.offset = atPosition + (sectionData.addralign - (atPosition % sectionData.addralign));
			atPosition += (sectionData.addralign - (atPosition % sectionData.addralign)) + sectionData.size;
		}

		// Then write
		sectionData.writeData(header.ident.littleEndian);

	}

	// Now return position
	return atPosition;

}

void BBP::ELF::ELF::saveFile(std::conststring path)
{
	// string table data
	BBP::ELF::Section stringTableData;

	// Register string table
	header.e_shstrndx = 1 + registerSection(sectionStringTable, stringTableData, ".shstrtab");

	// Then change a whole bunch of stuff
	stringTableData.type = 3;

	// Write stuff
	header.writeHeaderFields();
	stringTableData.writeData(header.ident.littleEndian);

	// Calculate file size
	std::size_t fileSize = pack();

	// Create a page and allocator
	std::pageAllocator allocator;
	std::PAGE<std::string_element> bytes;

	// Then allocate that amount
	bytes = std::PAGE<std::string_element>(fileSize, (std::string_element *)allocator.calloc(fileSize, sizeof(std::string_element)));

	// Then create a stack
	std::Stack<std::string_element> stack = &bytes;

	// Then a file
	std::FILE file(stack, path);

	// Now copy first 52 bytes
	for (std::index_t idx = 0; idx < 52; idx++)
		bytes[idx] = header[idx];

	// Then copy section information
	for (std::index_t idx = 0; idx < header.e_shnum * header.e_shentsize; idx++)
		bytes[idx + header.e_shoff] = sectionTable[idx];

	// Then copy segment information
	for (std::index_t idx = 0; idx < header.e_phnum * header.e_phentsize; idx++)
		bytes[idx + header.e_phoff] = segmentTable[idx];

	// Create sectionTable
	BBP::ELF::Section sectionData;

	// Now loop over each section (skip over first since first is always null)
	for (std::index_t idx = 1; idx < header.e_shnum; idx++)
	{
		// If sections contains no information, do nothing
		if ((*sections)[idx - 1] == nullptr)
			continue;

		// Create section data
		sectionData.data = std::PAGE<std::byte>(40, &sectionTable[idx * 40]);

		// Now read values
		sectionData.readData(header.ident.littleEndian);

		// Copy over each byte
		for (std::index_t bidx = 0; bidx < sectionData.size; bidx++)
			bytes[bidx + sectionData.offset] = (*(*sections)[idx - 1])[bidx];
	}

	// Then set filesize
	stack.atElement = stack.max_elements;
	file.writeFileToDisk();
}

// Find a section based on a name
BBP::std::index_t BBP::ELF::ELF::find(std::string str)
{
	// Create section data
	BBP::ELF::Section sectionData;

	// Static data
	std::STATIC_PAGE<std::byte, 40> sectionDataStatic;

	// Then assign
	sectionData.data = sectionDataStatic;

	// Get string name
	std::size_t nameLength = std::strlen(str);

	// For each section count,
	for (BBP::std::index_t idx = 1; idx < header.e_shnum; idx++)
	{
		// Copy over bytes into sectionData
		for (std::index_t bidx = 0; bidx < header.e_shentsize; bidx++)
			sectionData.data[bidx] = sectionTable[bidx + header.e_shentsize * idx];

		// Then read
		sectionData.readData(header.ident.littleEndian);

		// Now check for name,
		std::offset_t name = sectionData.name;

		// Then compare name
		for (std::index_t nidx = 0; nidx < nameLength; nidx++)
			if (str[nidx] != sectionStringTable[nidx + name])
				goto loopend; // Break out of two loops
		
		// Name coincides, return index
		return idx;

	loopend:
		continue;
	}

	// Return 0
	return 0;
}

// Find a section based on index
BBP::elsa::Section *BBP::ELF::ELF::find(std::index_t idx)
{
	// If section is null, return nothing
	if (sections == nullptr)
		return nullptr;

	// Otherwise if index is 0, return nothing
	if (idx == 0)
		return nullptr;

	// Otherwise return based on index
	return (*sections)[idx - 1];
}

// Find a section based on name
BBP::elsa::Section *BBP::ELF::ELF::get(std::string str)
{
	return find(find(str));
}