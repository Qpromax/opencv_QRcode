cmake_minimum_required(VERSION 3.31)
project(opencv)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

add_executable(opencv main.cpp)
target_include_directories(opencv PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv PRIVATE ${OpenCV_LIBS})