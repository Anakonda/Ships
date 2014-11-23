cmake_minimum_required(VERSION 2.6)

project(Ships CXX)
set(executableName ships)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})


set(commonsources "src/object.cpp" "src/net.cpp" "src/packet.cpp" "src/point3.cpp" "src/utils.cpp" "src/timer.cpp")

set(clientsources "src/main.cpp" "src/render.cpp" "src/input.cpp" "src/objects/renderfunctionsclient.cpp")
set(serversources "src/servermain.cpp" "src/objects/renderfunctionsserver.cpp")

set(objects "src/objects/ship.cpp" "src/objects/star.cpp" "src/objects/planet.cpp")

add_executable(ships ${clientsources} ${objects} ${commonsources})
add_executable(server ${serversources} ${objects} ${commonsources})

target_link_libraries(ships oglft soil ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} enet ${FREETYPE_LIBRARIES} allegro)
target_link_libraries(server enet allegro)
