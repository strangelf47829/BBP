#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Resources.h"
#include "../include/ELF.h"
#include "../include/FileSys.h"
#include "../include/Lex.h"
#include "../include/CPPApplications.h"
#include "../include/Executable.h"
#include "../include/System.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/System.h"
#include "../include/Locale.h"
#include "../include/ELSA.h"
#include "../include/Threading.h"
#include "../include/Hyperv.h"
#include "../include/StateMachine.h"

BBP::userspace::HyperVisor hypervisor;
BBP::userspace::HyperVisor hypervisor2;
BBP::userspace::StateMachine state;


BBP::std::word printcall(BBP::userspace::StateMachine *state, BBP::userspace::HyperVisor *, BBP::userspace::Thread *thr, BBP::userspace::Instruction &Args)
{
	// Get a page
	BBP::userspace::xvalue val = BBP::userspace::xvalue(*state, Args.args[1]);
	BBP::std::c_string str = thr->executable.BinaryData.data + val.getOwnPhysicalAddress();
	BBP::std::word count = BBP::std::printf("%s", str);

	return count;
}


int BBP::smile_main(int argc, char **argv)
{


	/*
	
	std::window *w = &BBP::std::kernelDisplay;

	for (std::index_t idx = 0; idx < points - 1; idx++)
		std::R2D::Line(w, xpoints[idx], ypoints[idx], xpoints[idx + 1], ypoints[idx + 1]);
	std::R2D::Line(w, xpoints[0], ypoints[0], xpoints[points - 1], ypoints[points - 1]);

	std::R2D::Line(w, 770, 405, 770, 420);
	std::R2D::Line(w, 780, 405, 780, 420);

	std::R2D::Line(w, 740, 470, 810, 470);
	std::R2D::Line(w, 740, 470, 735, 465);
	std::R2D::Line(w, 810, 470, 815, 465);

	

	std::pos_t xo = w->cursorPosX;
	std::pos_t yo = w->cursorPosY;
	std::pos_t co = w->coloumn;
	std::pos_t lo = w->line;

	w->fontSize = 5;
	w->cursorPosX = 500;
	w->cursorPosY = 200;
	w->coloumn = 0;
	w->line = 0;
	const char *hello = "\xe1\xe1!\n";

	std::R2D::print(w, hello);

	w->fontSize = 0;
	w->cursorPosX = 0;
	w->cursorPosY = yo;
	w->coloumn = co;
	w->line = lo;
	
	*/

	// EmbeddabLe Simple Assembler
	// Roger Bemelmans
	std::PATH testFile("/boot/ELSA.esa");
	std::PATH dstFile("/boot/ELSA.o");

	// Compile source
	esa::processor.translate(testFile, dstFile);

	// Now try and deconstruct it
	std::FILE compiledFile(dstFile);

	// Get processor
	std::ELF::ELFBuilder compiled(compiledFile.b().page, std::activemem);


	// Set the correspondig syscall
	hypervisor.systemcalls.data[0] = (BBP::userspace::HyperVisor::syscall_t)printcall;




	// Get zero-th thread
	userspace::Thread *t = hypervisor.spawnThread(compiled, std::activemem);

	// Unload binary since we have a copy
	compiled.close();
	compiledFile.close();


	// Set active hypervisor
	state.setActiveHypervisor(&hypervisor);
	state.setActiveThread(t);

	hypervisor.advanceThread(state, 10);

	// Close
	hypervisor.destroyThread(0);


	/*

	std::PATH bootPath("/boot/test.lua");
	lua::initializeParser();
	lua::luaParser.parseFile(bootPath);

	*/



	/*
	// Emit header
	builder.emitHeader();

	builder.shstrtab = 1;
	builder.strtab = 2;
	builder.symtab = 3;

	builder.sections[0].type(std::ELF::SHT_NILL);

	builder._shstrtab().Append(64);
	builder._shstrtab().stack++;
	builder._shstrtab().stack <<= ".shstrtab";
	builder._shstrtab().type(std::ELF::SHT_STRTAB);
	builder._shstrtab().name(1);

	builder._strtab().Append(64);
	builder._strtab().name(builder._shstrtab().stack.atElement);
	builder._shstrtab().stack <<= ".strtab";
	builder._strtab().type(std::ELF::SHT_STRTAB);

	builder._symtab().Append(32);
	builder._symtab().type(std::ELF::SHT_SYMTAB);
	builder._symtab().name(builder._shstrtab().stack.atElement);
	builder._symtab().info(builder.strtab);
	builder._symtab().link(builder.strtab);
	builder._symtab().entsize(16);
	builder._strtab().stack++;
	builder._shstrtab().stack <<= ".symtab";

	builder.shstrndx(1);

	builder._shstrtab().Extend(16);

	*/
	
	/*
	std::ELF::ELFBuilder builder(std::activemem, 0, 13, 3);
	std::Stack<std::string_element> builderStack(&builder.file, std::seqlen(builder.file));
	std::FILE file(builderStack, "/bin/test.o");

	const char *msg = "Hello, world!";

	builder.formStandard(24, 14, 0, 0);

	builder.renameSymbol(1, "Hello");
	builder.renameSymbol(2, "HoHo");

	builder._data().stack <<= "Hello, World!";

	file.writeFileToDisk();
	builder.close();
	file.close();
	*/

	/*

	std::ESAM::dataEntry data("Hello, World!", std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);
	std::ESAM::dataEntry data2("4 + 5 is %d\n", std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);

	std::ESAM::dataEntry program("\x32\xa1\xa2", std::ELF::SYM_FILE | std::ELF::SYM_GLOBAL);
	std::ESAM::dataEntry program2("\xFF\xFF\xFF", std::ELF::SYM_SECTION | std::ELF::SYM_GLOBAL);


	std::ESAM::BinaryApplication application("/bin/test.o", 0, 0, 4);
	application.setSource("/home/test.esa");
	application.emitSymbol(application.builder.rodata, data, "myString");
	application.emitSymbol(application.builder.rodata, data2, "fmtString");
	application.emitSymbol(application.builder.text, program, "_ZfooIi");
	application.emitSymbol(application.builder.text, program2, "_ZfffIi");
	application.save();
	application.close();

	std::ESAM::BinaryApplication application2("/bin/test2.o", 0, 0, 4);
	application2.setSource("/home/test.esa");
	application2.emitSymbol(application2.builder.rodata, data, "myString");
	application2.emitSymbol(application2.builder.rodata, data2, "fmtString");
	application2.emitSymbol(application2.builder.text, program, "_ZfooIi");
	application2.emitSymbol(application2.builder.text, program2, "_ZfffIi");
	application2.appendSymbols(1);
	application2.emitSymbol(application2.builder.text, program2, "_ZfofIi");
	application2.save();
	application2.close();

	*/


	return 0;
}
