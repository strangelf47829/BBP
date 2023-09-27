// File Guard
#ifndef BBP_KERNEL_H
#define BBP_KERNEL_H

// Define kernel name and version
#define BBP_KERNEL_VERSION "0.1.0"
#define BBP_KERNEL_NAME "TSauce"

// Define memory structures
#ifndef BBP_MEMORY_LITTLEENDIAN
#define BBP_MEMORY_BIGENDIAN
#endif

/// Useless error
#ifdef BBP_CREATE_ERROR
#error Why would you enable this?
#endif



/// kernel entry returns this type
enum KERNEL_STATUS : int
{
	KERNEL_OK = 0x0000,			/// Returned when the kernel exits normally
	KERNEL_GENERICERROR = 0x1000,		/// Returned when the kernel encounters a generic error

	KERNEL_BIOSERROR = 0x2000,		/// Returned when the kernel cannot initialize or find BIOS properties
	KERNEL_BIOSERROR_ROM = 0x2010,		/// Returned when the kernel cannot initialize CMOS
	KERNEL_BIOSERROR_DISP = 0x2020,		/// Returned when the kernel cannot initialize the display
	KERNEL_BIOSERROR_SDMMC = 0x2030,	/// The SDMMC could not be initialized

	KERNEL_BOOTERROR = 0x3000,		/// Returned when the kernel cannot boot an OS
	KERNEL_NOVOLUME = 0x3010,		/// Returned when the kernel cannot find a volume to boot into
	KERNEL_NOBOOT = 0x3020,			/// Returned when the kernel cannot find a bootable file
	KERNEL_BOOTINVALID = 0x3030,		/// Returned when the OS does not initialize properly.

	KERNEL_INTERRUPT_ERROR = 0x4000,	/// Generic interrupt error
	KERNEL_INTERRUPT_OUTOFMEMORY = 0x4010 /// There is no more memory to allocate to interrupts.
};

#ifndef BBP_DEBUG
/// @brief Brief description
/// @return Returns a value!
extern KERNEL_STATUS kernel_entry();
#endif
#ifdef BBP_DEBUG
/// @brief Brief description
/// @return Returns a value!
extern KERNEL_STATUS kernel_entry(const char *);
#endif


// Inlcude lua files
extern "C"
{
	#include "lapi.h"
	#include "lauxlib.h"
	#include "lua.h"
}

#ifndef BBP_DEBUG
#define printf(x, y) static_assert(false, " !!! --- printf statement found in non-debugging context --- !!!")
#endif

// Define the type of the data stored in CMOS and SDMMC
typedef unsigned char u_char;
#ifdef BBP_MEMORY_TYPE
typedef mem_t BBP_MEMORY_TYPE;
#else
typedef u_char mem_t;
#endif

// Define the type of the time used 
typedef long long ll_int;
#ifdef BBP_TIME_TYPE
typedef time_t BBP_TIME_TYPE;
#else
typedef time_t ll_int;
#endif


namespace BBP
{
	
	struct application;

	namespace IO
	{
		namespace Memory
		{
			namespace ROM
			{
				extern int ROM_SIZE;

				/// @brief This is a brief explanation about what the function does which you can see here.
				/// @param offset This is a parameter, and this is the description of what the parameter does. Yup yup!
				/// @return This returns something, because if it didn't the program would SIGSEV!
				extern mem_t readROM(unsigned int offset);
				extern mem_t *readROMChunk(unsigned int start, unsigned int end);

				extern int readROMInt(unsigned int offset);
				extern int *readROMIntChunk(unsigned int start, unsigned int end);

				extern int initializeROM();
				extern void terminateROM();
			}

			namespace DMA
			{
				//TODO: Implement
			}
		}
		namespace SDMMC
		{
			struct VOLUME_INFO;
			struct DIRECTORY_INFO;
			struct FILE_HANDLE;


			struct VOLUME_INFO
			{
				int byteCapacity;
				int bytesUsed;

				int encoding;

				char label;

				bool readAccess;
				bool writeAccess;

				bool virtualVolume;
				bool bootable;
				bool mounted;

				DIRECTORY_INFO *root;
			};
			
			extern int getVolumeCount();
			extern VOLUME_INFO getVolumeInfo(int volume);

			struct DIRECTORY_INFO
			{
				DIRECTORY_INFO *parent;

				DIRECTORY_INFO **subDirectories;
				int subDirectoryCount;

				FILE_HANDLE **files;
				int fileCount;

				char *directoryName;
				int directoryNameLength;

