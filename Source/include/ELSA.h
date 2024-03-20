#include "FileSys.h"
#include "Lex.h"
#include "ELSABackend.h"
#include "Instructions.h"
#include "Opcodes.h"

#ifndef BBP_STDLIB_ELSA
#define BBP_STDLIB_ELSA

namespace BBP
{
	namespace esa
	{

		class esaProcessor
		{
		public:

			// The parser, and its initializer.
			std::Lexer::lex_context esaParser;
			esaProcessor();

			void trim();
			void finalize();

			// Reset the state of the processor to its initial values.
			void reset();

			// Public API
			std::errno_t translate(std::PATH &, std::PATH& );

			// Secondary functions
			std::conststring unkown(std::index_t from, std::index_t to);
			std::conststring resolve(std::index_t from, std::index_t to);
			std::conststring numerical(std::Lexer::numerical_t numerical, bool isFloat);
			std::conststring numericalArgument(std::Lexer::numerical_t numerical, bool isFloat);
			void reserveRegisters();
			void reserveBuiltins();

			// Linker stuff
			void emitRelocation(std::index_t, std::byte);
			std::word calculateArgumentOffset();
			std::word calculateAbsoluteArgumentOffset();

			// Symbol building Variables
			std::Lexer::lex_thing symbol;
			std::STATIC_PAGE<std::byte, 512> symbolDataPage;
			std::Stack<std::byte> symbolDataByte;
			std::PAGE<std::halfword> symbolDataPageHalfword;
			std::Stack<std::halfword> symbolDataHalfword;
			std::PAGE<std::word> symbolDataPageWord;
			std::Stack<std::word> symbolDataWord;

			// Instruction building variables
			userspace::Instruction instruction;
			std::byte atArgument;
			bool surpress[8];

			// Procedure building variables
			std::index_t startedAt;
			std::index_t endedAt;
			std::hash_t procedureHash;
			std::STATIC_PAGE<std::string_element, 100> procedureName;
			std::index_t procedureNameStart;
			std::index_t procedureNameEnd;
			std::byte procedureArgumentCount;
			bool doneDeclaringProcedure;
			bool externProcedure;

			// Generic Symbol Stuff
			std::STATIC_PAGE<std::hash_t, 256> reservedIdentifiers;
			std::STATIC_PAGE<userspace::Instruction::Arg, 256> reservedIdentifiersValues;
			std::STATIC_PAGE<std::hash_t, 256> quasiSymbols;
			std::STATIC_PAGE<std::word, 256> quasiSymbolValues;
			void addReservedIdentifier(std::conststring, userspace::Instruction::Arg);

			// Frontend: Symbol building functions
			std::conststring markVirtual();
			std::conststring markExtern();
			std::conststring markConst();
			std::conststring markStatic();
			std::conststring setHash(std::hash_t);
			std::conststring setSize(std::size_t);
			void addWord(std::word);
			void addHalfword(std::halfword);
			void addByte(std::byte);
			std::conststring addNumerical(std::Lexer::numerical_t, bool);
			void resetSymbol();
			std::index_t lookupSymbol(std::index_t, std::index_t, std::word *, std::hash_t &);

			// Frontend: Instruction building functions
			void setInstruction(userspace::OPCODES, std::byte, std::nibble flags, std::nibble args);
			void expectLiteral(std::byte n);
			void expectAddress(std::byte n);
			void expectAny(std::byte n);
			void surpressArgCountWarning(std::byte n);
			void surpressArgCountWarning();
			std::conststring setArg(userspace::Instruction::Arg &arg);
			std::conststring setReference(std::word address, std::word page);
			std::conststring setDereference(std::word address, std::word, std::byte count);
			void logInstruction();
			void resetInstruction();

			// Frontend: Procedure building functions
			void setProcedureAddress();
			void setProcedureIdentifier(std::index_t, std::index_t, std::hash_t);
			void setProcedureArgCount(std::byte);

			// Backend functions
			esa::BinaryApplication *application;
			void setupBackend(std::conststring, std::size_t, std::size_t, std::size_t);
			void saveAndClose();
			void discardAndClose();
			std::errno_t buildSymbol();

			// Frontend: Routines
			std::STATIC_PAGE<std::index_t, 32> routineStartsPage;
			std::STATIC_PAGE<std::index_t, 32> routineSubscriberCount;
			std::Stack<std::index_t> routineStarts;
			std::STATIC_PAGE<std::index_t, 128> subscribersPage;
			std::Stack<std::index_t> subscribers;

			// Backend procedure functions
			std::conststring emitProcedure();

			// Backend instruction function
			std::conststring emitInstruction();

			void saveIdentifier(std::index_t start, std::index_t end);
			std::hash_t hashIdentifier(BBP::esa::dataEntry &);
			BBP::std::conststring mangleSymbolIdentifier(BBP::esa::dataEntry &entry);

			// Auxiliary functions
			std::errno_t ExpectDelimiters(std::Lexer::lex_context *, std::index_t argc, std::Lexer::lex_keywordhandle const argv[], bool singleLine, std::index_t min, std::index_t maxDepth, std::Lexer::lex_keywordhandle escape, std::Lexer::lex_keywordhandle stop);
			std::conststring includeFile(std::Lexer::lex_context *, std::index_t);
			bool includeFromPath(std::FILE &, std::PATH &, std::Stack<std::string_element> &, std::PATH &out);

			// flags
			bool beVerbose;

			// These variables are passed between the actions and auxiliary actions
			std::Lexer::lex_keywordhandle handle;
			std::Lexer::lex_keywordhandle lastOpener;
			std::Lexer::lex_keywordhandle firstCloser;
			std::STATIC_PAGE<std::Lexer::lex_keywordhandle, 32> delimiterPage;
			std::Stack<std::Lexer::lex_keywordhandle> delimiterStack;
			std::word delimiterDepth = 0;

			bool lineIsDefiningSymbolDefinition;
			bool lineIsDefiningInstruction;
			bool lineIsDefiningProcedure;

		};

		// Types used by ELSA
		enum ELSA_TYPE : uint32_t
		{
			UNKOWN_T,
			SYMBOL_T,
			PROCEDURE_T,
			INSTRUCTION_T
		};

		enum SYMBOL_TYPE : uint32_t
		{
			// Defines the access of a symbol. If none used, it is a quasi-symbol.
			SYMTACC_VIRTUAL = 0b1,
			SYMTACC_STATIC = SYMTACC_VIRTUAL << 1,

			// Modifiers that can be used
			SYMTMOD_EXTERN = SYMTACC_STATIC << 1,
			SYMTMOD_CONST = SYMTMOD_EXTERN << 1,
			SYMTMOD_STATIC = SYMTMOD_CONST << 1,

			// The size of the symbol
			SYMTSZ_NIL = SYMTMOD_STATIC << 1,
			SYMTSZ_DN = SYMTSZ_NIL << 1,
			SYMTSZ_DB = SYMTSZ_DN << 1,
			SYMTSZ_DH = SYMTSZ_DB << 1,
			SYMTSZ_DI = SYMTSZ_DH << 1,
			SYMTSZ_DA = SYMTSZ_DI << 1,
			SYMTSZ_DW = SYMTSZ_DA << 1,

			SYMTMOD_SHARED = SYMTSZ_DW << 1
		};



		// Extern symbols
		extern esaProcessor processor;


	}
}

#endif
