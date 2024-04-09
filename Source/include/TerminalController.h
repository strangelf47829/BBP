#ifndef BBP_TERMINALCONTROLLER_H
#define BBP_TERMINALCONTROLLER_H

#include "Graphics.h"
#include "TerminalState.h"
#include "TerminalDevice.h"

// This header file defines a struct that, in combination with the window struct from BBP, allows
// The user to spawn a terminal fully compliant with ECMA35.

namespace BBP
{
	namespace std
	{
		namespace Terminal
		{
			// A class, because 
			class TerminalApplication
			{

				// The state this controller looks over.
				TerminalState state;

				// The active display
				TerminalDevice *device;

				// Current state of the terminal
				enum ControlState
				{
					// Printing as usual
					Normal,

					// The last sent CC is 'ESC'
					escaped,

					// Currently in a CSI
					ControlSequence,

					// String sequence
					Str

				} currentState;

			public:

				// C0 control codes (inclusive)
				constexpr static CC C0_Low = 0x00;
				constexpr static CC C0_High = 0x1E;

				// C1 control codes (inclusive)
				constexpr static CC C1_Low = 0x80;
				constexpr static CC C1_High = 0x9f;

				// The 'escape' code.
				constexpr static CC ESCAPE = 0x1B;

				// If 'escape' is followed by a character that, by adding this modifier, would bump it into the C1 set, do so anyway.
				constexpr static CC FE_modifier = 0x40;

				// The C0 set
				enum C0 : CC
				{
					NUL = 0,
					SOH,
					STX,
					ETX,
					EOT,
					ENQ,
					ACK,
					BEL,
					BS,
					HT,
					LF,
					VT,
					FF,
					CR,
					LS1,
					LS0,
					DLE,
					DC1,
					DC2,
					DC3,
					DC4,
					NAK,
					SYN,
					ETB,
					CAN,
					EM,
					SUB,
					ESC,
					IS4,
					IS3,
					IS2,
					IS1
				};

				// The C1 set
				enum C1 : CC
				{
					BPH = 0x42,
					NBH,

					NEL = 0x45,
					SSA,
					ESA,
					HTS,
					HTJ,
					VTS,
					PLD,
					PLU,
					RI,
					SS2,
					SS3,
					DCS,
					PU1,
					PU2,
					STS,
					CCH,
					MW,
					SPA,
					EPA,
					SOS,

					SCI = 0x5a,
					CSI,
					ST,
					OSC,
					PM,
					APC
				};

				// Parameter bytes stuff for CSI
				constexpr static CC CSI_P_Low = 0x30;
				constexpr static CC CSI_P_High = 0x3F;

				// Intermediate bytes stuff for CSI
				constexpr static CC CSI_I_Low = 0x20;
				constexpr static CC CSI_I_High = 0x2F;

				// Final bytes stuff for CSI
				constexpr static CC CSI_F_Low = 0x40;
				constexpr static CC CSI_F_High = 0x7E;

				// Private final bytes for CSI
				constexpr static CC CSI_F_Priv_Low = 0x70;

				// Final byte combinations for CSI commands without intermediate bytes
				enum CSI_F_NOINTER : CC
				{
					ICH = 0x40,
					CUU,
					CUD,
					CUF,
					CUB,
					CNL,
					CPL,
					CHA,
					CUP,
					CHT,
					ED,
					EL,
					IL,
					DL,
					EF,
					EA,
					DCH,
					SSE,
					CPR,
					SU,
					SD,
					NP,
					PP,
					CTC,
					ECH,
					CVT,
					CBT,
					SRS,
					PTX,
					SDS,
					SIMD,

					HPA = 0x60,
					HPR,
					REP,
					DA,
					VPA,
					VPR,
					HVP,
					TBC,
					SM,
					MC,
					HPB,
					VPB,
					RM,
					SGR,
					DSR,
					DAQ
				};

				// Final byte combinations for CSI commands with intermediate bytes
				enum CSI_F_INTER : CC
				{
					SL = 0x40,
					SR,
					GSM,
					GSS,
					FNT,
					TSS,
					JFY,
					SPI,
					QUAD,
					SSU,
					PFS,
					SHS,
					SVS,
					IGS,

