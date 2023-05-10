cmake_minimum_required(VERSION 3.25)

project(server)

###
# Global Flags
###
add_compile_options(-Wextra -Wpedantic)
###

###
# Targets
###
file(GLOB server_src CONFIGURE_DEPENDS "*.cpp" "**/*.cpp" "*.hpp" "**/*.hpp")
add_executable(server ${core_src} ${server_src})
target_link_libraries(server fmt::fmt flecs::flecs_static)
###
