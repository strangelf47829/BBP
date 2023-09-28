/**  \file Kernel.h
*	Header file for the compilation of BBP
* 
*	Something something blah blah
*/

#ifndef BBP_KERNEL_H
#define BBP_KERNEL_H


#define BBP_KERNEL_VERSION "0.2.0" /**< The version of the compiled binaries in string form. */
#define BBP_KERNEL_NAME "TSauce" /**< The name of the system which could be used to keep track of different distributions*/


#ifndef BBP_MEMORY_LITTLEENDIAN
#define BBP_MEMORY_BIGENDIAN /**< This flag governs the memory structure of the device. Big endian by default. To use little endian, define "BBP_MEMORY_LITTLEENDIAN" before your #include */
#endif

#ifndef BBP_KERNEL_BOOTLOCATION
#define BBP_KERNEL_BOOTLOCATION "V:\\BOOT\\OS.lua" /**< The default location to boot to in a file system. */
#endif

#ifndef BBP_ARCH
#define BBP_ARCH "None" /**< When the kernel is being compiled, this macro is automatically defined externally to indicate the target platform. If this is not done, it is 'none' by default */
#endif

#ifdef BBP_CREATE_ERROR
#undef BBP_CREATE_ERROR
/** If defined, the compiler generates an error. There is no reason to enable this other than to generate errors. */
#define BBP_CREATE_ERROR
/* ^^^ Stupid hack to get doxygen to see this macro */
#error Why would you enable this?
#endif

#ifdef BBP_DEBUG
#error This branch does not support debugging. Please disable this flag
#endif

#ifdef BBP_C_ENTRY
#error This branch does not support debugging. Please disable this flag
#endif

/**
 * @brief The kernel returns one of these values to tell the bootloader why the kernel returned.
*/
enum KERNEL_STATUS : int
{
	KERNEL_OK = 0x0000,						/**< Returned when the kernel exits normally */
	KERNEL_GENERICERROR = 0x1000,			/**< Returned when the kernel encounters a generic error */

	KERNEL_BIOSERROR = 0x2000,				/**< Returned when the kernel cannot initialize or find BIOS properties */
	KERNEL_BIOSERROR_ROM = 0x2010,			/**< Returned when the kernel cannot initialize CMOS */
	KERNEL_BIOSERROR_DISP = 0x2020,			/**< Returned when the kernel cannot initialize the display */
	KERNEL_BIOSERROR_SDMMC = 0x2030,		/**< The SDMMC could not be initialized */

	KERNEL_BOOTERROR = 0x3000,				/**< Returned when the kernel cannot boot an OS */
	KERNEL_NOVOLUME = 0x3010,				/**< Returned when the kernel cannot find a volume to boot into */
	KERNEL_NOBOOT = 0x3020,					/**< Returned when the kernel cannot find a bootable file */
	KERNEL_BOOTINVALID = 0x3030,			/**< Returned when the OS does not initialize properly. */

	KERNEL_INTERRUPT_ERROR = 0x4000,		/**< Generic interrupt error */
	KERNEL_INTERRUPT_OUTOFMEMORY = 0x4010,	/**< There is no more memory to allocate to interrupts. */

	IM_A_TEAPOT = 418						/**< The kernel is a teapot */
};

/**
 * @brief The entry point of the kernel. Should be called by the bootloader
 * @return The kernel returns a value which corresponds to the reason it returned to the bootloader.
*/
extern KERNEL_STATUS kernel_entry();


// Inlcude lua files
extern "C"
{
#include "lapi.h"
#include "lauxlib.h"
#include "lua.h"

	// Defined after these headers, as to not interfere with the declaration/definition of 'printf' in cstdio or whatever
#define printf(x, y) static_assert(false, " !!! --- printf statement found in non-debugging context --- !!!")
}

// Define the type of the data stored in CMOS and SDMMC

