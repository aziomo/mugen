cmake_minimum_required(VERSION 3.20)
project(mugen_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(mugen_cpp main.cpp oscillator.cpp oscillator.h MusicBox.cpp MusicBox.h WaveformType.h AudioAPI.cpp AudioAPI.h MainWindow.h Texture.h MainWindow.cpp WaveformMenu.cpp WaveformMenu.h)

find_library(portsf portsf lib)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

include_directories($(SDL2_INCLUDE_DIRS) ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(mugen_cpp ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
target_link_libraries(mugen_cpp portaudio)
target_link_libraries(mugen_cpp ${portsf})
target_link_libraries(mugen_cpp pthread)