				struct DIRECTORY_ATTRIBUTES
				{
					bool hidden = false;
					bool readAccess = true;
					bool writeAccess = true;
				} attributes;
			};

			struct FILE_HANDLE
			{
				mem_t *fileVector;
				unsigned int byteCount;

				mem_t* fileName;
				unsigned int fileNameLength;
			};

			extern FILE_HANDLE readFile(const char *fileName);

			extern bool initializeSDMMC();
			extern void terminateSDMMC();
		}

		namespace GPIO
		{
			//TODO: Implement
		}

	}

	namespace Services
	{
		extern unsigned int millis();
		extern void Free(void *);
		extern void *c_alloc(unsigned long long int, unsigned long long int);
		extern void *m_alloc(unsigned long long int);

		extern void Throw(KERNEL_STATUS);

		extern void Delay(long long int);

		namespace ApplicationServices
		{

			extern int createApplicationFromFile(BBP::IO::SDMMC::FILE_HANDLE);
			extern bool registerApplication(BBP::application *);

			extern bool deleteApplication(BBP::application *);
			extern bool closeApplication(BBP::application *, bool forceQuit);

			extern application **applicationsVector;
			extern unsigned int applicationsCount;

			

			namespace Lua
			{
				// Lua libraries
				extern void loadSystemLibrary(BBP::application *);
				extern void loadGraphicsLibrary(BBP::application *);
				extern void loadIOLibrary(BBP::application *);
				extern void loadApplicationLibrary(BBP::application *);

				extern void loadLuaLibraries(BBP::application *); // Load all libraries

				extern bool Execute(BBP::application *app, char *str, bool autoClear);
				extern unsigned int fetchIndexForLuaInstance(lua_State *);

				extern void Audit(BBP::application *, time_t);
				extern void AuditAll();

				extern bool ClearBackLog(BBP::application *);
				extern void AddBackLog(BBP::application *, char *action);
			}
		}

		namespace Interrupts
		{
			struct Interrupt_handle;

			extern Interrupt_handle **interrupts;
			extern u_char interrupt_count;

			extern int createInterrupt();
			extern bool Poll(int);
			extern bool Clear(int, bool);

			namespace Lua
			{
				struct Lua_Interrupt_handle
				{
					int handle;

					BBP::application *owner;
					char *functionName;
					int functionNameLength;

					bool addToBacklog;
				};

				extern Lua_Interrupt_handle **lua_interrupts;
				extern u_char lua_interrupts_count;

				extern int createLuaInterrupt(BBP::application *, char *);
				extern int attachLuaInterruptTo(int, BBP::application *, char *, bool);

				extern void Clear(int);
				extern bool Trigger(int);
				extern bool Trigger(Lua_Interrupt_handle **);
			}

			namespace Timer
			{
				struct Timer_Interrupt_handle
				{
					int handle;

					long long atTime;
					long long length;
					long long timeLeft;

					bool persistent;
				};

				extern Timer_Interrupt_handle **timers;
				extern u_char timers_count;

				extern int createTimerInterrupt(long long, bool);
				extern int attachTimerInterruptTo(int, long long, bool);

				extern void Clear(int);
				extern void UpdateTimers();
			}

			namespace Hardware
			{
				//TODO: implement
			}

			struct Interrupt_handle
			{
				bool triggered;

				int value_i;
				float value_f;
				mem_t value_m;
				void *value_p;

				Lua::Lua_Interrupt_handle **attachedLuaInterrupt = nullptr;
				Timer::Timer_Interrupt_handle **attachedTimerInterrupt = nullptr;

				// TODO: Extend ?
			};

			extern Interrupt_handle *kernelInterrupts;

			extern void updateInterrupts();
			extern void preupdateInterruptsExternal();
			extern void postupdateInterruptsExternal();

			extern bool initializeInterrupts();
			extern void terminateInterrupts();
		}
	}

	namespace Graphics
	{
		struct colour
		{
			u_char R = 0xFF;
			u_char G = 0xFF;
			u_char B = 0xFF;
			u_char A = 0xFF;
			int RGBA = 0xFFFFFFFF;
		};

		struct window
		{
			int *backBuffer = nullptr;

			int width = 0;
			int height = 0;

			int xPos = 0;
			int yPos = 0;
			int zPos = 0;

			colour Stroke = colour();
			colour Fill = colour();
			colour Chroma = colour();

			bool doStroke = true;
			bool doFill = true;
			
			u_char strokeWeight = 5;

			int *typeFace;
			int fontCount;
			
