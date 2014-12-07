cmake_minimum_required(VERSION 2.6)

project(Ships CXX)
set(executableName ships)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(objects "src/objects/ship.cpp" "src/objects/star.cpp" "src/objects/planet.cpp" "src/objects/renderfunctionsclient.cpp")

set(weapons "src/weapons/laser.cpp")
set(projectiles "src/projectiles/laser.cpp")

set(commonsources "src/point3.cpp" "src/object.cpp" "src/utils.cpp" "src/net.cpp" "src/packet.cpp" "src/timer.cpp" "src/weapon.cpp" "src/projectile.cpp" ${weapons} ${objects} ${projectiles})

add_library(lib SHARED ${commonsources})
set_target_properties(lib PROPERTIES OUTPUT_NAME "ships")
target_link_libraries(lib enet allegro ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

SET_TARGET_PROPERTIES(lib PROPERTIES LINK_FLAGS "-Wl,--export-all-symbols" )

set(clientsources "src/main.cpp" "src/render.cpp" "src/input.cpp" "src/render.cpp")
set(serversources "src/servermain.cpp" "src/client.cpp")

add_executable(client ${clientsources})
add_executable(server ${serversources})

set_target_properties(client PROPERTIES OUTPUT_NAME "ships")

target_link_libraries(client oglft SOIL ${FREETYPE_LIBRARIES} lib)
target_link_libraries(server lib)
