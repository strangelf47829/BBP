#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/ELSA/Symbol.h"
#include "../include/Hashmap.h"
#include "../include/Concepts.h"
#include "../include/Dictionary.hxx"
#include "../include/ELSA/Numerical.h"

BBP::elsa::symbol_db symboldb;
BBP::elsa::Numerical_t a = 2;
BBP::elsa::Numerical_t b = 3;

BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	// Initialize a new symbol, and get its handle
	std::index_t idx = symboldb.initSymbol();
	
	symboldb.setActiveSymbol("symbol");

	a = -a;

	/*
	std::ResourceManager *allocator = getKernelInstance().getCurrentSystemContext()->activemem;

	// Initialize database with 4 symbols.
	database.init(allocator, 4);

	// Create 3 new symbols
	BBP::elsa::symbol_t *a = database.nextSymbol();
	BBP::elsa::symbol_t *b = database.nextSymbol();
	BBP::elsa::symbol_t *c = database.nextSymbol();
	BBP::elsa::symbol_t *d = database.nextSymbol();

	// Create symbol 'a', and give it a constant value of 42 (byte)
	a->setUniqueName("a");
	a->setConstantValue(5, std::DYN_PAGE::Alignment::Byte);

	// Create symbol 'b', set it to byte value and write {data}
	std::string data = std::String("Some dummy data");

	b->setUniqueName("b");
	b->setElementSize(std::DYN_PAGE::Alignment::Byte);
	b->writeUniqueData(data);

	// Also using preallocation
	std::index_t preallocatedIndex = b->preallocate(data.dataSize + 1);
	
	// Then write individual bytes
	b->write('!', preallocatedIndex);
	b->write(data, preallocatedIndex + 1);

	// Create object 'c', and link it to a.
	c->setUniqueName("c");
	database.createSymbolLink(c, a);

	// Create empty symbol 'd'
	d->setUniqueName("d");
	d->setUndefined();

	// Resolve 'd'
	std::index_t d_idx = database.find("d");
	elsa::symbol_t *d_sym = database.fetch(d_idx); // database.resolve("d");

	// Then overwrite (with shallow data)
	std::string fileName = std::String("a.out");
	d_sym->writeShallowData(fileName);

	// Now set debug symbol info
	a->definedIn(d_sym);
	b->definedIn(d_sym);
	c->definedIn(d_sym);

	*/

	return 0;
}