					IDCS = 0x4F,
					PPA,
					PPR,
					PPB,
					SPD,
					DTA,
					SHL,
					SLL,
					FNK,
					SPQR,
					SEF,
					PEC,
					SSW,
					SACS,
					SAPV,
					STAB,
					GCC,
					TATE,
					TALE,
					TAC,
					TCC,
					TSR,
					SCO,
					SRCS,
					SCS,
					SLS,


					SCP = 0x6b
				};

			private:

				// Current string state
				enum StringState
				{
					none,	// Not doing anything
					Device,
					Private,
					Application,
					OS,
					Generic
				} stringState;

				// State stuff
				void calculateViewportHorizontal();
				void calculateViewportVertical();
				void calculateViewport();

				// Line information
				size_t getCurrentLineLength();

				// Visually scroll, but not move
				void ScrollBack(offset_t);
				void ScrollForward(offset_t);
				void ScrollUp(offset_t);
				void ScrollDown(offset_t);

				// Move active position, and then scroll
				void MoveBack(offset_t);
				void MoveForward(offset_t);
				void MoveUp(offset_t);
				void MoveDown(offset_t);

				// Line stuff
				void insertLine();
				void deleteLine();
				void extendLine();

				// SGR command
				void SGR_CMD(word);

				// Set home
				void setHorizontalHome();
				void setVerticalHome();

				// Check for control sequences
				bool isCCC0(CC);
				bool isCCC1Upper(CC);
				bool isCCC1Lower(CC);

				// Getting corresponding codes
				C0 getC0Action(CC);
				C1 getC1Action(CC);

				// State actions
				void actNormal(CC); // Hehehehehheheeh i already try mate. Its difficult :)
				void actEscaped(CC);
				void actString(CC);
				void actCSI(CC);

				// C0 actions
				void C0Action(C0);
				void BEL_C0();	// Calls the device bell
				void BS_C0();	// Moves the active presentation thing back if it can, otherwise no-op
				void HT_C0();	// Move to next tabulation
				void LF_C0();	// Line feed. Move to next line (but preserve active position horizontally).
				void VT_C0();	// Vertical tabulation? No idea
				void FF_C0();	// Go one page further and set to home position
				void CR_C0();	// Set active position to line home or line limit, depending on DCSM setting
				void DC1_C0();	// Restore mode
				void DC2_C0();	// Set mode 2
				void DC3_C0();	// Pause (do not receive code)
				void DC4_C0();	// Implementation dependant
				void SUB_C0();	// Display substitute character (usually glyph 0)
				void ESC_C0();	// Escape sequence
				void IS1_C0();	// Unit seperator
				void IS2_C0();	// Record seperator
				void IS3_C0();	// Group seperator
				void IS4_C0();	// File separator
				//void NUL_C0();	// No-op
				//void SOH_C0();	// No-op
				//void STX_C0();	// No-op
				//void ETX_C0();	// No-op
				//void EOT_C0();	// No-op
				//void ENQ_C0();	// No-op
				//void ACK_C0();	// No-op
				//void LS0_C0();	// No-op
				//void LS1_C0();	// No-op
				//void DLE_C0();	// No-op
				//void NAK_C0();	// No-op
				//void SYN_C0();	// No-op
				//void ETB_C0();	// No-op
				//void CAN_C0();	// No-op
				//void EM_C0(); 	// No-op

				// String stuff
				string activeString;
				Stack<string_element> stringStack;

				// C1 actions
				void C1Action(C1);
				void NEL_C1();	// Line feed, carriage return
				void SSA_C1();	// Set start of selected area
				void ESA_C1();	// Set end of selected area
				void PLD_C1();	// Set subscript or restore superscript
				void PLU_C1();	// Set superscript or restore subscript
				void DCS_C1();	// Begin a device command string context
				void PU1_C1();	// Private use
				void PU2_C1();	// Private use
				void CCH_C1();	// DEL
				void SPA_C1();	// Start of guarded area
				void EPA_C1();	// End of guarded area
				void SOS_C1();	// Begin a string context.
				void CSI_C1();	// Control sequence indicator.
				void ST_C1();	// Terminate a string context
				void OSC_C1();	// Operating system command, and begins a string context
				void PM_C1();	// Private message
				void APC_C1();	// Application program command, and begins a string context
				//void BPH_C1();	// No-op
				//void NBH_C1();	// No-op
				//void HTS_C1();	// No-op
				//void HTJ_C1();	// No-op
				//void VTS_C1();	// No-op
				//void RI_C1(); 	// No-op
				//void SS2_C1();	// No-op
				//void SS3_C1();	// No-op
				//void STS_C1();	// No-op
				//void MW_C1(); 	// No-op
				//void SCI_C1();	// No-op

