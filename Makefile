# Set the output file name based on the OS
OUTPUT =
EXECUTABLE =

ifeq ($(OS),Windows_NT)
	OUTPUT = pacman.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OUTPUT = pacman
		EXECUTABLE = pacman
	endif
	ifeq ($(UNAME_S),Darwin)
		OUTPUT = pacman.app
		EXECUTABLE = pacman.app/Contents/MacOS/pacman
	endif
endif

# Recursively run make in src
all:
	cd src; qmake; make
	cp -r src/$(OUTPUT) .
	rm -rf src/$(OUTPUT)

clean:
	cd src; make clean;
	rm -rf $(OUTPUT)
	rm -rf doc/

run:
	./$(EXECUTABLE)

doxygen:
	doxygen Doxyfile

pack:
	zip -r xbenci01_xpolia05.zip examples/maps src/ Makefile README.txt Doxyfile