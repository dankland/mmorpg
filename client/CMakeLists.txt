cmake_minimum_required(VERSION 3.25)

project(client)

###
# Targets
###
file(GLOB client_src CONFIGURE_DEPENDS "*.cpp" "**/*.cpp" "*.hpp" "**/*.hpp")
add_executable(client ${core_src} ${client_src})
target_link_libraries(client fmt::fmt flecs::flecs_static)
###
