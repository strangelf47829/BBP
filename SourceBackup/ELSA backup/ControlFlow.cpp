#include "Actions.h"
#include "../include/ELSA.h"


namespace BBP
{
	namespace esa
	{
		static const char *const keywords[] = { "#include", "#handler", "#define", "#undef", "#ifndef", "#ifdef", "#endif", "#else", "#atomic", "#handover", "#continue",
												"static", "const", "virtual", "extern", "procedure", "end", "label"};
		static const char *const mnemonics[] = { "mov", "add", "sub" };

		static const char *const tokens[] = { "\r", "\n", ";", "(", ")", ",", "[", "]", "{", "}", "%", "<", ">", "\"", "'", ":", "\\", "dn", "db", "dh", "di", "da", "dw"};

		static constexpr std::size_t keywordCount = 18;
		static constexpr std::size_t mnemonicsCount = 3;
		static constexpr std::size_t tokenCount = 23;

		static const std::Lexer::lex_action keywordActions[] = { pragma_include, pragma_handler, pragma_define, pragma_undef, pragma_ifndef, pragma_ifdef, pragma_endif, pragma_else,
																pragma_atomic, pragma_handover, pragma_continue, kw_static, kw_const, kw_virtual, kw_extern, kw_procedure, kw_end, kw_label};
		static const std::Lexer::lex_action mnemonicsActions[] = { mn_mov, mn_add, mn_sub };
		static const std::Lexer::lex_action tokenActions[] = { tk_newline, tk_newline, tk_semicolon, tk_openDelimiter, tk_closeDelimiter, tk_comma, tk_openDelimiter, tk_closeDelimiter,
																tk_openDelimiter, tk_closeDelimiter, tk_percent, tk_openDelimiter, tk_closeDelimiter, tk_doubleQuote, tk_singleQuote, tk_colon, tk_backslash,
																tk_size, tk_size, tk_size, tk_size, tk_size, tk_size};
		
		static const char flags[23][8] = {	{true, false, false, false, false, true, 0, 0}, 
											{true, false, false, false, false, true, 0, 0},
											{false, false, false, true, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, false, true, 0, 0},
											{false, false, false, false, true, true, 0, 1},
											{false, false, false, false, true, true, 0, 2},
											{false, false, false, false, false, true, 0, 0},
											{false, false, true, false, false, true, 0, 0},
											{false, false, false, false, false, false, 0, 0},
											{false, false, false, false, false, false, 0, 0}, 
											{false, false, false, false, false, false, 0, 0}, 
											{false, false, false, false, false, false, 0, 0}, 
											{false, false, false, false, false, false, 0, 0}, 
											{false, false, false, false, false, false, 0, 0} };
		
	}
}

BBP::std::Lexer::lex_context BBP::esa::esaParser;

BBP::std::conststring numerical(BBP::std::Lexer::numerical_t numerical, bool isFloat)
{
	if (isFloat)
		BBP::std::printf("Adding float: (0x%08x)\t%f\n", *((int *)&numerical.f), numerical.f);
	else
		BBP::std::printf("Adding integer: (0x%08x)\t%d\n", numerical.i, numerical.i);

	return nullptr;
}

BBP::std::conststring unkown(BBP::std::Lexer::lex_context *context, BBP::std::index_t at, BBP::std::Lexer::lex_keywordhandle lastspace)
{

	context->debugInformation.highlight = lastspace;
	context->debugInformation.highlightLength = at - lastspace;
	context->debugInformation.highlightmsg = "NHere";

	return "EUnrecognized character sequence.";
}

void BBP::esa::initializeParser()
{

	// Initialize keyword count to .
	esaParser.keywordStack.atElement = keywordCount + tokenCount + mnemonicsCount;

	// Read all keywords into parser
	for (unsigned int k = 0; k < keywordCount; k++)
		esaParser.keywordStack[k] = { keywords[k], std::strlen(keywords[k]), 0, keywordActions[k], false, false, false, false, false, false, 0, 0 };

	// Read instructions into parser
	for (unsigned int k = 0; k < mnemonicsCount; k++)
		esaParser.keywordStack[k + keywordCount] = { mnemonics[k], std::strlen(mnemonics[k]), 0, mnemonicsActions[k], false, false, false, false, false, false, 0, 0 };

	// Read tokens into parser
	for (unsigned int k = 0; k < tokenCount; k++)
		esaParser.keywordStack[k + keywordCount + mnemonicsCount] = { tokens[k], std::strlen(tokens[k]), 0, tokenActions[k], 
																	(bool)flags[k][0], (bool)flags[k][1], (bool)flags[k][2], (bool)flags[k][3], (bool)flags[k][4], (bool)flags[k][5], flags[k][6], flags[k][7] };

	// Manually set some stuff
	esaParser.keywordStack[ELSE_HANDLE].considerCommentDepth = 4;
	esaParser.keywordStack[ENDIF_HANDLE].considerCommentDepth = 4;

	esaParser.abort = false;
	
	// Set special functions
	esaParser.onNumericalLiteral = numerical;
	esaParser.onUnkown = unkown;

	// Initialize system paths
	esaParser.syspaths <<= "/usr/include/";
	esaParser.syspaths <<= "/usr/local/include/";

}
