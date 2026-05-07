.PHONY: all build run clean asan lsan tsan ausan debug db

all: run
# --------------------------------------------------
# Make flags
MAKEFLAGS += -j8
#MAKEFLAGS += --ignore-errors

SHELL:= /bin/zsh

GLBINDING  := external/glbinding
GLM := external/OpenGL-Mathematics
STB_IMAGE:= external/stb_image
KHR := external/KHR
FASTNOISELITE := external/FastNoiseLite
IMGUI := external/imgui
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	DEBUGGER 	:= lldb

else ifeq ($(UNAME),Linux)
	DEBUGGER := gdb
	$(errror setup lvvm prefix)
else
	$(error Unsupported platform.)
endif
# --------------------------------------------------


db:
	@mv compile_commands.json compile_commands.json.bak
	compiledb -n make clean all
# --------------------------------------------------

EXE := ./bin/mc
CXX         :=/opt/gcc-16/bin/g++
CXXFLAGS    :=-std=c++23

APP_SRC := $(shell find ./src -type f -name '*.cpp')
APP_OBJ := $(patsubst ./src/%.cpp,./build/%.o,$(APP_SRC))

IMGUI_SRC:= $(shell find ./external/imgui -maxdepth 1 -type f -name '*.cpp')
IMGUI_OBJ:= $(patsubst ./external/imgui/%.cpp,./build/%.o,$(IMGUI_SRC))

OBJ := $(APP_OBJ) $(IMGUI_OBJ)
DEPS := $(OBJ:.o=.d)
# --------------------------------------------------

# --------------------------------------------------
CPPFLAGS 	:= -Iinclude 


CXXFLAGS 	+=-Wall -Werror
CXXFLAGS 	+=-Wimplicit-fallthrough
CXXFLAGS 	+=-Wno-unused
CXXFLAGS 	+=-MMD -MP
CXXFLAGS 	+=$(shell pkg-config --cflags glfw3)



CFLAGS := -Wall -Werror
CFLAGS += -Wimplicit-fallthrough
CFLAGS += -MMD -MP

# --------------------------------------------------
# Link flags


# DEP: glfw3 (install via brew or apt or pacman or something similar)
LDLIBS  	+= $(GL_LIBS) $(shell pkg-config --libs glfw3)

CPPFLAGS += -isystem /opt/gcc-16/include/c++/17.0.0
CPPFLAGS += -isystem /opt/gcc-16/include/c++/17.0.0/aarch64-apple-darwin23.5.0
CPPFLAGS += -isystem /opt/gcc-16/lib/gcc/aarch64-apple-darwin23.5.0/17.0.0/include
# DEP: GLBINDING (header only, no install)
CPPFLAGS	+= -I$(KHR)
CPPFLAGS	+= -I$(GLM)
CPPFLAGS	+= -I$(STB_IMAGE)

CPPFLAGS	+= -I$(GLBINDING)/source/glbinding/include
CPPFLAGS	+= -I$(GLBINDING)/source/glbinding-aux/include
CPPFLAGS 	+= -I$(GLBINDING)/build/source/glbinding/include
CPPFLAGS 	+= -I$(GLBINDING)/build/source/glbinding-aux/include

CPPFLAGS 	+= -I$(FASTNOISELITE)
CPPFLAGS 	+= -I$(IMGUI)

LDLIBS		+= -L$(GLBINDING)/build -Wl,-rpath,$(PWD)/external/glbinding/build
LDLIBS 		+= -lglbinding -lglbinding-aux

# DEP: FastNoiseLite


test: CXXFLAGS+= -DTESTING
test: clean all
build: $(EXE)

fastest: CXXFLAGS+= -O3 -ffast-math
fastest: run

fast: CXXFLAGS+= -O1 
fast: run

fast-math: CXXFLAGS+= -ffast-math
fast-math: run

#run: CXXFLAGS+= -O1 
run: $(EXE) 
	$(EXE)


# --------------------------------------------------
# Compile C++ app sources

./build/%.o: ./src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

./build/%.o: ./external/imgui/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJ) 
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# --------------------------------------------------
# Dependencies

-include $(DEPS) 

# --------------------------------------------------
# Sanitizers

asan: export ASAN_OPTIONS=detect_leaks=0
asan: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
asan: CFLAGS   += -fsanitize=address -fno-omit-frame-pointer -g
asan: LDFLAGS  += -fsanitize=address -fno-omit-frame-pointer -g
asan: all

lsan: export ASAN_OPTIONS=detect_leaks=1
lsan: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
lsan: CFLAGS   += -fsanitize=address -fno-omit-frame-pointer -g
lsan: LDFLAGS  += -fsanitize=address -fno-omit-frame-pointer -g
lsan: all

tsan: CXXFLAGS += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: CFLAGS   += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: LDFLAGS  += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: all

ausan: CXXFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: CFLAGS   += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: LDFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: all

# --------------------------------------------------

clean:
	rm -rf build bin

debug: CXXFLAGS += -g
debug: $(EXE)
	$(DEBUGGER) -o -- $(EXE) $(ARGS)
