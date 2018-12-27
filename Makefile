.DEFAULT_GOAL := all

SOURCE_FILE = src/game.cpp

TARGET_DEBUGER = bin/gamed.x
TARGET_PROFILE = bin/gamep.x
TARGET_RELEASE = bin/game.x

debug:   $(TARGET_DEBUGER)
profile: $(TARGET_PROFILE)
release: $(TARGET_RELEASE)

all:     $(TARGET_DEBUGER) $(TARGET_PROFILE) $(TARGET_RELEASE)

clean:
	$(RM) $(TARGET_DEBUGER)
	$(RM) $(TARGET_PROFILE)
	$(RM) $(TARGET_RELEASE)

CC_FLAGS += -Wall -Wextra
CC_INCS   = $(SDL_INC)

DEBUGER_FLAGS = $(CC_FLAGS) -DDEBUG=1   -g
PROFILE_FLAGS = $(CC_FLAGS) -DPROFILE=1 -g -O2
RELEASE_FLAGS = $(CC_FLAGS)                -O2


ifeq ($(OS),Windows_NT)
    CC_FLAGS += -DOS_WIN
    CC        = cl
    RM        = del /iq
else
  UNAME_S := $(shell uname -s)
  # TODO: -R ???
  RM       = rm -f
    ifeq ($(UNAME_S),Linux)
        CC_FLAGS += -DOS_LINUX
        SDL_INC   = -I/usr/local/include/SDL2
        SDL_LIB   = -Wl,-rpath,/usr/local/lib -L/usr/local/lib -lSDL2 -lpthread -lm -lGL
        CC_LIBS   = -L/usr/local/lib $(SDL_LIB)
        CC        = g++
        GDB       = gdb
    endif
    ifeq ($(UNAME_S),Darwin)
        # -v  Verbose mode for extra debugging
        CC_FLAGS += -DOS_OSX -Wno-trigraphs
        # OGL_INC = -FOpenGL
        OGL_LIB   = -framework OpenGL
        SDL_INC   = -I/Library/Frameworks/SDL2.framework/Headers/
        SDL_LIB   = -framework SDL2
        CC_LIBS   = -L/usr/local/lib -L/Library/Frameworks $(OGL_LIB) -framework Cocoa $(SDL_LIB)
        CC        = g++
        GDB       = lldb
    endif
endif


GAME_INC = $(wildcard src/*.h)

GAME_SRC = $(wildcard src/*.cpp)


# Debug build
$(TARGET_DEBUGER): $(GAME_INC) $(GAME_SRC)
	@echo "\n\n=== DEBUG ==="
	mkdir -p bin
	$(CC) $(DEBUGER_FLAGS) $(CC_INCS) $(SOURCE_FILE) -o $@ $(CC_LIBS)

# Profile build
$(TARGET_PROFILE): $(GAME_INC) $(GAME_SRC)
	@echo "\n\n=== PROFILE ==="
	mkdir -p bin
	$(CC) $(PROFILE_FLAGS) $(CC_INCS) $(SOURCE_FILE) -o $@ $(CC_LIBS)

# Release build
$(TARGET_RELEASE): $(GAME_INC) $(GAME_SRC)
	@echo "\n\n=== RELEASE ==="
	mkdir -p bin
	$(CC) $(RELEASE_FLAGS) $(CC_INCS) $(SOURCE_FILE) -o $@ $(CC_LIBS)

