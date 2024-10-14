cmake_minimum_required(VERSION 3.14)

project(LINALG_VERSION_0.1v)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inlcude)

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cxx)

set(MAIN_FILE ${CMAKE_SOURCE_DIR}/main.cxx)

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SRC_FILES})