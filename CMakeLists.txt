cmake_minimum_required(VERSION 2.6)

project(Ships CXX)
set(executableName ships)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(objects "src/objects/ship.cpp" "src/objects/star.cpp" "src/objects/planet.cpp")

set(commonsources "src/point3.cpp" "src/object.cpp" "src/utils.cpp" "src/net.cpp" "src/packet.cpp" "src/timer.cpp")

add_library(ships SHARED ${commonsources})
target_link_libraries(ships enet allegro)

SET_TARGET_PROPERTIES(ships PROPERTIES LINK_FLAGS "-Wl,--export-all-symbols" )

set(clientsources "src/main.cpp" "src/render.cpp" "src/input.cpp" "src/objects/renderfunctionsclient.cpp")
set(serversources "src/servermain.cpp" "src/client.cpp" "src/objects/renderfunctionsserver.cpp")

add_executable(client ${clientsources} ${objects})
add_executable(server ${serversources} ${objects})

set_target_properties(client PROPERTIES OUTPUT_NAME "ships")

target_link_libraries(client oglft SOIL ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ships)
target_link_libraries(server ships)
