# +========================================+
# | BBP Compilation makefile utility       |
# |                                        |
# | Author: Rafael de Bie, 2024            |
# | Contact: 2107628bie@zuyd.nl            |
# +========================================+

# +========================================+
# | The instructions to install the BBP    |
# | environment are written in the README, |
# | however it just boils down to running  |
# | some make commands.                    |
# +========================================+

# +========================================+
# | Licensing information can be found in  |
# | this directory                         |
# +========================================+


# Calling this makefile without arguments defaults to the help page
default: setup build

help:
	@echo "Run (make man pages) and read the manual to receive help."

version:
	@echo version 1.1.1.1

.PHONY: build

# +========================================+
# | The setup target sets the platform     |
# | specific stuff.                        |
# +========================================+

setup:
	@cd config/Platforms; ./guess.sh


# +========================================+
# | The build command actually builds the  |
# | libraries and whatnots                 |
# +========================================+

build:
	@cd build; make --no-print-directory

# +========================================+
# | Clean returns everything to how it was |
# +========================================+

clean:
	@cd build; make clean --no-print-directory

# Count the amount of lines of code
count:
	cloc ./Source --exclude-dir=SDL2

all:
	cd ./samples; make all;
	make docs

docs: doxygen sphinx

doxygen:
	cd config; doxygen doxygen.cfg

test:
	cd config; doxygen testing.cfg

sphinx:
	cd config/sphinx; make html

cclean:
#	cd ./samples; make clean
#	cd ./build; make clean_all
	cd ./doc/doxygen/; rm -r html; rm -r latex; rm -r xml
	cd ./doc/sphinx/; rm -r html; rm -r doctrees
