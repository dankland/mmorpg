cmake_minimum_required(VERSION 3.25)

project(client)

###
# Dependencies
###
include(FetchContent)
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e # 3.3.8
    FIND_PACKAGE_ARGS NAMES glfw3
)
FetchContent_Declare(
    GLEW
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG f456deace7b408655109aaeff71421ef2d3858c6 # 2.2.0
    FIND_PACKAGE_ARGS NAMES GLEW
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
)
FetchContent_MakeAvailable(glfw3)
FetchContent_MakeAvailable(GLEW)
FetchContent_MakeAvailable(glm)
###

###
# Targets
###
file(GLOB client_src CONFIGURE_DEPENDS "*.cpp" "**/*.cpp" "*.hpp" "**/*.hpp")
add_executable(client ${core_src} ${client_src})
target_include_directories(client PRIVATE ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})
target_link_libraries(client PRIVATE fmt::fmt flecs::flecs_static glfw libglew_static glm ${OPENGL_LIBRARIES})
###
