BLOODWORD=BloodSword.cpp
BLOODWORD_OUTPUT=BloodSword.exe
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CC=clang++
	LINKER_FLAGS += -stdlib=libc++
	INCLUDES += -I/usr/local/include/SDL2 -I/usr/local/opt/sdl2/include/SDL2 -I/usr/local/opt/sdl2_image/include/SDL2 -I/usr/local/opt/sdl2_ttf/include/SDL2 -I/usr/local/opt/sdl2_mixer/include/SDL2
else
	CC=clang++
	LINKER_FLAGS += -lstdc++fs
endif

all: clean blood-sword

blood-sword:
	$(CC) -Wall $(BLOODWORD) $(LINKER_FLAGS) $(INCLUDES) -o $(BLOODWORD_OUTPUT)

clean:
	rm -f *.exe *.o
