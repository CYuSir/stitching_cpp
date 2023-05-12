cmake_minimum_required(VERSION 3.10)
project(ImageStitching)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 4.1.1 REQUIRED)

add_library(image_stitching image_stitching.cpp image_stitching.h)
target_link_libraries(image_stitching ${OpenCV_LIBS})

add_executable(main main.cpp)
target_link_libraries(main image_stitching ${OpenCV_LIBS})
