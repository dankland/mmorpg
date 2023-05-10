cmake_minimum_required(VERSION 3.25)

project(mmorpg)

###
# Base directory relative includes for everyone
###
include_directories(.)
###

###
# Global Flags
###
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g3 -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_compile_options(-Wall)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
###

###
# Dependencies
###
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 9.1.0
)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs
    GIT_TAG v3.2.1
)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(flecs)
###

###
# Common sources
###
file(GLOB core_src CONFIGURE_DEPENDS "core/*.hpp" "core/*.cpp" "core/*/*.hpp" "core/*/*.cpp")
###

###
# Subdirectories
###
add_subdirectory(client)
add_subdirectory(server)
###