			mem_t *charMapping;
			mem_t fontMapping[256];

			u_char fontSize = 3;
			u_char fontSpacing = 8;

			unsigned int cursorPosX = 0;
			unsigned int cursorPosY = 0;

			u_char coloumn = 0;
			u_char line = 0;
		};

		namespace Driver
		{
			extern void drawWindow(window*);

			extern int initializeGraphics(window *, int width, int height);
			extern void destructGraphics();
		}

		namespace R2D
		{

			int convertUCHARtoINT(u_char r, u_char g, u_char b, u_char a);
			void convertINTtoUCHAR(u_char* r, u_char* g, u_char* b, int rgb);
			void convertINTtoUCHAR(u_char* r, u_char* g, u_char* b, u_char* a, int rgba);

			void newWindow(window*, int width, int height, u_char r, u_char g, u_char b);
			void newWindow(window*, u_char r, u_char g, u_char b);

			void background(window*, u_char r, u_char g, u_char b);
			void background(window*, u_char r, u_char g, u_char b, u_char a);
			void background(window*, int rgb);
			void background(window*, int rgb, u_char a);

			void stroke(window*, u_char r, u_char g, u_char b);
			void stroke(window*, u_char r, u_char g, u_char b, u_char a);
			void stroke(window*, int rgb);
			void stroke(window*, int rgb, u_char a);
			void strokeWeight(window*, int weight);
			void noStroke(window*);

			void fill(window*, u_char r, u_char g, u_char b);
			void fill(window*, u_char r, u_char g, u_char b, u_char a);
			void fill(window*, int rgb);
			void fill(window*, int rgb, u_char a);
			void noFill(window*);

			void Line(window*, int x1, int y1, int x2, int y2);
			void Hrule(window*, int x1, int x2, int y);
			void Vrule(window*, int y1, int y2, int x);

			void Line(window*, int x1, int y1, int x2, int y2, u_char r, u_char g, u_char b);
			void Hrule(window*, int x1, int x2, int y, u_char r, u_char g, u_char b);
			void Vrule(window*, int y1, int y2, int x, u_char r, u_char g, u_char b);

			void Line(window*, int x1, int y1, int x2, int y2, int rgb);
			void Hrule(window*, int x1, int x2, int y, int rgb);
			void Vrule(window*, int y1, int y2, int x, int rgb);

			void Rect(window*, int x, int y, int w, int h);
			void Box(window*, int x, int y, int w, int h);
			void Frame(window*, int x, int y, int w, int h);

			void RectPixelMask(window*, int x, int y, int mask);
			void RectPixelMask(window*, int x, int y, int size, int mask);
			void RectPixelMaskAligned(window*, int x, int y, int size, int mask);

			void RectPixelMask(window*, int x, int y, int* maskv, int maskc);
			void RectPixelMask(window*, int x, int y, int size, int* maskv, int maskc);
			void RectPixelMaskAligned(window*, int x, int y, int size, int* maskv, int maskc);

			inline int readPixel(window*, int x, int y);

			inline void setPixel(window*, int x, int y);
			inline void setPixel(window*, int x, int y, int size);
			inline void setPixelAligned(window*, int x, int y, int size);

			void PixelMask(window*, int x, int y, int mask);
			void PixelMask(window*, int x, int y, int size, int mask);
			void PixelMaskAligned(window*, int x, int y, int size, int mask);

			void PixelMask(window*, int x, int y, int* maskv, int maskc);
			void PixelMask(window*, int x, int y, int size, int* maskv, int maskc);
			void PixelMaskAligned(window*, int x, int y, int size, int* maskv, int maskc);

			void PrintCharacterAt(window*, int x, int y, mem_t ch);
			void PrintCharacterAt(window*, int x, int y, mem_t ch, int size);

			void print(window*, mem_t ch);
			void print(window*, mem_t* str);
			void print(window*, const char* str);
			void print(window*, const mem_t* str);
			void print(window*, mem_t* str, int str_c);
			void setCursorPos(window*, int x, int y);


			void GetFontFromROM(window*, int startAdress, int count);
		}

		namespace R3D
		{
			// TODO: Implement
		}
	}

	/// @brief Brief description
	///
	/// This can be extended by a bigger description!
	struct application
	{
		/// @brief Brief PID descriptions
		unsigned int PID; 

		/// @brief Brief name description
		char *name;
		unsigned int nameLength;

		BBP::Graphics::window renderer;

		lua_State *applicationState;

		bool busy;
		bool auditActive = true;

