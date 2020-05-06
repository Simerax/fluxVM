cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)

project("fluxVM")

set(lib_dir "${PROJECT_SOURCE_DIR}/lib")
file(GLOB source_files "${lib_dir}/*.c" "${lib_dir}/*.h")

add_library(${PROJECT_NAME} SHARED ${source_files})

add_subdirectory(bin)


