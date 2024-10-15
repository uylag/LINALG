cmake_minimum_required(VERSION 3.14)

project(LINALG_VERSION_0.1v)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/linalg)

file(GLOB SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.hxx 
    ${CMAKE_SOURCE_DIR}/src/matrix/*.hxx 
    ${CMAKE_SOURCE_DIR}/src/sparsemtxes/*.hxx
    ${CMAKE_SOURCE_DIR}/src/matrix_instantiations.cxx
)

set(MAIN_FILE ${CMAKE_SOURCE_DIR}/main.cxx)

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)