		time_t executingSince = 0;
		time_t lastAudit = 0;
		time_t timeout = 10000;

		char *backlog = nullptr;
		unsigned int backlog_len = 0;
		unsigned int backlog_c = 0;
	};


}




#endif

#ifndef BBP_LOCAL
#define BBP_LOCAL

#define B607_JP(ch) BBP::Encodings::Japanese::B607_Hiragana::ch

namespace BBP
{
	namespace Encodings
	{
		namespace Roman
		{
			enum B607 : const unsigned char
			{
				space = 0x20,
				exclamationpoint = 0x21,
				doublequotes = 0x22,
				hash = 0x23,
				dollar = 0x24,
				percent = 0x25,
				quote = 0x27,
				openbracket = 0x28,
				closebracket = 0x29,
				asterisk = 0x2a,
				plus = 0x2b,
				comma = 0x2c,
				hyphen = 0x2d,
				period = 0x2e,
				forwardslash = 0x2f,
				zero = 0x30,
				one = 0x31,
				two = 0x32,
				three = 0x33,
				four = 0x34,
				five = 0x35,
				six = 0x36,
				seven = 0x37,
				eight = 0x38,
				nine = 0x39,
				colon = 0x3a,
				semicolon = 0x3b,
				openingcurlybracket = 0x3c,
				equals = 0x3d,
				closingcurlybracket = 0x3e,
				questionmark = 0x3f,
				at = 0x40,
				A = 0x41,
				B = 0x42,
				C = 0x43,
				D = 0x44,
				E = 0x45,
				F = 0x46,
				G = 0x47,
				H = 0x48,
				I = 0x49,
				J = 0x4a,
				K = 0x4b,
				L = 0x4c,
				M = 0x4d,
				N = 0x4e,
				O = 0x4f,
				P = 0x50,
				Q = 0x51,
				R = 0x52,
				S = 0x53,
				T = 0x54,
				U = 0x55,
				V = 0x56,
				W = 0x57,
				X = 0x58,
				Y = 0x59,
				Z = 0x5a,
				a = 0x61,
				b = 0x62,
				c = 0x63,
				d = 0x64,
				e = 0x65,
				f = 0x66,
				g = 0x67,
				h = 0x68,
				i = 0x69,
				j = 0x6a,
				k = 0x6b,
				l = 0x6c,
				m = 0x6d,
				n = 0x6e,
				o = 0x6f,
				p = 0x70,
				q = 0x71,
				r = 0x72,
				s = 0x73,
				t = 0x74,
				u = 0x75,
				v = 0x76,
				w = 0x77,
				x = 0x78,
				y = 0x79,
				z = 0x7a,
				openingcurlybrace = 0x7b,
				pipe = 0x7c,
				closingcurlybrace = 0x7d,
				tilde = 0x7e,
				openingsquarebracket = 0x5b,
				closingsquarebracket = 0x5d,
				backslash = 0x5c,
				nil = 0x00,
				underscore = 0x5f,
				caret = 0x5e,
				ampersand = 0x26,
				returncarriage = 0x0d,
				tab = 0x09
			};
		}

		namespace Japanese
		{
			enum B607_Hiragana
			{
				space = 0x20,
				exclamationpoint = 0x21,
				doublequotes = 0x22,
				hash = 0x23,
				dollar = 0x24,
				percent = 0x25,
				quote = 0x27,
				openbracket = 0xcf,
				closebracket = 0xd0,
				asterisk = 0x2a,
				plus = 0x2b,
				comma = 0x2c,
				hyphen = 0xcd,
				period = 0xce,
				forwardslash = 0x2f,
				zero = 0x30,
				one = 0x31,
				two = 0x32,
				three = 0x33,
				four = 0x34,
				five = 0x35,
				six = 0x36,
				seven = 0x37,
				eight = 0x38,
				nine = 0x39,
				colon = 0x3a,
				semicolon = 0x3b,
				openingcurlybracket = 0x3c,
				equals = 0x3d,
				closingcurlybracket = 0x3e,
				questionmark = 0x3f,
				at = 0x40,
				openingcurlybrace = 0x7b,
				pipe = 0x7c,
				closingcurlybrace = 0x7d,
				tilde = 0x7e,
				openingsquarebracket = 0x5b,
				closingsquarebracket = 0x5d,
				backslash = 0x5c,
				nil = 0x00,
				underscore = 0x5f,
				caret = 0x5e,
				ampersand = 0x26,
				returncarriage = 0x0d,
				tab = 0x09,

