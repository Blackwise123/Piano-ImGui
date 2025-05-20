cmake_minimum_required(VERSION 3.10)
project(Piano VERSION 1.0)

# packages needed
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/sdl
)

# add imgui sources
set(IMGUI_SOURCES
    Resource Files/imgui.cpp
    Resource Files/imgui_demo.cpp
    Resource Files/imgui_draw.cpp
    Resource Files/imgui_tables.cpp
    Resource Files/imgui_widgets.cpp
    Resource Files/imgui_impl_sdl2.cpp
    Resource Files/imgui_impl_sdlrenderer2.cpp
)

# add app sources
set(APP_SOURCES
    src/main.cpp
    src/MusicTheory.cpp
)

# create exe
add_executable(Piano ${APP_SOURCES} ${IMGUI_SOURCES})

# link lib from finding package
target_link_libraries(Piano
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# copy assert
file(COPY ${CMAKE_SOURCE_DIR}/piano-mp3 DESTINATION ${CMAKE_BINARY_DIR})

