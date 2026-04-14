.PHONY: all build run clean asan lsan tsan ausan debug

# --------------------------------------------------
# Make flags
#MAKEFLAGS += -j8
#MAKEFLAGS += --ignore-errors

UNAME := $(shell uname)


# --------------------------------------------------
# Project layout

APP_EXE := ./bin/server

APP_SRC := $(shell find ./src -type f -name '*.cpp')
APP_OBJ := $(patsubst ./src/%.cpp,./build/%.o,$(APP_SRC))
APP_DEPS := $(APP_OBJ:.o=.d)

# --------------------------------------------------
# GLAD

GLAD_DIR := ./glad
GLAD_SRC := $(GLAD_DIR)/src/gl.c
GLAD_OBJ := ./build/glad/glad.o
GLAD_DEP := $(GLAD_OBJ:.o=.d)

# --------------------------------------------------
# Includes / flags

CPPFLAGS := -Iinclude -I$(GLAD_DIR)/include

CXXFLAGS := -std=c++23
CXXFLAGS += -Wall -Werror
CXXFLAGS += -Wimplicit-fallthrough
CXXFLAGS += -Wno-unused
CXXFLAGS += -MMD -MP
CXXFLAGS += $(shell pkg-config --cflags glfw3)


CFLAGS := -Wall -Werror
CFLAGS += -Wimplicit-fallthrough
CFLAGS += -MMD -MP

# Formatting of compiler errors (and sanitizers)
CXXFLAGS += -fdebug-prefix-map=$(PWD)=.
CXXFLAGS += -fno-show-column
CXXFLAGS += -fno-diagnostics-show-option
CXXFLAGS += -fdiagnostics-fixit-info

CFLAGS += -fdebug-prefix-map=$(PWD)=.
CFLAGS += -fno-show-column
CFLAGS += -fno-diagnostics-show-option
CFLAGS += -fdiagnostics-fixit-info

# --------------------------------------------------
# Link flags
# Add GLFW here if you are linking it manually.

LDFLAGS :=
LDLIBS := $(GL_LIBS) $(shell pkg-config --libs glfw3)

ifeq ($(UNAME),Darwin)
	CC       := clang
	DEBUGGER := lldb
	GL_LIBS  := -framework OpenGL
	LLVM_PREFIX := /opt/homebrew/opt/llvm
	SDKROOT     := $(shell xcrun --show-sdk-path)

	CXX         := $(LLVM_PREFIX)/bin/clang++
	CPPFLAGS    += -D_LIBCPP_DISABLE_AVAILABILITY
	CXXFLAGS    += -std=c++23 -isysroot $(SDKROOT) -Wno-unused 
	LDFLAGS     += -isysroot $(SDKROOT)
	LDFLAGS     += -L$(LLVM_PREFIX)/lib/c++ -L$(LLVM_PREFIX)/lib/unwind
	LDFLAGS     += -Wl,-rpath,$(LLVM_PREFIX)/lib/c++ -Wl,-rpath,$(LLVM_PREFIX)/lib/unwind
	LDLIBS      += -lunwind
else ifeq ($(UNAME),Linux)
	CXX      := clang++-22
	CC       := clang-22
	DEBUGGER := gdb
	GL_LIBS  := -lGL
else
	$(error Unsupported platform.)
endif
# --------------------------------------------------

all: run

build: $(APP_EXE)

run: $(APP_EXE)
	@./scripts/exec_wrapper.sh $(APP_EXE) $(ARGS)

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
