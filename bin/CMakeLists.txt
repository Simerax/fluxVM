cmake_minimum_required(VERSION 3.5)

project("flux")

file(GLOB source_files "*.c")

include_directories("../lib/")

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME} PUBLIC fluxVM)
