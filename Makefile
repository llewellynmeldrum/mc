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


# --------------------------------------------------

APP_EXE := ./bin/mc
CXX         :=clang++ 
CXXFLAGS    :=-std=c++23 

APP_SRC := $(shell find ./src -type f -name '*.cpp')
APP_OBJ := $(patsubst ./src/%.cpp,./build/%.o,$(APP_SRC))
APP_DEPS := $(APP_OBJ:.o=.d)

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


LDLIBS  	+= $(GL_LIBS) $(shell pkg-config --libs glfw3)

CPPFLAGS 	+= -I$(GLBINDING)/include
CPPFLAGS 	+= -Iexternal/
LDLIBS		+= -L$(GLBINDING)/lib -lglbinding -lglbinding-aux

all: run

test: CXXFLAGS+= -DTESTING
test: clean all
build: $(APP_EXE)

run: $(APP_EXE)
	$(APP_EXE)


# --------------------------------------------------
# Compile C++ app sources

./build/%.o: ./src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


$(APP_EXE): $(APP_OBJ) 
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# --------------------------------------------------
# Dependencies

-include $(APP_DEPS) 

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