				A = 0xd5,
				a = 0xd6,
				I = 0xd7,
				i = 0xd8,
				U = 0xd9,
				u = 0xda,
				E = 0xdb,
				e = 0xdc,
				O = 0xdd,
				o = 0xde,
				ka = 0x8a,
				ki = 0x8c,
				ku = 0xdf,
				ke = 0xe0,
				ko = 0xe1,
				ga = 0x8b,
				gi = 0x8d,
				gu = 0x8f,
				ge = 0x8e,
				go = 0x90,
				sa = 0x91,
				shi = 0x93,
				su = 0x95,
				se = 0x97,
				so = 0x99,
				za = 0x92,
				ji = 0x94,
				zu = 0x96,
				ze = 0x98,
				zo = 0x9a,
				ta = 0x9b,
				chi = 0xa1,
				Tsu = 0x9d,
				tsu = 0x9f,
				te = 0xa2,
				to = 0xa4,
				da = 0x9c,
				dji = 0x9e,
				dzu = 0xa0,
				de = 0xa3,
				Do = 0xa5,

				na = 0xa6,
				ni = 0xa7,
				nu = 0xa8,
				ne = 0xa9,
				no = 0xaa,

				ha = 0xab,
				hi = 0xae,
				fu = 0xe2,
				he = 0xb3,
				ho = 0xb6,
				ba = 0xac,
				bi = 0xaf,
				bu = 0xb1,
				be = 0xb4,
				bo = 0xb7,
				pa = 0xad,
				pi = 0xb0,
				pu = 0xb2,
				pe = 0xb5,
				po = 0xb8,

				ma = 0xb9,
				mi = 0xba,
				mu = 0xbb,
				me = 0xbc,
				mo = 0xbd,

				Ya = 0xbe,
				Yu = 0xc0,
				Yo = 0xc2,
				ya = 0xbf,
				yu = 0xc1,
				yo = 0xc3,

				ra = 0xc4,
				ri = 0xc5,
				ru = 0xc6,
				re = 0xc7,
				ro = 0xc8,

				Wa = 0xc9,
				Wo = 0xca,
				wa = 0xcb,
				n = 0xcc
			};
		}


	}
}
#endif


#ifndef BBP_LUALIBS_H
#define BBP_LUALIBS_H


#pragma region GRAPHICS

/* Vars:
* 
*/

int l_g_fill(lua_State *);
int l_g_stroke(lua_State *);

int l_g_noFill(lua_State *);
int l_g_noStroke(lua_State *);

int l_g_strokeWeight(lua_State *);
int l_g_setFontSize(lua_State *);
int l_g_setCursorPos(lua_State *);

int l_g_line(lua_State *);
int l_g_rect(lua_State *);
int l_g_frame(lua_State *);
int l_g_box(lua_State *);

int l_g_Hrule(lua_State *);
int l_g_Vrule(lua_State *);

int l_g_background(lua_State *);
int l_g_update(lua_State *);

int l_g_print(lua_State *);
int l_g_println(lua_State *);

#pragma endregion

#pragma region LOCALIZATION

int l_l_japanese(lua_State *);

#pragma endregion

#pragma region SYSTEM

/* Vars:
* screen_width: number
*	The width of the display (not window) in pixels
* 
* screen_height: number
*	The height of the display (not window) in pixels
*/

int l_s_millis(lua_State *);
int l_s_delta(lua_State *);
int l_s_delay(lua_State *);

int l_s_createInterrupt(lua_State *);
int l_s_attachTimer(lua_State *);
int l_s_attachAction(lua_State *);

int l_s_PollInterrupt(lua_State *);
int l_s_ClearInterrupt(lua_State *);
int l_s_silentClearInterrupt(lua_State *);

int l_s_TimeLeft(lua_State *);
int l_s_updateInterrupts(lua_State *);

#pragma endregion

#pragma region APPLICATION

int l_a_noAudit(lua_State *);
int l_a_Audit(lua_State *);
int l_a_setTimeout(lua_State *);
int l_a_echo(lua_State *);

#pragma endregion

#endif



#ifdef BBP_DEBUG

#ifndef BBP_DEBUG_H
#define BBP_DEBUG_H

namespace BBP
{
	namespace Debug
	{
		void printToConsole(char a);

		void SetTerminalColor(int colour);

		void setTerminalForegroundColor(int colour);
		void setTerminalBackgroundColor(int colour);

		int ConvertColorToOffset(int colour);

		void Capture();
		void Restore();
	}

}

#endif
#endif
