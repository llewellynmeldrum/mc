.PHONY: all build run clean asan lsan tsan ausan debug db

all: run
# --------------------------------------------------
# Make flags
MAKEFLAGS += -j8
#MAKEFLAGS += --ignore-errors

SHELL:= /bin/zsh
LLVM_PREFIX := /opt/homebrew/opt/llvm@22
clang++:= $(LLVM_PREFIX)/bin:$(PATH)
export clang++

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	CC       	:= clang
	DEBUGGER 	:= lldb
	GLBINDING  := $(shell brew --prefix glbinding)
	LLVM_PREFIX := /opt/homebrew/opt/llvm@22
	SDKROOT     := $(shell xcrun --show-sdk-path)
	# to use llvm-22 clang++

else ifeq ($(UNAME),Linux)
	CXX      := clang++-22
	CC       := clang-22
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
CXX         :=clang++
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

CPPFLAGS    +=-D_LIBCPP_DISABLE_AVAILABILITY
LDFLAGS     +=-isysroot $(SDKROOT)
LDFLAGS     +=-L$(LLVM_PREFIX)/lib/c++ 
LDFLAGS     +=-L$(LLVM_PREFIX)/lib/unwind
LDFLAGS     +=-Wl,-rpath,$(LLVM_PREFIX)/lib/c++ 
LDFLAGS		+=-Wl,-rpath,$(LLVM_PREFIX)/lib/unwind
LDLIBS      +=-lunwind

CXXFLAGS 	+=-Wall -Werror
CXXFLAGS 	+=-Wimplicit-fallthrough
CXXFLAGS 	+=-Wno-unused
CXXFLAGS 	+=-MMD -MP
CXXFLAGS 	+=$(shell pkg-config --cflags glfw3)


# format of compiler errors (and sanitizers)
CXXFLAGS += -fdebug-prefix-map=$(PWD)=.
CXXFLAGS += -fno-show-column
CXXFLAGS += -fno-diagnostics-show-option
CXXFLAGS += -fdiagnostics-fixit-info

CFLAGS := -Wall -Werror
CFLAGS += -Wimplicit-fallthrough
CFLAGS += -MMD -MP
CFLAGS += -fdebug-prefix-map=$(PWD)=.
CFLAGS += -fno-show-column
CFLAGS += -fno-diagnostics-show-option
CFLAGS += -fdiagnostics-fixit-info

# --------------------------------------------------
# Link flags


# DEP: glfw3 (install via brew or apt or pacman or something similar)
LDLIBS  	+= $(GL_LIBS) $(shell pkg-config --libs glfw3)

# DEP: GLBINDING (header only, no install)
CPPFLAGS 	+= -I$(GLBINDING)/include
CPPFLAGS 	+= -Iexternal/
LDLIBS		+= -L$(GLBINDING)/lib -lglbinding -lglbinding-aux

# DEP: FastNoise2
# NOTE: (run ./install_fastnoise.sh once before building)
FASTNOISE_DIR     := external/FastNoise2
FASTNOISE_LIB_DIR := $(FASTNOISE_DIR)/build/Release/lib

CXXFLAGS += \
  -I$(FASTNOISE_DIR)/include \
  -I$(FASTNOISE_DIR)/build/_deps/fastsimd-src/include \
  -DFASTNOISE_STATIC_LIB

LDFLAGS += -L$(FASTNOISE_LIB_DIR)
LDLIBS  += -lFastNoise
test: CXXFLAGS+= -DTESTING
test: clean all
build: $(EXE)

fastest: CXXFLAGS+= -O3 -ffast-math
fastest: run

fast: CXXFLAGS+= -O1 
fast: run

fast-math: CXXFLAGS+= -ffast-math
fast-math: run

run: CXXFLAGS+= -O1 
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
