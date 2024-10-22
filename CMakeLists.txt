cmake_minimum_required(VERSION 3.5)

project(play
    LANGUAGES CXX C
)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 17)

add_executable(play)
target_sources(play PRIVATE
	src/main.cpp

	src/LittleEndianReader.cpp
	src/LittleEndianReader.h

	src/Track.cpp
	src/track_play.cpp
	src/Track.h

	src/OutputStream.cpp
	src/OutputStream.h
)
set_target_properties(play PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic -g3")
target_include_directories(play PUBLIC src)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME portaudio
    GITHUB_REPOSITORY PortAudio/portaudio
    VERSION 19.7.0
)
target_include_directories(play PUBLIC ${portaudio_SOURCE_DIR}/include)
target_link_libraries(play portaudio)
