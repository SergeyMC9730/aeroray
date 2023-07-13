cmake_minimum_required(VERSION 3.0.0)
project(aeroray VERSION 0.1.0 LANGUAGES C CXX)

file(COPY resources DESTINATION .)
file(COPY resources_replacement DESTINATION .)

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_subdirectory(raylib)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address,leak")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,leak")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,leak")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB aeroray_src
    "src/*.hpp"
    "src/*.h"
    "src/*.cpp"
)
file(GLOB libdwm_src
    "libdwm*.hpp"
    "libdwm/*.h"
    "libdwm/*.cpp"
)

add_library(dwm ${libdwm_src})

target_include_directories(dwm PRIVATE
    raylib/src
)

target_link_libraries(dwm raylib)

add_executable(aeroray ${aeroray_src})

target_include_directories(aeroray PRIVATE
    raylib/src
    libdwm
)

target_link_libraries(aeroray raylib dwm)