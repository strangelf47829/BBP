
INCDIR= ../Source/include

CXX_MINGW64= x86_64-w64-mingw32-g++
CXX_EMULATOR= x86_64-w64-mingw32-g++
CXX_UBUNTU= g++
CXX= $(CXX_$(WSL_TYPE))
#CXX= g++
CXXFLAGS= -g
EXTCXXFLAG= -g

TGT_MINGW64= a.exe
TGT_EMULATOR= a.exe
TGT_UBUNTU= a.out
TGT= $(TGT_$(WSL_TYPE))

default: all

clean:
	@cd ../Source/stdlib; make clean
	@cd ../Source/ELF; make clean
	@cd ../Source/env_$(WSL_TYPE); make clean
	@cd ../Source/File; make clean
	@cd ../Source/Applications; make clean
	@cd ../Source/Graphics; make clean
	@cd ../Source/Parser; make clean
	@cd ../Source/ELSA; make clean
	@cd ../Source/RAE; make clean

all:
	
	@cd ../Source/stdlib; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/ELF; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/env_$(WSL_TYPE); make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/File; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/Applications; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/Graphics; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/Parser; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/ELSA; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	@cd ../Source/RAE; make EXTCXXFLAGS=$(EXTCXXFLAG) CXX=$(CXX)
	$(CXX) Boot.cpp -o $(TGT) -lSDL2 -Wl,--start-group -L../Source/File/ -lfilesys -L../Source/stdlib/ -lstdlib -L../Source/env_$(WSL_TYPE)/ -lENV -L../Source/ELF/ -lELF -L../Source/Applications/ -lApps -L../Source/Graphics/ -lGlib -L../Source/Parser/ -lparser -L../Source/ELSA/ -lELSA -L../Source/RAE/ -lRAE -L../deps/ -lSDL2 -Wl,--end-group $(CXXFLAGS) -pthread

elf:
	./a.out
	readelf -a /mnt/v/boot/ELSA.o -x 2 -x 5 -x 6 -x 8
compare:
	./a.out
	readelf -S -s /mnt/v/bin/test.o
	readelf -S -s /mnt/v/bin/test2.o

mem: all
	valgrind --tool=massif ./a.out
