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
	mv src/$(OUTPUT) .

clean:
	cd src; rm -rf .qmake.stash main.o moc_pacman.cpp moc_pacman.o moc_predefs.h pacman.o src.pro.user Makefile ui_pacman.h
	rm -rf $(OUTPUT)