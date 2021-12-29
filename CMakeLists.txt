cmake_minimum_required(VERSION 3.20)
project(mugen)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(mugen main.cpp
        source/Oscillator.cpp include/Oscillator.h
        source/MusicBox.cpp     include/MusicBox.h
        source/AudioAPI.cpp     include/AudioAPI.h
        source/InstrumentMenu.cpp include/InstrumentMenu.h
        source/MainWindow.cpp   include/MainWindow.h
        include/WaveformType.h
        include/Texture.h
        source/SelectControl.cpp include/SelectControl.h source/Instrument.cpp include/Instrument.h include/SelectorType.h include/Direction.h source/CheckboxControl.cpp include/CheckboxControl.h include/Control.h source/Envelope.cpp include/Envelope.h source/GraphPainter.cpp include/GraphPainter.h source/ItemList.cpp include/ItemList.h source/CompositionMenu.cpp include/CompositionMenu.h source/Timeline.cpp include/Timeline.h source/Segment.cpp include/Segment.h)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories($(SDL2_INCLUDE_DIRS) ${SDL2TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(mugen ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(mugen portaudio)
target_link_libraries(mugen sndfile)
target_link_libraries(mugen pthread)