cmake_minimum_required(VERSION 3.25)

project(client)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})

###
# Targets
###
file(GLOB client_src CONFIGURE_DEPENDS "*.cpp" "**/*.cpp" "*.hpp" "**/*.hpp")
add_executable(client ${core_src} ${client_src})
target_link_libraries(client fmt::fmt flecs::flecs_static glfw GLEW::glew ${OPENGL_LIBRARIES})
###
