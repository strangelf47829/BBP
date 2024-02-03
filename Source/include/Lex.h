#include "Strings.h"
#include "FileSys.h"

#ifndef BBP_STDLIB_LEX_H
#define BBP_STDLIB_LEX_H

namespace BBP
{
	namespace std
	{
		namespace Lexer
		{

			class lex_context;

			typedef uint32_t lex_type;

			// Used to define enums
			typedef uint32_t lex_specialty;

			// This type is used pass around references to keywords
			typedef uint32_t lex_keywordhandle;

			// When keyword is found: This action is done
			typedef std::conststring(*lex_action)(lex_context *, std::index_t, lex_keywordhandle);

			union numerical_t { std::word i; float f; };

			// Keyword
			struct lex_keyword
			{
				std::conststring word;
				std::size_t length;

				uint32_t mask;
				lex_action action;

				//Flags used by the lexer to determine ...
				bool forceEvaluation; // If true, this keyword is always checked.
				bool forceParseInComments; // If true, this keyword is never skipped when inside a comment
				bool forceParseInLiterals; // If true, this keyword is never skipped when inside a literal
				bool forceParseOutsideComments; // If true, this keyword is also considered when commentDepth is 0.
				bool forceParseOutsideLiterals; // If true, this keyword is also considered when literalDepth is 0.
				bool actAsDelimiter; // If true, this keyword will be treated like a whitespace.

				char considerCommentDepth; // If 'forceParseInComments' is false, his keyword is considered only if 'commentDepth' matches this value
				char considerLiteralDepth; // if 'forceParseInLiterals' is false, his keyword is considered only if 'literalDepth' matches this value
			};

			
			// This stores a list of 'things'
			struct lex_thing
			{
				// The context in which this "thing" is in.
				lex_context *context;

				// The start and end of this "thing".
				index_t start;
				index_t end;

				// If this "Thing" is inside of another thing.
				lex_thing *insideOf;

				// What type of thing is this?
				lex_type type;

				// What type of 'type' is this?
				lex_specialty subtype;

				// Hash to speed up searching
				hash_t hash;
			};

			// This structure stores various variables needed to debug an issue
			struct lex_debugInfo
			{
				// The coloumns and lines
				std::index_t coloumn = 0;
				std::index_t line = 0;
				std::index_t character = 0;

				std::conststring msg = nullptr;
				std::conststring errName = nullptr;

				std::PAGE<std::string_element> procedure;

				bool showLinesAndColoumns = true;
				bool showWhere = true;

				const char *highlightmsg = nullptr;

				// 0 = zero terminated by 'highlight'.
				std::index_t highlight;
				std::size_t highlightLength = 0;

			};

			class lex_context
			{
			public:

				lex_context();

				// A more complete way to compile a file
				void translateUnit(std::PATH &);

				// This function ...
				void parseFile(); // Old func is L_findAllkeywords

				void parseFile(std::PATH &);

				// This function just executes any given keyword
				void executeKeywordAction(lex_keywordhandle, lex_keyword *&);
				void executeSpecialFunction(std::index_t a, std::index_t b);

				// The name of the translation unit being parsed.
				std::static_string<128> activeFile;

				// The vector to the data that must be parsed.
				std::PAGE<string_element> data;
				std::size_t dataSize;

				// Where in the vector the lexer is.
				std::index_t atPosition;

				// The keywords
				std::STATIC_PAGE<lex_keyword, 256> keywords;

				// A stack with all keywords
				std::Stack<lex_keyword> keywordStack;

				// A list of special functions.
				bool inhibitSpecialFunctions;
				std::conststring onUnkown(index_t, index_t);
				std::conststring onNumericalLiteral(numerical_t numerical, bool isFloat);

				// Used to keep track of lexing state
				index_t lastWhiteSpace;

				char commentDepth; // 0 if not in comment, 1 for single line comment, 2 for multiline comment
				char literalDepth; // 0 if not in literal, otherwise used to mask keywords.

				// Debug information
				lex_debugInfo debugInformation;

				// Continue compilation?
				bool abort;
				bool illformed;

				// User and system path info
				std::static_string<1024> usrpathPage;
				std::static_string<1024> syspathPage;
				std::Stack<std::string_element> usrpaths;
				std::Stack<std::string_element> syspaths;

				// Application builder
				

				// Helper functions
				bool isWhitespace(char);
				bool isNewLine(char);

				char isBinaryNumeral(char);
				char isOctalNumeral(char);
				char isDecimalNumeral(char);
				char isHexadecimalNumeral(char);
				char isNumericalBase(std::string_element, std::byte&);

				bool tryParseNumerical(std::index_t from, std::index_t to, bool &isFloat, float &fresult, int &iresult);

				// Used to move the lexer up and down
				void moveToEndOfLastWord(bool stopAtNewLine);
				void moveToStartOfLastWord(bool stopAtNewLine);

				void moveToEndOfThisWord(bool stopAtNewLine);
				void moveToStartOfThisWord(bool stopAtNewLine);

				void moveToEndOfNextWord(bool stopAtNewLine);
				void moveToStartOfNextWord(bool stopAtNewLine);

				bool moveToAndHashThisWord(std::hash_t &, char);
				bool moveToAndHashNextWord(std::hash_t &, char);

				void calculateDebugPosition();

				lex_keywordhandle moveToNextKeyword();

				// Logging functions
				void resetInfo(lex_debugInfo &info);

				void Log(lex_debugInfo &info);
				void Log(std::conststring msg);
				void Log(std::conststring from, std::conststring msg, std::conststring name);

				void LogError(std::conststring from, std::conststring msg, std::conststring name);
				void LogNote(std::conststring from, std::conststring msg, std::conststring name);
				void LogWarning(std::conststring from, std::conststring msg, std::conststring name);
				void LogAbort(std::conststring from, std::conststring msg, std::conststring name);

				void Highlight(lex_debugInfo info);


			};


		}
	}
}

#endif