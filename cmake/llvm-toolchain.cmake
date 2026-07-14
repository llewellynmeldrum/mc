if(NOT DEFINED LLVM_PREFIX)
  if(DEFINED ENV{HOMEBREW_PREFIX})
    set(LLVM_PREFIX "$ENV{HOMEBREW_PREFIX}/opt/llvm")
  else()
    set(LLVM_PREFIX "/opt/homebrew/opt/llvm")  
  endif()
endif()

if(NOT DEFINED CMAKE_OSX_SYSROOT OR CMAKE_OSX_SYSROOT STREQUAL "")
  execute_process(
    COMMAND xcrun --show-sdk-path
    OUTPUT_VARIABLE _macos_sdk
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_OSX_SYSROOT "${_macos_sdk}" CACHE PATH "macOS SDK for Homebrew clang")
endif()

set(CMAKE_C_COMPILER      "${LLVM_PREFIX}/bin/clang")
set(CMAKE_CXX_COMPILER    "${LLVM_PREFIX}/bin/clang++")

set(CMAKE_OBJC_COMPILER   "${LLVM_PREFIX}/bin/clang")
set(CMAKE_OBJCXX_COMPILER "${LLVM_PREFIX}/bin/clang++")

set(CMAKE_CXX_FLAGS_INIT "-stdlib=libc++ -D_LIBCPP_DISABLE_AVAILABILITY")

set(_llvm_link
    "-stdlib=libc++ \
     -L${LLVM_PREFIX}/lib/c++ -L${LLVM_PREFIX}/lib/unwind -lunwind \
     -Wl,-rpath,${LLVM_PREFIX}/lib/c++ -Wl,-rpath,${LLVM_PREFIX}/lib/unwind")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "${_llvm_link}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${_llvm_link}")
