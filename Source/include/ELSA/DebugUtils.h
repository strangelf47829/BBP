#ifndef BBP_ELSA_DEBUGUTILS_H
#define BBP_ELSA_DEBUGUTILS_H

#include "../Strings.h"

namespace BBP
{
	namespace elsa
	{
		
		// Numerical representation of emission type
		using EmissionMode_numt = std::byte;

		// Structure used to represent an emission mode
		struct DebugEmissionType_t
		{
			// The mode used to represent this emission type
			EmissionMode_numt mode;

			// Default emission types
			enum EmissionMode_t : EmissionMode_numt
			{
				Error = 'e',
				Warning = 'w',
				Log = 'l',
				Note = 'n'
			};

			// Colour used to represent this emission type
			std::byte fg;
			std::byte bg;
		};

		// Used to record the source of a message.
		struct DebugEmissionSource_t
		{
			// Coloumn and row information. If either of them is 0, do not print this information.
			std::index_t row;
			std::index_t coloumn;

			// File or stream name
			std::string inputName;

			// Length of contents
			std::size_t contentSize;
		};

		// A record used to emit something
		struct DebugEmissionRecord_t
		{
			// Emission mode
			EmissionMode_numt mode;

			// Actual message
			std::string msg;

			// Source of debug message
			DebugEmissionSource_t source;

		};


		// This class stores debug messages
		class Debug_db
		{
		public: // DEBUG

			// Emit diagnostic
			void EmitDiagnostic();

			// Emit message source preface
			void emitHeader(DebugEmissionSource_t &);

		public:

		};

	}
}

#endif
