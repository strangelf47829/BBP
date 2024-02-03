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

#define CARRIAGERETURN_HANDLE 18
#define NEWLINE_HANDLE 19
#define SEMICOLON_HANDLE 20
#define OPENBRACKET_HANDLE 21
#define CLOSEBRACKET_HANDLE 22
#define COMMA_HANDLE 23
#define OPENSQBRACKET_HANDLE 24
#define CLOSESQBRACKET_HANDLE 25
#define OPENCURLBRACKET_HANDLE 26
#define CLOSECURLBRACKET_HANDLE 27
#define PERCENT_HANDLE 28
#define OPENANGLEBRACKET_HANDLE 29
#define CLOSEANGLEBRACKET_HANDLE 30
#define DOUBLEQUOTES_HANDLE 31
#define SINGLEQUOTES_HANDLE 32
#define COLON_HANDLE 33
#define BACKSLASH_HANDLE 34

#define DN_HANDLE 35
#define DB_HANDLE 36
#define DH_HANDLE 37
#define DI_HANDLE 38
#define DA_HANDLE 39
#define DW_HANDLE 40

// NIL instructions
#define NIL_HANDLE 41

// STACK instructions
#define PUSH_HANDLE 42
#define POP_HANDLE 43
#define POPA_HANDLE 44

// CTRL instructions
#define RET_HANDLE 45
#define CALL_HANDLE 46
#define CALLP_HANDLE 47
#define SYSCALL_HANDLE 48

// MEM instructions
#define MOVB_HANDLE 49
#define MOVH_HANDLE 50
#define MOVW_HANDLE 51
#define MOVA_HANDLE 52

#define MOVBH_HANDLE 53
#define MOVBW_HANDLE 54
#define MOVBA_HANDLE 55

#define MOVHB_HANDLE 56
#define MOVHW_HANDLE 57
#define MOVHA_HANDLE 58

#define MOVWB_HANDLE 59
#define MOVWH_HANDLE 60
#define MOVWA_HANDLE 61

#define MOVAB_HANDLE 62
#define MOVAH_HANDLE 63
#define MOVAW_HANDLE 64


// MATH instructions
#define INC_HANDLE 65
#define DEC_HANDLE 66
#define NEG_HANDLE 67
#define ADD_HANDLE 68
#define SUB_HANDLE 69
#define MUL_HANDLE 70
#define DIV_HANDLE 71
#define MOD_HANDLE 72
#define INCF_HANDLE 73
#define DECF_HANDLE 74
#define NEGF_HANDLE 75
#define ADDF_HANDLE 76
#define SUBF_HANDLE 77
#define MULF_HANDLE 78
#define DIVF_HANDLE 79
#define MODF_HANDLE 80
#define ADDFF_HANDLE 81
#define SUBFF_HANDLE 82
#define MULFF_HANDLE 83
#define DIVFF_HANDLE 84
#define MODFF_HANDLE 85
#define ADDIF_HANDLE 86
#define SUBIF_HANDLE 87
#define MULIF_HANDLE 88
#define DIVIF_HANDLE 89
#define MODIF_HANDLE 90

#define LBS_HANDLE 91
#define RBS_HANDLE 92
#define OR_HANDLE 93
#define XOR_HANDLE 94
#define AND_HANDLE 95
#define NOT_HANDLE 96
#define ORB_HANDLE 97
#define XORB_HANDLE 98
#define ANDB_HANDLE 99
#define NOTB_HANDLE 100
#define LBSIP_HANDLE 101
#define RBSIP_HANDLE 102
#define ORIP_HANDLE 103
#define XORIP_HANDLE 104
#define ANDIP_HANDLE 105
#define NOTIP_HANDLE 106
#define ORBIP_HANDLE 107
#define XORBIP_HANDLE 108
#define ANDBIP_HANDLE 109
#define NOTBIP_HANDLE 110

// FLAG instructions
#define COMP_HANDLE 111
#define FLAG_HANDLE 112

// Extended CTRL instructions
#define ROUTINE_HANDLE 113

// JUMP instructions
#define JMP_HANDLE 114
#define JLZ_HANDLE 115
#define JGZ_HANDLE 116
#define JEZ_HANDLE 117
#define JLT_HANDLE 118
#define JGT_HANDLE 119
#define JLZN_HANDLE 120
#define JGZN_HANDLE 121
#define JEZN_HANDLE 122
#define JLTN_HANDLE 123
#define JGTN_HANDLE 124
#define JMPR_HANDLE 125
#define JLZR_HANDLE 126
#define JGZR_HANDLE 127
#define JEZR_HANDLE 128
#define JLTR_HANDLE 129
#define JGTR_HANDLE 130
#define JLZNR_HANDLE 131
#define JGZNR_HANDLE 132
#define JEZNR_HANDLE 133
#define JLTNR_HANDLE 134
#define JGTNR_HANDLE 135

// PAGE instructions
#define PAGE_HANDLE 136
#define PAGEAW_HANDLE 137
#define PAGEAR_HANDLE 138
#define PAGEW_HANDLE 139
#define PAGER_HANDLE 140
#define PALLOC_HANDLE 141
#define PFREE_HANDLE 142
#define PAGES_HANDLE 143
#define PAGEBC_HANDLE 144
#define PASSIGN_HANDLE 145

// SIGNAL instructions
#define SIGNAL_HANDLE 146
#define SIGNALF_HANDLE 147

// HALT instructions
#define HLT_HANDLE 148
#define HCF_HANDLE 149
#define HBU_HANDLE 150

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
		ACTION(endroutine);

		// Tokens used
		TOKEN(newline);
		TOKEN(semicolon);
		TOKEN(openDelimiter);
		TOKEN(closeDelimiter);
		TOKEN(comma);
		TOKEN(caret);
		TOKEN(doubleQuote);
		TOKEN(singleQuote);
		TOKEN(colon);
		TOKEN(backslash);
		TOKEN(size);


		// Instructions
		INSTRUCTION(nil);
		INSTRUCTION(stack);
		INSTRUCTION(ctrl);
		INSTRUCTION(mem);
		INSTRUCTION(inc);
		INSTRUCTION(dec);
		INSTRUCTION(neg);
		INSTRUCTION(add);
		INSTRUCTION(sub);
		INSTRUCTION(mul);
		INSTRUCTION(div);
		INSTRUCTION(mod);
		INSTRUCTION(lbs);
		INSTRUCTION(rbs);
		INSTRUCTION(or );
		INSTRUCTION(xor);
		INSTRUCTION(and);
		INSTRUCTION(not);
		INSTRUCTION(flag);
		INSTRUCTION(jmp);
		INSTRUCTION(jmpc);
		INSTRUCTION(jmpz);
		INSTRUCTION(page);
		INSTRUCTION(signal);
		INSTRUCTION(halt);



	}

}

#undef ACTION
#undef TOKEN
#undef PRAGMA
#undef INSTRUCTION
#define ACTION(name) BBP::std::conststring BBP::esa::kw_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define TOKEN(name) BBP::std::conststring BBP::esa::tk_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define PRAGMA(name) BBP::std::conststring BBP::esa::pragma_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define INSTRUCTION(name) BBP::std::conststring BBP::esa::mn_##name (BBP::std::Lexer::lex_context *context, BBP::std::index_t index, BBP::std::Lexer::lex_keywordhandle handle)
#define AUX(name) BBP::std::conststring BBP::esa::##name (BBP::std::Lexer::lex_context *, BBP::std::index_t, BBP::std::Lexer::lex_keywordhandle)

#endif