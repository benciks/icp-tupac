# Set the output file name based on the OS
OUTPUT =

ifeq ($(OS),Windows_NT)
	OUTPUT = pacman.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OUTPUT = pacman
	endif
	ifeq ($(UNAME_S),Darwin)
		OUTPUT = pacman.app
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