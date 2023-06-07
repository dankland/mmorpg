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
add_compile_options(-Wall -Wno-deprecated)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
add_definitions(-DMSGPACK_USE_BOOST=OFF -DMSGPACK_NO_BOOST -DMSGPACK_CXX20=ON)
###

###
# Dependencies
###
find_package(Threads)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 9.1.0
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.11.0
)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs
    GIT_TAG v3.2.1
)
FetchContent_Declare(
    msgpackc
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c
    GIT_TAG cpp-6.0.0
)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio
    GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(flecs)
FetchContent_MakeAvailable(msgpackc)

FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
target_link_libraries(asio INTERFACE Threads::Threads)
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
