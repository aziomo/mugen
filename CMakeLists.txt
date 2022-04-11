cmake_minimum_required(VERSION 3.20)
project(mugen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(mugen main.cpp
        include/Oscillator.h        source/Oscillator.cpp
        include/MusicBox.h          source/MusicBox.cpp
        include/AudioAPI.h          source/AudioAPI.cpp
        include/InstrumentMenu.h    source/InstrumentMenu.cpp
        include/MainWindow.h        source/MainWindow.cpp
        include/SelectControl.h     source/SelectControl.cpp
        include/CheckboxControl.h   source/CheckboxControl.cpp
        include/CompositionMenu.h   source/CompositionMenu.cpp
        include/OptionsMenu.h       source/OptionsMenu.cpp
        include/Serializer.h        source/Serializer.cpp
        include/ButtonControl.h     source/ButtonControl.cpp
        include/Deserializer.h      source/Deserializer.cpp
        include/Instrument.h        source/Instrument.cpp
        include/WaveformType.h
        include/Texture.h
        include/SelectorType.h
        include/Direction.h
        include/Control.h
        include/Envelope.h
        include/ItemList.h
        include/Timeline.h
        include/Segment.h
        include/SegmentManager.h
        include/json.h
        source/Config.cpp include/Config.h)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories($(SDL2_INCLUDE_DIRS) ${SDL2TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(mugen ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(mugen portaudio)
target_link_libraries(mugen sndfile)
target_link_libraries(mugen pthread)