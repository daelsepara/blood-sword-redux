BSWORD_TEST=BloodSwordTest.cpp
BSWORD_TEST_OUTPUT=BloodSwordTest.exe
BSWORD_BATTLES=BattleBoard.cpp
BSWORD_BATTLES_OUTPUT=BattleBoard.exe
BSWORD_MAP=MapTemplate.cpp
BSWORD_MAP_OUTPUT=MapTemplate.exe
BSWORD_MAP_RENDERER=MapRenderer.cpp
BSWORD_MAP_RENDERER_OUTPUT=MapRenderer.exe
BSWORD_SPRITE_SHEET=SpriteSheet.cpp
BSWORD_SPRITE_SHEET_OUTPUT=SpriteSheet.exe

DEBUG=-g -O0
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -fdata-sections -ffunction-sections
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CC=clang++
	LINKER_FLAGS += -stdlib=libc++ -Wl,-dead_strip -L/usr/local/lib
	INCLUDES += -I/usr/local/include/SDL2
else
	CC=clang++
	LINKER_FLAGS += -lstdc++fs -Wl,--gc-sections
endif

all: clean blood-sword-test battle-board map map-renderer sprite-sheet

map:
	$(CC) -Wall $(BSWORD_MAP) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_MAP_OUTPUT)

map-renderer:
	$(CC) -Wall $(BSWORD_MAP_RENDERER) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_MAP_RENDERER_OUTPUT)

sprite-sheet:
	$(CC) -Wall $(BSWORD_SPRITE_SHEET) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_SPRITE_SHEET_OUTPUT)

blood-sword-test:
	$(CC) -Wall $(BSWORD_TEST) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_TEST_OUTPUT) -DDEBUG

battle-board:
	$(CC) -Wall $(BSWORD_BATTLES) $(LINKER_FLAGS) $(INCLUDES) -o $(BSWORD_BATTLES_OUTPUT) -DDEBUG

clean-blood-sword-test:
	rm -f $(BSWORD_TEST_OUTPUT) *.o

clean-battle-board:
	rm -f $(BSWORD_BATTLES_OUTPUT) *.o

clean-map:
	rm -f $(BSWORD_MAP_OUTPUT) *.o

clean-map-renderer:
	rm -f $(BSWORD_MAP_RENDERER_OUTPUT) *.o

clean-sprite-sheet:
	rm -f $(BSWORD_SPRITE_SHEET_OUTPUT) *.o

clean: clean-blood-sword-test clean-battle-board clean-map clean-map-renderer clean-sprite-sheet
