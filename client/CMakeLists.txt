cmake_minimum_required(VERSION 3.25)

project(client)

include(FetchContent)

###
# glfw3
###
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e
    FIND_PACKAGE_ARGS NAMES glfw3
)
FetchContent_MakeAvailable(glfw3)
###

###
# GLEW
###
FetchContent_Declare(
    GLEW
    GIT_REPOSITORY https://sourceforge.net/projects/glew
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e
    FIND_PACKAGE_ARGS NAMES GLEW
)
FetchContent_MakeAvailable(GLEW)
###

###
# Targets
###
file(GLOB client_src CONFIGURE_DEPENDS "*.cpp" "**/*.cpp" "*.hpp" "**/*.hpp")
add_executable(client ${core_src} ${client_src})
target_link_libraries(client fmt::fmt flecs::flecs_static glfw GLEW::glew ${OPENGL_LIBRARIES})
###
