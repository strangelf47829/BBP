#include "../include/ELSA/TranslationUnit.h"
#include "../include/ELSA/ELFRelocation.h"

void BBP::elsa::TranslationUnit::emitRelocation()
{
	// This is the page for the relocation
	ELF::Relocation relocation;

	// Get relocation
	application.getRelocationPage(relocation.data);

	// Then nullify
	relocation.Nullify();

	// Now set something random idrc
	relocation.value = 0;

	// Then write
	relocation.writeData(true);
}