/** Alias for 'unsigned char'. */
typedef unsigned char u_char;
#ifdef BBP_MEMORY_TYPE
/**
 * @brief The type with which data is encoded into memory.
 *
 * @details When the kernel does basic IO functions (such as reading from ROM for bios operations) the kernel uses this type to define what one 'element' is. @n
 * By default, ::mem_t is of type 'unsigned char'.
 * If the architecture requires it, the default type can be overriden by defining 'BBP_MEMORY_TYPE'.
*/
typedef mem_t BBP_MEMORY_TYPE;
#else
/**
 * @brief The type with which data is encoded into memory.
 * 
 * @details When the kernel does basic IO functions (such as reading from ROM for bios operations) the kernel uses this type to define what one 'element' is. @n
 * By default, ::mem_t is of type 'unsigned char'.
 * If the architecture requires it, the default type can be overridden by defining 'BBP_MEMORY_TYPE'.
*/
typedef u_char mem_t;
#endif

// Define the type of the time used 
/** Alias for 'long long' */
typedef long long ll_int;
#ifdef BBP_TIME_TYPE
/**
 * @brief The type which is used to represent time.
 *
 * @details By default, this is 'long long'.
 * This can be overridden by defining 'BBP_TIME_TYPE'.
 * However, since 32bit systems cannot represent 'long long', this type is forced to be 'int' when compiling to a 32bit system.
*/
typedef BBP_TIME_TYPE b_time_t;
#else
/**
 * @brief The type which is used to represent time.
 *
 * @details By default, this is 'long long'.
 * This can be overridden by defining 'BBP_TIME_TYPE'.
 * However, since 32bit systems cannot represent 'long long', this type is forced to be 'int' when compiling to a 32bit system.
*/
typedef ll_int b_time_t;
#endif


namespace BBP
{

	struct application;

	/**
	 * @addtogroup IO
	 * @{
	*/

	/**
	 * @brief A collection of methods and structures for BIOS.
	*/
	namespace IO
	{
		
		/**
		 * @addtogroup Memory
		 * @{
		*/

		/**
		 * @brief High level access to low level memory components
		 *
		 * The memory namespace provides an interface to interact with low level memory components (such as ROM and DMA).
		*/
		namespace Memory
		{
			/**
			 * @addtogroup ROM
			 * @{
			*/
			
			/**
			 * @brief High level access to ROM
			 *
			 * The ROM namespace defines functions that give the kernel access to ROM memory.
			 * @n
			 * This namespace has 3 functions which must be defined externally:
			 * @li BBP::IO::Memory::ROM::readROM
			 * @li BBP::IO::Memory::ROM::initializeROM
			 * @li BBP::IO::Memory::ROM::terminateROM
			 * @n
			 * @n
			 * The booting sequence of the kernel expects at least \p 20 bytes of data.
			 * Even if the implementation does not support ROM, these \p 20 bytes must still be available to the kernel.
			 * The main page explains the meaning of these \p 20 bytes in greater detail.
			*/
			namespace ROM
			{
				/**
				 * @brief The size (in bytes) of the ROM.
				 * 
				 * @details This value represents the maximum amount of bytes stored in ROM (in bytes).
				 * This value should be set externally by BBP::IO::Memory::ROM::initializeROM.
				*/
				extern int ROM_SIZE;

				/**
				 * @brief Reads a single ::mem_t from ROM at the specified offset.
				 * @param offset The offset to read memory from (in bytes).
				 * @return returns the value stored in ROM at the offset.
				 * @details This function must be implemented externally.
				 * The implementation of BBP::IO::Memory::ROM::readROM should have definitions for both BBP_MEMORY_LITTLEENDIAN and BBP_MEMORY_BIGENDIAN
				*/
				extern mem_t readROM(unsigned int offset);
				/**
				 * @brief Read a chunk of memory from ROM.
				 * @param start The offset (in bytes) to start reading from.
				 * @param end The offset (in bytes) to stop reading from (inclusive).
				 * @return An array of data from ROM. <B> Note that the caller must also free this memory! </B>
				 * @sa BBP::Services::Free
				 * @details This function calls BBP::IO::Memory::ROM::readROM internally. This function cannot be defined externally.
				*/
				extern mem_t *readROMChunk(unsigned int start, unsigned int end);

