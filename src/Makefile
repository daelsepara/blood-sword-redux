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
LIBS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -fdata-sections -ffunction-sections
INCLUDES=-I/usr/include/SDL2
UNAME_S=$(shell uname -s)
UNAME_M=$(shell uname -m)

ifeq ($(UNAME_S),Darwin)
	LLVM=$(shell brew --prefix llvm)
	CC=$(LLVM)/bin/clang++
	LIBS += -stdlib=libc++ -Wl,-dead_strip -L$(LLVM)/lib/c++ -L/usr/local/lib
	INCLUDES += -I/usr/local/include/SDL2
else ifeq ($(UNAME_M),aarch64)
	CC=g++
	LIBS += -lstdc++fs -Wl,--gc-sections
else ifeq ($(UNAME_M),aarch32)
	LIBS += -lstdc++fs -Wl,--gc-sections
	CC=g++
else
	CC=clang++
	LIBS += -lstdc++fs -Wl,--gc-sections
endif

.PHONY: all clean

all: clean blood-sword-test battle-board map-template map-renderer sprite-sheet

map-template:
	$(CC) -Wall $(BSWORD_MAP) $(LIBS) $(INCLUDES) -o $(BSWORD_MAP_OUTPUT)

map-renderer:
	$(CC) -Wall $(BSWORD_MAP_RENDERER) $(LIBS) $(INCLUDES) -o $(BSWORD_MAP_RENDERER_OUTPUT)

sprite-sheet:
	$(CC) -Wall $(BSWORD_SPRITE_SHEET) $(LIBS) $(INCLUDES) -o $(BSWORD_SPRITE_SHEET_OUTPUT)

blood-sword-test:
	$(CC) -Wall $(BSWORD_TEST) $(LIBS) $(INCLUDES) -o $(BSWORD_TEST_OUTPUT) -DDEBUG

battle-board:
	$(CC) -Wall $(BSWORD_BATTLES) $(LIBS) $(INCLUDES) -o $(BSWORD_BATTLES_OUTPUT) -DDEBUG

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
