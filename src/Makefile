CC=clang++
BLOODWORD=BloodSword.cpp
BLOODWORD_OUTPUT=BloodSword.exe
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LINKER_FLAGS += -stdlib=libc++
	INCLUDES += -I/usr/local/include/SDL2
else
	LINKER_FLAGS += -lstdc++fs
endif

all: clean blood-sword

blood-sword:
	$(CC) -Wall $(BLOODWORD) $(LINKER_FLAGS) $(INCLUDES) -o $(BLOODWORD_OUTPUT)

debug:
	$(CC) -Wall $(BLOODWORD) $(LINKER_FLAGS) $(INCLUDES) -o $(BLOODWORD_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o
