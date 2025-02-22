BSWORD_TEST=BloodSwordTest.cpp
BSWORD_TEST_OUTPUT=BloodSwordTest.exe
BSWORD_BATTLES=BattleBoard.cpp
BSWORD_BATTLES_OUTPUT=BattleBoard.exe
BSWORD_MAP=MapTemplate.cpp
BSWORD_MAP_OUTPUT=MapTemplate.exe

LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CC=clang++
	LINKER_FLAGS += -stdlib=libc++
	INCLUDES += -I/usr/local/include/SDL2
else
	CC=clang++
	LINKER_FLAGS += -lstdc++fs
endif

all: clean blood-sword battle-board map

map:
	$(CC) -Wall $(BSWORD_MAP) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_MAP_OUTPUT)

blood-sword:
	$(CC) -Wall $(BSWORD_TEST) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_TEST_OUTPUT)

battle-board:
	$(CC) -Wall $(BSWORD_BATTLES) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_BATTLES_OUTPUT)

clean-blood-sword:
	rm -f $(BSWORD_TEST_OUTPUT) *.o

clean-battle-board:
	rm -f $(BSWORD_BATTLES_OUTPUT) *.o

clean: clean-blood-sword clean-battle-board
