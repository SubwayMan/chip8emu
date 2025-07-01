cmake_minimum_required(VERSION 3.10)
project(EmuProject CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add SDL as a subdirectory
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

# Collect all source files
file(GLOB SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the executable
add_executable(emu ${SOURCES})

# Include directory for headers
target_include_directories(emu PRIVATE ${PROJECT_SOURCE_DIR}/inc)

# SDL3
target_link_libraries(emu PRIVATE SDL3::SDL3)

# Extra compile flags
target_compile_options(emu PRIVATE -Wall -g)
