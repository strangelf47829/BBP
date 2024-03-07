#include "../include/ELSA.h"
#include "../include/ELSABackend.h"
#include "Actions.h"

namespace BBP
{
	namespace esa
	{
		static const char *const keywords[] = { "#include", "#handler", "#define", "#undef", "#ifndef", "#ifdef", "#endif", "#else", "#atomic", "#handover", "#continue",
												"static", "const", "virtual", "extern", "procedure", "end", "@@" };

		static const char *const tokens[] = { "\r", "\n", ";", "(", ")", ",", "[", "]", "{", "}", "^", "<", ">", "\"", "'", ":", "\\", "dn", "db", "dh", "di", "da", "dw" };

		static const char *const mnemonics[] = {"nil$","push$","pop$","popa$","ret$","call$",
		"callp$","syscall$",
		"movb$","movh$","movw$","mova$",
		"movbh$", "movbw$", "movba$",
		"movhb$", "movhw$", "movha$",
		"movwb$", "movwh$", "movwa$",
		"movab$", "movah$", "movaw$",
		"inc$","dec$","neg$",
		"add$","sub$","mul$","div$","mod$","incf$","decf$",
		"negf$","addf$","subf$","mulf$","divf$","modf$","addff$",
		"subff$","mulff$","divff$","modff$","addif$","subif$",
		"mulif$","divif$","modif$","lbs$","rbs$","or$","xor$",
		"and$","not$","orb$","xorb ","andb$","notb$","lbsip$","rbsip$","orip$",
		"xorip$","andip$","notip$","orbip$","xorbip$","andbip ","notbip$",
		"comp$","flag$","routine$","jmp$","jlz$","jgz$","jez$","jlt$",
		"jgt$","jlzn$","jgzn$","jezn$","jltn$","jgtn$","jmpr$","jlzr$",
		"jgzr$","jezr$","jltr$","jgtr$","jlznr$","jgznr$","jeznr$",
		"jltnr$","jgtnr$","page$","pageaw$","pagear$","pagew$",
		"pager$","palloc$","pfree$","pages$","pagebc$","passign$",
		"signal$","signalf$","hlt$","hcf$","hbu$", "palign$"};

		static constexpr std::size_t keywordCount = 18;
		static constexpr std::size_t tokenCount = 23;
		static constexpr std::size_t mnemonicsCount = 111;

		static const std::Lexer::lex_action keywordActions[] = { pragma_include, pragma_handler, pragma_define, pragma_undef, pragma_ifndef, pragma_ifdef, pragma_endif, pragma_else,
																pragma_atomic, pragma_handover, pragma_continue, kw_static, kw_const, kw_virtual, kw_extern, kw_procedure, kw_end, kw_endroutine };
		static const std::Lexer::lex_action tokenActions[] = { tk_newline, tk_newline, tk_semicolon, tk_openDelimiter, tk_closeDelimiter, tk_comma, tk_openDelimiter, tk_closeDelimiter,
																tk_openDelimiter, tk_closeDelimiter, tk_caret, tk_openDelimiter, tk_closeDelimiter, tk_doubleQuote, tk_singleQuote, tk_colon, tk_backslash,
																tk_size, tk_size, tk_size, tk_size, tk_size, tk_size };
		static const std::Lexer::lex_action mnemonicsActions[] = {mn_nil,mn_stack,mn_stack,mn_stack,mn_ctrl,\
	mn_ctrl,mn_ctrl,mn_ctrl,mn_mem,mn_mem,\
	mn_mem,mn_mem,mn_mem,mn_mem,mn_mem,mn_mem,\
	mn_mem,mn_mem,mn_mem,mn_mem,mn_mem,mn_mem,\
	mn_mem,mn_mem,mn_inc,mn_dec,mn_neg,mn_add,\
	mn_sub,mn_mul,mn_div,mn_mod,mn_inc,mn_dec,\
	mn_neg,mn_add,mn_sub,mn_mul,mn_div,mn_mod,\
	mn_add,mn_sub,mn_mul,mn_div,mn_mod,mn_add,\
	mn_sub,mn_mul,mn_div,mn_mod,mn_lbs,mn_rbs,\
	mn_or,mn_xor,mn_and,mn_not,mn_or,mn_xor,mn_and,\
	mn_not,mn_lbs,mn_rbs,mn_or,mn_xor,mn_and,\
	mn_not,mn_or,mn_xor,mn_and,mn_not,mn_flag,mn_flag,mn_ctrl,\
	mn_jmp,mn_jmpz,mn_jmpz,mn_jmpz,mn_jmpc,mn_jmpc,\
	mn_jmpz,mn_jmpz,mn_jmpz,mn_jmpc,mn_jmpc,mn_jmp,\
	mn_jmpz,mn_jmpz,mn_jmpz,mn_jmpc,mn_jmpc,mn_jmpz,\
	mn_jmpz,mn_jmpz,mn_jmpc,mn_jmpc,mn_page,mn_page,\
	mn_page,mn_page,mn_page,mn_page,mn_page,\
	mn_page,mn_page,mn_page,mn_signal,mn_signal,\
	mn_halt,mn_halt,mn_halt,mn_page};

