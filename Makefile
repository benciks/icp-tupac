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


all:
	cd src; qmake; make
	cp -r src/$(OUTPUT) .
	rm -rf src/$(OUTPUT)

clean:
	cd src; make clean;
	rm -rf $(OUTPUT)
	rm -rf doc/

run: all
	./$(EXECUTABLE)

doxygen:
	doxygen Doxyfile

pack: clean
	zip -r xbenci01_xpolia05.zip examples/maps src/logic/ src/data/ src/src.pro src/resources.qrc src/MainWindow.ui src/MainWindow.cpp src/MainWindow.h src/main.cpp Makefile README.txt Doxyfile