				/**
				 * @brief Reads a single (32bit) integer from rom at the specified offset.
				 * @param offset The offset to read memory from (in dwords).
				 * @return returns the value store in ROM at the offset.
				 * @details This function calls BBP::IO::Memory::ROM::readROM internally. This function cannot be defined externally.
				*/
				extern int readROMInt(unsigned int offset);
				/**
				 * @brief Read a chunk of memory from ROM.
				 * @param start The offset (in dwords) to start reading from.
				 * @param end The offset (in dwords) to stop reading from (inclusive).
				 * @return An array of the requested data. <B> Note that the caller must also free this memory! </B>
				 * @sa BBP::Services::Free
				 * @details This function calls BBP::IO::Memory::ROM::readROM internally. This function cannot be defined externally.
				*/
				extern int *readROMIntChunk(unsigned int start, unsigned int end);

				/**
				 * @brief Initializes and configures the ROM.
				 * @return An error code if something went wrong, or '0' if initialization went ok.
				 * @details This function must be defined externally.
				 * <B> There is no internal check to see if ROM has been initialized</B>: make sure it is called before the kernel calls BBP::IO::Memory::ROM::readROM.
				*/
				extern int initializeROM();
				/**
				 * @brief deconfigures and terminates ROM services.
				 * @return An error code if something went wrong, or '0' if initialization went ok.
				 * @details This function must be defined externally.
				*/
				extern void terminateROM();
			}
			/**
			 * @}
			*/

			/**
			 * @addtogroup DMA
			 * @{
			*/

			/**
			 * @brief High level DMA access.
			 *
			 * This namespace will provide access to DMA (Direct Memory Access) features on architectures that support it. @n
			 * @remark This is currently not available.
			*/
			namespace DMA
			{
				//TODO: Implement
			}

			/** 
			* @}
			*/
		}
		
		/**
		 * @}
		*/

		/**
		 * @addtogroup File
		 * @{
		*/

		/**
		 * @brief Defines a simple abstraction to allow for traversal in any file system
		*/
		namespace File
		{
			struct VOLUME_INFO;
			struct DIRECTORY_INFO;
			struct FILE_HANDLE;
			

			/**
			 * @brief Structure used to define volume information.
			 * 
			 * This structure allows for 'virtual volumes'
			 * Since, for example, the @c root directory could be directory similar to /dev/.../
			*/
			struct VOLUME_INFO
			{
				/** @brief The amount of bytes this volume is capable of storing */
				int byteCapacity; 
				/** @brief The amount of bytes that this volume has taken up. */
				int bytesUsed; 

				/**
				 * @brief A dword that tells the kernel what type of text encoding the volume uses, assuming the encodings stay consistent with @e file @e allocation types.
				*/
				enum ENCODING_TYPE : int
				{
					FAT8 = 0x46413038, /**< 'FA08' */
					FAT12 = 0x46413132, /**< 'FA12' */
					FAT16 = 0x46413136, /**< 'FA16' */
					FAT24 = 0x46413234, /**< 'FA24' */
					FAT32 = 0x46413332, /**< 'FA32' */
					EFI = 0x45454649, /**< 'EEFI' */
					UEFI = 0x55454649, /**< 'UEFI' */
					B607 = 0x42363037 /**< 'B607' */
				} encoding; /**< @brief The encoding/allocation style of the volume. */

				 /** @brief The label used to uniquely identify the volume (such as 'C' or 'V') */
				char label;

				/** @brief True if it is possible to read from the volume. Otherwise, false. */
				bool readAccess; 
				/** @brief True if it is possible to write to the volume. Otherwise, false. */
				bool writeAccess; 

				/**
				 * @brief True if the volume is a virtual volume. 
				 * 
				 * A virtual volume is a volume that is not associated with a physical storage location but a folder within another volume. @n
				 * This is similar to how COM ports work on windows, or linux's /dev/ folder.
				*/
				bool virtualVolume; 
				/** @brief True if a bootable file has been found on the device. */
				bool bootable;

				/** @brief The root of the volume. */
				DIRECTORY_INFO *root; 
			};

			/**
			 * @brief Find the amount of volumes.
			 * @return The amount of volumes found.
			*/
			extern int getVolumeCount();