		static const char flags[23][9] = { {true, false, false, false, false, true, true, 0, 0},
											{true, false, false, false, false, true, true, 0, 0},
											{false, false, false, true, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, false, false, true, true, true, 0, 1},
											{false, false, false, false, true, true, true, 0, 2},
											{false, false, false, false, false, true, true, 0, 0},
											{false, false, true, false, false, true, true, 0, 0},
											{false, false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, false, 0, 0} };

		esaProcessor processor;
	}
}

BBP::esa::esaProcessor::esaProcessor()
	: symbolDataByte(&symbolDataPage, 512),
	symbolDataPageHalfword(256, (std::halfword *)symbolDataPage.data),
	symbolDataHalfword(&symbolDataPageHalfword, 256),
	symbolDataPageWord(128, (std::word *)symbolDataPage.data),
	symbolDataWord(&symbolDataPageWord, 128),
	application(nullptr),
	delimiterStack(&delimiterPage, 32),
	routineStarts(&routineStartsPage, routineStartsPage.dataSize),
	subscribers(&subscribersPage, subscribersPage.dataSize)
{
	// Initialize keyword count to .
	esaParser.keywordStack.atElement = keywordCount + tokenCount + mnemonicsCount;

	// Read all keywords into parser
	for (unsigned int k = 0; k < keywordCount; k++)
		esaParser.keywordStack[k] = { keywords[k], std::strlen(keywords[k]), 0, keywordActions[k], false, false, false, false, false, false, false, 0, 0 };

	// Read instructions into parser
	for (unsigned int k = 0; k < mnemonicsCount; k++)
		esaParser.keywordStack[k + keywordCount + tokenCount] = { mnemonics[k], std::strlen(mnemonics[k]), 0, mnemonicsActions[k], false, false, false, false, false, false, false, 0, 0 };

	// Read tokens into parser
	for (unsigned int k = 0; k < tokenCount; k++)
		esaParser.keywordStack[k + keywordCount] = { tokens[k], std::strlen(tokens[k]), 0, tokenActions[k],
																	(bool)flags[k][0], (bool)flags[k][1], (bool)flags[k][2], (bool)flags[k][3], (bool)flags[k][4], (bool)flags[k][5], (bool)flags[k][6], flags[k][7], flags[k][8] };

	// Manually set some stuff
	esaParser.keywordStack[ELSE_HANDLE].considerCommentDepth = 4;
	esaParser.keywordStack[ENDIF_HANDLE].considerCommentDepth = 4;

	esaParser.abort = false;

	// Initialize system paths
	esaParser.syspaths <<= "/usr/include/";
	esaParser.syspaths <<= "/usr/local/include/";

	// Set debug information
	esaParser.debugInformation.procedure.data = procedureName.static_data;
	esaParser.debugInformation.procedure.dataSize = procedureName.dataSize;

	// Add reserved identifier
	reserveRegisters();
	reserveBuiltins();
}

BBP::std::conststring BBP::std::Lexer::lex_context::onUnkown(BBP::std::index_t from, BBP::std::index_t to)
{
	// If not defining an instruction, do this
	if (!BBP::esa::processor.lineIsDefiningInstruction)
		return BBP::esa::processor.unkown(to, from);
	return BBP::esa::processor.resolve(from, to);
}

BBP::std::conststring BBP::std::Lexer::lex_context::onNumericalLiteral(BBP::std::Lexer::numerical_t numerical, bool isFloat)
{
	if (!BBP::esa::processor.lineIsDefiningInstruction)
		return BBP::esa::processor.numerical(numerical, isFloat);
	return BBP::esa::processor.numericalArgument(numerical, isFloat);
}

BBP::std::errno_t BBP::esa::esaProcessor::translate(std::PATH &path, std::PATH& dst)
{

	setupBackend(dst.relName(), 0, 0, 0);
	esaParser.translateUnit(path);

	
	if (esaParser.illformed || esaParser.abort)
		discardAndClose();
	else
	{

		// Finalize then trim

		trim();
		finalize();

		saveAndClose();
	}

	return 0;
}

void BBP::esa::esaProcessor::trim()
{

	// We want .got, .handler, and .symtab to be unchanged
	application->builder._got().stack.atElement = application->builder._got().data.dataSize;
	application->builder._handlers().stack.atElement = application->builder._handlers().data.dataSize;
	application->builder._symtab().stack.atElement = application->builder._symtab().data.dataSize;

	// Get running length. Start it at 0
	std::size_t runningLength = application->builder.calculateHeaderSize();

	// This will ensure the order of what we are doing.
	std::PAGE<std::string_element> *p = application->builder.file.nextPage;
	while (p)
	{
		// Loop until the index of the section whose pointer this is is found
		std::index_t found = 0;
		for (found = 0; &(application->builder.sections[++found].data) != p;);

		// Get it's sizes
		std::size_t currentSize = application->builder.sections[found].data.dataSize;
		std::size_t currentStack = application->builder.sections[found].stack.atElement;
		std::size_t currentOffset = application->builder.sections[found].offset();

		// If it has no elements in the stack, set offset to 0 and stop
		if (currentStack == 0)
		{
			application->builder.sections[found].size(0);

			p = p->nextPage;
			continue;
		}

		// Set offset to running length
		application->builder.sections[found].size(currentStack);
		
		// Update running length
		runningLength += currentStack;

		// Then print
		p = p->nextPage;
	}

	

}