				// Current argument
				word currentArgument;
				word currentArgumentRadix;
				
				// Push arguments and receive arguments
				void pushArgument(word);
				size_t getArgumentCount();
				word getArgument(index_t);

				// Push I stuff
				bool startingIntermediateBytes;
				void pushIntermediateByte(word);
				void getIntermediateBytes(string &);

				// CSI actions without intermediate stuff
				void CSIActionNoInters(CC finalByte);
				void ICH_CSINo();	// No-op
				void CUU_CSINo();	// (1 arg) (default: 1) move up
				void CUD_CSINo();	// (1 arg) (default: 1) move down
				void CUF_CSINo();	// (1 arg) (default: 1) move forward
				void CUB_CSINo();	// (1 arg) (default: 1) move back
				void CNL_CSINo();	// (1 arg) (default: 1) move n lines down, then home
				void CPL_CSINo();	// (1 arg) (default: 1) move n lines up, then home
				void CHA_CSINo();	// (1 arg) (default: 1) move to nth position on line
				void CUP_CSINo();	// (2 arg) (default: 1,1) move cursor to n,m. 
				void CHT_CSINo();	// (1 arg) (default: 1) move n tabs forward
				void ED_CSINo();	// No-op
				void EL_CSINo();	// No-op
				void IL_CSINo();	// (1 arg) (default: 1) insert n lines
				void DL_CSINo();	// (1 arg) (default: 1) delete n lines
				void EF_CSINo();	// No-op
				void EA_CSINo();	// No-op
				void DCH_CSINo();	// (1 arg) (default: 1) Delete a character
				void SSE_CSINo();	// No-op
				void CPR_CSINo();	// No-op
				void SU_CSINo();	// (1 arg) (default: 1) Scroll up n lines
				void SD_CSINo();	// (1 arg) (default: 1) Scroll down n lines
				void NP_CSINo();	// (1 arg) (default: 1) Scroll n pages down
				void PP_CSINo();	// (1 arg) (default: 1) Scroll n pages up
				void CTC_CSINo();	// No-op
				void ECH_CSINo();	// No-op
				void CVT_CSINo();	// (1 arg) (default: 1) Vertical tab n times
				void CBT_CSINo();	// No-op
				void SRS_CSINo();	// No-op
				void PTX_CSINo();	// No-op
				void SDS_CSINo();	// No-op
				void SIMD_CSINo();	// (1 arg) (default: 1) set SIMD
				void HPA_CSINo();	// (1 arg) (default: 1) set absolute position to n
				void HPR_CSINo();	// (1 arg) (default: 1) move n positions forward
				void REP_CSINo();	// (1 arg) (default: 1) repeat past character n times
				void DA_CSINo();	// No-op
				void VPA_CSINo();	// (1 arg) (default: 1) Set absolute line to n
				void VPR_CSINo();	// (1 arg) (default: 1) move n lines forward
				void HVP_CSINo();	// (2args) (default:1,1) move n chars forward, m lines forward
				void TBC_CSINo();	// No-op
				void SM_CSINo();	// No-op
				void MC_CSINo();	// No-op
				void HPB_CSINo();	// (1 arg) (default: 1) move back n positions
				void VPB_CSINo();	// (1 arg) (default: 1) move forward n positions
				void RM_CSINo();	// No-op
				void SGR_CSINo();	// Set graphics rendition
				void DSR_CSINo();	// No-op
				void DAQ_CSINo();	// No-op

				// CSI actions with intermediate stuff
				void CSIActionInters(CC finalByte);

				// Add a character to the current position
				void addCharacterToPosition(CC);

				// Pass a character (does not add character to terminal stuff)
				void PassCharacter(CC);

				// Present entire terminal page to screen
				void presentTerminal();

			public:

				// Constructors
				TerminalApplication(TerminalState::TerminalRect vp_size, size_t lc, ResourceManager *allocator, TerminalDevice *dev);

				// Functions to interact with the terminal controller
				void input(CC);
				void input(string &);

				// Input some characters, and then render
				void render(CC);
				void render(string &);
				void render(conststring);

				void set(size_t, size_t);

			};
		}
	}
}

#endif