			/**
			 * @brief Fetch information of a volume
			 * @param volume The volume index (the indexing is the same as that of BBP::IO::File::getVolumeCount.
			 * @return Returns the information of a volume.
			*/
			extern VOLUME_INFO getVolumeInfo(int volume);

			/**
			 * @brief A structure to store information about a directory inside a volume.
			*/
			struct DIRECTORY_INFO
			{
				/** @brief The parent directory. If this directory is root, this folder is its own parent. */
				DIRECTORY_INFO *parent; 

				/** @brief References to subdirectories stored within this directory. */
				DIRECTORY_INFO **subDirectories; 
				/** @brief The amount of subdirectories present in this directory. */
				int subDirectoryCount; 

				/** @brief A handle to each file stored within this directory. */
				FILE_HANDLE **files; 
				/** @brief The amount of files present in this directory. */
				int fileCount; 

				/** @brief The name of this directory. */
				char *directoryName; 
				/** @brief The length (in bytes) of the directory name, excluding terminating @p 0. */
				int directoryNameLength; 

				/**
				 * @brief A structure to store directory attribute metadata.
				*/
				struct DIRECTORY_ATTRIBUTES
				{
					/** @brief implementation dependant. */
					bool hidden = false; 
					/** @brief true if the @e contents (not the directory itself) can be read from. */
					bool readAccess = true; 
					/** @brief true if the directory can be modified (including the contents within. */
					bool writeAccess = true; 
				} attributes; /**< @brief Directory attribute information. */
			};

			/**
			 * @brief A simple structure to store information about files.
			 * 
			 * @todo implement metadata
			*/
			struct FILE_HANDLE
			{
				/** @brief A vector to the contents of a file. */
				mem_t *fileVector;
				/** @brief The length of the file in bytes. */
				unsigned int byteCount;

				/** @brief A vector to the name of the file */
				mem_t *fileName;
				/** @brief The length of the file name in bytes */
				unsigned int fileNameLength;
			};
		}

		/**
		 * @addtogroup SDMMC
		 * @{
		*/

		/**
		 * @brief Provides an interface to SD cards.
		*/
		namespace SDMMC
		{
			/**
			 * @brief Read a file with absolute path.
			 * @param fileName The absolute path of a file
			 * @return The handle to the file. If file is not present, the handle will have invalid content.
			*/
			extern File::FILE_HANDLE readFile(const char *fileName);

			/**
			 * @brief Initialize the SD card
			 * @return false if initialization was successful. False otherwise
			*/
			extern bool initializeSDMMC();
			/**
			 * @brief Prepare the SD card for termination.
			*/
			extern void terminateSDMMC();
		}

		/**
		* @}
		*/

		/**
		 * @addtogroup GPIO
		 * @{
		*/

		/**
		 * @todo implement
		*/
		namespace GPIO
		{
			//TODO: Implement
		}
		
		/**
		* @}
		*/

	}

	/**
	 * @}
	*/

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

			extern int createApplicationFromFile(BBP::IO::File::FILE_HANDLE);
			extern bool registerApplication(BBP::application *);

			extern bool deleteApplication(BBP::application *);
			extern bool closeApplication(BBP::application *, bool forceQuit);

			extern application **applicationsVector;
			extern unsigned int applicationsCount;

			namespace Lua
			{
				
				extern void loadSystemLibrary(BBP::application *);
				extern void loadGraphicsLibrary(BBP::application *);
				extern void loadIOLibrary(BBP::application *);
				extern void loadApplicationLibrary(BBP::application *);

				extern void loadLuaLibraries(BBP::application *);

				extern bool Execute(BBP::application *app, char *str, bool autoClear);
				extern unsigned int fetchIndexForLuaInstance(lua_State *);

				extern void Audit(BBP::application *, b_time_t);
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
			extern void drawWindow(window *);

			extern int initializeGraphics(window *, int width, int height);
			extern void destructGraphics();
		}

		namespace R2D
		{