void BBP::esa::esaProcessor::finalize()
{

	// Set executable flags
	application->builder.segments[application->builder.textSegment].flags(0x1);
	application->builder.segments[application->builder.handlerSegment].flags(0x1);
	application->builder.segments[application->builder.gotSegment].flags(0x1);

	// Get executable segment sizes
	std::size_t textSize = application->builder._text().stack.atElement;
	std::size_t handlerSize = application->builder._handlers().size();
	std::size_t gotSize = application->builder._got().size();

	// Handler offsets is on physical address 0 because the thread will then always know how to handle errors.
	// After that comes the got. This is so that the thread knows that the GOT is always at physical address 0+255.
	// After that comes .text
	application->builder.segments[application->builder.handlerSegment].paddr(0);
	application->builder.segments[application->builder.gotSegment].paddr(handlerSize);
	application->builder.segments[application->builder.textSegment].paddr(handlerSize + gotSize);
	
	// For virtual memory it is the exact opposite. First .text, then .got, then .handlers. This is because
	// keeping .text at '0' means that the page stuff emitted by elsa is consistent with this virtual memory
	std::address_t executablePage = application->builder.text << 16;
	application->builder.segments[application->builder.textSegment].vaddr(executablePage);
	application->builder.segments[application->builder.gotSegment].vaddr(executablePage + textSize);
	application->builder.segments[application->builder.handlerSegment].vaddr(executablePage + textSize + gotSize);

	// Also update entry point if set
	if (application->builder.entry() || application->builder.type() == std::ELF::ET_EXEC)
		application->builder.entry(application->builder.entry() + executablePage);

	// Now set sizes
	application->builder.segments[application->builder.textSegment].filesz(textSize);
	application->builder.segments[application->builder.textSegment].memsz(textSize);

	application->builder.segments[application->builder.gotSegment].filesz(gotSize);
	application->builder.segments[application->builder.gotSegment].memsz(gotSize);

	application->builder.segments[application->builder.handlerSegment].filesz(handlerSize);
	application->builder.segments[application->builder.handlerSegment].memsz(handlerSize);
	
	// Now set offsets
	application->builder.segments[application->builder.textSegment].offset(application->builder._text().offset());
	application->builder.segments[application->builder.handlerSegment].offset(application->builder._handlers().offset());
	application->builder.segments[application->builder.gotSegment].offset(application->builder._got().offset());

	// Get sizes of .data, .rodata, and .bss
	std::size_t dataSize = application->builder._data().stack.atElement;
	std::size_t rodataSize = application->builder._rodata().stack.atElement;
	std::size_t bssSize = application->builder._bss().stack.atElement;

	std::address_t dataPage = application->builder.data << 16;
	std::address_t rodataPage = application->builder.rodata << 16;
	std::address_t bssPage = application->builder.bss << 16;

	// Next set .data, .rodata, .bss
	application->builder.segments[application->builder.dataSegment].filesz(dataSize);
	application->builder.segments[application->builder.dataSegment].memsz(dataSize);

	application->builder.segments[application->builder.rodataSegment].filesz(rodataSize);
	application->builder.segments[application->builder.rodataSegment].memsz(rodataSize);

	application->builder.segments[application->builder.bssSegment].filesz(0);
	application->builder.segments[application->builder.bssSegment].memsz(bssSize);

	// Also set flags
	application->builder.segments[application->builder.dataSegment].flags(0b110);
	application->builder.segments[application->builder.rodataSegment].flags(0b100);
	application->builder.segments[application->builder.bssSegment].flags(0b110);

	// Now set virtual memories
	application->builder.segments[application->builder.dataSegment].vaddr(dataPage);
	application->builder.segments[application->builder.rodataSegment].vaddr(rodataPage);
	application->builder.segments[application->builder.bssSegment].vaddr(bssPage);

	// Set offsets
	application->builder.segments[application->builder.dataSegment].offset(application->builder._data().offset());
	application->builder.segments[application->builder.rodataSegment].offset(application->builder._rodata().offset());
	application->builder.segments[application->builder.bssSegment].offset(application->builder._bss().offset());

	// Set physical addresses. First comes rodata, then data, then bss
	std::index_t executableSize = handlerSize + gotSize + textSize;
	application->builder.segments[application->builder.dataSegment].paddr(executableSize + rodataSize);
	application->builder.segments[application->builder.rodataSegment].paddr(executableSize);
	application->builder.segments[application->builder.bssSegment].paddr(executableSize + rodataSize + dataSize);
	
	// Set .data and .bss to Thread local data
	application->builder.segments[application->builder.dataSegment].type(std::ELF::PT_TLS);
	application->builder.segments[application->builder.bssSegment].type(std::ELF::PT_TLS);




}
