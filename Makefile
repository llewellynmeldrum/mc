.PHONY: all build run clean asan lsan tsan ausan debug

# --------------------------------------------------
# Make flags
#MAKEFLAGS += -j8
#MAKEFLAGS += --ignore-errors

LLVM_PREFIX := /opt/homebrew/opt/llvm@22
clang++:= $(LLVM_PREFIX)/bin:$(PATH)
export clang++

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	CC       	:= clang
	DEBUGGER 	:= lldb
	GL_LIBS  	:= -framework OpenGL
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
#

# --------------------------------------------------

APP_EXE := ./bin/mc
CXX         :=clang++
CXXFLAGS    :=-std=c++23 

APP_SRC := $(shell find ./src -type f -name '*.cpp')
APP_OBJ := $(patsubst ./src/%.cpp,./build/%.o,$(APP_SRC))
APP_DEPS := $(APP_OBJ:.o=.d)

# --------------------------------------------------
GLAD_DIR := ./glad_4-1-core
GLAD_SRC := $(GLAD_DIR)/src/gl.c
GLAD_OBJ := ./build/glad/glad.o
GLAD_DEP := $(GLAD_OBJ:.o=.d)

# --------------------------------------------------
CPPFLAGS 	:= -Iinclude -I$(GLAD_DIR)/include

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


# Formatting of compiler errors (and sanitizers)
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

LDFLAGS +=
LDLIBS  += $(GL_LIBS) $(shell pkg-config --libs glfw3)


all: run

build: $(APP_EXE)

run: $(APP_EXE)
	$(APP_EXE)


# --------------------------------------------------
# Compile C++ app sources

./build/%.o: ./src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compile GLAD C source

$(GLAD_OBJ): $(GLAD_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Link final executable

$(APP_EXE): $(APP_OBJ) $(GLAD_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# --------------------------------------------------
# Dependencies

-include $(APP_DEPS) $(GLAD_DEP)

# --------------------------------------------------
# Sanitizers

asan: export ASAN_OPTIONS=detect_leaks=0
asan: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
asan: CFLAGS   += -fsanitize=address -fno-omit-frame-pointer -g
asan: LDFLAGS  += -fsanitize=address -fno-omit-frame-pointer -g
asan: clean all

lsan: export ASAN_OPTIONS=detect_leaks=1
lsan: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
lsan: CFLAGS   += -fsanitize=address -fno-omit-frame-pointer -g
lsan: LDFLAGS  += -fsanitize=address -fno-omit-frame-pointer -g
lsan: clean all

tsan: CXXFLAGS += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: CFLAGS   += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: LDFLAGS  += -fsanitize=thread -fno-omit-frame-pointer -g
tsan: clean all

ausan: CXXFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: CFLAGS   += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: LDFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: clean all

# --------------------------------------------------

clean:
	rm -rf build bin

debug: $(APP_EXE)
	$(DEBUGGER) -o run -- $(APP_EXE) $(ARGS)
