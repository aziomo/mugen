cmake_minimum_required(VERSION 3.20)
project(mugen_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(mugen_cpp main.cpp
        source/oscillator.cpp   include/oscillator.h
        source/MusicBox.cpp     include/MusicBox.h
        source/AudioAPI.cpp     include/AudioAPI.h
        source/WaveformMenu.cpp include/WaveformMenu.h
        source/MainWindow.cpp   include/MainWindow.h
        include/WaveformType.h
        include/Texture.h
        source/SelectControl.cpp include/SelectControl.h)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories($(SDL2_INCLUDE_DIRS) ${SDL2TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(mugen_cpp ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(mugen_cpp portaudio)
target_link_libraries(mugen_cpp pthread)