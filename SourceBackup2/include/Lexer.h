#ifndef LUA_PARSER_H
#define LUA_PARSER_H

namespace BBP
{
	namespace Services
	{
		namespace Lexer
		{

			enum lex_thingType
			{
				TOKEN_T,
				BODY_T
			};

			enum lex_tokenType : unsigned int
			{
				VARIABLE_DECL = 0,
				CONST_VAL_STR = 1,
				CONST_VAL_NUM = 2,
				CONST_VAL_LOG = 3,
			};

			enum lex_bodyType : unsigned int
			{
				GLOBAL = 0,
				FUNCTION = 1,
				TOKEN = 2
			};



			struct lex_context;
			struct lex_thing;

			typedef const char *(*lex_action)(lex_context *, unsigned int, unsigned int);

			struct lex_keyword
			{
				char *word; // The word
				unsigned char length;

				unsigned int mask;
				lex_action lex_foundAction; // This action is called when kw found
			};


			struct lex_thing
			{
				lex_context *context; // In what context is this thing?
				unsigned int start; // Where in the context does this thing start?
				unsigned int end; // Where in the context does this thing end?

				lex_thing *enclosedWithin; // Is this thing enclosed in another thing?

				lex_thingType type; // What type of thing is this? (token or body?)
				unsigned int specialty; // What type of 'type' is this thing?
			};





			struct lex_body
			{
				lex_thingType type; // The type of thing this is

				lex_context *context; // In what context is this?
				unsigned int start; // Where in the file does this thing start?
				unsigned int end; // Where in the file does this thing end?
			};

			struct lex_context
			{
				const char *activeFile = nullptr;

				char *data = nullptr; // Pointer to file
				unsigned int dataSize = 0; // Size of the file
				unsigned int atLocation = 0; // Where in the file this context currently is

				bool isInLiteralContext = false; // Flag to tell lexer to stop looking for kewords (handy for strings)
				unsigned int escapeIndex = 0; // Which keyword can escape the literal context?
				unsigned int enforceIndicies = 0; // Which keywords will ALWAYS be searched?

				unsigned int tokenIndex = 0; // Tokens are treated differently when parsed, so ...
				unsigned int tokenCount = 0; // The amount of tokens

				bool isInComment = 0; // Is the lexer currently inside a comment?
				unsigned int commentDepth = 0; // What is the 'depth' of the comment? (For lua, you can have 'nested' comments)
				unsigned int commentStartedAt = 0; // Where did this comment begin?

				lex_keyword *words = nullptr; // Array of keywords
				unsigned int keywordCount = 0; // The amount of keywords

				lex_thing **things = nullptr;
				unsigned int *thingCount = nullptr;
				unsigned int thingCategoryCount = 0;

				unsigned int auxStart = 0; // Aux register
				unsigned int auxEnd = 0; // Aux register

				unsigned int newline = 0; // Action to be done at every (meaningful) newline
				bool careAboutNewLine = false; // Do something at next newline

				bool flags[100]; // 100 flags to use.

				// DEBUG INFO
				bool showLines = true;
				bool showWhere = true;
				unsigned int highlightStart = 0;
				unsigned int highlightEnd = 0;
				unsigned int atLine = 0; // Which line in the file (source, not edited), this context currently is.
				unsigned int atCol = 0; // Which coloumn in the file (source, not edited), this context currently is.

				char charStack[255];
				unsigned int charIndex[255];
				unsigned char charStackIndex = 0;
				char *strRegister = nullptr;
				unsigned char strRegisterIndex = 0;

				unsigned int lastWhiteSpace = 0;
				bool wasLastCharWhitespace = 0;
			};

			void L_createLexer(lex_context **, const char *);

			void L_destroyLexer(lex_context *);

			void L_ignore(lex_context *, char);
			void L_emitFile(lex_context *, const char *);

			void L_setKeywords(lex_context *, unsigned int);
			void L_expandKeywords(lex_context *, unsigned int);
			lex_keyword *L_setKeywordTo(lex_context *, const char *, unsigned int, const char *(*)(lex_context *, unsigned int, unsigned int));

			unsigned int L_findNextKeyword(unsigned int *, lex_context *);
			void L_findAllKeywords(lex_context *);

			void L_pushNextWord(unsigned int *, lex_context *);

			bool L_pushCharacter(lex_context *, unsigned int);
			bool L_pushCharacter(lex_context *, unsigned int, char);
			void L_popCharacter(lex_context *);
			void L_popCharacter(lex_context *, unsigned char);

			// Only valid until char pop!
			void L_pushString(lex_context *, unsigned int, unsigned int);
			void L_popString(lex_context *);

			// Move stuff up and down
			void L_moveToEndOfWord(lex_context *);
			void L_moveToStartOfNextWord(lex_context *);

			void L_highlight(lex_context *, unsigned int start, unsigned int end, const char *col);
			void L_Log(lex_context *, unsigned int, const char *, const char *);

			void L_insert(lex_context *, unsigned int _where, char *str);
			void L_insertNewline(lex_context *, unsigned int _where, char *str);

			inline bool L_isWhitespace(char c)
			{
				return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == 1 || c == 2);
			}

			inline bool L_isWhitespace(lex_context *context, unsigned int index)
			{
				if (index + 1 == context->dataSize) { return true; }
				char c = context->data[index];
				return L_isWhitespace(c);
			}

		}
	}
}

#endif