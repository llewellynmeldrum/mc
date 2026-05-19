# CMake Wrapper Makefile
# --------------------------------------------------
all: run

BUILD_DIR      := build
BUILD_FAST     := build-fast
BUILD_ASAN     := build-asan
BUILD_TSAN     := build-tsan
BUILD_AUSAN    := build-ausan

GENERATOR      := Ninja
CXX            := /opt/gcc-16/bin/g++
APP            := mc

JOBS ?= $(shell sysctl -n hw.logicalcpu 2>/dev/null || nproc)
BUILD_FLAGS := -j $(JOBS)
# WARNING: CONCURRENCY DISABLED IN BUILD FOR DEBUGGING
BUILD_FLAGS := 

CMAKE_COMMON := -G "$(GENERATOR)" \
	-DCMAKE_CXX_COMPILER="$(CXX)" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON

.PHONY: help configure build run clean rebuild \
        fast run-fast \
        debug run-debug \
        asan run-asan \
        tsan run-tsan \
        ausan run-ausan \
        compile-commands

configure:
	cmake -S . -B $(BUILD_DIR) $(CMAKE_COMMON) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DMC_O1=ON

build: configure
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS)

run: build
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS) --target run

debug: configure
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

run-asan: asan
	ASAN_OPTIONS=detect_leaks=0 cmake --build $(BUILD_ASAN) $(BUILD_FLAGS) --target run

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

run-ausan: ausan
	ASAN_OPTIONS=detect_leaks=0 cmake --build $(BUILD_AUSAN) $(BUILD_FLAGS) --target run

# --------------------------------------------------
# Utilities

db: configure
	cp $(BUILD_DIR)/compile_commands.json ./compile_commands.json

clean:
	rm -rf $(BUILD_DIR) $(BUILD_FAST) $(BUILD_ASAN) $(BUILD_TSAN) $(BUILD_AUSAN) bin

rebuild: clean build
