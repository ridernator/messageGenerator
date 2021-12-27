cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(messageGenerator VERSION 1.0)

find_package(XercesC 3 REQUIRED)

file(GLOB SRC_FILES src/*.cpp src/*.cxx)
add_executable(messageGenerator ${SRC_FILES})

target_include_directories(messageGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                   ${XercesC_INCLUDE_DIR})

target_link_libraries(messageGenerator PUBLIC ${XercesC_LIBRARIES}
                                              ${XercesC_DEPENDENCIES})
