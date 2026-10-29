cmake_minimum_required(VERSION 3.16)
project(nurgle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Layer 1: pure logic, no OS dependencies.
add_library(engine STATIC
    libs/engine/src/corruptor_engine.cpp)
target_include_directories(engine PUBLIC libs/engine/include)

# Layer 2: OS interaction (POSIX: mmap/fcntl/unistd). Depends on engine.
add_library(manager STATIC
    libs/manager/src/file_manager.cpp)
target_include_directories(manager PUBLIC libs/manager/include)
target_link_libraries(manager PUBLIC engine)

# Layer 3a: command-line front-end.
add_executable(nurgle apps/cli/main.cpp)
target_link_libraries(nurgle PRIVATE manager)

# Layer 3b: deadman-switch daemon (stub; sources are still empty).
# Uncomment once apps/daemon/main.cpp defines main().
# add_executable(nurgle-daemon apps/daemon/main.cpp)
# target_include_directories(nurgle-daemon PRIVATE apps/daemon)
# target_link_libraries(nurgle-daemon PRIVATE manager)

# Unit tests for the engine (no OS dependencies).
enable_testing()
# add_subdirectory(tests)   # enable once tests/ contains a CMakeLists.txt
