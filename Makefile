.PHONY: all build test test-build clean
# ---
#MAKEFLAGS   +=-j8
#MAKEFLAGS   +=--ignore-errors

CC			:=clang-22 -v
CPPFLAGS    :=-Iinclude
CFLAGS      :=-std=c++23
CFLAGS      +=-Wall -Werror
CFLAGS      +=-Wimplicit-fallthrough 
CFLAGS      +=-Wno-unused
CFLAGS      +=-MMD -MP


# Formatting of compiler errors (and sanitizers)
CFLAGS		+=-fdebug-prefix-map=$(PWD)=.
CFLAGS      +=-fno-show-column
CFLAGS      +=-fno-diagnostics-show-option
CFLAGS      +=-fdiagnostics-fixit-info

LDFLAGS     := 
LDLIBS      := -lstdc++
# -lc++ 	-> GNU 
# -lstdc++ 	-> LLVM/CLANG

# ---
APP_SRC     :=$(shell find ./src -type f -name '*.cpp')
APP_OBJ     :=$(patsubst ./src/%.cpp,./build/%.o,$(APP_SRC))
APP_DEPS    :=$(APP_OBJ:.o=.d)
APP_EXE     :=./bin/server


all: run



build: /bin $(APP_EXE)

run: $(APP_EXE)
	@./scripts/exec_wrapper.sh $(APP_EXE) $(ARGS)

# NOTE: Compile APP
./build/%.o : ./src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# NOTE: Link APP (build main executable)
$(APP_EXE): $(APP_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@


# TESTING STUFF: make /test
#TEST_SRC    :=$(shell find ./test -type f -name '*.cpp')
#TEST_OBJ    :=$(patsubst ./test/%.cpp,./build/test/%.o,$(TEST_SRC))
#TEST_DEPS   :=$(TEST_OBJ:.o=.d)
#TEST_EXES   := $(patsubst ./build/test/%.o,./bin/test/%,$(TEST_OBJ))
#TEST_CPPFLAGS :=-Itest
#
## for use with testing - each test file defines their own main fn
#CORE_OBJ    :=$(filter-out ./build/main.o,$(APP_OBJ))
## ---
#
#
#    
#test-build: $(TEST_EXES) 
#
#test: $(TEST_EXES)
#	@for t in $^; do \
#	echo "Running $$t"; \
#		$$t || exit 1; \
#	done
#
## NOTE: Link TESTS (build each executable)
#$(TEST_EXES): ./bin/test/%: ./build/test/%.o $(CORE_OBJ)
#	@mkdir -p $(dir $@)
#	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
#
## NOTE: Compile TESTS
#./build/test/%.o : ./test/%.cpp
#	@mkdir -p $(dir $@)
#	$(CC) $(CPPFLAGS) $(TEST_CPPFLAGS) $(CFLAGS) -c $< -o $@
#
-include $(APP_DEPS) $(TEST_DEPS)

asan: export ASAN_OPTIONS=detect_leaks=0
asan: CFLAGS+=-fsanitize=address -fno-omit-frame-pointer  -g
asan: LDFLAGS+=-fsanitize=address -fno-omit-frame-pointer -g
asan: clean all

lsan: export ASAN_OPTIONS=detect_leaks=1
lsan: CFLAGS+=-fsanitize=address -fno-omit-frame-pointer  -g
lsan: LDFLAGS+=-fsanitize=address -fno-omit-frame-pointer -g
lsan: clean all

tsan: CFLAGS+=-fsanitize=thread -fno-omit-frame-pointer -g
tsan: LDFLAGS+=-fsanitize=thread -fno-omit-frame-pointer -g
tsan: clean all

ausan: CFLAGS+=-fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: LDFLAGS+=-fsanitize=address,undefined -fno-omit-frame-pointer -g
ausan: clean all

# ---
clean: 
	rm -rf build bin

# ---
debug: $(APP_EXE)
	$(DEBUGGER) -o run -- $(APP_EXE) $(ARGS)

# ---
UNAME:= $(shell uname)
ifeq ($(UNAME), Darwin)
	CC :=clang
	DEBUGGER :=lldb
else ifeq ($(UNAME), Linux)
	DEBUGGER :=gdb
	CC :=clang-22 
else
	$(error Unsupported platform.)
endif
