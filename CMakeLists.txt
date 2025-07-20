cmake_minimum_required(VERSION 3.20)
project(memory_pool)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O3 -march=native")

option(ENABLE_SANITIZERS "Enable Clang sanitizers" OFF)

enable_testing()

# Get includes, which declares the `memory_pool` library
add_subdirectory(include)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.24)
  cmake_policy(SET CMP0135 NEW) # allows handling of changes to remote content
endif()

# Download dependencies
include(FetchContent)

# Enable sanitizers
if (ENABLE_SANITIZERS)
    set(SANITIZER_FLAGS "-fsanitize=address,undefined")
    target_compile_options(memory_pool INTERFACE ${SANITIZER_FLAGS} -g -O1 -fno-omit-frame-pointer)
    target_link_options(memory_pool INTERFACE ${SANITIZER_FLAGS})
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Dependencies needed only for examples/test
  # Google test
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # libfmt
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.1.3)
  FetchContent_MakeAvailable(fmt)

  add_subdirectory(examples)
  add_subdirectory(test)
endif()
