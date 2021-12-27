cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(stressTest VERSION 1.0)

file(GLOB SRC_FILES src/*.cpp src/*.cxx)
add_executable(stressTest ${SRC_FILES})

target_include_directories(stressTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

