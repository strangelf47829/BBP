

default: clean sphinx

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

clean:
#	cd ./samples; make clean
#	cd ./build; make clean_all
	cd ./doc/doxygen/; rm -r html; rm -r latex; rm -r xml
	cd ./doc/sphinx/; rm -r html; rm -r doctrees