			int convertUCHARtoINT(u_char r, u_char g, u_char b, u_char a);
			void convertINTtoUCHAR(u_char *r, u_char *g, u_char *b, int rgb);
			void convertINTtoUCHAR(u_char *r, u_char *g, u_char *b, u_char *a, int rgba);

			void newWindow(window *, int width, int height, u_char r, u_char g, u_char b);
			void newWindow(window *, u_char r, u_char g, u_char b);

			void background(window *, u_char r, u_char g, u_char b);
			void background(window *, u_char r, u_char g, u_char b, u_char a);
			void background(window *, int rgb);
			void background(window *, int rgb, u_char a);

			void stroke(window *, u_char r, u_char g, u_char b);
			void stroke(window *, u_char r, u_char g, u_char b, u_char a);
			void stroke(window *, int rgb);
			void stroke(window *, int rgb, u_char a);
			void strokeWeight(window *, int weight);
			void noStroke(window *);

			void fill(window *, u_char r, u_char g, u_char b);
			void fill(window *, u_char r, u_char g, u_char b, u_char a);
			void fill(window *, int rgb);
			void fill(window *, int rgb, u_char a);
			void noFill(window *);

			void Line(window *, int x1, int y1, int x2, int y2);
			void Hrule(window *, int x1, int x2, int y);
			void Vrule(window *, int y1, int y2, int x);

			void Line(window *, int x1, int y1, int x2, int y2, u_char r, u_char g, u_char b);
			void Hrule(window *, int x1, int x2, int y, u_char r, u_char g, u_char b);
			void Vrule(window *, int y1, int y2, int x, u_char r, u_char g, u_char b);

			void Line(window *, int x1, int y1, int x2, int y2, int rgb);
			void Hrule(window *, int x1, int x2, int y, int rgb);
			void Vrule(window *, int y1, int y2, int x, int rgb);

			void Rect(window *, int x, int y, int w, int h);
			void Box(window *, int x, int y, int w, int h);
			void Frame(window *, int x, int y, int w, int h);

			void RectPixelMask(window *, int x, int y, int mask);
			void RectPixelMask(window *, int x, int y, int size, int mask);
			void RectPixelMaskAligned(window *, int x, int y, int size, int mask);

			void RectPixelMask(window *, int x, int y, int *maskv, int maskc);
			void RectPixelMask(window *, int x, int y, int size, int *maskv, int maskc);
			void RectPixelMaskAligned(window *, int x, int y, int size, int *maskv, int maskc);

			inline int readPixel(window *, int x, int y);

			inline void setPixel(window *, int x, int y);
			inline void setPixel(window *, int x, int y, int size);
			inline void setPixelAligned(window *, int x, int y, int size);

			void PixelMask(window *, int x, int y, int mask);
			void PixelMask(window *, int x, int y, int size, int mask);
			void PixelMaskAligned(window *, int x, int y, int size, int mask);

			void PixelMask(window *, int x, int y, int *maskv, int maskc);
			void PixelMask(window *, int x, int y, int size, int *maskv, int maskc);
			void PixelMaskAligned(window *, int x, int y, int size, int *maskv, int maskc);

			void PrintCharacterAt(window *, int x, int y, mem_t ch);
			void PrintCharacterAt(window *, int x, int y, mem_t ch, int size);

			void print(window *, mem_t ch);
			void print(window *, mem_t *str);
			void print(window *, const char *str);
			void print(window *, const mem_t *str);
			void print(window *, mem_t *str, int str_c);
			void setCursorPos(window *, int x, int y);


			void GetFontFromROM(window *, int startAdress, int count);
		}

		namespace R3D
		{
			// TODO: Implement
		}
	}

	struct application
	{
		unsigned int PID;

		char *name;
		unsigned int nameLength;

		BBP::Graphics::window renderer;

		lua_State *applicationState;

		bool busy;
		bool auditActive = true;

		b_time_t executingSince = 0;
		b_time_t lastAudit = 0;
		b_time_t timeout = 10000;

		char *backlog = nullptr;
		unsigned int backlog_len = 0;
		unsigned int backlog_c = 0;
	};


}




#endif

#ifndef BBP_LOCAL
#define BBP_LOCAL

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
