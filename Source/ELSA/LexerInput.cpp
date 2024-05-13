#include "../include/ELSA/Lexer.h"
#include "../include/FileSys.h"

BBP::elsa::LexerInputStream_t::LexerInputStream_t(std::FILE &file)
{
	stk = &file.b();
}

BBP::std::string_element BBP::elsa::LexerInputStream_t::Consume()
{
	

}