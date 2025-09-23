BSWORD=BloodSword.cpp
BSWORD_OUTPUT=BloodSword.exe
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
BSWORD_BATTLEPITS=Battlepits.cpp
BSWORD_BATTLEPITS_OUTPUT=Battlepits.exe
BSWORD_ROGUE=BloodSwordRogue.cpp
BSWORD_ROGUE_OUTPUT=BloodSwordRogue.exe
DEBUG=-g -O0
LIBS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lminizip
INCLUDES=-I/usr/include/SDL2 -I/usr/include/minizip
UNAME_S=$(shell uname -s)
UNAME_M=$(shell uname -m)
DEBIAN_ARCH=amd64
DEBIAN=blood-sword.$(DEBIAN_ARCH)

ifeq ($(UNAME_S),Darwin)
	DEBIAN_ARCH=
	DEBIAN=
	LLVM=$(shell brew --prefix llvm)
	CC=$(LLVM)/bin/clang++
	LIBS += -stdlib=libc++ -Wl,-dead_strip -fdata-sections -ffunction-sections -L$(LLVM)/lib/c++ -L/usr/local/lib
	INCLUDES += -I/usr/local/include/SDL2 -I/usr/local/include/minizip
else ifeq ($(UNAME_M),aarch64)
	DEBIAN_ARCH=arm64
	DEBIAN=blood-sword.$(DEBIAN_ARCH)
	CC=g++
	LIBS += -lstdc++fs
else ifeq ($(UNAME_M),aarch32)
	DEBIAN_ARCH=
	DEBIAN=
	LIBS += -lstdc++fs
	CC=g++
else
	CC=clang++
	LIBS += -fdata-sections -ffunction-sections -lstdc++fs -Wl,--gc-sections
endif

.PHONY: all clean

all: clean blood-sword-test battle-board map-template map-renderer sprite-sheet blood-sword battlepits rogue

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

blood-sword:
	$(CC) -Wall $(BSWORD) $(LIBS) $(INCLUDES) -o $(BSWORD_OUTPUT)

battlepits:
	$(CC) -Wall $(BSWORD_BATTLEPITS) $(LIBS) $(INCLUDES) -o $(BSWORD_BATTLEPITS_OUTPUT)

rogue:
	$(CC) -Wall $(BSWORD_ROGUE) $(LIBS) $(INCLUDES) -o $(BSWORD_ROGUE_OUTPUT)

clean-blood-sword-test:
	rm -f $(BSWORD_TEST_OUTPUT) *.o

clean-battle-board:
	rm -f $(BSWORD_BATTLES_OUTPUT) *.o

clean-blood-sword:
	rm -f $(BSWORD_OUTPUT) *.o

clean-map:
	rm -f $(BSWORD_MAP_OUTPUT) *.o

clean-map-renderer:
	rm -f $(BSWORD_MAP_RENDERER_OUTPUT) *.o

clean-sprite-sheet:
	rm -f $(BSWORD_SPRITE_SHEET_OUTPUT) *.o

clean-battlepits:
	rm -f $(BSWORD_BATTLEPITS_OUTPUT) *.o

clean-rogue:
	rm -f $(BSWORD_ROGUE_OUTPUT) *.o

clean: clean-blood-sword-test clean-battle-board clean-map clean-map-renderer clean-sprite-sheet clean-blood-sword clean-battlepits clean-rogue

.ONESHELL:

debian: blood-sword
	@if  [ "$(DEBIAN)" != "" ]; then
		echo "Building Debian $(DEBIAN_ARCH) package ..."
		rm -f $(DEBIAN).deb
		rm -fR $(DEBIAN)
		mkdir -p $(DEBIAN)/DEBIAN
		mkdir -p $(DEBIAN)/usr/share/applications
		mkdir -p $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp ../build/debian/$(DEBIAN_ARCH)/control $(DEBIAN)/DEBIAN
		cp ../build/debian/BattlepitsOfKrarth.desktop $(DEBIAN)/usr/share/applications
		cp -R fonts $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp -R help $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp -R images $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp -R modules $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp -R sounds $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		cp -R $(BSWORD_OUTPUT) $(DEBIAN)/usr/local/games/BattlepitsOfKrarth
		dpkg-deb --build $(DEBIAN)
		rm -fR $(DEBIAN)
	else
		echo "Not a Debian build environment ..."
	fi
