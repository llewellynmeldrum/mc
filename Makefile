# CMake Wrapper Makefile
# --------------------------------------------------
all: run-asan

BUILD_DIR      := build
BUILD_FAST     := build-fast
BUILD_ASAN     := build-asan
BUILD_TSAN     := build-tsan
BUILD_AUSAN    := build-ausan

ASAN_OPTS := detect_leaks=0:color=always:abort_on_error=1:halt_on_error=1
UBSAN_OPTS := color=always:print_stacktrace=1:halt_on_error=1

GENERATOR      := Ninja
CXX            := /opt/gcc-16/bin/g++
APP            := mc

JOBS ?= $(shell sysctl -n hw.logicalcpu 2>/dev/null || nproc)
BUILD_FLAGS := -j $(JOBS)

CMAKE_COMMON := -G "$(GENERATOR)" \
	-DCMAKE_CXX_COMPILER="$(CXX)" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON

.PHONY: help configure build run clean clean-all rebuild \
        fast run-fast \
        debug run-debug \
        asan run-asan \
        tsan run-tsan \
        ausan run-ausan \
        compile-commands

configure:
	cmake -S . -B $(BUILD_DIR) $(CMAKE_COMMON) \
		-DMC_O1=ON
#		-DCMAKE_BUILD_TYPE=Debug \

configure-debug:
	cmake -S . -B $(BUILD_DIR) $(CMAKE_COMMON) \
		-DMC_O1=ON \
		-DCMAKE_BUILD_TYPE=Debug 

build: configure
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS)

run: build
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS) --target run

debug: configure-debug
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS) 

run-debug: debug
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS) --target debug

# --------------------------------------------------
# Optimized build

fast:
	cmake -S . -B $(BUILD_FAST) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug\
		-DMC_O2=ON

	cmake --build $(BUILD_FAST) $(BUILD_FLAGS)

faster:
	cmake -S . -B $(BUILD_FAST) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DMC_O3=ON

	cmake --build $(BUILD_FAST) $(BUILD_FLAGS)

run-fast: fast
	cmake --build $(BUILD_FAST) $(BUILD_FLAGS) --target run

run-faster: faster
	cmake --build $(BUILD_FAST) $(BUILD_FLAGS) --target run
# --------------------------------------------------
# Sanitizers

asan:
	cmake -S . -B $(BUILD_ASAN) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DMC_ENABLE_ASAN=ON
	cmake --build $(BUILD_ASAN) $(BUILD_FLAGS)



tsan:
	cmake -S . -B $(BUILD_TSAN) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DMC_ENABLE_TSAN=ON
	cmake --build $(BUILD_TSAN) $(BUILD_FLAGS)

run-tsan: tsan
	cmake --build $(BUILD_TSAN) $(BUILD_FLAGS) --target run

ausan:
	cmake -S . -B $(BUILD_AUSAN) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DMC_ENABLE_ASAN=ON \
		-DMC_ENABLE_UBSAN=ON
	cmake --build $(BUILD_AUSAN) $(BUILD_FLAGS)

run-asan: asan
	ASAN_OPTIONS="$(ASAN_OPTS)" cmake --build $(BUILD_ASAN) $(BUILD_FLAGS) --target run

run-ausan: ausan
	ASAN_OPTIONS="$(ASAN_OPTS)" UBSAN_OPTIONS="$(UBSAN_OPTS)" cmake --build $(BUILD_AUSAN) $(BUILD_FLAGS) --target run

# --------------------------------------------------
# Utilities

db: configure
	cp $(BUILD_DIR)/compile_commands.json ./compile_commands.json

clean:
	find $(BUILD_DIR)/CMakeFiles/mc.dir/src -name '*.o' -delete 2>/dev/null || true
	find $(BUILD_DIR)/CMakeFiles/mc.dir/src -name '*.o.d' -delete 2>/dev/null || true
	rm -f bin/$(APP)

clean-all:
	rm -rf $(BUILD_DIR) $(BUILD_FAST) $(BUILD_ASAN) $(BUILD_TSAN) $(BUILD_AUSAN) bin

rebuild: clean build
