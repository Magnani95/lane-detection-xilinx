cmake_minimum_required(VERSION 3.9)

project(HopeBlood)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

find_package(OpenCV REQUIRED)

link_directories    (${CMAKE_BINARY_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(
    memory_allocator
    src/memory_allocator.cpp
)
add_library(
    LaneDetector
    src/LaneDetector.cpp
)
add_library(
    gauss
    src/kernel/gaussian_blur.cpp
    src/xtimer.cpp
)

add_library(
    cvt_color
    src/kernel/cvtcolor_bgr2gray.cpp   
)

add_library(
    utils
    src/utils.cpp
)
add_library(
    xtimer
    src/xtimer.cpp
)

add_library(
    threshold
    src/kernel/threshold.cpp
)
add_library(
    filter2d
    src/kernel/filter2d.cpp
)
add_library(
    mask
    src/kernel/mask.cpp
)
add_library(
    kernels
    src/kernels.cpp
)
#-------------------------------------------------------------------------------
add_executable(
    main
    src/main.cpp
)
#   ----------------------------------------------------------------------------
target_link_libraries(
    kernels
    mask
    gauss
    cvt_color
    threshold
    filter2d
    utils
)

target_link_libraries(
    main
    memory_allocator
    xtimer
    kernels
    utils
    LaneDetector
)
#-------------------------------------------------------------------------------
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(LaneDetector ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV non trovato!")
endif()