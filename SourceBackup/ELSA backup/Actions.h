#include "../include/Lex.h"

#ifndef BBP_ELSA_ACTIONS_H
#define BBP_ELSA_ACTIONS_H

#define ACTION(name) BBP::std::conststring kw_##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)
#define TOKEN(name) BBP::std::conststring tk_##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)
#define PRAGMA(name) BBP::std::conststring pragma_##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)
#define INSTRUCTION(name) BBP::std::conststring mn_##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)

#define INCLUDE_HANDLE 0
#define HANDLER_HANDLE 1
#define DEFINE_HANDLE 2
#define UNDEF_HANDLE 3
#define IFNDEF_HANDLE 4
#define IFDEF_HANDLE 5
#define ENDIF_HANDLE 6
#define ELSE_HANDLE 7
#define ATOMIC_HANDLE 8
#define HANDOVER_HANDLE 9
#define CONTINUE_HANDLE 10

#define STATIC_HANDLE 11
#define CONST_HANDLE 12
#define VIRTUAL_HANDLE 13
#define EXTERN_HANDLE 14
#define PROCEDURE_HANDLE 15
#define END_HANDLE 16
#define LABEL_HANDLE 17

#define MOV_HANDLE 18
#define ADD_HANDLE 19
#define SUB_HANDLE 20

#define CARRIAGERETURN_HANDLE 21
#define NEWLINE_HANDLE 22
#define SEMICOLON_HANDLE 23
#define OPENBRACKET_HANDLE 24
#define CLOSEBRACKET_HANDLE 25
#define COMMA_HANDLE 26
#define OPENSQBRACKET_HANDLE 27
#define CLOSESQBRACKET_HANDLE 28
#define OPENCURLBRACKET_HANDLE 29
#define CLOSECURLBRACKET_HANDLE 30
#define PERCENT_HANDLE 31
#define OPENANGLEBRACKET_HANDLE 32
#define CLOSEANGLEBRACKET_HANDLE 33
#define DOUBLEQUOTES_HANDLE 34
#define SINGLEQUOTES_HANDLE 35
#define COLON_HANDLE 36
#define BACKSLASH_HANDLE 37

#define DN_HANDLE 38
#define DB_HANDLE 39
#define DH_HANDLE 40
#define DI_HANDLE 41
#define DA_HANDLE 42
#define DW_HANDLE 43

namespace BBP
{

	namespace esa
	{

		// ESA directives
		PRAGMA(include);		// Analogous to #include
		PRAGMA(handler);	// See documentation
		PRAGMA(define);		// Defines a macro, but also keeps track of which ones are defined
		PRAGMA(undef);		// Undefines a macro
		PRAGMA(ifndef);		// If a macro is not defined, go ahead
		PRAGMA(ifdef);		// If a macro is defined, go ahead
		PRAGMA(endif);		// Stop skipping stuff
		PRAGMA(else);		// Do the other thing from before.
		PRAGMA(atomic);		// Force the processor to stay on this thread
		PRAGMA(handover);	// Force the processor to move to another thread here
		PRAGMA(continue);	// Stop forcing the processor to do stuff

		// The keywords used
		ACTION(static);
		ACTION(const);
		ACTION(virtual);
		ACTION(extern);
		ACTION(procedure);
		ACTION(end);
		ACTION(label);

		// Instructions
		INSTRUCTION(mov);
		INSTRUCTION(add);
		INSTRUCTION(sub);

		// Tokens used
		TOKEN(newline);
		TOKEN(semicolon);
		TOKEN(openDelimiter);
		TOKEN(closeDelimiter);
		TOKEN(comma);
		TOKEN(percent);
		TOKEN(doubleQuote);
		TOKEN(singleQuote);
		TOKEN(colon);
		TOKEN(backslash);
		TOKEN(size);
		
	}

}

#undef ACTION
#undef TOKEN
#undef PRAGMA
#undef INSTRUCTION
#define ACTION(name) BBP::std::conststring BBP::esa::kw_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define TOKEN(name) BBP::std::conststring BBP::esa::tk_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define PRAGMA(name) BBP::std::conststring BBP::esa::pragma_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define INSTRUCTION(name) BBP::std::conststring BBP::esa::mn_##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)
#define AUX(name) BBP::std::conststring BBP::esa::##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)

#endif